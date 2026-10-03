//
// C++ Implementation: tree_impl
//
// Description: Tree widget on GtkColumnView (GListStore of XguiRow -> GtkTreeListModel -> GtkSingleSelection)
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "tree_impl.h"
#include "tree.h"
#include "callback.h"
#include "model.h"
#include "image.h"
#include "image_impl.h"
#include "row_style.h"

/** Data key telling that the expansion of a tree row is watched. */
static char const * const WATCHED_KEY = "xgui-tree-watched";

/** Data key holding the last reported expansion state of a tree row (1 collapsed, 2 expanded). */
static char const * const EXPANDED_KEY = "xgui-tree-expanded";

/** Encodes an expansion state for EXPANDED_KEY. */
static gpointer int_expanded_data ( gboolean expanded )
{
	return GINT_TO_POINTER ( expanded ? 2 : 1 );
}

/** Returns the parent of a model node. */
static xgui::Model * int_parent ( xgui::Model * node )
{
	return dynamic_cast<xgui::Model *>(node->getParent());
}

/* Text column: an expander holding the icon and the text of the node */

static void int_text_setup ( GtkSignalListItemFactory *, GObject * item, xguimpl::Tree * )
{
	GtkWidget * box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);
	GtkWidget * icon = gtk_picture_new();
	gtk_picture_set_can_shrink(GTK_PICTURE(icon), FALSE);
	GtkWidget * label = gtk_label_new(NULL);
	gtk_label_set_xalign(GTK_LABEL(label), 0.0f);
	gtk_box_append(GTK_BOX(box), icon);
	gtk_box_append(GTK_BOX(box), label);

	GtkWidget * expander = gtk_tree_expander_new();
	gtk_tree_expander_set_child(GTK_TREE_EXPANDER(expander), box);
	gtk_list_item_set_child(GTK_LIST_ITEM(item), expander);
}

static void int_text_bind ( GtkSignalListItemFactory *, GObject * item, xguimpl::Tree * tree )
{
	GtkTreeExpander * expander = GTK_TREE_EXPANDER(gtk_list_item_get_child(GTK_LIST_ITEM(item)));
	gtk_tree_expander_set_list_row(expander, GTK_TREE_LIST_ROW(gtk_list_item_get_item(GTK_LIST_ITEM(item))));

	GtkWidget * box = gtk_tree_expander_get_child(expander);
	GtkWidget * icon = gtk_widget_get_first_child(box);
	GtkWidget * label = gtk_widget_get_next_sibling(icon);
	XguiRow * row = xguimpl::listItemRow(item);
	if (row) tree->bindCell(GTK_LABEL(label), GTK_PICTURE(icon), row);
}

static void int_text_unbind ( GtkSignalListItemFactory *, GObject * item, xguimpl::Tree * )
{
	gtk_tree_expander_set_list_row(GTK_TREE_EXPANDER(gtk_list_item_get_child(GTK_LIST_ITEM(item))), NULL);
}

/** Compares two rows on their first string through the user callback. */
struct TreeSortInformations {
	xgui::Tree * tree;
	xgui::TextPairCallback * compare_cb;
};

static gint int_sort_func ( gconstpointer a, gconstpointer b, gpointer data )
{
	TreeSortInformations * si = (TreeSortInformations *)data;
	xgui::Model * node_a = xgui_row_get_node(XGUI_ROW((gpointer)a));
	xgui::Model * node_b = xgui_row_get_node(XGUI_ROW((gpointer)b));
	std::string str1 = node_a->numberOfStrings() ? node_a->getString(0) : std::string();
	std::string str2 = node_b->numberOfStrings() ? node_b->getString(0) : std::string();
	return si->compare_cb->call( si->tree, str1, str2 );
}

namespace xguimpl
{
	Tree::Tree ( GtkWidget * real_w )
	: View (real_w), root_store(0), tree_model(0), selection(0), column_view(0), check_column(0), text_column(0),
	  this_tree(0), has_icons(false), has_checks(false), search_enabled(false), updating(false) { }

