//
// C++ Implementation: widget_impl
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


static std::string map_virtual_key ( int vk, int code )
{
	std::string c = "";

	if ((vk >= '0' && vk <= '9') ||
		(vk >= 'A' && vk <= 'Z')) c = (char)vk;
	else {
		switch ( vk )
		{
			case VK_BACK: c = "BackSpace"; break;
			case VK_TAB: c = "Tab"; break;
			case VK_CLEAR: c = "Clear"; break;
			case VK_RETURN: c = "Return"; break;
			case VK_PAUSE: c = "Pause"; break;
			case VK_CAPITAL: c = "Caps_Lock"; break;
			case VK_ESCAPE: c = "Escape"; break;
			case VK_SPACE: c = " "; break;
			case VK_PRIOR: c = "Page_Up"; break;
			case VK_NEXT: c = "Page_Down"; break;
			case VK_END: c = "End"; break;
			case VK_HOME: c = "Home"; break;
			case VK_LEFT: c = "Left"; break;
			case VK_UP: c = "Up"; break;
			case VK_RIGHT: c = "Right"; break;
			case VK_DOWN: c = "Down"; break;
			case VK_SELECT: c = "Select"; break;
			case VK_PRINT: c = "Print"; break;
			case VK_INSERT: c = "Insert"; break;
			case VK_DELETE: c = "Delete"; break;
			case VK_LWIN: c = "Win-L"; break;
			case VK_RWIN: c = "Win-R"; break;
			case VK_APPS: c = "Apps"; break;
			case VK_SLEEP: c = "Sleep"; break;
			case VK_HELP: c = "Help"; break;
			case VK_EXECUTE: c = "Execute"; break;
			case VK_NUMPAD0: c = "KP_0"; break;
			case VK_NUMPAD1: c = "KP_1"; break;
			case VK_NUMPAD2: c = "KP_2"; break;
			case VK_NUMPAD3: c = "KP_3"; break;
			case VK_NUMPAD4: c = "KP_4"; break;
			case VK_NUMPAD5: c = "KP_5"; break;
			case VK_NUMPAD6: c = "KP_6"; break;
			case VK_NUMPAD7: c = "KP_7"; break;
			case VK_NUMPAD8: c = "KP_8"; break;
			case VK_NUMPAD9: c = "KP_9"; break;
			case VK_MULTIPLY: c = "KP_Multiply"; break;
			case VK_ADD: c = "KP_Add"; break;
			case VK_SEPARATOR: c = "KP_Separator"; break;
			case VK_SUBTRACT: c = "KP_Subtract"; break;
			case VK_DECIMAL: c = "KP_Decimal"; break;
			case VK_DIVIDE: c = "KP_Divide"; break;
			case VK_F1: c = "F1"; break;
			case VK_F2: c = "F2"; break;
			case VK_F3: c = "F3"; break;
			case VK_F4: c = "F4"; break;
			case VK_F5: c = "F5"; break;
			case VK_F6: c = "F6"; break;
			case VK_F7: c = "F7"; break;
			case VK_F8: c = "F8"; break;
			case VK_F9: c = "F9"; break;
			case VK_F10: c = "F10"; break;
			case VK_F11: c = "F11"; break;
			case VK_F12: c = "F12"; break;
			case VK_F13: c = "F13"; break;
			case VK_F14: c = "F14"; break;
			case VK_F15: c = "F15"; break;
			case VK_F16: c = "F16"; break;
			case VK_F17: c = "F17"; break;
			case VK_F18: c = "F18"; break;
			case VK_F19: c = "F19"; break;
			case VK_F20: c = "F20"; break;
			case VK_F21: c = "F21"; break;
			case VK_F22: c = "F22"; break;
			case VK_F23: c = "F23"; break;
			case VK_F24: c = "F24"; break;
			case VK_NUMLOCK: c = "Num_Lock"; break;
			case VK_SCROLL: c = "Scroll_Lock"; break;
			case VK_LSHIFT: c = "Shift_L"; break;
			case VK_RSHIFT: c = "Shift_R"; break;
			case VK_SHIFT: c = "Shift"; break;
			case VK_LCONTROL: c = "Control_L"; break;
			case VK_RCONTROL: c = "Control_R"; break;
			case VK_CONTROL: c = "Control"; break;
			case VK_LMENU: c = "Alt_L"; break;
			case VK_RMENU: c = "Alt_R"; break;
			case VK_MENU: c = "Alt"; break;
			case VK_OEM_MINUS: c = "minus"; break;
			case VK_OEM_PLUS: c = "plus"; break;
			case VK_OEM_COMMA: c = "comma"; break;
			case VK_OEM_PERIOD: c = "period"; break;

			default:
				{
					// GetKeyNameText requires proper LONG parameter cast
					char str[256];
					GetKeyNameTextA(static_cast<LONG>(code), str, sizeof(str) - 1);
					c = str;
				}
		}
	}

	return c;
}


