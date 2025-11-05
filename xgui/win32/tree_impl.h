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

namespace xgui { class Window; }

namespace xguimpl
{
	struct Tree : public View
	{
		xgui::TextPairCallback * sort_callback;
		std::map < xgui::Model *, HTREEITEM > tree_map;

		xgui::Tree * this_tree;

		//methods
		DLLEXPORT Tree ( HWND real_w );
		DLLEXPORT Tree ( xgui::Container * parent );
		DLLEXPORT virtual ~Tree();

		DLLEXPORT virtual void setAccessor ( xgui::Widget * w );

		DLLEXPORT bool linkEvent( std::string const &name );
		DLLEXPORT char const * bindWMNOTIFY ( LPNMHDR nmhdr );

		DLLEXPORT bool set_tree_prop ( std::string const &name, std::string const &val );
		DLLEXPORT bool get_tree_prop ( std::string const &name, std::string &dest );

		DLLEXPORT void refresh();
		DLLEXPORT void insertItem ( xgui::Model * parent, int child_pos, xgui::Model * child );
		DLLEXPORT void removeItem ( xgui::Model * parent, int child_pos, xgui::Model * child );

		DLLEXPORT bool isSelected ( xgui::Model * parent, int child_pos );
		DLLEXPORT void select ( xgui::Model * parent, int child_pos, bool state );
		DLLEXPORT void selectAll ( xgui::Model * parent, bool state );

		DLLEXPORT void sort ( xgui::Model * node, xgui::TextPairCallback * callback );
		DLLEXPORT void expand ( xgui::Model * node );
		DLLEXPORT void collapse ( xgui::Model * node );

		DLLEXPORT bool isChecked ( xgui::Model * parent, int child_pos );
		DLLEXPORT void check ( xgui::Model * parent, int child_pos, bool state );
		DLLEXPORT void checkAll ( xgui::Model * parent, bool state );

		DLLEXPORT void _insertSubTree ( xgui::Model * parent, xgui::Model * model );

		DLLEXPORT static int CALLBACK CompareFunc ( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort );
		DLLEXPORT static int OnRefresh ( xgui::Tree * t );
		DLLEXPORT static int OnSelect ( xgui::Tree * t, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, bool deselect );
		DLLEXPORT static int OnCheck ( xgui::Tree * t, xgui::Model * model, bool uncheck );
		DLLEXPORT static int OnExpand ( xgui::Tree * t, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		DLLEXPORT static int OnCustomDraw ( xgui::Tree * t, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	};
}

#endif
