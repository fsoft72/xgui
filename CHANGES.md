# Changes

## 2026-10-03

- Added `OPTIMIZE.md` with a prioritized list of bugs and optimization opportunities found during a code review of the GTK 3 backend and core library.
- Fixed format string vulnerability in `MessageDialog`: the text is now passed as an argument to `"%s"`. Added `tests/unit/run.sh` headless regression runner.
- Fixed release builds: `base.h` now includes `<iostream>` so `XGUI_ERROR` compiles without `-D_DEBUG`. Added a release compile check to `tests/unit/run.sh`.