	Tree::Tree ( xgui::Container * parent )
	: View(gtk_scrolled_window_new()), root_store(0), tree_model(0), selection(0), column_view(0), check_column(0), text_column(0),
	  this_tree(0), has_icons(false), has_checks(false), search_enabled(false), updating(false)
	{
		root_store = g_list_store_new ( XGUI_TYPE_ROW );
		tree_model = gtk_tree_list_model_new ( G_LIST_MODEL ( g_object_ref ( root_store ) ), FALSE, FALSE, OnCreateChildren, this, NULL );
		selection = gtk_single_selection_new ( G_LIST_MODEL ( g_object_ref ( tree_model ) ) );
		gtk_single_selection_set_autoselect ( selection, FALSE );
		gtk_single_selection_set_can_unselect ( selection, TRUE );
		column_view = GTK_COLUMN_VIEW ( gtk_column_view_new ( GTK_SELECTION_MODEL ( g_object_ref ( selection ) ) ) );

		text_column = newColumn ( "", G_CALLBACK(int_text_setup), G_CALLBACK(int_text_bind), G_CALLBACK(int_text_unbind), this );
		gtk_column_view_column_set_expand ( text_column, TRUE );
		gtk_column_view_column_set_resizable ( text_column, TRUE );
		gtk_column_view_append_column ( column_view, text_column );
		g_object_unref ( text_column );
		setColumnViewHeadersVisible ( column_view, false );

		g_signal_connect ( selection, "notify::selected-item", G_CALLBACK ( OnSelectionChanged ), this );
		g_signal_connect ( tree_model, "items-changed", G_CALLBACK ( OnRowsChanged ), this );

		gtk_scrolled_window_set_child ( GTK_SCROLLED_WINDOW ( widget ), GTK_WIDGET ( column_view ) );
		gtk_scrolled_window_set_policy ( GTK_SCROLLED_WINDOW ( widget ), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC );
	}

	Tree::~Tree()
	{
		if ( !tree_model ) return;

		g_signal_handlers_disconnect_by_data ( selection, this );
		g_signal_handlers_disconnect_by_data ( tree_model, this );
		guint n = g_list_model_get_n_items ( G_LIST_MODEL(tree_model) );
		for ( guint i = 0; i < n; ++i ) {
			GObject * row = G_OBJECT ( g_list_model_get_item ( G_LIST_MODEL(tree_model), i ) );
			g_signal_handlers_disconnect_by_data ( row, this );
			g_object_unref ( row );
		}
		// Unbinds every cell: no factory callback may reach this object any more
		if ( GTK_IS_WIDGET(column_view) ) gtk_column_view_set_model ( column_view, NULL );

		for ( std::map<xgui::Model *, GListStore *>::iterator s = child_stores.begin(); s != child_stores.end(); ++s )
			g_object_unref ( s->second );
		g_object_unref ( selection );
		g_object_unref ( tree_model );
		g_object_unref ( root_store );
	}

	bool Tree::linkEvent( std::string const &name )
	{
		// The selection is reported through "onselect"; "onchange" is kept for compatibility
		if ( (name == "onselect") || (name == "onchange") )
			return true;
		else if ( name == "onrefresh" )
			return true;
		else if ( name == "oncheck" )
			return true;
		else if ( name == "onexpand" )
			return true;

		return Widget::linkEvent(name);
	}

	void Tree::setAccessor(xgui::Widget * w)
	{
		xgui::Tree * downcasted_widget = dynamic_cast<xgui::Tree*>(w);
		ASSERT(downcasted_widget);
		this_tree = downcasted_widget;

		Widget::setAccessor(w);
	}

