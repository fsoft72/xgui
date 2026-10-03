// Characterization test: rows of List and Tree must carry the style attributes of their style name.
#include <cstdio>
#include <string>
#include <vector>

#include <gtk/gtk.h>

#include "master.h"
#include "window.h"
#include "list.h"
#include "tree.h"
#include "model.h"
#include "style.h"
#include "gtk/list_impl.h"
#include "gtk/tree_impl.h"

/** Reads an int-like cell of the first row of a tree model. */
static int cell_int(GtkTreeModel * store, int column)
{
	GtkTreeIter iter;
	gint value = -1;
	gtk_tree_model_get_iter_first(store, &iter);
	gtk_tree_model_get(store, &iter, column, &value, -1);
	return value;
}

/** Returns true when the foreground cell of the first row is set. */
static bool fg_is_set(GtkTreeModel * store, int column)
{
	GtkTreeIter iter;
	GValue val = G_VALUE_INIT;
	gtk_tree_model_get_iter_first(store, &iter);
	gtk_tree_model_get_value(store, &iter, column, &val);
	bool set = g_value_get_boxed(&val) != NULL;
	g_value_unset(&val);
	return set;
}

/** Checks bold, italic and foreground of the first row; returns the number of failures. */
static int check_row(GtkTreeModel * store, int bold_col, int italic_col, int fg_col, char const * label)
{
	int failures = 0;
	if (cell_int(store, bold_col) != PANGO_WEIGHT_BOLD) { std::fprintf(stderr, "%s: bold not set\n", label); ++failures; }
	if (cell_int(store, italic_col) != PANGO_STYLE_ITALIC) { std::fprintf(stderr, "%s: italic not set\n", label); ++failures; }
	if (!fg_is_set(store, fg_col)) { std::fprintf(stderr, "%s: foreground not set\n", label); ++failures; }
	return failures;
}

int main()
{
	xgui::Window * win = xgui::Master::CreateWindow();
	xgui::StyleManager * sm = xgui::Master::CreateStyleManager();
	sm->addStyle("fancy", 0, "#ff0000", "#00ff00", xgui::StyleManager::TxtBold | xgui::StyleManager::TxtItalic);

	std::vector<std::string> row;
	row.push_back("text");
	int failures = 0;

	// List, refresh path: the row exists before the model is set
	xgui::List * list = xgui::Master::CreateList(win, sm);
	list->addColumn(0, "c", "C", 100);
	xgui::Model * list_model = xgui::Master::CreateModel();
	list_model->appendChild(row, "fancy");
	list->setModel(list_model);
	xguimpl::List * list_impl = static_cast<xguimpl::List *>(list->getImpl());
	GtkTreeModel * list_store = gtk_tree_view_get_model(list_impl->list_view);
	failures += check_row(list_store, list_impl->bold_column_id, list_impl->italic_column_id, list_impl->fg_column_id, "list refresh");

	// List, insertItem path: the row is appended once the model is linked
	xgui::List * list2 = xgui::Master::CreateList(win, sm);
	list2->addColumn(0, "c", "C", 100);
	xgui::Model * list_model2 = xgui::Master::CreateModel();
	list2->setModel(list_model2);
	list_model2->appendChild(row, "fancy");
	xguimpl::List * list_impl2 = static_cast<xguimpl::List *>(list2->getImpl());
	failures += check_row(gtk_tree_view_get_model(list_impl2->list_view), list_impl2->bold_column_id,
	                      list_impl2->italic_column_id, list_impl2->fg_column_id, "list insert");

	// Tree
	xgui::Tree * tree = xgui::Master::CreateTree(win, sm);
	xgui::Model * tree_model = xgui::Master::CreateModel();
	tree->setModel(tree_model);
	tree_model->appendChild(row, "fancy");
	xguimpl::Tree * tree_impl = static_cast<xguimpl::Tree *>(tree->getImpl());
	failures += check_row(gtk_tree_view_get_model(tree_impl->tree_view), tree_impl->bold_column_id,
	                      tree_impl->italic_column_id, tree_impl->fg_column_id, "tree");

	return failures ? 1 : 0;
}
