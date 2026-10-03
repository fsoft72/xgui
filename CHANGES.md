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
