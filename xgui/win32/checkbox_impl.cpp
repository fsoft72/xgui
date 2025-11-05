//
// C++ Implementation: checkbox_impl
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
	static HWND MkCheckbox ( xgui::Container * parent, std::string const &text )
	{
		return CreateWindowEx( WS_EX_NOPARENTNOTIFY, "BUTTON", text.c_str(),
			WS_TABSTOP | WS_VISIBLE | BS_NOTIFY | WS_CLIPSIBLINGS | WS_CHILD | BS_AUTOCHECKBOX,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, (HWND)parent->getImpl()->widget,
			NULL, MAIN_HINSTANCE, NULL );
	}


	Checkbox::Checkbox ( HWND real_w ) : Widget ( real_w )
	{
		LONG origWndProc = GetWindowLong ( widget, GWL_WNDPROC );
		SetProp ( widget, "xgui_orig_wnd_proc", reinterpret_cast<HANDLE>(origWndProc) );
		SetWindowLong ( widget, GWL_WNDPROC, (LONG)Master::subProc );

		DWORD style = GetClassLong ( widget, GCL_STYLE );
		SetClassLong ( widget, GCL_STYLE, style | CS_DBLCLKS );
	}

	Checkbox::Checkbox ( xgui::Container * parent, std::string const &text ) :
		Widget ( MkCheckbox ( parent, text ) )
	{
		LONG origWndProc = GetWindowLong ( widget, GWL_WNDPROC );
		SetProp ( widget, "xgui_orig_wnd_proc", reinterpret_cast<HANDLE>(origWndProc) );
		SetWindowLong ( widget, GWL_WNDPROC, (LONG)Master::subProc );

		DWORD style = GetClassLong ( widget, GCL_STYLE );
		SetClassLong ( widget, GCL_STYLE, style | CS_DBLCLKS );

		SendMessage ( widget, WM_SETFONT, (WPARAM) GetStockObject ( DEFAULT_GUI_FONT ), MAKELPARAM ( TRUE, 0 ) );

		SIZE sz;
		HDC hdc = GetDC ( widget );
		GetTextExtentPoint32 ( hdc, text.c_str(), text.size(),  &sz );
		ReleaseDC ( widget, hdc );

		min_x = sz.cx + 20;
		min_y = sz.cy;
	}


	Checkbox::~Checkbox()
	{
	}


	char const * Checkbox::bindWMCOMMAND ( WORD command_id )
	{
		switch ( command_id ) 
		{
			case BN_CLICKED: return "onclick";
		}

		return "";
	}


	bool Checkbox::linkEvent ( std::string const &name )
	{
		if ( name == "onclick" )
			h_events["onclick"] = OnClick;
		else
			return Widget::linkEvent(name);

		return true;
	}


	bool Checkbox::set_checkbox_prop ( std::string const &name, std::string const &vals )
	{
		if ( name == "alignment" ) {

			DWORD style = GetWindowLong(widget, GWL_STYLE);
			style &= ~(BS_LEFT | BS_RIGHT | BS_CENTER | BS_TOP | BS_BOTTOM | BS_VCENTER);

			if ( vals == "left" ) style |= BS_LEFTTEXT;
			else style &= ~BS_LEFTTEXT;

			SetWindowLong(widget, GWL_STYLE, style);
			
			InvalidateRect(widget, NULL, TRUE);
			UpdateWindow(widget);

		} else if ( name == "checked" ) {

			SendMessage ( widget, BM_SETCHECK, vals == "1" ? BST_CHECKED : BST_UNCHECKED, 0 );
		
		} else
			return false;

		return true;
	}


	bool Checkbox::get_checkbox_prop ( std::string const &name, std::string &vals )
	{
		if ( name == "alignment" ) {

			vals = "";

			DWORD style = GetWindowLong(widget, GWL_STYLE);
			if ((style & BS_LEFTTEXT) == BS_LEFTTEXT)
				vals = "left";
			else
				vals = "right";

		} else if ( name == "checked" ) {

			DWORD ret = SendMessage ( widget, BM_GETCHECK, 0, 0 );
			vals = ( ret == BST_CHECKED ) ? "1" : "0";

		} else
			return false;

		return true;
	}


	int Checkbox::OnClick ( xgui::Checkbox * c, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		hWnd; msg; wParam; lParam;

        xgui::TextCallback  * cb = dynamic_cast<xgui::TextCallback *>(c->getEvent("onclick"));
		DWORD ret = SendMessage ( c->getImpl()->widget, BM_GETCHECK, 0, 0 );
		if ( cb ) cb->call ( dynamic_cast<xgui::Widget *>(c), (ret == BST_CHECKED) ? c->value_ : "" );

		return 0;	
	}
}