namespace xguimpl
{
	Widget::Widget ( HWND real_w ) :
		this_widget(0), widget(real_w), redrawing(false), in_redraw(false),
		min_x(1), min_y(1), cur_x(0), cur_y(0), pos_x(0), pos_y(0),
		proxy_notify(0), mouse_in(false), h_resize(false), v_resize(false)

	{
		xgui::Master::Instance()->getImpl()->registerWidget ( this );

		h_events["ondestroy"] = OnDestroy;
		h_events["onmousedown"] = OnMouseDown;
		h_events["onmouseup"] = OnMouseUp;
		h_events["onmousedblclick"] = OnMouseDblClick;
	}

	Widget::~Widget()
	{
		xgui::Master::Instance()->getImpl()->unregisterWidget ( this );
	}


	bool Widget::isMouseInWindow()
	{
		RECT rc;
		GetClientRect ( widget, &rc );

		MapWindowPoints ( widget, NULL, (POINT *)(&rc), 2 );

		POINT pt;
		GetCursorPos ( &pt );

		return (pt.x >= rc.left && pt.x <= rc.right  &&
				pt.y >= rc.top && pt.y <= rc.bottom);
	}


	bool Widget::linkEvent(std::string const &name)
	{
		if ( name == "onfocus" )
			h_events["onfocus"] = OnFocus;
		else if ( name == "onblur" )
			h_events["onblur"] = OnBlur;
		else if ( name == "onmousedown" )
			h_events["onmousedown"] = OnMouseDown;
		else if ( name == "onmouseup" )
			h_events["onmouseup"] = OnMouseUp;
		else if ( name == "onmousedblclick" )
			h_events["onmousedblclick"] = OnMouseDblClick;
		else if ( name == "onmousemove" )
			h_events["onmousemove"] = OnMouseMove;
		else if ( name == "onmouseout" ) {
			h_events["onmousemove"] = OnMouseMove;
			h_events["onmouseout"] = OnMouseOut;
		} else if ( name == "onmouseover" ) {
			h_events["onmousemove"] = OnMouseMove;
		} else if ( name == "onhide" )
			h_events["onhide"] = OnHide;
		else if ( name == "onshow" )
			h_events["onshow"] = OnShow;
		else if ( name == "onenable" )
			h_events["onenable"] = OnEnable;
		else if ( name == "ondisable" )
			h_events["ondisable"] = OnDisable;
		else if ( name == "onresize" )
			h_events["onresize"] = OnResize;
		else if ( name == "onmove" )
			h_events["onmove"] = OnMove;
		else if ( name == "onkeydown" )
			h_events["onkeydown"] = OnKeyDown;
		else if ( name == "onkeyup" )
			h_events["onkeyup"] = OnKeyUp;
		else if ( name == "onkeypress" ) {
			h_events["onkeydown"] = OnKeyDown;
			h_events["onkeyup"] = OnKeyUp;
		} else
			return false;

		return true;
	}


