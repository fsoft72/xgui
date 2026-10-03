# GTK4 port of the xgui GTK backend - Design

Date: 2026-10-03
Branch: `gtk4`

## Goal

Port the GTK backend (`xgui/gtk/`) from GTK 3.24 to GTK 4 (4.20 installed), dropping GTK 3 entirely.
The public xgui API (`xgui/*.h`) and the Python bindings keep working unchanged, except for one
new optional static method (`Master::SetApplicationId`). The end state uses no deprecated GTK 4 API:
List, Tree and ComboBox move to the list-model widgets (`GtkColumnView`, `GtkTreeListModel`,
`GtkDropDown`).

## Decisions

| Topic | Decision |
|---|---|
| GTK 3 backend | Replaced in place, no parallel backend, no `#if` version switches |
| Deprecated widgets | Not allowed in the end state (`GtkTreeView`, `GtkComboBox`, `GtkDialog`, `GtkFileChooser*` removed) |
| Application model | `GtkApplication` |
| Application id | Optional `Master::SetApplicationId(id, unique = false)` before `Instance()`; default id derived from the program name, `G_APPLICATION_NON_UNIQUE` |
| Execution strategy | Switch to GTK 4 in one commit (faithful port, List/Tree/ComboBox temporarily on the deprecated widgets), then one commit per modernized widget |
| Window position / `onmove` | Not supported (GTK 4 has no API); setter warns and returns false, getter returns false, `linkEvent("onmove")` returns false |
| Menu item icons | Passed as `G_MENU_ATTRIBUTE_ICON`; GTK 4 usually does not render them next to labels, accepted |
| `GuiLock`/`GuiUnlock` | Kept as a plain recursive mutex; documented that `registerIdleCallback` is the only safe way to touch the GUI from another thread |

## 1. Lifecycle, windows and events

### Master (`master_impl.cpp`)

- Creates a `GtkApplication` with the configured id (or `org.xgui.<sanitized prgname>`), flags
  `G_APPLICATION_NON_UNIQUE` unless `unique` was requested, then calls `g_application_register()`.
  Registering emits `startup`, which initializes GTK, so windows can be attached to the
  application as soon as they are created.
- `g_application_hold()` keeps the application alive until `Quit()`: the application exits on
  `Quit()`, not when the last window closes (same semantic as today).
- `Run()`: the outermost level runs `g_application_run(app, 0, NULL)`; nested levels run a
  dedicated `GMainLoop` (a stack of loops). `Quit()` stops the innermost level
  (`g_main_loop_quit` for nested ones, `g_application_release` + `g_application_quit` for the
  outermost).
- `destroyWidget`: `gtk_window_destroy` for windows, `gtk_widget_unparent`/container specific
  remove for the other widgets.
- A shared internal helper `run_until_done(GMainLoop *)` is used by synchronous dialogs and
  `Window::modalRun()`.

### Window (`window_impl.cpp`)

- `gtk_window_new()` + `gtk_window_set_application()`; the inner vbox is set with
  `gtk_window_set_child`.
- Modal windows no longer use `GtkDialog`: a plain `GtkWindow` with `set_modal` and
  `set_transient_for`; `modalRun()` runs a nested loop that ends on `close-request`.
- Size: `gtk_window_set_default_size` and `gtk_widget_get_width/height`.
- `position` and `onmove`: unsupported (see Decisions).

### Events (`widget_impl.cpp`)

The generic `"event"` handler and every `GdkEvent*` signature go away. `linkEvent` attaches a
dedicated controller per event:

| xgui event | GTK 4 |
|---|---|
| onmousedown / onmouseup / onmouseclick / onmousedblclick | `GtkGestureClick` (button 0 = all); `n_press == 2` is a double click; click = release on the widget that got the press |
| onmousemove / onmouseover / onmouseout | `GtkEventControllerMotion` (`motion`, `enter`, `leave`) |
| onkeydown / onkeyup / onkeypress | `GtkEventControllerKey` (`key-pressed`, `key-released`), same autorepeat logic as today |
| onfocus / onblur | `GtkEventControllerFocus` |
| onshow / onhide / onenable / ondisable / ondestroy | unchanged signals |
| onresize | `notify::default-width/height` on windows, size polling through a tick callback on other widgets |

- The time based de-duplication goes away: controllers run in bubble phase and claim the
  gesture / return `TRUE` when the callback consumes the event, so only the innermost widget
  handles it.
- `decodeButtonEvent(guint button, GdkModifierType state, ...)`; `GDK_MOD1_MASK` becomes
  `GDK_ALT_MASK`.

## 2. Containers, simple widgets, dialogs, Menu and Toolbar

### Mechanical replacements

- `gtk_container_add` / `gtk_box_pack_*` become `gtk_box_append`/`prepend`, `gtk_frame_set_child`,
  `gtk_scrolled_window_set_child`, `gtk_window_set_child`, `gtk_notebook_append_page`; expand /
  fill map to `hexpand`/`vexpand` and `halign`/`valign`.
- `gtk_widget_show_all` and `GtkEventBox` are removed.
- Image: `GdkPixbuf` loading becomes `GdkTexture`; `GtkPicture` for ImageView, `GtkImage` for
  icons.
- Checkbox radio groups use `gtk_check_button_set_group`; Calendar uses `GDateTime`; Entry uses
  `GtkEditable` text accessors; Spin, Slider, Progressbar, Label, Tab, Frame only need signature
  fixes.

### Dialogs (public synchronous signatures unchanged)

