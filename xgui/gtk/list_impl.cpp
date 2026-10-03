//
// C++ Implementation: list_impl
//
// Description: List widget on GtkColumnView (GListStore of XguiRow -> GtkMultiSelection -> GtkColumnView)
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "list_impl.h"
#include "list.h"
#include "callback.h"
#include "model.h"
#include "image.h"
#include "image_impl.h"
#include "row_style.h"

static float int_map_align ( std::string const & alignment )
{
	if (alignment == "right") return 1.0f;
	else if (alignment == "center") return 0.5f;
	else return 0.0f;
}

/* Data columns: a label showing one string of the model node */

static void int_text_setup ( GtkSignalListItemFactory *, GObject * item, xguimpl::ListColumn * col )
{
	GtkWidget * label = gtk_label_new(NULL);
	gtk_label_set_xalign(GTK_LABEL(label), col->xalign);
	gtk_list_item_set_child(GTK_LIST_ITEM(item), label);
}

static void int_text_bind ( GtkSignalListItemFactory *, GObject * item, xguimpl::ListColumn * col )
{
	if ( !col->list ) return;
	col->list->bindLabel(GTK_LABEL(gtk_list_item_get_child(GTK_LIST_ITEM(item))), xguimpl::listItemRow(item), col->field);
}

static void int_column_destroy ( gpointer data )
{
	delete (xguimpl::ListColumn *)data;
}

/* Icon column: the icon of the style of the row */

static void int_icon_setup ( GtkSignalListItemFactory *, GObject * item, xguimpl::List * )
{
	GtkWidget * picture = gtk_picture_new();
	gtk_picture_set_can_shrink(GTK_PICTURE(picture), FALSE);
	gtk_list_item_set_child(GTK_LIST_ITEM(item), picture);
}

static void int_icon_bind ( GtkSignalListItemFactory *, GObject * item, xguimpl::List * list )
{
	list->bindIcon(GTK_PICTURE(gtk_list_item_get_child(GTK_LIST_ITEM(item))), xguimpl::listItemRow(item));
}

/** Compares two rows on one of their strings through the user callback. */
struct SortInformations {
	xguimpl::List * list_ptr;
	xgui::TextPairCallback * compare_cb;
	int field;
};

static std::string int_field ( xgui::Model * node, int field )
{
	return (field >= 0 && (unsigned int)field < node->numberOfStrings()) ? node->getString(field) : std::string();
}

static gint int_sort_func ( gconstpointer a, gconstpointer b, gpointer data )
{
	SortInformations * si = (SortInformations *)data;
	std::string str1 = int_field(xgui_row_get_node(XGUI_ROW((gpointer)a)), si->field);
	std::string str2 = int_field(xgui_row_get_node(XGUI_ROW((gpointer)b)), si->field);
	return si->compare_cb->call( dynamic_cast<xgui::Widget*>(si->list_ptr->this_list), str1, str2 );
}

namespace xguimpl
{
	List::List ( GtkWidget * real_w )
	: View ( real_w ), store(0), selection(0), column_view(0), check_column(0), icon_column(0),
	  has_icons(false), has_checks(false), search_enabled(false), this_list(0) { }

	List::List ( xgui::Container * parent )
	: View ( gtk_scrolled_window_new() ), store(0), selection(0), column_view(0), check_column(0), icon_column(0),
	  has_icons(false), has_checks(false), search_enabled(false), this_list(0)
	{
		store = g_list_store_new ( XGUI_TYPE_ROW );
		selection = gtk_multi_selection_new ( G_LIST_MODEL ( g_object_ref ( store ) ) );
		column_view = GTK_COLUMN_VIEW ( gtk_column_view_new ( GTK_SELECTION_MODEL ( g_object_ref ( selection ) ) ) );
		gtk_column_view_set_reorderable ( column_view, FALSE );

		g_signal_connect ( selection, "selection-changed", G_CALLBACK ( OnSelectionChanged ), this );
		g_signal_connect ( gtk_column_view_get_sorter ( column_view ), "changed", G_CALLBACK ( OnSorterChanged ), this );

		gtk_scrolled_window_set_child ( GTK_SCROLLED_WINDOW ( widget ), GTK_WIDGET ( column_view ) );
		gtk_scrolled_window_set_policy ( GTK_SCROLLED_WINDOW ( widget ), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC );
	}

