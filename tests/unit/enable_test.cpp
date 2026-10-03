// Regression test: onenable/ondisable must fire for the new state of the widget.
#include <cstdio>

#include "master.h"
#include "window.h"
#include "vbox.h"
#include "entry.h"
#include "callback.h"

static int g_enabled = 0;
static int g_disabled = 0;

/** Counts onenable events. */
static int on_enable(xgui::Widget *) { ++g_enabled; return 0; }

/** Counts ondisable events. */
static int on_disable(xgui::Widget *) { ++g_disabled; return 0; }

int main()
{
	xgui::Window * win = xgui::Master::CreateWindow();
	xgui::VBox * box = xgui::Master::CreateVBox(win, 0, false);
	xgui::Entry * entry = xgui::Master::CreateEntry(box, "text");

	xgui::CppFCallback enable_cb(on_enable);
	xgui::CppFCallback disable_cb(on_disable);
	entry->linkEvent("onenable", &enable_cb);
	entry->linkEvent("ondisable", &disable_cb);

	entry->set("enabled", "0");
	if (g_disabled != 1 || g_enabled != 0) {
		std::fprintf(stderr, "after disabling: ondisable=%d onenable=%d, expected 1 and 0\n", g_disabled, g_enabled);
		return 1;
	}

	entry->set("enabled", "1");
	if (g_disabled != 1 || g_enabled != 1) {
		std::fprintf(stderr, "after enabling: ondisable=%d onenable=%d, expected 1 and 1\n", g_disabled, g_enabled);
		return 1;
	}
	return 0;
}
