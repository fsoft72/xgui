//
// C++ Interface: tree_impl
//
// Description:
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_TREE_IMPL_H_
#define _XGUI_TREE_IMPL_H_

#include "widget_impl.h"
#include "view_impl.h"
#include "xgui_row.h"
#include "callback.h"

#include <map>
#include <set>

namespace xgui { class Window; class Tree; }

namespace xguimpl
{
	struct Tree : public View
	{
		/** Rows of the top level nodes. */
		GListStore * root_store;
		/** Rows of the children of a node, created when GTK first asks for them. */
		std::map<xgui::Model *, GListStore *> child_stores;
		/** Nodes GTK asked children for while they had none: their row is not expandable. */
		std::set<xgui::Model *> leaf_nodes;
		/** Row of every model node shown by the tree. */
		std::map<xgui::Model *, XguiRow *> rows;

		GtkTreeListModel * tree_model;
		GtkSingleSelection * selection;
		GtkColumnView * column_view;
		GtkColumnViewColumn * check_column;
		GtkColumnViewColumn * text_column;

		xgui::Tree * this_tree;
		bool has_icons;
		bool has_checks;
		bool search_enabled;
		/** True while the rows are changed by the tree itself: selection changes are not reported. */
		bool updating;

		//methods
		Tree ( GtkWidget * real_w );
		Tree ( xgui::Container * parent );
		virtual ~Tree();

		virtual void setAccessor ( xgui::Widget * w );

		bool linkEvent( std::string const &name );

		bool set_tree_prop ( std::string const &name, std::string const &val );
		bool get_tree_prop ( std::string const &name, std::string &dest );

		void refresh();
		void insertItem ( xgui::Model * parent, int child_pos, xgui::Model * child );
		void removeItem ( xgui::Model * parent, int child_pos, xgui::Model * child );

		void sort ( xgui::Model * node, xgui::TextPairCallback * callback );
		void expand ( xgui::Model * node );
		void collapse ( xgui::Model * node );

		bool isSelected(xgui::Model * parent, int pos);
		void select(xgui::Model * parent, int pos, bool state);
		void selectAll(xgui::Model * parent, bool state);

		bool isChecked(xgui::Model * parent, int pos);
		void check(xgui::Model * parent, int pos, bool state);
		void checkAll(xgui::Model * parent, bool state);

		/** Returns the row of a model node, or NULL. */
		XguiRow * rowOf ( xgui::Model * node );
		/** Returns the store holding the rows of the children of a node (the root store for the model), or NULL. */
		GListStore * storeOf ( xgui::Model * parent );
		/** Creates the row of a node and remembers it (its children get rows when GTK asks for them). */
		XguiRow * newRow ( xgui::Model * node );
		/** Forgets the rows of a node and of all its descendants. */
		void forgetSubTree ( xgui::Model * node );
		/** Returns the children store of a node, creating it from the model; NULL for a node without children. */
		GListModel * childrenOf ( xgui::Model * node );
		/** Returns the tree row of a node, expanding its ancestors if requested; NULL if it is not reachable. Returns a new reference. */
		GtkTreeListRow * listRowOf ( xgui::Model * node, bool expand_ancestors );
		/** Shows the text, style and icon of a model node in a cell. */
		void bindCell ( GtkLabel * label, GtkPicture * icon, XguiRow * row );
		/** Reports a checkbox toggled by the user through the oncheck event. */
		virtual void notifyCheck ( XguiRow * row );
		/** Reports the selection changes through onselect and updates the cache. */
		void updateSelection ();
		/** Calls the onrefresh event. */
		void notifyRefresh ();
		/** Watches the expansion of the tree rows that appeared in the flattened model. */
		void watchRows ( guint position, guint added );

		static GListModel * OnCreateChildren ( gpointer item, gpointer data );
		static void OnSelectionChanged ( GObject * selection, GParamSpec * pspec, Tree * xt );
		static void OnRowsChanged ( GListModel * model, guint position, guint removed, guint added, Tree * xt );
		static void OnRowExpanded ( GtkTreeListRow * row, GParamSpec * pspec, Tree * xt );
	};
}

#endif