- `OpenFileDialog` / `SaveFileDialog`: `GtkFileDialog` (`open`, `open_multiple`, `save`) with
  `GtkFileFilter`s built from `FileExtensionsVector`, initial folder and file name.
- `MessageDialog`: `GtkAlertDialog` with buttons built from the `buttons` string; return codes
  stay as today.
- The async callback stores the result and quits the nested loop. A NULL parent means no
  transient parent.

### Menu (`menu_impl.cpp`, rewrite)

- Each `xgui::Menu` owns a root `GMenu`, a `GtkPopoverMenuBar` and a `GSimpleActionGroup`
  inserted in the window under a unique prefix; every item maps to action `<prefix>.<id>`.
- Plain item: stateless action. `"check"`: boolean stateful action. `radio_begin`/`radio_end`:
  one string-stateful action per group (target = item id). `"separator"`: starts a new section.
- `enableItem`/`disableItem` use `g_simple_action_set_enabled`; check state uses the action
  state; `setItemText` rebuilds the immutable `GMenuItem` at the same position; an internal map
  `id -> (parent GMenu, position)` supports `delItem`.
- `popupMenu(id)`: `gtk_popover_menu_new_from_model` on the submenu, parented to the window and
  pointing at the last known pointer position (tracked by a motion controller on the window).

### Toolbar (`toolbar_impl.cpp`)

- A horizontal `GtkBox` with the `toolbar` CSS class. Items are `GtkButton`, `GtkToggleButton`
  (`"check"`) or grouped `GtkToggleButton`s (radio), with icon and/or label and tooltip;
  separators are vertical `GtkSeparator`s. Public methods work on an `id -> GtkWidget*` map.

## 3. List, Tree, ComboBox, DnD

### Shared row object (`xgui_row.h/.cpp`, new)

- `XguiRow`: a minimal `GObject` holding an `xgui::Model *` (with `ref()`/`unref()`). Cells read
  strings, style and flags from the node at bind time, so no style columns are copied into a
  store and the rebuild/idle machinery of the current List disappears.
- `row_style.h` becomes `applyRowStyle(GtkWidget *label, const xgui::Style &)`: Pango
  attributes for foreground, italic, bold, underline, strikethrough; background through a CSS
  provider class.

### List (`list_impl.cpp`, rewrite)

- `GListStore<XguiRow>` -> `GtkSortListModel` -> `GtkMultiSelection` -> `GtkColumnView`.
- One `std::map<xgui::Model*, XguiRow*>`; positions via `g_list_store_find`.
- `addColumn`/`delColumn`: one `GtkColumnViewColumn` with a `GtkSignalListItemFactory` per column
  (field index and alignment stored on the factory).
- Check column: `GtkCheckButton` writing the node flag; icon shown in the first column.
- `sort(column_id, callback)`: `GtkCustomSorter` calling the user `TextPairCallback`.
- Selection: `selection-changed` with a diff against the cached selection.

### Tree (`tree_impl.cpp`, rewrite)

- Root `GListStore<XguiRow>` -> `GtkTreeListModel` (children stores created once per node and
  kept in `std::map<xgui::Model*, GListStore*>`) -> `GtkMultiSelection` -> `GtkColumnView` with a
  `GtkTreeExpander` in the first column.
- `expand`/`collapse` through `gtk_tree_list_row_set_expanded`; `onexpand`/`oncollapse` through
  `notify::expanded`.
- `sort(node, callback)` sorts the children of a node (the sort is applied to the child store
  of that node).

### ComboBox (`combobox_impl.cpp`)

- Not editable: `GtkDropDown` over a `GtkStringList`; `onchange` from `notify::selected`.
- Editable: `GtkBox` with a `GtkEntry` and a `GtkMenuButton` whose popover hosts a `GtkListView`
  on the same `GtkStringList`; choosing an item writes the entry text. `popup()`/`popdown()` drive
  the popover.

### DndList (`dndlist_impl.cpp`)

- `GtkDragSource` on the column view with the model path as `text/plain` payload, `GtkDropTarget`
  for `G_TYPE_STRING` computing the destination row; the move is applied on the `Model`. The xgui
  events exposed today stay the same.

## Testing

- `tests/unit/run.sh` uses `pkg-config gtk4` and `gtk4-broadwayd`; the deprecated API check
  becomes `-Werror=deprecated-declarations` with no exceptions once the last widget is ported.
- Existing tests keep their behavior assertions; tests that inspect GTK internals
  (`event_mask_test`, `menu_icon_test`, `scroll_test`, `row_style_test`, `add_column_test`) are
  rewritten for the new internals.
- New tests: application lifecycle (`SetApplicationId`, nested `Run`/`Quit`), event controllers
  (handlers invoked directly with fake values), menu actions, Toolbar, Tree, both ComboBox
  variants, DnD drop handler.
- Every `examples/*.json` must load (`examples_test`). `make -f Makefile.gtk python` must build.

## Commit sequence

1. Switch build and tests to GTK 4, faithful port of the whole backend (List/Tree/ComboBox on the
   deprecated GTK 4 widgets with deprecation warnings allowed only for those files).
2. `XguiRow` + List on `GtkColumnView`.
3. Tree on `GtkTreeListModel`.
4. ComboBox on `GtkDropDown` / entry + popover.
5. DndList on `GtkDragSource`/`GtkDropTarget`.
6. Deprecated API check without exceptions, docs (`README.md`, `COMPILING.md`), `CHANGES.md`.

## Out of scope

- macOS, GNUstep and Win32 backends.
- New widgets or new public features beyond `SetApplicationId`.
