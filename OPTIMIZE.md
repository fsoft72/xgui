# Optimization TODO

> Generated on 2026-10-03. Items sorted by importance.
>
> GTK backend: **GTK 3** (`pkg-config gtk+-3.0`, installed 3.24.50). Many calls are GTK 2 leftovers deprecated in GTK 3 and removed in GTK 4.

## Critical

- [ ] **Format string vulnerability in MessageDialog** - `gtk_message_dialog_new(..., text.c_str(), 0)` uses the user-supplied text as a printf format string, so any `%s`/`%n` in a message reads or writes arbitrary memory; pass `"%s", text.c_str()` instead.
  - File(s): `xgui/gtk/master_impl.cpp`
- [ ] **Release builds do not compile** - `XGUI_ERROR` expands to `std::cerr` but `<iostream>` is only included by `debug.h` under `_DEBUG`, so building without `-D_DEBUG` fails with 220 "cerr is not a member of std" errors; include `<iostream>` in `base.h`.
  - File(s): `xgui/base.h`, `xgui/debug.h`
- [ ] **Uninitialized `threaded` flag** - `xguimpl::Master::threaded` is never set in the constructor, so `GuiLock()`/`GuiUnlock()` may randomly call `gdk_threads_enter/leave` without `InitThreads()` (undefined behaviour, possible deadlock); initialize it to `false`.
  - File(s): `xgui/gtk/master_impl.cpp`, `xgui/gtk/master_impl.h`
- [ ] **List selection/check mapping breaks after `sort()`** - `sort()` reorders the `GtkListStore` rows but the `xgui::Model` keeps its original order, while `OnSelect` and `swapCheck` map GTK row index `N` to `model->getChild(N)`; after a sort the wrong items are reported as selected/checked. Resolve rows through `iter_map` (or reorder the model) instead of by index.
  - File(s): `xgui/gtk/list_impl.cpp`
- [ ] **Uncaught JSON type errors crash `LoadJson`** - only the parse step is inside `try`; `std::string node_name = j["type"]` (non-string `type`) or a malformed child throws `nlohmann::json::type_error` out of `ParseJsonValue` and terminates the application; wrap the whole parse and validate types.
  - File(s): `xgui/dmaster.cpp`

## High

- [ ] **`"focused"` property passes a pointer instead of a value** - `g_object_set(widget, "has-focus", &val, NULL)` sends the address of `val` as the gboolean; in GTK 3 `has-focus` is also not the way to grab focus. Use `gtk_widget_grab_focus()` when the value is true.
  - File(s): `xgui/gtk/widget_impl.cpp`
- [ ] **Event masks are never applied** - the `Widget` constructor adds button/key/crossing masks only if `gtk_widget_get_window()` is non-null, which is never the case before realization, and `GDK_POINTER_MOTION_MASK` is missing entirely; `onmousemove`, `onkey*` and `onmouseclick` silently do not fire on many widgets. Use `gtk_widget_add_events()`.
  - File(s): `xgui/gtk/widget_impl.cpp`
- [ ] **`int_find_column` returns the wrong column when not found** - if no title matches, it returns the last column pointer with position `0`, so `delColumn("missing")` removes the last column and `sort()` sorts on a random field; return `{NULL, -1}` and check it. Also the sort column index subtracts 1 for icons but not for the checkbox column.
  - File(s): `xgui/gtk/list_impl.cpp`
- [ ] **`iter_map[...]` / `tree_map[...]` silently insert empty iterators** - `isSelected`, `select`, `isChecked`, `check`, `checkAll`, `removeItem` and `Tree::insertSubTree` use `operator[]`, so an unknown model node inserts a zeroed `GtkTreeIter` that is passed to GTK (criticals or crashes); use `find()` and bail out.
  - File(s): `xgui/gtk/list_impl.cpp`, `xgui/gtk/tree_impl.cpp`
- [ ] **`Model::removeChild` has no bounds check** - `_children[pos]` is read before `TreeNode::removeChild` validates `pos`, so an out-of-range index is undefined behaviour; add the guard at the top.
  - File(s): `xgui/model.cpp`
- [ ] **Null parent dereference in dialogs** - `openFileDialog`, `saveFileDialog` and `messageDialog` call `parent->getImpl()` unconditionally, so calling them without a parent window (common from Python) segfaults; pass `NULL` as transient parent when `parent == 0`.
  - File(s): `xgui/gtk/master_impl.cpp`
- [ ] **JSON loader keeps going after a failed `create()`** - when `class_builder->create()` returns `0`, children are still parsed with a null parent (creating orphan top-level widgets) and `finalize(result)` is called with a null object; also `FinalizeObject` dereferences a possibly null `cinfo`.
  - File(s): `xgui/dmaster.cpp`
- [ ] **`Master::Quit()` leaves a dangling singleton** - it deletes `Instance()` but never resets `instance_` to `0`, so any later call to a `Master::*` static (e.g. from a destructor or Python finalizer) uses freed memory.
  - File(s): `xgui/master.cpp`

