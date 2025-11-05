//
// C++ Implementation: frame_impl
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
	static HWND MkFrame ( xgui::Container * parent, const std::string & label )
	{
		return CreateWindowEx ( WS_EX_CONTROLPARENT,
			"BUTTON", label.c_str(), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT, (HWND)parent->getImpl()->widget, NULL, MAIN_HINSTANCE, NULL );
	}

	Frame::Frame ( HWND real_w ) :
		Widget ( real_w ), child(0)
	{
		LONG origWndProc = GetWindowLong ( widget, GWL_WNDPROC );
		SetProp ( widget, "xgui_orig_wnd_proc", (HANDLE)origWndProc );
		SetWindowLong ( widget, GWL_WNDPROC, (LONG)Master::subProc );

		DWORD style = GetClassLong ( widget, GCL_STYLE );
		SetClassLong ( widget, GCL_STYLE, style | CS_DBLCLKS );

		h_events["onresize"] = OnResize;
		h_events["onmove"] = OnMove;
	}

	Frame::Frame ( xgui::Container * parent, const std::string & label ) :
		Widget ( MkFrame ( parent, label ) ), child(0)
	{
		LONG origWndProc = GetWindowLong ( widget, GWL_WNDPROC );
		SetProp ( widget, "xgui_orig_wnd_proc", reinterpret_cast<HANDLE>(origWndProc) );
		SetWindowLong ( widget, GWL_WNDPROC, (LONG)Master::subProc );

		DWORD style = GetClassLong ( widget, GCL_STYLE );
		SetClassLong ( widget, GCL_STYLE, style | CS_DBLCLKS );

		SendMessage ( widget, WM_SETFONT, (WPARAM) GetStockObject ( DEFAULT_GUI_FONT ), MAKELPARAM ( TRUE, 0 ) );

		HDC hdc = GetDC ( widget );
		SIZE sz;
		GetTextExtentPoint32 ( hdc, label.c_str(), label.size(), &sz );
		ReleaseDC ( widget, hdc );

		min_x = sz.cx + 6;
		min_y = sz.cy + 4;

		h_events["onresize"] = OnResize;
		h_events["onmove"] = OnMove;
	}

	Frame::~Frame()
	{
	}


	bool Frame::linkEvent ( std::string const &name )
	{
		if ( name == "onmove" )
			return true;
		else if ( name == "onresize" )
			return true;
		else
			return Widget::linkEvent(name);
	}


	bool Frame::addChild ( xgui::Widget * w )
	{
		if (child) return false;

		child = w;

		Widget * wid = w->getImpl();

		wid->h_resize = true;
		wid->v_resize = true;

		this_widget->getContext()->redraw();

		HDC hdc = GetDC ( wid->widget );
		SIZE sz;
		GetTextExtentPoint32 ( hdc, "X", 1, &sz );
		ReleaseDC ( widget, hdc );

		wid->setPos(std::make_pair(3 + pos_x, pos_y + sz.cy + 1));

		SetParent ( wid->widget, this_widget->getParent()->getImpl()->getInternalAsParent() );

		RECT rc;
		GetClientRect ( widget, &rc );
		rc.right -= 3 + pos_x;
		rc.bottom -= pos_y + sz.cy + 1;

		wid->giveSize ( std::make_pair(rc.right, rc.bottom) );

		return true;
	}


	void Frame::delChild ( xgui::Widget * w )
	{
		if ( w == child ) child = NULL;
	}


	void Frame::giveSize ( std::pair<int, int> size )
	{
		cur_x = size.first;
		cur_y = size.second;

		if ( !child ) return;

		HDC hdc = GetDC ( widget );
		SIZE sz;
		GetTextExtentPoint32 ( hdc, "X", 1, &sz );
		ReleaseDC ( widget, hdc );

		int w = cur_x - 6;
		int h = cur_y - sz.cy - 4;

		child->getImpl()->giveSize ( std::make_pair(w, h) );

		SetWindowPos ( widget, NULL, 0, 0, cur_x, cur_y, SWP_NOMOVE | SWP_NOZORDER );
	}


	void Frame::recalcLayout()
	{
		HDC hdc = GetDC ( widget );
		SIZE sz;
		GetTextExtentPoint32 ( hdc, "X", 1, &sz );
		ReleaseDC ( widget, hdc );
		min_y = sz.cy + 4;
		min_x = 4;

		if (child) {
			Widget * ci = child->getImpl();
			ci->recalcLayout();
		
			min_x += ci->min_x;
			min_y += ci->min_y;
		}
	}

	
	bool Frame::set_frame_prop ( std::string const &name, std::string const &vals )
	{
		if ( name == "alignment" ) {

			DWORD style = GetWindowLong ( widget, GWL_STYLE );
			style &= (BS_LEFT | BS_CENTER | BS_RIGHT);
			if ( vals == "right" ) style |= BS_RIGHT;
			else if ( vals == "center" ) style |= BS_CENTER;
			else style |= BS_LEFT;
			SetWindowLong ( widget, GWL_STYLE, style );

		} else if ( name == "text" ) {
			
			set_widget_prop ( "text", vals );

		} else
			return false;

		return true;
	}


	bool Frame::get_frame_prop ( std::string const &name, std::string &vals )
	{
		if ( name == "alignment" ) {

			DWORD style = GetWindowLong ( widget, GWL_STYLE );
			if ( ( style & BS_RIGHT ) == BS_RIGHT ) vals = "right";
			else if ( ( style & BS_CENTER ) == BS_CENTER ) vals = "center";
			else vals = "left";
		
		} else if ( name == "text" ) {
			
			get_widget_prop ( "text", vals );

		} else
			return false;

		return true;
	}


	HWND Frame::getInternalAsParent()
	{
		return this_widget->getParent()->getImpl()->getInternalAsParent();
	}


	xgui::Widget * Frame::getChild()
	{
		return child;
	}


	int Frame::OnMove ( xgui::Frame * frame, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		msg; wParam; lParam;

		Frame * f = dynamic_cast<Frame *>(frame->getImpl());

		if ( !f->child ) return FALSE;

		SIZE sz;
		HDC hdc = GetDC ( hWnd );
		GetTextExtentPoint32 ( hdc, "X", 1, &sz );
		ReleaseDC ( hWnd, hdc );

		xgui::Widget * w = f->child;
		HWND hw = w->getImpl()->widget;

		w->getImpl()->setPos ( std::make_pair ( f->pos_x + 3, f->pos_y + sz.cy + 1 ) );

		InvalidateRect ( hw, NULL, TRUE );
		UpdateWindow ( hw );

		return FALSE;
	}


	int Frame::OnResize ( xgui::Frame * f, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		f; msg; wParam; lParam;

		HDC hdc = GetDC ( hWnd );

		SIZE sz;
		GetTextExtentPoint32 ( hdc, "X", 1, &sz );

		RECT rc;
		GetClientRect ( hWnd, &rc );

		ExcludeClipRect ( hdc, 3, sz.cy + 1, rc.right - 3, rc.bottom - 3 );
		FillRect ( hdc, &rc, (HBRUSH)(COLOR_BTNFACE + 1) );

		ReleaseDC ( hWnd, hdc );
		
		return FALSE;
	}


	void Frame::setAccessor ( xgui::Widget * w )
	{
		this_frame = dynamic_cast<xgui::Frame *>(w);
		ASSERT(this_frame);

		Widget::setAccessor ( w );
	}
}
