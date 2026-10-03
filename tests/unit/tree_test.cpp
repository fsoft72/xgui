// Regression test: Tree on GtkTreeListModel (nested rows, expansion, late children, checks, sort, selection).
#include <cstdio>
#include <cstring>
#include <string>

#include "master.h"
#include "window.h"
#include "tree.h"
#include "model.h"
#include "callback.h"
#include "gtk/tree_impl.h"

static std::string events_log;

static int on_expand(xgui::Widget *, std::string const &path, int state) { events_log += "exp:" + path + "=" + std::to_string(state) + ";"; return 1; }
static int on_select(xgui::Widget *, std::string const &path, int state) { events_log += "sel:" + path + "=" + std::to_string(state) + ";"; return 1; }
static int compare_desc(xgui::Widget *, std::string const &a, std::string const &b) { return std::strcmp(b.c_str(), a.c_str()); }

/** Compares a condition and prints the step on failure; returns 1 on failure. */
static int expect(bool ok, char const * step)
{
	if (ok) return 0;
	std::fprintf(stderr, "%s failed (log '%s')\n", step, events_log.c_str());
	return 1;
}

/** Returns the number of visible rows of the tree. */
static guint visible_rows(xguimpl::Tree * impl)
{
	return g_list_model_get_n_items(G_LIST_MODEL(impl->tree_model));
}

/** Returns the first string of the node shown at a visible position. */
static std::string text_at(xguimpl::Tree * impl, guint pos)
{
	GtkTreeListRow * list_row = GTK_TREE_LIST_ROW(g_list_model_get_item(G_LIST_MODEL(impl->tree_model), pos));
	if (!list_row) return "";
	XguiRow * row = XGUI_ROW(gtk_tree_list_row_get_item(list_row));
	std::string text = xgui_row_get_node(row)->getString(0);
	g_object_unref(row);
	g_object_unref(list_row);
	return text;
}

int main()
{
	xgui::Window * win = xgui::Master::CreateWindow();
	xgui::Tree * tree = xgui::Master::CreateTree(win, 0);
	xguimpl::Tree * impl = static_cast<xguimpl::Tree *>(tree->getImpl());

	// root: a (a1, a2), b
	xgui::Model * model = xgui::Master::CreateModel();
	model->appendChild("a");
	model->appendChild("b");
	xgui::Model * a = model->getChild(0);
	xgui::Model * b = model->getChild(1);
	a->appendChild("a1");
	a->appendChild("a2");
	tree->setModel(model);

	xgui::CppFTextStatusCallback expand_cb(on_expand), select_cb(on_select);
	int failures = 0;
	failures += expect(tree->linkEvent("onexpand", &expand_cb), "link onexpand");
	failures += expect(tree->linkEvent("onselect", &select_cb), "link onselect");
	failures += expect(!xguimpl::columnViewHeadersVisible(impl->column_view), "headers hidden");

	failures += expect(visible_rows(impl) == 2, "collapsed tree shows the top level");
	tree->expand(a);
	failures += expect(visible_rows(impl) == 4 && text_at(impl, 1) == "a1", "expand");
	failures += expect(events_log == "exp:" + a->path() + "=1;", "onexpand on expand");
	events_log.clear();
	tree->collapse(a);
	failures += expect(visible_rows(impl) == 2 && events_log == "exp:" + a->path() + "=0;", "collapse");
	events_log.clear();

	// A child appended under a collapsed parent shows up when the parent is expanded
	a->appendChild("a3");
	tree->expand(a);
	failures += expect(visible_rows(impl) == 5 && text_at(impl, 3) == "a3", "child of collapsed parent");
	events_log.clear();

	// A leaf GTK already asked children for becomes expandable with its first child
	tree->expand(b);
	b->appendChild("b1");
	tree->expand(b);
	failures += expect(visible_rows(impl) == 6 && text_at(impl, 5) == "b1", "first child of a leaf");
	events_log.clear();

	// Checks
	tree->check(a, 1, true);
	failures += expect(tree->isChecked(a, 1) && !tree->isChecked(a, 0), "check");
	tree->checkAll(a, false);
	failures += expect(!tree->isChecked(a, 1), "uncheck all");

	// Sorting the children of a node
	xgui::CppFTextPairCallback compare_cb(compare_desc);
	tree->sort(a, &compare_cb);
	failures += expect(text_at(impl, 1) == "a3" && text_at(impl, 3) == "a1", "sort children");

	// Selection of a nested node, also hidden in a collapsed parent
	tree->collapse(a);
	events_log.clear();
	tree->select(a, 0, true);
	failures += expect(tree->isSelected(a, 0) && events_log.find("sel:" + a->getChild(0)->path() + "=1;") != std::string::npos, "select nested node");
	events_log.clear();
	tree->select(model, 1, true);
	failures += expect(events_log == "sel:" + a->getChild(0)->path() + "=0;sel:" + b->path() + "=1;", "single selection moves");
	events_log.clear();

	// Removing the selected subtree forgets its rows
	model->removeChild(1);
	failures += expect(impl->rowOf(b) == NULL && !impl->isSelectionCached(b), "remove selected subtree");
	return failures ? 1 : 0;
}
