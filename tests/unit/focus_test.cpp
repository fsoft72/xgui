// Regression test: setting the "focused" property must grab the keyboard focus.
#include <cstdio>

#include "master.h"
#include "window.h"
#include "vbox.h"
#include "entry.h"

/** Returns true when the "focused" property of the widget is set. */
static bool is_focused(xgui::Widget * w)
{
	std::string focused;
	w->get("focused", focused);
	return focused == "1";
}

int main()
{
	xgui::Window * win = xgui::Master::CreateWindow();
	xgui::VBox * box = xgui::Master::CreateVBox(win, 0, false);
	xgui::Entry * first = xgui::Master::CreateEntry(box, "first");
	xgui::Entry * second = xgui::Master::CreateEntry(box, "second");

	first->set("focused", "1");
	if (!is_focused(first)) {
		std::fprintf(stderr, "first entry did not get the focus\n");
		return 1;
	}

	second->set("focused", "1");
	if (!is_focused(second) || is_focused(first)) {
		std::fprintf(stderr, "focus did not move to the second entry\n");
		return 1;
	}
	return 0;
}
