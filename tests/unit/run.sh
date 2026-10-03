#!/bin/bash
# Headless regression checks for the xgui core. Run from anywhere: tests/unit/run.sh
# Each check prints PASS/FAIL; the script exits non-zero if any check fails.

ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
cd "$ROOT" || exit 1

CPP=${CPP:-g++}
GTK_FLAGS=$(pkg-config gtk+-3.0 --cflags)
INCLUDES="-I$ROOT/xgui -I$ROOT/xgui/gtk $GTK_FLAGS -Wno-deprecated-declarations"
OUT=$(mktemp -d)
trap 'rm -rf "$OUT"' EXIT
FAILED=0

# check NAME CMD... : run CMD and report PASS/FAIL
check() {
	local name="$1"; shift
	if "$@" > "$OUT/log" 2>&1; then
		echo "PASS: $name"
	else
		echo "FAIL: $name"; grep -v "Appending Child" "$OUT/log" | tail -20; FAILED=1
	fi
}

# Item: format string vulnerability in MessageDialog
check "master_impl has no non-literal printf format" \
	$CPP -fsyntax-only -Wformat -Werror=format-nonliteral -Werror=format-security $INCLUDES xgui/gtk/master_impl.cpp

# Item: release builds (no -D_DEBUG) must compile
check "core and GTK sources compile without -D_DEBUG" \
	bash -c "for f in xgui/*.cpp xgui/gtk/*.cpp; do $CPP -fsyntax-only -fpermissive -w $INCLUDES \$f || exit 1; done"

# Item: Master::threaded must be initialized in the constructor
check "xguimpl::Master initializes threaded" \
	grep -Pzq "Master\(xgui::Master \* master\)\s*:\s*threaded\(false\)" xgui/gtk/master_impl.cpp

# GUI checks run on a headless GTK Broadway display (needs broadwayd)
BROADWAY_PORT=:97
_gui_check() {
	local name="$1" src="$2"; shift 2
	if [ -z "$LIB_BUILT" ]; then
		make -B -j8 -f Makefile.gtk lib CPPFLAGS="-g -w -fPIC -D_DEBUG -O0 -fpermissive -D_GLIBCXX_ASSERTIONS" > "$OUT/lib.log" 2>&1 || { echo "FAIL: libxgui build"; tail -20 "$OUT/lib.log"; FAILED=1; return; }
		broadwayd $BROADWAY_PORT > /dev/null 2>&1 &
		BROADWAYD_PID=$!
		trap 'kill $BROADWAYD_PID 2>/dev/null; rm -rf "$OUT"' EXIT
		sleep 1
		LIB_BUILT=1
	fi
	$CPP -fpermissive -w -D_DEBUG $INCLUDES "$src" "$@" -L"$ROOT" -lxgui -Wl,-rpath,"$ROOT" $(pkg-config gtk+-3.0 --libs) -o "$OUT/gui_test" \
		&& check "$name" env GDK_BACKEND=broadway BROADWAY_DISPLAY=$BROADWAY_PORT "$OUT/gui_test" "$OUT"
}

# Item: selection must map to the right model node after sort()
_gui_check "list selection maps to model after sort" tests/unit/list_sort_test.cpp

# Item: malformed JSON must not crash LoadJson
_gui_check "LoadJson survives malformed JSON" tests/unit/json_loader_test.cpp

# Item: "focused" property must grab focus
_gui_check "focused property grabs focus" tests/unit/focus_test.cpp

# Item: event masks must be applied at construction time
_gui_check "widgets subscribe to mouse/key events" tests/unit/event_mask_test.cpp

# Item: column lookup must not fall back to the last column
_gui_check "list column lookup and sort column index" tests/unit/list_columns_test.cpp

# Item: unknown model nodes must not insert empty iterators into iter_map
_gui_check "list ignores nodes without a GTK row" tests/unit/iter_map_test.cpp

# Item: Model::removeChild bounds check
_gui_check "Model::removeChild ignores bad positions" tests/unit/model_test.cpp

# Item: dialogs without a parent window must not crash
_gui_check "dialogs work without a parent window" tests/unit/dialog_test.cpp

# Item: JSON loader must stop at a node whose create() failed
_gui_check "LoadJson skips children of failed nodes" tests/unit/json_failed_create_test.cpp

# Item: Master::Quit must reset the singleton
_gui_check "Master::Quit does not leave a dangling singleton" tests/unit/quit_test.cpp

# Item: row style handling shared by List and Tree
_gui_check "list and tree rows carry their style" tests/unit/row_style_test.cpp

# Item: tree views must not be wrapped in a viewport
_gui_check "list and tree views are not wrapped in a viewport" tests/unit/scroll_test.cpp

# Item: addColumn must not rebuild the store every time
_gui_check "addColumn batches store rebuilds" tests/unit/add_column_test.cpp

# Item: OnSelect must keep the selection cache correct
_gui_check "list selection cache follows selection changes" tests/unit/selection_test.cpp

# Item: onenable/ondisable must not be inverted
_gui_check "onenable and ondisable fire for the new state" tests/unit/enable_test.cpp

exit $FAILED