	List::~List()
	{
		if ( selection ) g_signal_handlers_disconnect_by_data ( selection, this );
		if ( column_view && GTK_IS_WIDGET(column_view) ) {
			g_signal_handlers_disconnect_by_data ( gtk_column_view_get_sorter ( column_view ), this );
			// Unbinds every cell: no factory callback may reach this object any more
			gtk_column_view_set_model ( column_view, NULL );
			for ( std::vector<ListColumn *>::iterator c = columns.begin(); c != columns.end(); ++c )
				(*c)->list = 0;
		}
		if ( selection ) g_object_unref ( selection );
		if ( store ) g_object_unref ( store );
	}

	bool List::linkEvent( std::string const &name )
	{
		// The selection is reported through "onselect"; "onchange" is kept for compatibility
		if ( (name == "onselect") || (name == "onchange") )
			return true;
		else if ( name == "onheaderclick" )
			return true;
		else if ( name == "onrefresh" )
			return true;
		else if ( name == "oncheck" )
			return true;

		return Widget::linkEvent(name);
	}

	void List::setAccessor(xgui::Widget * w)
	{
		xgui::List * downcasted_widget = dynamic_cast<xgui::List*>(w);
		ASSERT(downcasted_widget);
		this_list = downcasted_widget;

		Widget::setAccessor(w);
	}

	XguiRow * List::rowOf ( xgui::Model * node )
	{
		std::map<xgui::Model *, XguiRow *>::iterator found = rows.find(node);
		return found == rows.end() ? 0 : found->second;
	}

	int List::positionOf ( XguiRow * row )
	{
		guint pos = 0;
		if ( !row || !g_list_store_find ( store, row, &pos ) ) return -1;
		return (int)pos;
	}

	ListColumn * List::findColumn ( std::string const &name )
	{
		for ( std::vector<ListColumn *>::iterator c = columns.begin(); c != columns.end(); ++c )
			if ( ((*c)->id == name) || ((*c)->title == name) ) return *c;
		return 0;
	}

	void List::bindLabel ( GtkLabel * label, XguiRow * row, int field )
	{
		xgui::Model * node = xgui_row_get_node(row);
		gtk_label_set_text ( label, int_field(node, field).c_str() );
		applyLabelStyle ( label, this_list->styleman_, node->getStyle() );
	}

	void List::bindIcon ( GtkPicture * picture, XguiRow * row )
	{
		GdkTexture * icon = rowIcon ( this_list->styleman_, xgui_row_get_node(row)->getStyle() );
		gtk_picture_set_paintable ( picture, icon ? GDK_PAINTABLE(icon) : NULL );
	}

	void List::notifyRefresh ()
	{
		xgui::Callback * refresh_cb = this_list->getEvent("onrefresh");
		if (refresh_cb) refresh_cb->call( this_list );
	}

	bool List::get_list_prop ( std::string const &name, std::string &vals )
	{
		if ( name == "search-enabled" ) {
			vals = search_enabled ? "1" : "0";
		} else if ( name == "visible-headers" ) {
			vals = columnViewHeadersVisible ( column_view ) ? "1" : "0";
		} else if ( name == "has-icons" ) {
			vals = has_icons ? "1" : "0";
		} else if ( name == "has-checkboxes" ) {
			vals = has_checks ? "1" : "0";
		} else
			return false;

		return true;
	}

	bool List::set_list_prop ( std::string const &name, const std::string &vals )
	{
		if ( name == "search-enabled" ) {
			// GtkColumnView has no interactive search: the value is only kept
			search_enabled = (vals == "1");
		} else if ( name == "visible-headers" ) {
			setColumnViewHeadersVisible ( column_view, vals != "0" );
		} else if ( name == "has-icons" ) {
			bool should_put_icons = (vals == "1");
			if ( should_put_icons == has_icons ) return true;
			has_icons = should_put_icons;

			if (has_icons) {
				icon_column = newColumn ( "", G_CALLBACK(int_icon_setup), G_CALLBACK(int_icon_bind), NULL, this );
				gtk_column_view_insert_column ( column_view, has_checks ? 1 : 0, icon_column );
				g_object_unref ( icon_column );
			}
			else {
				gtk_column_view_remove_column ( column_view, icon_column );
				icon_column = 0;
			}
		} else if ( name == "has-checkboxes" ) {
			bool should_put_checks = (vals == "1");
			if ( should_put_checks == has_checks ) return true;
			has_checks = should_put_checks;

			if (has_checks) {
				check_column = newCheckColumn ( this );
				gtk_column_view_insert_column ( column_view, 0, check_column );
				g_object_unref ( check_column );
			}
			else {
				gtk_column_view_remove_column ( column_view, check_column );
				check_column = 0;
			}
		} else
			return false;

		return true;
	}

