# Changes

## 2026-10-03

- Added `OPTIMIZE.md` with a prioritized list of bugs and optimization opportunities found during a code review of the GTK 3 backend and core library.
- Fixed format string vulnerability in `MessageDialog`: the text is now passed as an argument to `"%s"`. Added `tests/unit/run.sh` headless regression runner.
- Fixed release builds: `base.h` now includes `<iostream>` so `XGUI_ERROR` compiles without `-D_DEBUG`. Added a release compile check to `tests/unit/run.sh`.
- Initialized `xguimpl::Master::threaded` to `false` in the constructor, avoiding undefined `gdk_threads_enter/leave` calls.
- Fixed List selection and check mapping after `sort()`: `OnSelect` and `swapCheck` now resolve rows through `iter_map` instead of the GTK row index. Added headless GUI test (`tests/unit/list_sort_test.cpp`, runs on a Broadway display).
- Note: `List::OnSelect` reads the `onselect` event but `List::linkEvent` only accepts `onchange`, so the selection callback is never invoked for lists (not fixed yet).
- Fixed `LoadJson` crashing on malformed input: the `type` field is validated as a string and the whole parse is wrapped in `try/catch`. Added `tests/unit/json_loader_test.cpp`.
- Fixed the `focused` widget property: the setter calls `gtk_widget_grab_focus()` (it passed a pointer as `gboolean` to `has-focus`), the getter uses `gtk_widget_is_focus()`. Added `tests/unit/focus_test.cpp`.
- Fixed event masks: the `Widget` constructor now uses `gtk_widget_add_events()` (the GdkWindow does not exist before realization) and includes `GDK_POINTER_MOTION_MASK`. Added `tests/unit/event_mask_test.cpp`.
- Fixed `int_find_column` in the GTK list: it now returns `{NULL, -1}` when the column is missing (so `delColumn`/`sort` no longer hit the last column) and matches the xgui column id as well as the title. `sort()` now accounts for the checkbox column when computing the data column. Added `tests/unit/list_columns_test.cpp`.
- Replaced `iter_map[...]` / `tree_map[...]` lookups with `find()` in the GTK List and Tree: unknown model nodes are now ignored instead of passing a zeroed `GtkTreeIter` to GTK. Added `tests/unit/iter_map_test.cpp` (List only; Tree uses the same pattern).
- Added a bounds check at the top of `Model::removeChild` (out-of-range positions were read before validation). Added `tests/unit/model_test.cpp`; the test runner now builds `libxgui` with `-D_GLIBCXX_ASSERTIONS`.
- Fixed null parent dereference in `openFileDialog`, `saveFileDialog` and `messageDialog`: a missing parent window now means no transient parent. Added `tests/unit/dialog_test.cpp`.
- `ParseJsonValue` now stops when `create()` fails (no orphan child widgets, no `finalize(NULL)`), and `FinalizeObject` tolerates an unknown class. Added `tests/unit/json_failed_create_test.cpp`.
- `Master::Quit()` now resets the singleton pointer after deleting it (it is reset after the delete because the destructor still calls `Instance()`). Added `tests/unit/quit_test.cpp`.
- Extracted the triplicated row-style code of `List::insertItem`, `List::refresh` and `Tree::insertSubTree` into `applyRowStyle()` (`xgui/gtk/row_style.h`). Added `tests/unit/row_style_test.cpp`.
- List and Tree colors now use `GdkRGBA` (`gdk_rgba_parse` with a validity check, `foreground-rgba`/`background-rgba` renderer properties); an invalid color string is reported and leaves the cell empty. Extended `tests/unit/row_style_test.cpp`.
- Replaced the deprecated `gtk_scrolled_window_add_with_viewport` with `gtk_container_add` in List, Tree and ImageView, so tree views are no longer wrapped in a viewport (lazy row rendering, fixed headers). Added `tests/unit/scroll_test.cpp`.
- `List::addColumn` no longer rebuilds and repopulates the whole store on every call: the store is flagged dirty and rebuilt once (on first use or from an idle handler). Rebuilding now also clears `iter_map` so stale iterators of the replaced store are never used (this surfaced as GTK criticals through `OnSelect`). Added `tests/unit/add_column_test.cpp`; the test runner log now shows the tail of a failing run.
- `List::OnSelect` now uses `gtk_tree_selection_get_selected_rows()` and diffs against the cached selection instead of scanning every row. Added a reverse row map (`row_map`) so a store row resolves to its model node in O(log n). Added `tests/unit/selection_test.cpp`.
- Fixed `onenable`/`ondisable`: they now use `notify::sensitive` and the current sensitivity (the old `state-changed` handler reported the previous state, inverted), and the shared handler is connected only once when both events are linked. Added `tests/unit/enable_test.cpp`.
- JSON numbers are now passed to properties with `dump()` instead of `std::to_string(double)`: integers stay exact (`200`, not `200.000000`) and the output no longer depends on the locale. Added `tests/unit/json_numbers_test.cpp`.
- Removed the dead XML pipeline: `LoadXml`, `SaveXml`, `GenerateDTD` and `ParseXmlNode` (master.h, dmaster.cpp, pywrap, docs) and the matching "Save Json"/"Generate Dtd" menu entries of the `plan` tool. `dump()` is kept as a debug aid and now escapes attribute values. Added `tests/unit/dump_test.cpp`.
- Replaced GTK 2 era APIs in the GTK backend: `gtk_scale_new_with_range`, icon menu items built from a `GtkBox` (no `GtkImageMenuItem`; label lookup is now tolerant of that layout), `gtk_menu_popup_at_pointer`, and a `GRecMutex` plus `gdk_display_flush` for `GuiLock()`/`GuiUnlock()` instead of the deprecated GDK thread lock. Added `tests/unit/menu_icon_test.cpp` and a deprecated-API check in the runner.
- Build flags: `Makefile.gtk` no longer uses `-fpermissive` and enables `-Wextra` (with `-Wno-unused-parameter`, because virtual interface stubs ignore parameters on purpose). Fixed the warnings it revealed: `GValue` initializers now use `G_VALUE_INIT`, `xguimpl::Style` has an explicit defaulted copy constructor, and the unused `trim()` helper in `dmaster.cpp` was removed. The runner checks that the sources build with `-Wall -Wextra -Werror`.
- Extracted `Widget::decodeButtonEvent()` (button name and modifier mask) shared by `OnMouseDown`, `OnMouseUp` and `OnGdkEvent`. The time-based de-duplication in `OnGdkEvent` was left global on purpose: it makes only the innermost widget handle an event that GTK propagates to its parents. Added `tests/unit/button_decode_test.cpp`.
- Class lookups are now indexed: `Master` keeps `std::map`s by class name and by tag (first registration wins) instead of scanning a pointer-ordered `std::set`; `availableClasses()` returns sorted names. Added `tests/unit/class_lookup_test.cpp` and a smoke test that loads every `examples/*.json` (`tests/unit/examples_test.cpp`).
- Added core unit tests without GUI dependency on a display: `semantic_cast` (`tests/unit/semantic_cast_test.cpp`) and `Model`/`TreeNode` structure handling (`tests/unit/model_tree_test.cpp`), and a `make -f Makefile.gtk unit` target that runs `tests/unit/run.sh` (GUI tests use a headless Broadway display, `broadwayd` is required).
- All items of `OPTIMIZE.md` are now marked as done.