	void Widget::recalcLayout()
	{
		
	}


	char const * Widget::bindWMCOMMAND ( WORD command_id )
	{
		command_id;
		return "";
	}


	char const * Widget::bindWMNOTIFY ( LPNMHDR nmhdr )
	{
		nmhdr;
		return "";
	}


	void Widget::giveSize(std::pair<int, int> size)
	{
		cur_x = size.first;
		cur_y = size.second;

		SetWindowPos ( widget, NULL, 0, 0, cur_x, cur_y, SWP_NOMOVE | SWP_NOZORDER );

		InvalidateRect ( widget, NULL, TRUE );
	}


	HWND Widget::getInternalAsParent()
	{
		return widget;
	}


	int Widget::OnFocus ( xgui::Widget * w, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		hWnd; msg; wParam; lParam;

		xgui::Callback * cb = w->getEvent ( "onfocus" );
		DMESSAGE ( "onfocus()\n" );
		if ( cb ) return ( cb->call ( dynamic_cast<xgui::Widget*>(w) ) );

		return 0;
	}


	int Widget::OnBlur ( xgui::Widget * w, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		hWnd; msg; wParam; lParam;

		xgui::Callback * cb = w->getEvent ( "onblur" );
		DMESSAGE ( "onblur()\n" );
		if ( cb ) return ( cb->call ( dynamic_cast<xgui::Widget*>(w) ) );

		return 0;
	}


	int Widget::OnMouseDown ( xgui::Widget * w, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		hWnd; msg;

		std::string button;
		int status = 0;

		switch (lParam)
		{
			case WM_LBUTTONDOWN: button = "left"; break;
			case WM_MBUTTONDOWN: button = "middle"; break;
			case WM_RBUTTONDOWN: button = "right"; break;
		}

		if (wParam & MK_LBUTTON) status |= xgui::mouseLeftMask;
		if (wParam & MK_MBUTTON) status |= xgui::mouseMiddleMask;
		if (wParam & MK_RBUTTON) status |= xgui::mouseRightMask;

		if (wParam & MK_CONTROL) status |= xgui::keyControlMask;
		if (wParam & MK_SHIFT) status |= xgui::keyShiftMask;

		SHORT s = GetKeyState(VK_MENU);
		if (s >> 15) status |= xgui::keyAltMask;

		xgui::Master::Instance()->getImpl()->mouse_down_widget = w;

		xgui::TextStatusCallback * cb = dynamic_cast<xgui::TextStatusCallback *>(w->getEvent ( "onmousedown" ));
		if ( cb ) return ( cb->call ( dynamic_cast<xgui::Widget*>(w), button, status ) );

		DMESSAGE ( "mousedown()\n" );

		return 0;
	}


	int Widget::OnMouseUp ( xgui::Widget * w, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		hWnd; msg;

		std::string button;
		int status = 0;

		switch (lParam)
		{
			case WM_LBUTTONUP: button = "left"; break;
			case WM_MBUTTONUP: button = "middle"; break;
			case WM_RBUTTONUP: button = "right"; break;
		}

		if (wParam & MK_LBUTTON) status |= xgui::mouseLeftMask;
		if (wParam & MK_MBUTTON) status |= xgui::mouseMiddleMask;
		if (wParam & MK_RBUTTON) status |= xgui::mouseRightMask;

		if (wParam & MK_CONTROL) status |= xgui::keyControlMask;
		if (wParam & MK_SHIFT) status |= xgui::keyShiftMask;

		SHORT s = GetKeyState(VK_MENU);
		if (s >> 15) status |= xgui::keyAltMask;

		if (xgui::Master::Instance()->getImpl()->mouse_down_widget == w)
		{
			xgui::TextStatusCallback * cb = dynamic_cast<xgui::TextStatusCallback *>(w->getEvent ( "onmouseclick" ));
			if ( cb ) return ( cb->call ( dynamic_cast<xgui::Widget*>(w), button, status ) );

			DMESSAGE ( "mouseclick()\n" );
		}

		xgui::Master::Instance()->getImpl()->mouse_down_widget = 0;

		xgui::TextStatusCallback * cb = dynamic_cast<xgui::TextStatusCallback *>(w->getEvent ( "onmouseup" ));
		if ( cb ) return ( cb->call ( dynamic_cast<xgui::Widget*>(w), button, status ) );

		DMESSAGE ( "mouseup()\n" );

		return 0;
	}


