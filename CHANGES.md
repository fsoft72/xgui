# Changes

## 2026-10-03

- Added `OPTIMIZE.md` with a prioritized list of bugs and optimization opportunities found during a code review of the GTK 3 backend and core library.
- Fixed format string vulnerability in `MessageDialog`: the text is now passed as an argument to `"%s"`. Added `tests/unit/run.sh` headless regression runner.
- Fixed release builds: `base.h` now includes `<iostream>` so `XGUI_ERROR` compiles without `-D_DEBUG`. Added a release compile check to `tests/unit/run.sh`.
- Initialized `xguimpl::Master::threaded` to `false` in the constructor, avoiding undefined `gdk_threads_enter/leave` calls.
- Fixed List selection and check mapping after `sort()`: `OnSelect` and `swapCheck` now resolve rows through `iter_map` instead of the GTK row index. Added headless GUI test (`tests/unit/list_sort_test.cpp`, runs on a Broadway display).
- Note: `List::OnSelect` reads the `onselect` event but `List::linkEvent` only accepts `onchange`, so the selection callback is never invoked for lists (not fixed yet).
