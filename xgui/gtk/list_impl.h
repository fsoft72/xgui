//
// C++ Interface: list_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_LIST_IMPL_H_
#define _XGUI_LIST_IMPL_H_

#include "style.h"
#include "list.h"
#include "view_impl.h"

namespace xgui { class List; }

namespace xguimpl
{
	struct List : public View
	{
		GtkCellRenderer   *left_align_text_renderer;
		GtkCellRenderer   *center_align_text_renderer;
		GtkCellRenderer   *right_align_text_renderer;
		GtkCellRenderer   *icon_renderer;
		GtkCellRenderer   *check_renderer;

		GtkTreeView * list_view;

		int fg_column_id;
		int bg_column_id;
		int italic_column_id;
		int bold_column_id;
		int underline_column_id;
		int strike_column_id;
		int check_column_id;
		int icon_column_id;
		int first_fields_column;
		bool has_icons;
		bool has_checks;

		std::map<xgui::Model *, GtkTreeIter> iter_map;
		xgui::List * this_list;

		//methods
		List ( GtkWidget * real_w );
		List ( xgui::Container * parent );
		virtual ~List();
		virtual void setAccessor(xgui::Widget * w);


		bool get_list_prop ( std::string const &name, std::string &vals );
		bool set_list_prop ( std::string const &name, const std::string &vals );
		bool linkEvent( std::string const &name );
		void rebuildGtkModel();

		virtual void insertItem ( xgui::Model * parent, int child_pos, xgui::Model * child );
		virtual void removeItem ( xgui::Model * parent, int child_pos, xgui::Model * child );
		virtual void refresh();

		bool isSelected(xgui::Model * parent, int pos);
		void select(xgui::Model * parent, int pos, bool state);
		void selectAll(xgui::Model * parent, bool state);

		bool isChecked(xgui::Model * parent, int pos);
		void check(xgui::Model * parent, int pos, bool state);
		void checkAll(xgui::Model * parent, bool state);

		void addColumn ( int cpos, const std::string & cid, const std::string & cname, int cwidth, const std::string & calign );
		bool delColumn ( const std::string & id );
		void sort ( const std::string & column_id, xgui::TextPairCallback * callback );

		void swapCheck ( GtkTreePath * path );

		static void OnSelect ( GtkTreeSelection * sel, List *xl );
		static void OnHeaderClick ( GtkTreeViewColumn *tvc, List * l );
		static void OnCheck ( GtkCellRendererToggle *cell, char const * const path_string, List * xl);
	};
}

#endif