	void List::insertItem ( xgui::Model * parent, int child_pos, xgui::Model * child )
	{
		xgui::Model * model = this_list->getModel();
		if ( (!store) || (!model) || (parent != model) || rowOf(child) ) return;

		guint n = g_list_model_get_n_items ( G_LIST_MODEL(store) );
		guint pos = (child_pos < 0 || (guint)child_pos > n) ? n : (guint)child_pos;

		XguiRow * row = xgui_row_new ( child );
		rows[child] = row;
		g_list_store_insert ( store, pos, row );
		g_object_unref ( row );

		notifyRefresh();
	}

	void List::removeItem ( xgui::Model * parent, int child_pos, xgui::Model * child )
	{
		if ( !store ) return;

		XguiRow * row = rowOf(child);
		int pos = positionOf(row);
		rows.erase(child);
		delSelection(child);
		if ( pos >= 0 ) g_list_store_remove ( store, pos );

		notifyRefresh();
	}

	void List::refresh()
	{
		xgui::Model * model = this_list->getModel();
		if ( (!store) || (!model) ) return;

		rows.clear();
		std::vector<gpointer> items;
		for ( xgui::Model::iterator iter = model->begin(); iter != model->end(); ++iter ) {
			xgui::Model * child = dynamic_cast<xgui::Model *>(*iter);
			if ( !child || rows.count(child) ) continue;
			XguiRow * row = xgui_row_new ( child );
			rows[child] = row;
			items.push_back ( row );
		}

		// One splice: the view is notified once for the whole model
		g_list_store_splice ( store, 0, g_list_model_get_n_items ( G_LIST_MODEL(store) ), items.data(), items.size() );
		for ( std::vector<gpointer>::iterator i = items.begin(); i != items.end(); ++i )
			g_object_unref ( *i );

		notifyRefresh();
	}

	void List::addColumn ( int cpos, const std::string & cid, const std::string & cname, int cwidth, const std::string & calign )
	{
		ListColumn * col = new ListColumn();
		col->list = this;
		col->id = cid;
		col->title = cname;
		col->field = cpos;
		col->xalign = int_map_align(calign);

		col->column = newColumn ( cname.c_str(), G_CALLBACK(int_text_setup), G_CALLBACK(int_text_bind), NULL, col );
		g_object_set_data_full ( G_OBJECT(col->column), "xgui-list-column", col, int_column_destroy );
		gtk_column_view_column_set_fixed_width ( col->column, cwidth );
		gtk_column_view_column_set_resizable ( col->column, TRUE );
		gtk_column_view_column_set_expand ( col->column, FALSE );
		// The sorter only makes the header clickable: clicks are reported by onheaderclick
		GtkSorter * sorter = GTK_SORTER ( gtk_custom_sorter_new ( NULL, NULL, NULL ) );
		gtk_column_view_column_set_sorter ( col->column, sorter );
		g_object_unref ( sorter );

		// Data columns follow the optional checkbox and icon columns
		guint first = (has_checks ? 1 : 0) + (has_icons ? 1 : 0);
		guint n = g_list_model_get_n_items ( gtk_column_view_get_columns ( column_view ) );
		guint pos = first + (cpos < 0 ? n : (guint)cpos);
		if (pos > n) pos = n;
		gtk_column_view_insert_column ( column_view, pos, col->column );
		g_object_unref ( col->column );

		columns.push_back ( col );
	}

	bool List::delColumn ( const std::string & column_name )
	{
		ListColumn * col = findColumn ( column_name );
		if ( !col ) return false;

		for ( std::vector<ListColumn *>::iterator c = columns.begin(); c != columns.end(); ++c )
			if ( *c == col ) { columns.erase(c); break; }
		// Removing the column releases it, and with it the ListColumn
		gtk_column_view_remove_column ( column_view, col->column );
		return true;
	}

	void List::sort ( std::string const & column_name, xgui::TextPairCallback * compare_cb )
	{
		ListColumn * col = findColumn ( column_name );
		if ( !col || !compare_cb ) return;

		// A one shot sort of the rows, as with GTK 3: later rows are inserted at their model position
		SortInformations si = { this, compare_cb, col->field };
		g_list_store_sort ( store, int_sort_func, &si );
	}

	bool List::isSelected(xgui::Model * model, int pos)
	{
		int row_pos = positionOf ( rowOf ( model->getChild(pos) ) );
		if ( row_pos < 0 ) return false;
		return gtk_selection_model_is_selected ( GTK_SELECTION_MODEL(selection), row_pos );
	}