	int Widget::OnMouseDblClick ( xgui::Widget * w, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		hWnd; msg;

		std::string button;
		int status = 0;

		switch (lParam)
		{
			case WM_LBUTTONDBLCLK: button = "left"; break;
			case WM_MBUTTONDBLCLK: button = "middle"; break;
			case WM_RBUTTONDBLCLK: button = "right"; break;
		}

		if (wParam & MK_LBUTTON) status |= xgui::mouseLeftMask;
		if (wParam & MK_MBUTTON) status |= xgui::mouseMiddleMask;
		if (wParam & MK_RBUTTON) status |= xgui::mouseRightMask;

		if (wParam & MK_CONTROL) status |= xgui::keyControlMask;
		if (wParam & MK_SHIFT) status |= xgui::keyShiftMask;

		SHORT s = GetKeyState(VK_MENU);
		if (s >> 15) status |= xgui::keyAltMask;

		xgui::Master::Instance()->getImpl()->mouse_down_widget = 0;

		xgui::TextStatusCallback * cb = dynamic_cast<xgui::TextStatusCallback *>(w->getEvent ( "onmousedown" ));
		if ( cb ) return ( cb->call ( dynamic_cast<xgui::Widget*>(w), button, status ) );

		cb = dynamic_cast<xgui::TextStatusCallback *>(w->getEvent ( "onmousedblclick" ));
		if ( cb ) return ( cb->call ( dynamic_cast<xgui::Widget*>(w), button, status ) );

		DMESSAGE ( "mousedown()\n" );
		DMESSAGE ( "mousedblclick()\n" );

		return 0;
	}


	int Widget::OnMouseMove ( xgui::Widget * w, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		hWnd; msg; wParam;

		if (!w->getImpl()->mouse_in) {
			w->getImpl()->mouse_in = true;

			TRACKMOUSEEVENT evt;
			evt.cbSize = sizeof ( evt );
			evt.dwFlags = TME_LEAVE;
			evt.hwndTrack = w->getImpl()->widget;
			evt.dwHoverTime = HOVER_DEFAULT;
			_TrackMouseEvent ( &evt );

			xgui::Callback * cb = w->getEvent ( "onmouseover" );
			if ( cb ) return ( cb->call ( dynamic_cast<xgui::Widget*>(w) ) );

			DMESSAGE ( "onmouseover() - " << dynamic_cast<xgui::Widget*>(w)->className().c_str() );
		}

		RECT rc;
		GetWindowRect(w->getImpl()->widget, &rc);

		std::string position = xgui::semantic_cast<std::string>( std::make_pair(LOWORD(lParam) - rc.left,
			HIWORD(lParam) - rc.top ) );
		xgui::TextCallback * cb = dynamic_cast<xgui::TextCallback *>(w->getEvent ( "onmousemove" ));
		if ( cb ) return ( cb->call ( dynamic_cast<xgui::Widget*>(w), position ) );

		return 0;
	}


	int Widget::OnMouseOut ( xgui::Widget * w, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		hWnd; msg; wParam; lParam;

		xgui::Callback * cb = w->getEvent ( "onmouseout" );
		if ( cb ) return ( cb->call ( dynamic_cast<xgui::Widget*>(w) ) );

		DMESSAGE ( "onmouseout() - %s" << dynamic_cast<xgui::Widget*>(w)->className().c_str() );

		return 0;
	}


