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
		echo "FAIL: $name"; head -20 "$OUT/log"; FAILED=1
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

exit $FAILED
