//
// C++ Interface: window_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_WINDOW_IMPL_H_
#define _XGUI_WINDOW_IMPL_H_

#include "widget_impl.h"

namespace xgui { class Window; class Image; }

namespace xguimpl
{
	struct Menu;

	struct Window : public Widget
	{
		int nc_x;
		int nc_y;

		HWND hStatus;
		HWND modal_parent;

		bool modal_running;
		bool shown;
		bool must_center;

		//methods
		DLLEXPORT Window ( xgui::Window * parent = 0 );
		DLLEXPORT Window ( HWND real_w );
		DLLEXPORT virtual ~Window();	

		DLLEXPORT void show();
	
		DLLEXPORT int modalRun();
		DLLEXPORT void modalReturn ( int response );
	
		DLLEXPORT bool addChild ( xgui::Widget * w );
		DLLEXPORT void delChild ( xgui::Widget * w );
	
		DLLEXPORT void maximize();
		DLLEXPORT void minimize();
		DLLEXPORT void restore();
		DLLEXPORT void centre();

		DLLEXPORT bool linkEvent( std::string const &name );

		DLLEXPORT bool get_window_prop ( std::string const &name, std::string &vals );
		DLLEXPORT bool set_window_prop ( std::string const &name, const std::string &vals );

		DLLEXPORT void setIcon ( xgui::Image * img );
	
		DLLEXPORT void giveSize ( std::pair<int, int> size );
		DLLEXPORT void recalcLayout();

		DLLEXPORT static int OnClose ( xgui::Window * w, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		DLLEXPORT static int OnResize ( xgui::Window * w, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		DLLEXPORT static int OnGetMinMaxInfo ( xgui::Window * w, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	};
}

#endif
