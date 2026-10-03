// Regression test: operations on model nodes that have no GTK row must be ignored,
// not forwarded to GTK with a zeroed GtkTreeIter (which raises GLib criticals).
#include <cstdio>
#include <string>
#include <vector>

#include "master.h"
#include "window.h"
#include "list.h"
#include "model.h"
#include <gtk/gtk.h>

int main()
{
	xgui::Window * win = xgui::Master::CreateWindow();
	xgui::List * list = xgui::Master::CreateList(win, 0);

	// From here on any GLib critical aborts the test (the Broadway cursor warning is unrelated)
	g_log_set_always_fatal((GLogLevelFlags)(G_LOG_LEVEL_CRITICAL | G_LOG_FLAG_FATAL));

	// No columns: the list ignores the rows, so the model nodes have no GTK iterator
	xgui::Model * model = xgui::Master::CreateModel();
	std::vector<std::string> row;
	row.push_back("a");
	model->appendChild(row);
	model->appendChild(row);
	list->setModel(model);

	if (list->isSelected(model, 0)) return 1;
	list->select(model, 0, true);
	list->select(model, 1, false);
	if (list->isChecked(model, 0)) return 1;
	list->check(model, 0, true);
	list->checkAll(model, true);
	return 0;
}