## 2026-10-03 - GTK 4 port (branch `gtk4`)

- Design spec `docs/superpowers/specs/2026-10-03-gtk4-port-design.md` and plan `docs/superpowers/plans/2026-10-03-gtk4-port.md`.
- The GTK backend now builds against GTK 4 (`pkg-config gtk4`); GTK 3 is no longer supported. The tests run on `gtk4-broadwayd`.
- `Master` drives a `GtkApplication`: it is registered when the Master is created (this initializes GTK), windows are attached to it, the outermost `Run()` is `g_application_run()` and nested `Run()` levels use their own `GMainLoop`. The application lives until `Quit()`, as before. New `Master::SetApplicationId(id, unique = false)` (also in the Python bindings); the default id is `org.xgui.<program name>` with `G_APPLICATION_NON_UNIQUE`.
- Mouse, key, focus and pointer crossing events use GTK 4 event controllers attached on demand by `linkEvent()`; click, double click and key events are still reported only for the innermost widget. `onresize` compares the widget size on every frame. `decodeButtonEvent()` takes a button number and a `GdkModifierType`.
- Not supported any more (GTK 4 has no API for them): the window `position` property (`"center"` is accepted silently, coordinates print an error), the `onmove` event, window icons from image files, and the message type icon of `MessageDialog`.
- Modal windows are plain `GtkWindow`s run in a nested loop (`GtkDialog` is deprecated); closing one ends `modalRun()` with -1 and keeps the window. The status bar is a label (`GtkStatusbar` is deprecated).
- `OpenFileDialog`/`SaveFileDialog` use `GtkFileDialog` and `MessageDialog` uses `GtkAlertDialog`; their synchronous signatures are unchanged (a nested loop waits for the answer).
- Menus are a `GtkPopoverMenuBar` on a `GMenu` model with one action per item (check items: boolean state, radio groups: one string state action); separators start a new section; top level items are always submenus. GTK 4 usually does not draw menu item icons.
- Fixed: the menu reported item selection to the `onselect` event, which `linkEvent` rejected, while applications link `onclick`: menu callbacks were never called. Both names are accepted now.
- The toolbar is a box of flat buttons (`GtkToolbar` is gone), keeping the item API, styles, check and radio buttons.
- Images are `GdkTexture`s shown by `GtkPicture` at their pixel size; `ImageView` stretches the picture to the view size or scrolls it at natural size.
- `DndList` uses `GtkDragSource`/`GtkDropTarget` (the payload is the first string of the selected row; a move removes it from the source).
- Entry, Spin and editable Combobox validate text through the `insert-text`/`delete-text` signals of the GTK 4 delegate `GtkText`. Fixed the text passed to `onchange` on insertion in Entry and Combobox (the old text was inserted into itself).
- The `focused` property also reports focus held by an inner child (GTK 4 composite widgets like `GtkEntry`).
- Fixed duplicated include guards of `menu_impl.h`, `toolbar_impl.h`, `imageview_impl.h` and `vbox_impl.h`.
- List, Tree and Combobox still use the GTK 4 deprecated `GtkTreeView`/`GtkComboBox` widgets until their rewrite; the test runner allows deprecation warnings only in those files.
- Tests: `event_mask_test` replaced by `event_controller_test`; new `app_lifecycle_test`, `menu_actions_test`, `toolbar_test`, `dnd_test`; `dialog_test` answers the GTK 4 dialogs by pressing their buttons.
