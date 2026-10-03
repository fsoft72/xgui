// Regression test: widgets must subscribe to mouse and key events as soon as they are created.
#include <cstdio>

#include "master.h"
#include "window.h"
#include "vbox.h"
#include "entry.h"
#include "button.h"
#include "gtk/widget_impl.h"

/** Returns the GTK widget behind an xgui widget. */
static GtkWidget * gtk_of(xgui::Widget * w)
{
	return static_cast<xguimpl::Widget *>(w->getImpl())->widget;
}

/** Returns 0 when the widget has every required event mask, 1 otherwise. */
static int check_masks(xgui::Widget * w, char const * label)
{
	int required = GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK
	             | GDK_ENTER_NOTIFY_MASK | GDK_LEAVE_NOTIFY_MASK | GDK_POINTER_MOTION_MASK;
	int missing = required & ~gtk_widget_get_events(gtk_of(w));
	if (missing) {
		std::fprintf(stderr, "%s: missing event mask bits 0x%x\n", label, missing);
		return 1;
	}
	return 0;
}

int main()
{
	xgui::Window * win = xgui::Master::CreateWindow();
	xgui::VBox * box = xgui::Master::CreateVBox(win, 0, false);
	xgui::Entry * entry = xgui::Master::CreateEntry(box, "text");
	xgui::Button * button = xgui::Master::CreateButton(box, "ok");

	int failures = 0;
	failures += check_masks(entry, "entry");
	failures += check_masks(button, "button");
	return failures ? 1 : 0;
}
