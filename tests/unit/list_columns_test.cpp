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

/** Returns the first string of the model node shown in the first row. */
static std::string first_row_name(xguimpl::List * impl)
{
	XguiRow * row = XGUI_ROW(g_list_model_get_item(G_LIST_MODEL(impl->store), 0));
	if (!row) return "";
	std::string result = xgui_row_get_node(row)->getString(0);
	g_object_unref(row);
	return result;
}

/** Returns the number of columns of the column view. */
static guint n_columns(xguimpl::List * impl)
{
	return g_list_model_get_n_items(gtk_column_view_get_columns(impl->column_view));
}

int main()
{
	xgui::Window * win = xgui::Master::CreateWindow();
	xgui::List * list = xgui::Master::CreateList(win, 0);
	list->addColumn(0, "col_name", "Name", 100);
	list->addColumn(1, "col_value", "Value", 100);
	xguimpl::List * impl = static_cast<xguimpl::List *>(list->getImpl());

	guint columns_before = n_columns(impl);
	impl->delColumn("missing");
	if (n_columns(impl) != columns_before) {
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
