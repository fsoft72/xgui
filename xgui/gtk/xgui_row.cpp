//
// C++ Implementation: xgui_row
//
// Description: GObject wrapping a xgui::Model node, the item type of the GTK 4 list models of List and Tree
//
//

#include "xgui_row.h"
#include "view_impl.h"

#include <cstring>

/** Data key of the row bound to a checkbox. */
static char const * const ROW_KEY = "xgui-row";

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

/* Checkbox column: a check button following the "checked" property of the row */

static void int_check_toggled ( GtkCheckButton * button, xguimpl::View * view )
{
	XguiRow * row = (XguiRow *)g_object_get_data(G_OBJECT(button), ROW_KEY);
	if (!row) return;

	gboolean active = gtk_check_button_get_active(button);
	if (active == xgui_row_get_checked(row)) return;
	xgui_row_set_checked(row, active);
	view->notifyCheck(row);
}

static void int_row_checked_changed ( XguiRow * row, GParamSpec *, GtkCheckButton * button )
{
	gtk_check_button_set_active(button, xgui_row_get_checked(row));
}

static void int_check_setup ( GtkSignalListItemFactory *, GObject * item, xguimpl::View * view )
{
	GtkWidget * button = gtk_check_button_new();
	g_signal_connect(button, "toggled", G_CALLBACK(int_check_toggled), view);
	gtk_list_item_set_child(GTK_LIST_ITEM(item), button);
}

static void int_check_bind ( GtkSignalListItemFactory *, GObject * item, xguimpl::View * )
{
	GtkWidget * button = gtk_list_item_get_child(GTK_LIST_ITEM(item));
	XguiRow * row = xguimpl::listItemRow(item);
	if (!row) return;

	// The row is attached after the state is copied, so that binding does not look like a user toggle
	g_object_set_data(G_OBJECT(button), ROW_KEY, NULL);
	gtk_check_button_set_active(GTK_CHECK_BUTTON(button), xgui_row_get_checked(row));
	g_object_set_data(G_OBJECT(button), ROW_KEY, row);
	g_signal_connect_object(row, "notify::checked", G_CALLBACK(int_row_checked_changed), button, (GConnectFlags)0);
}

static void int_check_unbind ( GtkSignalListItemFactory *, GObject * item, xguimpl::View * )
{
	GtkWidget * button = gtk_list_item_get_child(GTK_LIST_ITEM(item));
	XguiRow * row = (XguiRow *)g_object_get_data(G_OBJECT(button), ROW_KEY);
	g_object_set_data(G_OBJECT(button), ROW_KEY, NULL);
	if (row) g_signal_handlers_disconnect_by_func(row, (gpointer)int_row_checked_changed, button);
}

namespace xguimpl
{
	XguiRow * listItemRow ( GObject * list_item )
	{
		gpointer item = gtk_list_item_get_item(GTK_LIST_ITEM(list_item));
		if (GTK_IS_TREE_LIST_ROW(item)) {
			// The store keeps the row alive: the reference returned by the tree row is not needed
			gpointer inner = gtk_tree_list_row_get_item(GTK_TREE_LIST_ROW(item));
			if (inner) g_object_unref(inner);
			item = inner;
		}
		return (item && XGUI_IS_ROW(item)) ? XGUI_ROW(item) : NULL;
	}

	GtkColumnViewColumn * newColumn ( char const * title, GCallback setup, GCallback bind, GCallback unbind, gpointer data )
	{
		GtkListItemFactory * factory = gtk_signal_list_item_factory_new();
		g_signal_connect(factory, "setup", setup, data);
		g_signal_connect(factory, "bind", bind, data);
		if (unbind) g_signal_connect(factory, "unbind", unbind, data);
		return gtk_column_view_column_new(title, factory);
	}

	GtkColumnViewColumn * newCheckColumn ( View * view )
	{
		return newColumn("", G_CALLBACK(int_check_setup), G_CALLBACK(int_check_bind), G_CALLBACK(int_check_unbind), view);
	}

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