## Medium

- [ ] **Triplicated row-style code** - the block that applies icon, fg/bg colour, bold, italic, underline and strike to a row is copy-pasted in `List::insertItem`, `List::refresh` and `Tree::insertSubTree`; extract a single `applyRowStyle(store, iter, style)` helper shared by List and Tree.
  - File(s): `xgui/gtk/list_impl.cpp`, `xgui/gtk/tree_impl.cpp`
- [ ] **`gdk_color_parse` result ignored** - an invalid colour string leaves `GdkColor` uninitialized and stores garbage in the model; check the return value. Migrate to `GdkRGBA` / `gdk_rgba_parse` and the `foreground-rgba`/`background-rgba` renderer properties at the same time.
  - File(s): `xgui/gtk/list_impl.cpp`, `xgui/gtk/tree_impl.cpp`
- [ ] **TreeView wrapped in a viewport** - `gtk_scrolled_window_add_with_viewport` on a `GtkTreeView` (which is already `GtkScrollable`) disables lazy row rendering and keeps headers from staying fixed, a real slowdown on large lists; use `gtk_container_add`.
  - File(s): `xgui/gtk/list_impl.cpp`, `xgui/gtk/tree_impl.cpp`, `xgui/gtk/imageview_impl.cpp`
- [ ] **O(rows x columns) rebuild on every `addColumn`** - each `addColumn` recreates the whole `GtkListStore` and calls `refresh()`, so a list with N columns is fully repopulated N times; batch column creation and rebuild once (e.g. on finalize).
  - File(s): `xgui/gtk/list_impl.cpp`
- [ ] **`OnSelect` scans every row on each selection change** - it allocates a `GtkTreePath` per row and walks the whole model for every `changed` signal; use `gtk_tree_selection_get_selected_rows()` and diff against the cached set.
  - File(s): `xgui/gtk/list_impl.cpp`
- [ ] **`onenable`/`ondisable` logic is inverted and uses a deprecated signal** - `state-changed` passes the *previous* state, so `ondisable` fires when a widget becomes enabled; switch to `notify::sensitive` and read `gtk_widget_get_sensitive()`.
  - File(s): `xgui/gtk/widget_impl.cpp`
- [ ] **JSON numbers become `"200.000000"`** - `std::to_string(get<double>())` turns integers into float strings; it works by accident for `int` parsing but breaks string properties and large integers. Use `dump()` for integers and keep doubles for real numbers.
  - File(s): `xgui/dmaster.cpp`
- [ ] **Remove dead XML pipeline or make it consistent** - `LoadXml`/`ParseXmlNode` only print an error, yet `SaveXml`, `dump()` and `GenerateDTD()` (with a `"ciao"` placeholder header) still produce XML that cannot be loaded back, and attribute values are not escaped; either drop them or add `SaveJson`.
  - File(s): `xgui/dmaster.cpp`, `xgui/object.cpp`, `xgui/container.cpp`, `xgui/master.h`

## Low / Nice to have

- [ ] **Replace GTK 2 era APIs** - `gtk_hscale/vscale_new_with_range`, `GtkImageMenuItem`, `gtk_menu_popup`, `gdk_threads_enter/leave`, `gdk_flush`, `GdkColor` are deprecated in GTK 3 and removed in GTK 4; replacing them is the prerequisite for any GTK 4 port.
  - File(s): `xgui/gtk/slider_impl.cpp`, `xgui/gtk/menu_impl.cpp`, `xgui/gtk/master_impl.cpp`
- [ ] **Drop `-fpermissive` and enable `-Wextra`** - `-fpermissive` downgrades real errors to warnings; with `-Wextra` the build also reveals a narrowing conversion in `List::sort` and a deprecated implicit copy in `style.cpp`.
  - File(s): `Makefile.gtk`, `xgui/gtk/list_impl.cpp`, `xgui/style.cpp`
- [ ] **Deduplicate mouse event decoding** - button name and modifier-mask computation is copy-pasted in `OnMouseDown`, `OnMouseUp` and `OnGdkEvent`; extract a `decodeButtonEvent()` helper. The `static` time-based de-duplication in `OnGdkEvent` is also global across all widgets and fragile.
  - File(s): `xgui/gtk/widget_impl.cpp`
- [ ] **Linear class lookups** - `FindClass`/`FindClassByTag` walk a `std::set<ClassInfo*>` ordered by pointer for every JSON node; index them in `std::map<std::string, ClassInfo*>` by name and tag.
  - File(s): `xgui/dmaster.cpp`, `xgui/master.h`
- [ ] **No automated tests for the core** - `Model`/`TreeNode`, `semantic_cast` and the JSON loader are pure C++ and could be unit-tested without a display, but only manual GUI examples exist.
  - File(s): `tests/`, `xgui/model.cpp`, `xgui/base.h`, `xgui/dmaster.cpp`
