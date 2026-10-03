// Regression test: setting the "focused" property must grab the keyboard focus.
#include <cstdio>

#include "master.h"
#include "window.h"
#include "vbox.h"
#include "entry.h"
#include "gtk/widget_impl.h"

/** Returns the GTK widget behind an xgui widget. */
static GtkWidget * gtk_of(xgui::Widget * w)
{
	return static_cast<xguimpl::Widget *>(w->getImpl())->widget;
}

int main()
{
	xgui::Window * win = xgui::Master::CreateWindow();
	xgui::VBox * box = xgui::Master::CreateVBox(win, 0, false);
	xgui::Entry * first = xgui::Master::CreateEntry(box, "first");
	xgui::Entry * second = xgui::Master::CreateEntry(box, "second");

	first->set("focused", "1");
	if (!gtk_widget_is_focus(gtk_of(first))) {
		std::fprintf(stderr, "first entry did not get the focus\n");
		return 1;
	}

	second->set("focused", "1");
	if (!gtk_widget_is_focus(gtk_of(second)) || gtk_widget_is_focus(gtk_of(first))) {
		std::fprintf(stderr, "focus did not move to the second entry\n");
		return 1;
	}
	return 0;
}
