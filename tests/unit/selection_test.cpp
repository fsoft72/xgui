// Characterization test: the selection cache of a List follows select/unselect/selectAll,
// also when the rows are sorted differently from the model.
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

/** Descending string comparison, so the GTK order is the reverse of the model order. */
static int compare_desc(xgui::Widget *, std::string const & a, std::string const & b)
{
	return std::strcmp(b.c_str(), a.c_str());
}

/** Returns a string such as "101" describing which of the first 3 model rows are cached as selected. */
static std::string cached_state(xguimpl::View * view, xgui::Model * model)
{
	std::string state;
	for (int i = 0; i < 3; ++i)
		state += view->isSelectionCached(model->getChild(i)) ? '1' : '0';
	return state;
}

/** Compares the cached selection with the expected one and reports a mismatch. */
static int expect_state(xguimpl::View * view, xgui::Model * model, char const * expected, char const * label)
{
	std::string state = cached_state(view, model);
	if (state != expected) {
		std::fprintf(stderr, "%s: cached selection %s, expected %s\n", label, state.c_str(), expected);
		return 1;
	}
	return 0;
}

int main()
{
	xgui::Window * win = xgui::Master::CreateWindow();
	xgui::List * list = xgui::Master::CreateList(win, 0);
	list->addColumn(0, "c", "C", 100);

	xgui::Model * model = xgui::Master::CreateModel();
	char const * names[] = { "a", "b", "c" };
	for (int i = 0; i < 3; ++i) {
		std::vector<std::string> row;
		row.push_back(names[i]);
		model->appendChild(row);
	}
	list->setModel(model);
	xguimpl::View * view = static_cast<xguimpl::View *>(list->getImpl());

	xgui::CppFTextPairCallback compare_cb(compare_desc);
	list->sort("c", &compare_cb);

	int failures = 0;
	list->select(model, 0, true);
	list->select(model, 2, true);
	failures += expect_state(view, model, "101", "select 0 and 2");
	list->select(model, 0, false);
	failures += expect_state(view, model, "001", "unselect 0");
	list->selectAll(model, true);
	failures += expect_state(view, model, "111", "select all");
	list->selectAll(model, false);
	failures += expect_state(view, model, "000", "unselect all");
	return failures ? 1 : 0;
}
