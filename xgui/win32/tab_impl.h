//
// C++ Interface: tab_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_TAB_IMPL_H_
#define _XGUI_TAB_IMPL_H_

#include "widget_impl.h"

namespace xguimpl
{
	struct Tab : public Widget
	{
		xgui::Widget * current;
		int page_count;
		HIMAGELIST hImageList;

		xgui::Tab * this_tab;

		//methods
		DLLEXPORT Tab ( xgui::Container * parent, const std::string & alignment );
		DLLEXPORT Tab ( HWND real_w );
		DLLEXPORT virtual ~Tab();

		DLLEXPORT virtual void setAccessor ( xgui::Widget * w );

		DLLEXPORT bool linkEvent ( std::string const &name );
		DLLEXPORT char const * bindWMNOTIFY ( LPNMHDR nmhdr );

		DLLEXPORT bool addChild ( xgui::Widget * w );
		DLLEXPORT void delChild ( xgui::Widget * w );

		DLLEXPORT void giveSize ( std::pair<int, int> size );
		DLLEXPORT void recalcLayout();

		DLLEXPORT bool get_tab_prop ( std::string const &name, std::string &vals );
		DLLEXPORT bool set_tab_prop ( std::string const &name, std::string const &vals );

		DLLEXPORT HWND getInternalAsParent();

		DLLEXPORT void setPageIcon ( xgui::Widget * w, xgui::Image * img );
		DLLEXPORT void setPageLabel ( xgui::Widget * w, const std::string & label );

		DLLEXPORT static int OnMove ( xgui::Tab * t, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		DLLEXPORT static int OnSelect ( xgui::Tab * t, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	};
}

#endif
