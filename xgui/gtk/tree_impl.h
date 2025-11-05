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
#include "callback.h"

namespace xgui { class Window; }

namespace xguimpl
{
	struct Tree : public View
	{
		GtkTreeView * tree_view;
		std::map < xgui::Model *, GtkTreeIter> tree_map;

		xgui::Tree * this_tree;
		bool has_icons;
		bool has_checks;

		int fg_column_id;
		int bg_column_id;
		int italic_column_id;
		int bold_column_id;
		int underline_column_id;
		int strike_column_id;
		int check_column_id;
		int icon_column_id;
		int field_column_id;

		GtkCellRenderer   *left_align_text_renderer;
		GtkCellRenderer   *center_align_text_renderer;
		GtkCellRenderer   *right_align_text_renderer;
		GtkCellRenderer   *icon_renderer;
		GtkCellRenderer   *check_renderer;
		
		//methods
		Tree ( GtkWidget * real_w );
		Tree ( xgui::Container * parent );
		virtual ~Tree();

		virtual void setAccessor ( xgui::Widget * w );

		void buildGtkModel();
		bool linkEvent( std::string const &name );

		bool set_tree_prop ( std::string const &name, std::string const &val );
		bool get_tree_prop ( std::string const &name, std::string &dest );

		void insertSubTree(xgui::Model * parent, int child_pos, xgui::Model * child);
		void removeSubTree ( xgui::Model * child );
		void selectSubTree(GtkTreeSelection * sel, xgui::Model * model, int child_pos, xgui::TextStatusCallback *);
		void swapCheck ( GtkTreePath * path );
	
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

		static void OnSelect ( GtkTreeSelection * sel, Tree *xl );
		static void OnCheck ( GtkCellRendererToggle *cell, char const * const path_string, Tree * xl);
		static void OnExpand ( GtkTreeView *treeview, GtkTreeIter *arg1, GtkTreePath *arg2, Tree * xl);
		static void OnCollapse ( GtkTreeView *treeview, GtkTreeIter *arg1, GtkTreePath *arg2, Tree * xl);
	};
}

#endif
