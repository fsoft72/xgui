// Regression test: after List::sort() the GTK row order differs from the model order,
// but selection events must still report the model node that was actually selected.
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

/** Appends a one-column-value row to the model. */
static void add_row(xgui::Model * model, char const * name, char const * value)
{
	std::vector<std::string> row;
	row.push_back(name);
	row.push_back(value);
	model->appendChild(row);
}

int main()
{
	xgui::Window * win = xgui::Master::CreateWindow();
	xgui::List * list = xgui::Master::CreateList(win, 0);
	list->addColumn(0, "col_name", "Name", 100);
	list->addColumn(1, "col_value", "Value", 100);

	xgui::Model * model = xgui::Master::CreateModel();
	add_row(model, "c", "1");
	add_row(model, "a", "2");
	add_row(model, "b", "3");
	list->setModel(model);

	xgui::CppFTextPairCallback compare_cb(compare_text);
	list->sort("col_name", &compare_cb);

	// Model child 1 is "a", which is now the first GTK row.
	list->select(model, 1, true);

	xguimpl::View * view = static_cast<xguimpl::View *>(list->getImpl());
	bool a_cached = view->isSelectionCached(model->getChild(1));
	bool c_cached = view->isSelectionCached(model->getChild(0));
	if (!a_cached || c_cached) {
		std::fprintf(stderr, "selection cache wrong: 'a' cached=%d, 'c' cached=%d\n", a_cached, c_cached);
		return 1;
	}
	return 0;
}
