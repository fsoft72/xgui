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

/** Returns the attribute of a type set on a label, or NULL. */
static PangoAttribute * label_attr(GtkLabel * label, PangoAttrType type)
{
	PangoAttrList * attrs = gtk_label_get_attributes(label);
	if (!attrs) return NULL;
	PangoAttrIterator * it = pango_attr_list_get_iterator(attrs);
	PangoAttribute * attr = pango_attr_iterator_get(it, type);
	pango_attr_iterator_destroy(it);
	return attr;
}

/** Binds the first row of a list to a label and checks bold, italic and foreground; returns the number of failures. */
static int check_list_row(xgui::List * list, xgui::Model * model, char const * label)
{
	xguimpl::List * impl = static_cast<xguimpl::List *>(list->getImpl());
	XguiRow * row = impl->rowOf(model->getChild(0));
	if (!row) { std::fprintf(stderr, "%s: no row\n", label); return 1; }

	GtkWidget * cell = g_object_ref_sink(gtk_label_new(NULL));
	impl->bindLabel(GTK_LABEL(cell), row, 0);

	int failures = 0;
	PangoAttribute * weight = label_attr(GTK_LABEL(cell), PANGO_ATTR_WEIGHT);
	PangoAttribute * style = label_attr(GTK_LABEL(cell), PANGO_ATTR_STYLE);
	PangoAttribute * fg = label_attr(GTK_LABEL(cell), PANGO_ATTR_FOREGROUND);
	if (std::string(gtk_label_get_text(GTK_LABEL(cell))) != "text") { std::fprintf(stderr, "%s: wrong text\n", label); ++failures; }
	if (!weight || ((PangoAttrInt *)weight)->value != PANGO_WEIGHT_BOLD) { std::fprintf(stderr, "%s: bold not set\n", label); ++failures; }
	if (!style || ((PangoAttrInt *)style)->value != PANGO_STYLE_ITALIC) { std::fprintf(stderr, "%s: italic not set\n", label); ++failures; }
	PangoColor * color = fg ? &((PangoAttrColor *)fg)->color : NULL;
	if (!color || color->red != 65535 || color->green != 0 || color->blue != 0) { std::fprintf(stderr, "%s: foreground is not red\n", label); ++failures; }
	g_object_unref(cell);
	return failures;
}

/** Binds the first row of a tree to a cell and checks bold, italic and foreground; returns the number of failures. */
static int check_tree_row(xgui::Tree * tree, xgui::Model * model, char const * label)
{
	xguimpl::Tree * impl = static_cast<xguimpl::Tree *>(tree->getImpl());
	XguiRow * row = impl->rowOf(model->getChild(0));
	if (!row) { std::fprintf(stderr, "%s: no row\n", label); return 1; }

	GtkWidget * cell = g_object_ref_sink(gtk_label_new(NULL));
	GtkWidget * icon = g_object_ref_sink(gtk_picture_new());
	impl->bindCell(GTK_LABEL(cell), GTK_PICTURE(icon), row);

	int failures = 0;
	PangoAttribute * weight = label_attr(GTK_LABEL(cell), PANGO_ATTR_WEIGHT);
	PangoAttribute * style = label_attr(GTK_LABEL(cell), PANGO_ATTR_STYLE);
	PangoAttribute * fg = label_attr(GTK_LABEL(cell), PANGO_ATTR_FOREGROUND);
	if (!weight || ((PangoAttrInt *)weight)->value != PANGO_WEIGHT_BOLD) { std::fprintf(stderr, "%s: bold not set\n", label); ++failures; }
	if (!style || ((PangoAttrInt *)style)->value != PANGO_STYLE_ITALIC) { std::fprintf(stderr, "%s: italic not set\n", label); ++failures; }
	if (!fg) { std::fprintf(stderr, "%s: foreground not set\n", label); ++failures; }
	g_object_unref(cell);
	g_object_unref(icon);
	return failures;
}

/** Returns true when the label bound to the first row of a list has a foreground color. */
static bool list_row_has_fg(xgui::List * list, xgui::Model * model)
{
	xguimpl::List * impl = static_cast<xguimpl::List *>(list->getImpl());
	GtkWidget * cell = g_object_ref_sink(gtk_label_new(NULL));
	impl->bindLabel(GTK_LABEL(cell), impl->rowOf(model->getChild(0)), 0);
	bool has_fg = label_attr(GTK_LABEL(cell), PANGO_ATTR_FOREGROUND) != NULL;
	g_object_unref(cell);
	return has_fg;
}

int main()
{
	xgui::Window * win = xgui::Master::CreateWindow();
	xgui::StyleManager * sm = xgui::Master::CreateStyleManager();
	sm->addStyle("fancy", 0, "#ff0000", "#00ff00", xgui::StyleManager::TxtBold | xgui::StyleManager::TxtItalic);

	sm->addStyle("broken", 0, "not-a-color", "", 0);

	std::vector<std::string> row;
	row.push_back("text");
	int failures = 0;

	// List, refresh path: the row exists before the model is set
	xgui::List * list = xgui::Master::CreateList(win, sm);
	list->addColumn(0, "c", "C", 100);
	xgui::Model * list_model = xgui::Master::CreateModel();
	list_model->appendChild(row, "fancy");
	list->setModel(list_model);
	failures += check_list_row(list, list_model, "list refresh");

	// List, insertItem path: the row is appended once the model is linked
	xgui::List * list2 = xgui::Master::CreateList(win, sm);
	list2->addColumn(0, "c", "C", 100);
	xgui::Model * list_model2 = xgui::Master::CreateModel();
	list2->setModel(list_model2);
	list_model2->appendChild(row, "fancy");
	failures += check_list_row(list2, list_model2, "list insert");

	// An invalid color string must not produce a color
	xgui::Model * broken_model = xgui::Master::CreateModel();
	std::vector<std::string> broken_row;
	broken_row.push_back("text");
	xgui::List * list3 = xgui::Master::CreateList(win, sm);
	list3->addColumn(0, "c", "C", 100);
	list3->setModel(broken_model);
	broken_model->appendChild(broken_row, "broken");
	if (list_row_has_fg(list3, broken_model)) { std::fprintf(stderr, "invalid color produced a value\n"); ++failures; }

	// Tree
	xgui::Tree * tree = xgui::Master::CreateTree(win, sm);
	xgui::Model * tree_model = xgui::Master::CreateModel();
	tree->setModel(tree_model);
	tree_model->appendChild(row, "fancy");
	failures += check_tree_row(tree, tree_model, "tree");

	return failures ? 1 : 0;
}
