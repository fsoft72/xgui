// Regression test: adding several columns must not rebuild and repopulate the whole store each time.
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

	// Count how many times the GTK store is replaced while columns are added
	int replacements = 0;
	g_object_set_data(G_OBJECT(gtk_tree_view_get_model(impl->list_view)), "marker", (gpointer)1);
	for (int i = 1; i < 5; ++i) {
		char id[16];
		std::snprintf(id, sizeof(id), "c%d", i);
		list->addColumn(i, id, id, 100);
		GtkTreeModel * store = gtk_tree_view_get_model(impl->list_view);
		if (!g_object_get_data(G_OBJECT(store), "marker")) ++replacements;
		g_object_set_data(G_OBJECT(store), "marker", (gpointer)1);
	}
	if (replacements > 1) {
		std::fprintf(stderr, "the store was rebuilt %d times while adding 4 columns\n", replacements);
		return 1;
	}

	// A row added while the rebuild is pending must show up exactly once
	model->appendChild(row);

	// The store must be consistent once it is used
	list->refresh();
	GtkTreeModel * store = gtk_tree_view_get_model(impl->list_view);
	if (gtk_tree_model_get_n_columns(store) != impl->first_fields_column + 5) {
		std::fprintf(stderr, "store has %d columns, expected %d\n", gtk_tree_model_get_n_columns(store), impl->first_fields_column + 5);
		return 1;
	}
	if (gtk_tree_model_iter_n_children(store, NULL) != 101) {
		std::fprintf(stderr, "store has %d rows, expected 101\n", gtk_tree_model_iter_n_children(store, NULL));
		return 1;
	}
	return 0;
}
