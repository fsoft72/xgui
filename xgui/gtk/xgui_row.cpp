//
// C++ Implementation: xgui_row
//
// Description: GObject wrapping a xgui::Model node, the item type of the GTK 4 list models of List and Tree
//
//

#include "xgui_row.h"

#include <cstring>

struct _XguiRow
{
	GObject parent_instance;

	xgui::Model * node;
	gboolean checked;
};

G_DEFINE_FINAL_TYPE(XguiRow, xgui_row, G_TYPE_OBJECT)

enum { PROP_0, PROP_CHECKED, N_PROPS };
static GParamSpec * row_props[N_PROPS] = { NULL, NULL };

static void xgui_row_get_property ( GObject * object, guint prop_id, GValue * value, GParamSpec * pspec )
{
	XguiRow * row = XGUI_ROW(object);
	if (prop_id == PROP_CHECKED) g_value_set_boolean(value, row->checked);
	else G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
}

static void xgui_row_set_property ( GObject * object, guint prop_id, const GValue * value, GParamSpec * pspec )
{
	XguiRow * row = XGUI_ROW(object);
	if (prop_id == PROP_CHECKED) xgui_row_set_checked(row, g_value_get_boolean(value));
	else G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
}

static void xgui_row_class_init ( XguiRowClass * klass )
{
	GObjectClass * object_class = G_OBJECT_CLASS(klass);
	object_class->get_property = xgui_row_get_property;
	object_class->set_property = xgui_row_set_property;

	row_props[PROP_CHECKED] = g_param_spec_boolean("checked", NULL, NULL, FALSE,
	                                               (GParamFlags)(G_PARAM_READWRITE | G_PARAM_EXPLICIT_NOTIFY | G_PARAM_STATIC_STRINGS));
	g_object_class_install_properties(object_class, N_PROPS, row_props);
}

static void xgui_row_init ( XguiRow * row )
{
	row->node = NULL;
	row->checked = FALSE;
}

XguiRow * xgui_row_new ( xgui::Model * node )
{
	XguiRow * row = XGUI_ROW(g_object_new(XGUI_TYPE_ROW, NULL));
	row->node = node;
	return row;
}

xgui::Model * xgui_row_get_node ( XguiRow * row )
{
	return row->node;
}

gboolean xgui_row_get_checked ( XguiRow * row )
{
	return row->checked;
}

void xgui_row_set_checked ( XguiRow * row, gboolean checked )
{
	checked = checked ? TRUE : FALSE;
	if (row->checked == checked) return;
	row->checked = checked;
	g_object_notify_by_pspec(G_OBJECT(row), row_props[PROP_CHECKED]);
}

/** Returns the header widget of a column view (its child with the "header" CSS name), or NULL. */
static GtkWidget * int_find_header ( GtkWidget * w )
{
	if (std::strcmp(gtk_widget_get_css_name(w), "header") == 0) return w;
	for (GtkWidget * c = gtk_widget_get_first_child(w); c != NULL; c = gtk_widget_get_next_sibling(c)) {
		GtkWidget * found = int_find_header(c);
		if (found) return found;
	}
	return NULL;
}

namespace xguimpl
{
	void setColumnViewHeadersVisible ( GtkColumnView * view, bool visible )
	{
		GtkWidget * header = int_find_header(GTK_WIDGET(view));
		if (header) gtk_widget_set_visible(header, visible);
	}

	bool columnViewHeadersVisible ( GtkColumnView * view )
	{
		GtkWidget * header = int_find_header(GTK_WIDGET(view));
		return header && gtk_widget_get_visible(header);
	}
}
