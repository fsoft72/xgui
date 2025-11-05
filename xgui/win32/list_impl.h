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

#include "widget_impl.h"

namespace xgui { class Window; }

namespace xguimpl
{
	struct List : public View
	{
		int sort_column;
		xgui::TextPairCallback * sort_callback;

		IdMap col_map;

		xgui::List * this_list;

		int		last_sel_status;
		xgui::Model * last_sel_model;

		//methods
		DLLEXPORT List ( HWND real_w );
		DLLEXPORT List ( xgui::Container * parent );
		DLLEXPORT virtual ~List();

		DLLEXPORT void setAccessor ( xgui::Widget * w );

		DLLEXPORT bool linkEvent( std::string const &name );
		DLLEXPORT char const * bindWMNOTIFY ( LPNMHDR nmhdr );

		DLLEXPORT bool get_list_prop ( std::string const &name, std::string &vals );
		DLLEXPORT bool set_list_prop ( std::string const &name, const std::string &vals );

		DLLEXPORT void refresh();
		DLLEXPORT void insertItem ( xgui::Model * parent, int child_pos, xgui::Model * child );
		DLLEXPORT void removeItem ( xgui::Model * parent, int child_pos, xgui::Model * child );

		DLLEXPORT bool isSelected ( xgui::Model * parent, int child_pos );
		DLLEXPORT void select ( xgui::Model * parent, int child_pos, bool state );
		DLLEXPORT void selectAll ( xgui::Model * parent, bool state );

		DLLEXPORT void addColumn ( int cpos, const std::string & cid, const std::string & cname, int cwidth, const std::string & calign );
		DLLEXPORT bool delColumn ( const std::string & id );
		DLLEXPORT void sort ( const std::string & column_id, xgui::TextPairCallback * callback );

		DLLEXPORT bool isChecked ( xgui::Model * parent, int child_pos );
		DLLEXPORT void check ( xgui::Model * parent, int child_pos, bool state );
		DLLEXPORT void checkAll ( xgui::Model * parent, bool state );

		DLLEXPORT static int CALLBACK CompareFunc ( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort );
		DLLEXPORT static int OnRefresh ( xgui::List * l );
		DLLEXPORT static int OnSelect ( xgui::List * l, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		DLLEXPORT static int OnDeselect ( xgui::List * l, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		DLLEXPORT static int OnCheck ( xgui::List * l, xgui::Model * model, bool uncheck );
		DLLEXPORT static int OnHeaderClick ( xgui::List * l, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		DLLEXPORT static int OnCustomDraw ( xgui::List * l, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	};
}

#endif