	int Widget::OnHide ( xgui::Widget * w, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		hWnd; msg; wParam; lParam;

		xgui::Callback * cb = w->getEvent ( "onhide" );
		if ( cb ) return ( cb->call ( dynamic_cast<xgui::Widget*>(w) ) );
		return 0;
	}


	int Widget::OnShow ( xgui::Widget * w, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		hWnd; msg; wParam; lParam;

		xgui::Callback * cb = w->getEvent ( "onshow" );
		if ( cb ) return ( cb->call ( dynamic_cast<xgui::Widget*>(w) ) );
		return 0;
	}


	int Widget::OnEnable ( xgui::Widget * w, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		hWnd; msg; wParam; lParam;

		xgui::Callback * cb = w->getEvent ( "onenable" );
		if ( cb ) return ( cb->call ( dynamic_cast<xgui::Widget*>(w) ) );
		return 0;
	}


	int Widget::OnDisable ( xgui::Widget * w, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		hWnd; msg; wParam; lParam;

		xgui::Callback * cb = w->getEvent ( "ondisable" );
		if ( cb ) return ( cb->call ( dynamic_cast<xgui::Widget*>(w) ) );
		return 0;
	}

	
	int Widget::OnResize ( xgui::Widget * w, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		hWnd; msg; wParam;

		if (w->getImpl()->in_redraw) return 0;

		std::string size = xgui::semantic_cast<std::string>( std::make_pair(LOWORD(lParam),
			HIWORD(lParam) ) );

		xgui::TextCallback * cb = dynamic_cast<xgui::TextCallback *>(w->getEvent ( "onresize" ));

		DMESSAGE ( "resize()\n" );

		if ( cb ) return ( cb->call ( dynamic_cast<xgui::Widget*>(w), size ) );

		return 0;
	}


	int Widget::OnMove ( xgui::Widget * w, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		hWnd; msg; wParam;

		std::string pos = xgui::semantic_cast<std::string>( std::make_pair(LOWORD(lParam),
			HIWORD(lParam) ) );

		xgui::TextCallback * cb = dynamic_cast<xgui::TextCallback *>(w->getEvent ( "onmove" ));

		DMESSAGE ( "onmove()\n" );

		if ( cb ) return ( cb->call ( dynamic_cast<xgui::Widget*>(w), pos ) );

		return 0;
	}


	int Widget::OnKeyDown ( xgui::Widget * w, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		hWnd; msg;

		std::string str = map_virtual_key ( wParam, lParam );
		if (str.empty()) return 0;

		if ( w->getImpl()->last_key_pressed != str ) {

			//nuovo tasto
			xgui::TextCallback * cb = dynamic_cast<xgui::TextCallback *>(w->getEvent ( "onkeydown" ));
			if ( cb ) cb->call ( dynamic_cast<xgui::Widget*>(w), str );

			w->getImpl()->last_key_pressed = str;

		}

		xgui::TextCallback * cb = dynamic_cast<xgui::TextCallback *>(w->getEvent ( "onkeypress" ));
		if ( cb ) return ( cb->call ( dynamic_cast<xgui::Widget*>(w), str ) );

		return 0;
	}


	int Widget::OnKeyUp ( xgui::Widget * w, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		hWnd; msg;

		w->getImpl()->last_key_pressed = "";

		std::string c = map_virtual_key ( wParam, lParam );
		if (c.empty()) return 0;

		xgui::TextCallback * cb = dynamic_cast<xgui::TextCallback *>(w->getEvent ( "onkeyup" ));
		if ( cb ) return ( cb->call ( dynamic_cast<xgui::Widget*>(w), c ) );

		return 0;
	}


