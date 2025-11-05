//
// C++ Implementation: button_impl
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
	static HWND MkButton ( xgui::Container * parent, std::string const &text )
	{
		HWND hParent = parent ? parent->getImpl()->widget : NULL;

		return CreateWindowEx( WS_EX_NOPARENTNOTIFY, "BUTTON", text.c_str(),
			WS_TABSTOP | WS_CHILD | WS_VISIBLE,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hParent, NULL,
			MAIN_HINSTANCE, NULL );
	}


	Button::Button ( HWND real_w ) : Widget ( real_w )
	{
		LONG origWndProc = GetWindowLong ( widget, GWL_WNDPROC );
		SetProp ( widget, "xgui_orig_wnd_proc", reinterpret_cast<HANDLE>(origWndProc) );
		SetWindowLong ( widget, GWL_WNDPROC, (LONG)Master::subProc );

		DWORD style = GetClassLong ( widget, GCL_STYLE );
		SetClassLong ( widget, GCL_STYLE, style | CS_DBLCLKS );
	}

	Button::Button ( xgui::Container * parent, std::string const &text, xgui::Image * image,
        bool toggle_button ) :
		Widget ( MkButton ( parent, text ) )
	{
		LONG origWndProc = GetWindowLong ( widget, GWL_WNDPROC );
		SetProp ( widget, "xgui_orig_wnd_proc", reinterpret_cast<HANDLE>(origWndProc) );
		SetWindowLong ( widget, GWL_WNDPROC, (LONG)Master::subProc );

		LONG style = WS_TABSTOP | WS_VISIBLE | BS_NOTIFY | WS_CLIPSIBLINGS | WS_CHILD |
			BS_MULTILINE | BS_CENTER | BS_VCENTER;
		if ( image ) style |= BS_BITMAP;
		if ( toggle_button ) style |= BS_PUSHLIKE | BS_AUTOCHECKBOX;
		SetWindowLong( widget, GWL_STYLE, style );

		style = GetClassLong ( widget, GCL_STYLE );
		SetClassLong ( widget, GCL_STYLE, style | CS_DBLCLKS );

		if ( image ) {
			HBITMAP hBmp = image->getImpl()->getButtonBitmap();
			if ( image ) SendMessage ( widget, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBmp );
		}

		SendMessage ( widget, WM_SETFONT, (WPARAM) GetStockObject ( DEFAULT_GUI_FONT ), MAKELPARAM ( TRUE, 0 ) );

		SIZE sz;
		if ( image ) {
			sz.cx = image->getWidth();
			sz.cy = image->getHeight();
		} else {
			HDC hdc = GetDC ( widget );
			GetTextExtentPoint32 ( hdc, text.c_str(), text.size(),  &sz );
			ReleaseDC ( widget, hdc );
		}

		min_x = sz.cx + 8;
		min_y = sz.cy + 6;
	}


	Button::~Button()
	{
		HBITMAP hBmp = (HBITMAP)SendMessage ( widget, BM_GETIMAGE, IMAGE_BITMAP, 0 );
		if ( hBmp ) DeleteObject ( hBmp );
	}


	char const * Button::bindWMCOMMAND ( WORD command_id )
	{
		switch ( command_id ) 
		{
			case BN_CLICKED: return "onclick";
		}

		return "";
	}


	bool Button::linkEvent( std::string const &name )
	{
		if ( name == "onclick" )
			h_events["onclick"] = OnClick;
		else
			return Widget::linkEvent(name);

		return true;
	}


	void Button::setIcon ( xgui::Image * img )
	{
		xgui::Button * btn = dynamic_cast<xgui::Button *>(this_widget);

		if (img) {
			HBITMAP hBmp = btn->icon_->getImpl()->getButtonBitmap();
			SendMessage ( widget, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBmp );
			DeleteObject ( hBmp );
			min_x = btn->icon_->getWidth() + 6;
			min_y = btn->icon_->getHeight() + 8;
			MASTER_INSTANCE->redrawTopLevels();

			DWORD style = GetWindowLong(widget, GWL_STYLE);
			SetWindowLong(widget, GWL_STYLE, style | BS_BITMAP);
		} else {
			DWORD style = GetWindowLong(widget, GWL_STYLE);
			SetWindowLong(widget, GWL_STYLE, style & ~BS_BITMAP);
		}
	}


	bool Button::set_button_prop ( std::string const &name, std::string const &vals )
	{
		if ( name == "alignment" ) {

			DWORD style = GetWindowLong(widget, GWL_STYLE);
			style &= ~(BS_LEFT | BS_RIGHT | BS_CENTER | BS_TOP | BS_BOTTOM | BS_VCENTER);

			if ( vals == "top" ) style |= BS_TOP | BS_CENTER;
			else if ( vals == "top-left" ) style |= BS_TOP | BS_LEFT;
			else if ( vals == "top-right" ) style |= BS_TOP | BS_RIGHT;
			else if ( vals == "left" ) style |= BS_LEFT | BS_VCENTER;
			else if ( vals == "right" ) style |= BS_RIGHT | BS_VCENTER;
			else if ( vals == "bottom" ) style |= BS_CENTER | BS_BOTTOM;
			else if ( vals == "bottom-left" ) style |= BS_BOTTOM | BS_LEFT;
			else if ( vals == "bottom-right" ) style |= BS_BOTTOM | BS_RIGHT;
			else style |= BS_CENTER | BS_VCENTER;

			SetWindowLong(widget, GWL_STYLE, style);
			InvalidateRect(widget, NULL, TRUE);
			UpdateWindow(widget);

		} else if ( name == "pressed" ) {

			SendMessage ( widget, BM_SETCHECK, vals == "1" ? BST_CHECKED : BST_UNCHECKED, 0 );
		
		} else
			return false;

		return true;
	}


	bool Button::get_button_prop ( std::string const &name, std::string &vals )
	{
		if ( name == "alignment" ) {

			vals = "";

			DWORD style = GetWindowLong(widget, GWL_STYLE);
			if ((style & BS_VCENTER) != BS_VCENTER) {
				if (style & BS_TOP) vals = "top-";
				else if (style & BS_BOTTOM) vals = "bottom-";
			}

			if ((style & BS_CENTER) == BS_CENTER) vals += "center";
			else if (style & BS_LEFT) vals += "left";
			else vals += "right";

		} else if ( name == "pressed" ) {

			DWORD ret = SendMessage ( widget, BM_GETCHECK, 0, 0 );
			vals = ( ret == BST_CHECKED ) ? "1" : "0";

		} else
			return false;

		return true;
	}


	int Button::OnClick ( xgui::Button * b, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		hWnd; msg; wParam; lParam;

		xgui::Callback  * cb =  b->getEvent("onclick");
		if ( cb ) cb->call ( dynamic_cast<xgui::Widget *>(b) );
		return 0;
	}

}