	void List::select(xgui::Model * parent, int pos, bool state)
	{
		int row_pos = positionOf ( rowOf ( parent->getChild(pos) ) );
		if ( row_pos < 0 ) return;

		if (state)
			gtk_selection_model_select_item ( GTK_SELECTION_MODEL(selection), row_pos, FALSE );
		else
			gtk_selection_model_unselect_item ( GTK_SELECTION_MODEL(selection), row_pos );
	}

	void List::selectAll(xgui::Model * model, bool state)
	{
		if (state)
			gtk_selection_model_select_all ( GTK_SELECTION_MODEL(selection) );
		else
			gtk_selection_model_unselect_all ( GTK_SELECTION_MODEL(selection) );
	}

	bool List::isChecked(xgui::Model * parent, int pos)
	{
		XguiRow * row = rowOf ( parent->getChild(pos) );
		return row && xgui_row_get_checked ( row );
	}

	void List::check(xgui::Model * parent, int pos, bool state)
	{
		XguiRow * row = rowOf ( parent->getChild(pos) );
		if ( row ) xgui_row_set_checked ( row, state );
	}

	void List::checkAll(xgui::Model * parent, bool state)
	{
		for (xgui::Model::iterator i = parent->begin(); i != parent->end(); ++i) {
			XguiRow * row = rowOf ( dynamic_cast<xgui::Model*>(*i) );
			if ( row ) xgui_row_set_checked ( row, state );
		}
	}

	void List::notifyCheck ( XguiRow * row )
	{
		xgui::Callback * cb = this_list->getEvent("oncheck");
		if (!cb) return;

		xgui::TextStatusCallback * chk_cb = dynamic_cast<xgui::TextStatusCallback *>(cb);
		if (!chk_cb) {
			DMESSAGE("oncheck event of xgui::List expected a TextStatusCallback");
			return;
		}

		chk_cb->call(this_list, xgui_row_get_node(row)->path(), xgui_row_get_checked(row));
	}

	void List::updateSelection ( guint position, guint n_items )
	{
		xgui::TextStatusCallback * sel_cb = 0;
		xgui::Callback * cb = this_list->getEvent("onselect");
		if (!cb) cb = this_list->getEvent("onchange");
		if (cb) {
			sel_cb = dynamic_cast<xgui::TextStatusCallback*>(cb);
			if (!sel_cb)
				DMESSAGE("onselect event of xgui::List expected a TextStatusCallback");
		}

		// Compare the rows of the changed range with the cached selection
		std::vector<xgui::Model*> deselections;
		std::vector<xgui::Model*> selections;
		guint n = g_list_model_get_n_items ( G_LIST_MODEL(store) );
		for ( guint i = position; i < position + n_items && i < n; ++i ) {
			XguiRow * row = XGUI_ROW ( g_list_model_get_item ( G_LIST_MODEL(store), i ) );
			xgui::Model * node = xgui_row_get_node ( row );
			g_object_unref ( row );

			bool selected = gtk_selection_model_is_selected ( GTK_SELECTION_MODEL(selection), i );
			if ( selected && !isSelectionCached(node) ) selections.push_back(node);
			else if ( !selected && isSelectionCached(node) ) deselections.push_back(node);
		}

		for (std::vector<xgui::Model*>::iterator sel = deselections.begin(); sel != deselections.end(); ++sel)
			if ( !sel_cb || sel_cb->call( this_list, (*sel)->path(), 0 ) )
				delSelection(*sel);

		for (std::vector<xgui::Model*>::iterator sel = selections.begin(); sel != selections.end(); ++sel)
			if ( !sel_cb || sel_cb->call( this_list, (*sel)->path(), 1 ) )
				addSelection(*sel);
	}

	void List::OnSelectionChanged ( GtkSelectionModel * model, guint position, guint n_items, List * xl )
	{
		xl->updateSelection ( position, n_items );
	}

	void List::OnSorterChanged ( GtkSorter * sorter, GtkSorterChange change, List * xl )
	{
		// A click on a column title changes the sort column of the view sorter
		GtkColumnViewColumn * clicked = gtk_column_view_sorter_get_primary_sort_column ( GTK_COLUMN_VIEW_SORTER(sorter) );
		if ( !clicked ) return;

		xgui::Callback * base_cb = xl->this_list->getEvent("onheaderclick");
		if (!base_cb) return;

		xgui::TextCallback * cb = dynamic_cast<xgui::TextCallback *>(base_cb);
		if (!cb) {
			DMESSAGE("onheaderclick event of xgui::List expected a TextCallback");
			return;
		}

		ListColumn * col = (ListColumn *)g_object_get_data ( G_OBJECT(clicked), "xgui-list-column" );
		cb->call ( xl->this_list, ( col ? col->id : "" ) );
	}
}
