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
#include "xgui_row.h"

#include <map>
#include <vector>

namespace xgui { class List; }

namespace xguimpl
{
	struct List;

	/** A data column of the list: the model string it shows and how. */
	struct ListColumn
	{
		List * list;
		std::string id;
		std::string title;
		/** Index of the model string shown by the column. */
		int field;
		float xalign;
		GtkColumnViewColumn * column;
	};

	struct List : public View
	{
		/** Rows in display order: model order, until sort() reorders them. */
		GListStore * store;
		GtkMultiSelection * selection;
		GtkColumnView * column_view;

		/** Row of every model node shown by the list. */
		std::map<xgui::Model *, XguiRow *> rows;
		/** Data columns, in no particular order. */
		std::vector<ListColumn *> columns;

		GtkColumnViewColumn * check_column;
		GtkColumnViewColumn * icon_column;
		bool has_icons;
		bool has_checks;
		bool search_enabled;

		xgui::List * this_list;

		//methods
		List ( GtkWidget * real_w );
		List ( xgui::Container * parent );
		virtual ~List();
		virtual void setAccessor(xgui::Widget * w);

		bool get_list_prop ( std::string const &name, std::string &vals );
		bool set_list_prop ( std::string const &name, const std::string &vals );
		bool linkEvent( std::string const &name );

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

		/** Returns the data column with the given id (or title), or NULL. */
		ListColumn * findColumn ( std::string const &name );
		/** Returns the row of a model node, or NULL. */
		XguiRow * rowOf ( xgui::Model * node );
		/** Returns the position of a row in the store, or -1. */
		int positionOf ( XguiRow * row );
		/** Shows the text and style of a model node in the label of a cell. */
		void bindLabel ( GtkLabel * label, XguiRow * row, int field );
		/** Shows the icon of the style of a model node in the picture of a cell. */
		void bindIcon ( GtkPicture * picture, XguiRow * row );
		/** Reports a checkbox toggled by the user through the oncheck event. */
		void notifyCheck ( XguiRow * row );
		/** Reports the selection changes of a range of rows through onselect and updates the cache. */
		void updateSelection ( guint position, guint n_items );
		/** Calls the onrefresh event. */
		void notifyRefresh ();

		static void OnSelectionChanged ( GtkSelectionModel * model, guint position, guint n_items, List * xl );
		static void OnSorterChanged ( GtkSorter * sorter, GtkSorterChange change, List * xl );
	};
}

#endif
