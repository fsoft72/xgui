// Regression test: list widgets must be direct children of their scrolled window (no viewport),
// otherwise GTK cannot virtualize rows and headers scroll away.
#include <cstdio>

#include "master.h"
#include "window.h"
#include "list.h"
#include "tree.h"
#include "gtk/list_impl.h"
#include "gtk/tree_impl.h"

/** Returns 0 when the scrolled window holds the list widget directly. */
static int check_direct_child(GtkWidget * scrolled, char const * label)
{
	GtkWidget * child = gtk_scrolled_window_get_child(GTK_SCROLLED_WINDOW(scrolled));
	if (!GTK_IS_COLUMN_VIEW(child)) {
		std::fprintf(stderr, "%s: child of the scrolled window is not the list widget\n", label);
		return 1;
	}
	return 0;
}

int main()
{
	xgui::Window * win = xgui::Master::CreateWindow();
	xgui::List * list = xgui::Master::CreateList(win, 0);
	xgui::Tree * tree = xgui::Master::CreateTree(win, 0);

	int failures = 0;
	failures += check_direct_child(static_cast<xguimpl::List *>(list->getImpl())->widget, "list");
	failures += check_direct_child(static_cast<xguimpl::Tree *>(tree->getImpl())->widget, "tree");
	return failures ? 1 : 0;
}
