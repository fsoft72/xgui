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
