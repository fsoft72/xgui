// Regression test: adding several columns must not rebuild and repopulate the rows each time.
#include <cstdio>
#include <string>
#include <vector>

#include "master.h"
#include "window.h"
#include "list.h"
#include "model.h"
#include "gtk/list_impl.h"

int main()
{
	xgui::Window * win = xgui::Master::CreateWindow();
	xgui::List * list = xgui::Master::CreateList(win, 0);
	xguimpl::List * impl = static_cast<xguimpl::List *>(list->getImpl());

	list->addColumn(0, "c0", "C0", 100);
	xgui::Model * model = xgui::Master::CreateModel();
	std::vector<std::string> row;
	row.push_back("x");
	for (int i = 0; i < 100; ++i) model->appendChild(row);
	list->setModel(model);

	// Columns only add cells: the rows are not rebuilt
	GListStore * store = impl->store;
	XguiRow * first = XGUI_ROW(g_list_model_get_item(G_LIST_MODEL(store), 0));
	for (int i = 1; i < 5; ++i) {
		char id[16];
		std::snprintf(id, sizeof(id), "c%d", i);
		list->addColumn(i, id, id, 100);
	}
	XguiRow * first_after = XGUI_ROW(g_list_model_get_item(G_LIST_MODEL(store), 0));
	bool same_rows = (impl->store == store) && (first == first_after);
	g_object_unref(first);
	g_object_unref(first_after);
	if (!same_rows) {
		std::fprintf(stderr, "adding columns replaced the rows\n");
		return 1;
	}

	// A row added after the columns must show up exactly once
	model->appendChild(row);
	guint n_columns = g_list_model_get_n_items(gtk_column_view_get_columns(impl->column_view));
	if (n_columns != 5) {
		std::fprintf(stderr, "view has %u columns, expected 5\n", n_columns);
		return 1;
	}
	guint n_rows = g_list_model_get_n_items(G_LIST_MODEL(store));
	if (n_rows != 101) {
		std::fprintf(stderr, "store has %u rows, expected 101\n", n_rows);
		return 1;
	}
	return 0;
}