	int Widget::OnDestroy ( xgui::Widget * w, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		hWnd; msg; wParam; lParam;

		int rv = 0;

		if ( xgui::Master::Instance()->getImpl()->mouse_down_widget == w )
			xgui::Master::Instance()->getImpl()->mouse_down_widget = 0;

		xgui::Callback * cb = w->getEvent ( "ondestroy" );
		if ( cb ) rv = cb->call ( dynamic_cast<xgui::Widget*>(w) );

		delete w;

		return rv;
	}


	bool Widget::get_widget_prop ( std::string const &name, std::string &vals )
	{
		if ( name == "text" ) {

			// Use modern C++ memory management with std::vector
			int len = GetWindowTextLength ( widget );
			std::vector<char> buffer(len + 1);
			GetWindowText ( widget, buffer.data(), len + 1 );
			vals = buffer.data();
		
		} else if ( name == "tooltip-text" ) {
		
			vals = tip_text;
		
		} else if ( name == "visible" ) {
			
			vals = IsWindowVisible(widget) ? "1" : "0";
		
		} else if ( name == "enabled" ) {

			vals = IsWindowEnabled ( widget ) ? "1" : "0";

		} else if ( name == "width" ) {

			RECT rc;
			GetClientRect ( widget, &rc );
			vals = xgui::semantic_cast<std::string>(rc.right);

		} else if ( name == "height" ) {

			RECT rc;
			GetClientRect ( widget, &rc );
			vals = xgui::semantic_cast<std::string>(rc.bottom);

		} else if ( name == "focused" ) {

			vals = (GetFocus() == widget) ? "1" : "0";

		} else
			return false;

		return true;
	}


	bool Widget::set_widget_prop ( std::string const &name, const std::string &vals )
	{
		if ( name == "text" ) {

			SetWindowText ( widget, vals.c_str() );

		} else if ( name == "tooltip-text" ) {
			
			tip_text = vals;
			set_tooltip_text ( widget, tip_text.c_str() );

		} else if ( name == "visible" ) {
			
			if ( vals == "1" )
				ShowWindow( widget, SW_SHOW );
			else if ( vals == "0" )
				ShowWindow( widget, SW_HIDE );

		} else if ( name == "enabled" ) {
			
			if ( vals == "1" )
				EnableWindow ( widget, TRUE );
			else if ( vals == "0" )
				EnableWindow ( widget, FALSE );

		} else if ( name == "focused" ) {

			if ( vals == "1" )
				SetFocus ( widget );
			else if ( vals == "0" )
				SetFocus ( NULL );

		} else
			return false;

		return true;
	}


	void Widget::set_tooltip_text ( HWND hWnd, const char * text )
	{
		TOOLINFO ti;

		ti.cbSize = sizeof(TOOLINFO);
		ti.uFlags = TTF_SUBCLASS | TTF_IDISHWND;
		ti.hwnd = hWnd;
		ti.hinst = xgui::Master::Instance()->getImpl()->hInstance;
		ti.uId = (UINT_PTR)hWnd;
		ti.lpszText = (char *)text;

		SendMessage( xgui::Master::Instance()->getImpl()->tip_wnd,
			TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &ti );
	}


	void Widget::setPos ( std::pair<int, int> pos )
	{
		pos_x = pos.first;
		pos_y = pos.second;
		SetWindowPos ( widget, NULL, pos_x, pos_y, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
	}


	std::pair<int, int> Widget::getMousePos()
	{
		RECT rc;
		POINT p;
		GetCursorPos(&p);
		GetWindowRect(widget, &rc);
		return std::make_pair(p.x - rc.left, p.y - rc.top);
	}


	void Widget::redraw()
	{
		recalcLayout();

		if ( cur_x < min_x || cur_y < min_y )
			SetWindowPos ( widget, NULL, 0, 0, min_x, min_y, SWP_NOMOVE | SWP_NOZORDER );

		PostMessage ( widget, WM_REDRAW, 0, 0 );
	}


	void Widget::setAccessor ( xgui::Widget * w )
	{
		this_widget = w;
	}

}
