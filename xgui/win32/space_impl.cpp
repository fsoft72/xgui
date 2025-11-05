//
// C++ Implementation: space_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "precomp.h"

namespace xguimpl
{
	Space::Space ( HWND real_w ) : Widget ( real_w )
	{
		LONG origWndProc = GetWindowLong ( widget, GWL_WNDPROC );
		SetProp ( widget, "xgui_orig_wnd_proc", reinterpret_cast<HANDLE>(origWndProc) );
		SetWindowLong ( widget, GWL_WNDPROC, (LONG)Master::subProc );
		
		DWORD style = GetClassLong ( widget, GCL_STYLE );
		SetClassLong ( widget, GCL_STYLE, style | CS_DBLCLKS );
	}

	Space::Space ( xgui::Container * parent, int min_width, int min_height ) :
		Widget ( MkSpace ( parent ) )
	{
		LONG origWndProc = GetWindowLong ( widget, GWL_WNDPROC );
		SetProp ( widget, "xgui_orig_wnd_proc", reinterpret_cast<HANDLE>(origWndProc) );
		SetWindowLong ( widget, GWL_WNDPROC, (LONG)Master::subProc );
		
		DWORD style = GetClassLong ( widget, GCL_STYLE );
		SetClassLong ( widget, GCL_STYLE, style | CS_DBLCLKS );

		SendMessage ( widget, WM_SETFONT, (WPARAM) GetStockObject ( DEFAULT_GUI_FONT ), MAKELPARAM ( TRUE, 0 ) );

		min_x = min_width;
		min_y = min_height;
	}


	Space::~Space()
	{
	}


	HWND Space::MkSpace ( xgui::Container * parent )
	{
		return CreateWindowEx( WS_EX_NOPARENTNOTIFY, "STATIC", "", WS_CHILD | WS_VISIBLE,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, parent->getImpl()->widget, NULL,
			MAIN_HINSTANCE, NULL );
	}
}