	bool Tree::set_tree_prop ( std::string const &name, std::string const &val )
	{
		if ( name == "search-enabled" ) {
			// GtkColumnView has no interactive search: the value is only kept
			search_enabled = (val == "1");
		} else if ( name == "has-icons" ) {
			bool should_put_icons = (val == "1");
			if ( should_put_icons == has_icons ) return true;
			has_icons = should_put_icons;
			// The icon is part of the text cell: rebind the rows
			refresh();
		} else if ( name == "has-checkboxes" ) {
			bool should_put_checks = (val == "1");
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

	bool Tree::get_tree_prop ( std::string const &name, std::string &dest )
	{
		if ( name == "search-enabled" ) {
			dest = search_enabled ? "1" : "0";
		} else if ( name == "has-icons" ) {
			dest = has_icons ? "1" : "0";
		} else if ( name == "has-checkboxes" ) {
			dest = has_checks ? "1" : "0";
		} else
			return false;

		return true;
	}

	XguiRow * Tree::rowOf ( xgui::Model * node )
	{
		std::map<xgui::Model *, XguiRow *>::iterator found = rows.find(node);
		return found == rows.end() ? 0 : found->second;
	}

	GListStore * Tree::storeOf ( xgui::Model * parent )
	{
		if ( parent == this_tree->getModel() ) return root_store;
		std::map<xgui::Model *, GListStore *>::iterator found = child_stores.find(parent);
		return found == child_stores.end() ? 0 : found->second;
	}

	XguiRow * Tree::newRow ( xgui::Model * node )
	{
		XguiRow * row = xgui_row_new ( node );
		rows[node] = row;
		return row;
	}

	void Tree::forgetSubTree ( xgui::Model * node )
	{
		for ( xgui::Model::iterator c = node->begin(); c != node->end(); ++c )
			forgetSubTree ( dynamic_cast<xgui::Model *>(*c) );

		std::map<xgui::Model *, GListStore *>::iterator store = child_stores.find(node);
		if ( store != child_stores.end() ) {
			g_object_unref ( store->second );
			child_stores.erase ( store );
		}
		leaf_nodes.erase ( node );
		rows.erase ( node );
		delSelection ( node );
	}

	GListModel * Tree::childrenOf ( xgui::Model * node )
	{
		std::map<xgui::Model *, GListStore *>::iterator found = child_stores.find(node);
		if ( found != child_stores.end() ) return G_LIST_MODEL ( g_object_ref ( found->second ) );

		// A node without children is not expandable until it gets its first child
		if ( node->size() == 0 ) {
			leaf_nodes.insert ( node );
			return NULL;
		}

		GListStore * store = g_list_store_new ( XGUI_TYPE_ROW );
		for ( xgui::Model::iterator c = node->begin(); c != node->end(); ++c ) {
			XguiRow * row = newRow ( dynamic_cast<xgui::Model *>(*c) );
			g_list_store_append ( store, row );
			g_object_unref ( row );
		}
		child_stores[node] = store;
		return G_LIST_MODEL ( g_object_ref ( store ) );
	}

	GListModel * Tree::OnCreateChildren ( gpointer item, gpointer data )
	{
		Tree * tree = (Tree *)data;
		return tree->childrenOf ( xgui_row_get_node ( XGUI_ROW(item) ) );
	}

	GtkTreeListRow * Tree::listRowOf ( xgui::Model * node, bool expand_ancestors )
	{
		XguiRow * row = rowOf(node);
		xgui::Model * parent = row ? int_parent(node) : 0;
		if ( !parent ) return NULL;

		guint pos = 0;
		if ( parent == this_tree->getModel() ) {
			if ( !g_list_store_find ( root_store, row, &pos ) ) return NULL;
			return gtk_tree_list_model_get_child_row ( tree_model, pos );
		}

		GtkTreeListRow * parent_row = listRowOf ( parent, expand_ancestors );
		if ( !parent_row ) return NULL;
		if ( expand_ancestors ) gtk_tree_list_row_set_expanded ( parent_row, TRUE );

		GtkTreeListRow * result = NULL;
		GListStore * store = storeOf ( parent );
		if ( gtk_tree_list_row_get_expanded ( parent_row ) && store && g_list_store_find ( store, row, &pos ) )
			result = gtk_tree_list_row_get_child_row ( parent_row, pos );
		g_object_unref ( parent_row );
		return result;
	}

	void Tree::bindCell ( GtkLabel * label, GtkPicture * icon, XguiRow * row )
	{
		xgui::Model * node = xgui_row_get_node(row);
		gtk_label_set_text ( label, node->numberOfStrings() ? node->getString(0).c_str() : "" );
		applyLabelStyle ( label, this_tree->styleman_, node->getStyle() );

		GdkTexture * texture = has_icons ? rowIcon ( this_tree->styleman_, node->getStyle() ) : NULL;
		gtk_picture_set_paintable ( icon, texture ? GDK_PAINTABLE(texture) : NULL );
		gtk_widget_set_visible ( GTK_WIDGET(icon), texture != NULL );
	}

	void Tree::notifyRefresh ()
	{
		xgui::Callback * refresh_cb = this_tree->getEvent("onrefresh");
		if (refresh_cb) refresh_cb->call( this_tree );
	}

	void Tree::insertItem ( xgui::Model * parent, int child_pos, xgui::Model * child )
	{
		if ( (!root_store) || (!this_tree->getModel()) || rowOf(child) ) return;

		updating = true;
		GListStore * store = storeOf ( parent );
		if ( store ) {
			guint n = g_list_model_get_n_items ( G_LIST_MODEL(store) );
			guint pos = (child_pos < 0 || (guint)child_pos > n) ? n : (guint)child_pos;
			XguiRow * row = newRow ( child );
			g_list_store_insert ( store, pos, row );
			g_object_unref ( row );
		}
		else if ( leaf_nodes.count(parent) ) {
			// The parent was not expandable: replacing its row makes GTK ask for its children again
			leaf_nodes.erase ( parent );
			XguiRow * parent_row = rowOf ( parent );
			GListStore * parent_store = storeOf ( int_parent(parent) );
			guint pos = 0;
			if ( parent_row && parent_store && g_list_store_find ( parent_store, parent_row, &pos ) ) {
				bool was_selected = isSelectionCached ( parent );
				gpointer same[1] = { parent_row };
				g_list_store_splice ( parent_store, pos, 1, same, 1 );
				if ( was_selected ) {
					GtkTreeListRow * list_row = listRowOf ( parent, false );
					if ( list_row ) {
						gtk_single_selection_set_selected ( selection, gtk_tree_list_row_get_position ( list_row ) );
						g_object_unref ( list_row );
					}
				}
			}
		}
		// Otherwise GTK did not ask for the children of the parent yet: they are created from the model when it does
		updating = false;

		notifyRefresh();
	}

	void Tree::removeItem ( xgui::Model * parent, int child_pos, xgui::Model * child )
	{
		if ( !root_store ) return;

		XguiRow * row = rowOf ( child );
		GListStore * store = storeOf ( parent );
		guint pos = 0;
		bool found = row && store && g_list_store_find ( store, row, &pos );

		forgetSubTree ( child );
		updating = true;
		if ( found ) g_list_store_remove ( store, pos );
		updating = false;

		notifyRefresh();
	}

	void Tree::refresh()
	{
		xgui::Model * model = this_tree->getModel();
		if ( (!root_store) || (!model) ) return;

		updating = true;
		for ( std::map<xgui::Model *, GListStore *>::iterator s = child_stores.begin(); s != child_stores.end(); ++s )
			g_object_unref ( s->second );
		child_stores.clear();
		leaf_nodes.clear();
		rows.clear();

		std::vector<gpointer> items;
		for ( xgui::Model::iterator iter = model->begin(); iter != model->end(); ++iter )
			items.push_back ( newRow ( dynamic_cast<xgui::Model *>(*iter) ) );

		g_list_store_splice ( root_store, 0, g_list_model_get_n_items ( G_LIST_MODEL(root_store) ), items.data(), items.size() );
		for ( std::vector<gpointer>::iterator i = items.begin(); i != items.end(); ++i )
			g_object_unref ( *i );
		updating = false;

		notifyRefresh();
	}

	bool Tree::isSelected(xgui::Model * model, int pos)
	{
		GtkTreeListRow * row = listRowOf ( model->getChild(pos), false );
		if ( !row ) return false;
		bool selected = gtk_single_selection_get_selected ( selection ) == gtk_tree_list_row_get_position ( row );
		g_object_unref ( row );
		return selected;
	}

	void Tree::select(xgui::Model * parent, int pos, bool state)
	{
		GtkTreeListRow * row = listRowOf ( parent->getChild(pos), state );
		if ( !row ) return;

		guint position = gtk_tree_list_row_get_position ( row );
		g_object_unref ( row );
		if ( state )
			gtk_single_selection_set_selected ( selection, position );
		else if ( gtk_single_selection_get_selected ( selection ) == position )
			gtk_single_selection_set_selected ( selection, GTK_INVALID_LIST_POSITION );
	}

	void Tree::selectAll(xgui::Model * model, bool state)
	{
		// A single selection tree can only unselect everything
		if ( !state )
			gtk_single_selection_set_selected ( selection, GTK_INVALID_LIST_POSITION );
	}

	bool Tree::isChecked(xgui::Model * parent, int pos)
	{
		XguiRow * row = rowOf ( parent->getChild(pos) );
		return row && xgui_row_get_checked ( row );
	}

	void Tree::check(xgui::Model * parent, int pos, bool state)
	{
		XguiRow * row = rowOf ( parent->getChild(pos) );
		if ( row ) xgui_row_set_checked ( row, state );
	}

	void Tree::checkAll(xgui::Model * parent, bool state)
	{
		for (xgui::Model::iterator i = parent->begin(); i != parent->end(); ++i) {
			XguiRow * row = rowOf ( dynamic_cast<xgui::Model*>(*i) );
			if ( row ) xgui_row_set_checked ( row, state );
		}
	}

	void Tree::notifyCheck ( XguiRow * row )
	{
		xgui::Callback * cb = this_tree->getEvent("oncheck");
		if (!cb) return;

		xgui::TextStatusCallback * chk_cb = dynamic_cast<xgui::TextStatusCallback *>(cb);
		if (!chk_cb) {
			DMESSAGE("oncheck event of xgui::Tree expected a TextStatusCallback");
			return;
		}

		chk_cb->call(this_tree, xgui_row_get_node(row)->path(), xgui_row_get_checked(row));
	}

	void Tree::updateSelection ()
	{
		xgui::TextStatusCallback * sel_cb = 0;
		xgui::Callback * cb = this_tree->getEvent("onselect");
		if (!cb) cb = this_tree->getEvent("onchange");
		if (cb) {
			sel_cb = dynamic_cast<xgui::TextStatusCallback*>(cb);
			if (!sel_cb)
				DMESSAGE("onselect event of xgui::Tree expected a TextStatusCallback");
		}

		xgui::Model * selected = 0;
		GtkTreeListRow * list_row = GTK_TREE_LIST_ROW ( gtk_single_selection_get_selected_item ( selection ) );
		XguiRow * row = list_row ? XGUI_ROW ( gtk_tree_list_row_get_item ( list_row ) ) : 0;
		if ( row ) {
			selected = xgui_row_get_node ( row );
			g_object_unref ( row );
		}

		// Single selection: everything cached but the selected node was deselected
		std::vector<xgui::Model*> deselections;
		for ( std::set<xgui::Model*>::iterator c = selset_.begin(); c != selset_.end(); ++c )
			if ( *c != selected ) deselections.push_back(*c);

		for (std::vector<xgui::Model*>::iterator sel = deselections.begin(); sel != deselections.end(); ++sel)
			if ( !sel_cb || sel_cb->call( this_tree, (*sel)->path(), 0 ) )
				delSelection(*sel);

		if ( selected && !isSelectionCached(selected) )
			if ( !sel_cb || sel_cb->call( this_tree, selected->path(), 1 ) )
				addSelection(selected);
	}

	void Tree::OnSelectionChanged ( GObject * selection, GParamSpec * pspec, Tree * xt )
	{
		if ( xt->updating ) return;
		xt->updateSelection();
	}

	void Tree::watchRows ( guint position, guint added )
	{
		for ( guint i = position; i < position + added; ++i ) {
			GObject * row = G_OBJECT ( g_list_model_get_item ( G_LIST_MODEL(tree_model), i ) );
			if ( !g_object_get_data ( row, WATCHED_KEY ) ) {
				g_object_set_data ( row, WATCHED_KEY, GINT_TO_POINTER(1) );
				g_object_set_data ( row, EXPANDED_KEY, int_expanded_data ( gtk_tree_list_row_get_expanded ( GTK_TREE_LIST_ROW(row) ) ) );
				g_signal_connect ( row, "notify::expanded", G_CALLBACK ( OnRowExpanded ), this );
			}
			g_object_unref ( row );
		}
	}

	void Tree::OnRowsChanged ( GListModel * model, guint position, guint removed, guint added, Tree * xt )
	{
		xt->watchRows ( position, added );
	}

	void Tree::OnRowExpanded ( GtkTreeListRow * list_row, GParamSpec * pspec, Tree * xt )
	{
		// GTK also notifies leaves and the rows of a collapsing parent: only real changes are reported
		if ( !gtk_tree_list_row_is_expandable ( list_row ) ) return;
		gpointer state = int_expanded_data ( gtk_tree_list_row_get_expanded ( list_row ) );
		if ( g_object_get_data ( G_OBJECT(list_row), EXPANDED_KEY ) == state ) return;
		g_object_set_data ( G_OBJECT(list_row), EXPANDED_KEY, state );

		xgui::Callback * cb = xt->this_tree->getEvent("onexpand");
		if (!cb) return;

		xgui::TextStatusCallback * exp_cb = dynamic_cast<xgui::TextStatusCallback*>(cb);
		if (!exp_cb) {
			DMESSAGE("onexpand event of xgui::Tree expected a TextStatusCallback");
			return;
		}

		XguiRow * row = XGUI_ROW ( gtk_tree_list_row_get_item ( list_row ) );
		if ( !row ) return;
		xgui::Model * node = xgui_row_get_node ( row );
		g_object_unref ( row );

		exp_cb->call(xt->this_tree, node->path(), gtk_tree_list_row_get_expanded ( list_row ) ? 1 : 0);
	}

	void Tree::expand ( xgui::Model * node )
	{
		if (!node) return;
		GtkTreeListRow * row = listRowOf ( node, true );
		if ( !row ) return;
		gtk_tree_list_row_set_expanded ( row, TRUE );
		g_object_unref ( row );
	}

	void Tree::collapse ( xgui::Model * node )
	{
		if (!node) return;
		GtkTreeListRow * row = listRowOf ( node, false );
		if ( !row ) return;
		gtk_tree_list_row_set_expanded ( row, FALSE );
		g_object_unref ( row );
	}

	void Tree::sort ( xgui::Model * node, xgui::TextPairCallback * callback )
	{
		if ( !callback ) return;

		// Sorts the children of the node (the top level rows for the model or a NULL node)
		GListStore * store = storeOf ( node ? node : this_tree->getModel() );
		if ( !store ) return;

		TreeSortInformations si = { this_tree, callback };
		updating = true;
		g_list_store_sort ( store, int_sort_func, &si );
		updating = false;
	}
}
