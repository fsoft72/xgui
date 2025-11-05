//
// C++ Implementation: label_impl
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
	Label::Label ( HWND real_w ) : Widget ( real_w )
	{
		LONG origWndProc = GetWindowLong ( widget, GWL_WNDPROC );
		SetProp ( widget, "xgui_orig_wnd_proc", (HANDLE)origWndProc );
		SetWindowLong ( widget, GWL_WNDPROC, (LONG) Master::subProc );

		LONG style = GetClassLong ( widget, GCL_STYLE );
		SetClassLong ( widget, GCL_STYLE, style | CS_DBLCLKS );
	}

	Label::Label ( xgui::Container * parent, std::string const &text ) :
		Widget ( MkLabel ( parent, text ) )
	{
		LONG origWndProc = GetWindowLong ( widget, GWL_WNDPROC );
		SetProp ( widget, "xgui_orig_wnd_proc", (HANDLE)origWndProc );
		SetWindowLong ( widget, GWL_WNDPROC, (LONG) Master::subProc );

		LONG style = GetClassLong ( widget, GCL_STYLE );
		SetClassLong ( widget, GCL_STYLE, style | CS_DBLCLKS );

		SendMessage ( widget, WM_SETFONT, (WPARAM) GetStockObject ( DEFAULT_GUI_FONT ), MAKELPARAM ( TRUE, 0 ) );
		HDC hdc = GetDC ( widget );
		SIZE sz;
		GetTextExtentPoint32 ( hdc, text.c_str(), text.length(), &sz );
		ReleaseDC ( widget, hdc );

		min_x = sz.cx;
		min_y = sz.cy;
	}


	Label::~Label()
	{
	}


	HWND Label::MkLabel ( xgui::Container * parent, std::string const &text )
	{
		return CreateWindowEx( WS_EX_NOPARENTNOTIFY, "STATIC", text.c_str(), WS_VISIBLE | WS_CHILD |
			WS_CLIPSIBLINGS | SS_CENTER | ((text.find('\n') == std::string::npos) ? SS_CENTERIMAGE : 0),
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, (HWND)parent->getImpl()->widget,
			NULL, MAIN_HINSTANCE, NULL );
	}


	bool Label::set_label_prop ( std::string const &name, std::string const &vals )
	{
		if ( name == "alignment" ) {

			DWORD style = GetWindowLong ( widget, GWL_STYLE );
			style &= ~(SS_LEFT | SS_RIGHT | SS_CENTER);
			if ( vals == "center" ) style |= SS_CENTER;
			else if ( vals == "right" ) style |= SS_RIGHT;
			else style |= SS_LEFT;
			SetWindowLong ( widget, GWL_STYLE, style );

			InvalidateRect ( widget, NULL, TRUE );
			UpdateWindow ( widget );

		} else if ( name == "text" ) {

			LONG style = GetWindowLong ( widget, GWL_STYLE );
			if ( vals.find ( '\n' ) != std::string::npos )
				style &= ~(SS_CENTERIMAGE);
			else
				style |= SS_CENTERIMAGE;
			SetWindowLong ( widget, GWL_STYLE, style );

			set_widget_prop ( "text", vals );

		} else
			return false;

		return true;
	}


	bool Label::get_label_prop ( std::string const &name, std::string &vals )
	{
		if ( name == "alignment" ) {

			DWORD style = GetWindowLong ( widget, GWL_STYLE );
			if ((style & SS_CENTER) == SS_CENTER) vals = "center";
			else if ((style & SS_RIGHT) == SS_RIGHT) vals = "right";
			else vals = "left";

		} else if ( name == "text" ) {
			
			get_widget_prop ( "text", vals );

		} else
			return false;

		return true;
	}

}
