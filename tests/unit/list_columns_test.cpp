// Regression test: column lookup by name must fail cleanly when the column is missing,
// and sorting must use the right data column when the checkbox column is enabled.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include "master.h"
#include "window.h"
#include "list.h"
#include "model.h"
#include "callback.h"
#include "gtk/list_impl.h"

/** Ascending string comparison for the sort callback. */
static int compare_text(xgui::Widget *, std::string const & a, std::string const & b)
{
	return std::strcmp(a.c_str(), b.c_str());
}

/** Appends a row to the model. */
static void add_row(xgui::Model * model, char const * name, char const * value)
{
	std::vector<std::string> row;
	row.push_back(name);
	row.push_back(value);
	model->appendChild(row);
}

/** Returns the text of the first data column of the first GTK row. */
static std::string first_row_name(xguimpl::List * impl)
{
	GtkTreeModel * store = gtk_tree_view_get_model(impl->list_view);
	GtkTreeIter iter;
	gchar * text = 0;
	gtk_tree_model_get_iter_first(store, &iter);
	gtk_tree_model_get(store, &iter, impl->first_fields_column, &text, -1);
	std::string result = text ? text : "";
	g_free(text);
	return result;
}

int main()
{
	xgui::Window * win = xgui::Master::CreateWindow();
	xgui::List * list = xgui::Master::CreateList(win, 0);
	list->addColumn(0, "col_name", "Name", 100);
	list->addColumn(1, "col_value", "Value", 100);
	xguimpl::List * impl = static_cast<xguimpl::List *>(list->getImpl());

	int columns_before = gtk_tree_view_get_n_columns(impl->list_view);
	impl->delColumn("missing");
	if (gtk_tree_view_get_n_columns(impl->list_view) != columns_before) {
		std::fprintf(stderr, "delColumn(\"missing\") removed an unrelated column\n");
		return 1;
	}

	list->set("has-checkboxes", "1");
	xgui::Model * model = xgui::Master::CreateModel();
	add_row(model, "c", "1");
	add_row(model, "a", "2");
	add_row(model, "b", "3");
	list->setModel(model);

	xgui::CppFTextPairCallback compare_cb(compare_text);
	list->sort("col_name", &compare_cb);
	if (first_row_name(impl) != "a") {
		std::fprintf(stderr, "sort by col_name with checkboxes put '%s' first, expected 'a'\n", first_row_name(impl).c_str());
		return 1;
	}
	return 0;
}
