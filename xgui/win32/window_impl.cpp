//
// C++ Implementation: window_impl
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
	static HWND MkWindow ( xgui::Window * parent )
	{
		HWND hParent = parent ? parent->getImpl()->widget : NULL;

		return CreateWindowEx ( WS_EX_CONTROLPARENT,
			"XGUIWindowClass", "", WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT, hParent, NULL, xgui::Master::Instance()->getImpl()->hInstance, NULL );
	}

	Window::Window ( HWND real_w ) :
		Widget ( real_w ),
		nc_x(0), nc_y(0), hStatus(0), modal_running(false), shown(false), modal_parent(0), must_center(false)
	{
		SetWindowLongPtr ( widget, DWLP_DLGPROC, (LONG)(LONG_PTR)Master::dlgProc );

		h_events["onresize"] = OnResize;
		h_events["onclose"] = OnClose;
		h_events["ongetminmaxinfo"] = OnGetMinMaxInfo;

		cur_x = 100;
		cur_y = 0;
	}

	Window::Window ( xgui::Window * parent ) :
		Widget ( MkWindow ( parent ) ),
		nc_x(0), nc_y(0), hStatus(0), modal_running(false), shown(false), modal_parent(0), must_center(false)
	{
		SetWindowLongPtr ( widget, DWLP_DLGPROC, (LONG)(LONG_PTR)Master::dlgProc );

		h_events["onresize"] = OnResize;
		h_events["onclose"] = OnClose;
		h_events["ongetminmaxinfo"] = OnGetMinMaxInfo;

		cur_x = 100;
		cur_y = 0;

		if ( parent )
			modal_parent = parent->getImpl()->widget;
	}

	Window::~Window()
	{
	}

	void Window::show()
	{
		recalcLayout();

		//calculate nc_x, nc_y (non-client dimensions)
		RECT crc, wrc;

		GetClientRect(widget, &crc);
		GetWindowRect(widget, &wrc);

		nc_x = wrc.right - wrc.left - crc.right;
		nc_y = wrc.bottom - wrc.top - crc.bottom;

		SetWindowPos ( widget, NULL, 0, 0, cur_x + nc_x, cur_y + nc_y, SWP_NOMOVE | SWP_NOZORDER );

		if ( must_center ) centre();
		must_center = false;

		ShowWindow( widget, SW_SHOW );

		shown = true;
	}


	int Window::modalRun()
	{
		int dret = -1;

		xgui::Window * this_window = dynamic_cast<xgui::Window *>(this_widget);

		if ( this_window->isModal() ) {
			EnableWindow ( modal_parent, FALSE );
			modal_running = true;
			dret = MASTER_INSTANCE->run();
			modal_running = false;
			EnableWindow ( modal_parent, TRUE );
			SetActiveWindow ( modal_parent );
		}

		return ( dret );
	}


	void Window::modalReturn ( int response )
	{
		if ( modal_running )
			PostMessage ( widget, WM_QUIT, response, 0 );
	}


	bool Window::addChild(xgui::Widget * w)
	{
		xgui::Window * this_window = dynamic_cast<xgui::Window *>(this_widget);

		xguimpl::Menu * menu = dynamic_cast<xguimpl::Menu *>(w->getImpl());

		if ( menu ) {

			if ( menu->_visible ) {
				this_window->menu_ = (xgui::Menu *)w;
				SetMenu ( widget, (HMENU)menu->widget );
			}

		} else {

			if ( this_window->child_ ) return false;
			this_window->child_ = w;

		}

		recalcLayout();

		if ( cur_x < min_x || cur_y < min_y )
			SetWindowPos ( widget, NULL, 0, 0, min_x, min_y, SWP_NOMOVE | SWP_NOZORDER );

		return true;
	}

	void Window::delChild(xgui::Widget * child)
	{
		xgui::Window * this_window = dynamic_cast<xgui::Window*>(this_widget);
		ASSERT(this_window);

		if ( this_window->child_ == child ) this_window->child_ = NULL;
	}

	void Window::maximize()
	{
		ShowWindow ( widget, SW_MAXIMIZE );
	}


	void Window::minimize()
	{
		ShowWindow ( widget, SW_MINIMIZE );
	}


	void Window::restore()
	{
		ShowWindow ( widget, SW_RESTORE );
	}

	void Window::setIcon(xgui::Image * img)
	{
		if (img) {
			HICON hIcon = img->getImpl()->getIcon();
			SetClassLong ( widget, GCL_HICON, (LONG)hIcon );
			DeleteObject ( hIcon );
		}
	}
	
	int Window::OnClose ( xgui::Window * w, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		hWnd; msg; wParam; lParam;

		int rv = xgui::EVT_PROPAGATE;

		xgui::Callback * cb =  w->getEvent( "onclose" );
		if ( cb ) {
			rv = cb->call ( dynamic_cast<xgui::Widget *>(w) );
			if ( rv ) return 1;
		}

		if ( rv == (int)xgui::EVT_PROPAGATE ) {
			if (w->modal_)
				PostQuitMessage ( -1 );
			else
				DestroyWindow ( w->getImpl()->widget );
		}

		return 0;
	}


	int Window::OnResize ( xgui::Window * xw, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		int width = LOWORD(lParam);
		int height = HIWORD(lParam);

		xguimpl::Window * w = dynamic_cast<xguimpl::Window *>(xw->getImpl());

		if ( !w->shown ) return 0;

		bool should_send_event = (width != w->cur_x) || (height != w->cur_y);

		std::pair<int, int> size;
		size.first = w->min_x;
		size.second = w->min_y;

		if (width > size.first) size.first = width;
		if (height > size.second) size.second = height;

		w->giveSize(size);

		if ( w->hStatus ) {
			SendMessage ( w->hStatus, msg, wParam, lParam );
			InvalidateRect ( w->hStatus, NULL, TRUE );
		}

		if (should_send_event)
			return Widget::OnResize ( xw, hWnd, msg, wParam, MAKELPARAM(w->cur_x, w->cur_y) );
		else
			return -1;
	}


	int Window::OnGetMinMaxInfo ( xgui::Window * xw, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		hWnd; msg; wParam;

		xguimpl::Window *w = xw->window_impl_;

		MINMAXINFO * mm = ( MINMAXINFO * ) lParam;
		int min_x, min_y;

		min_x = w->nc_x + w->min_x;
		min_y = w->nc_y + w->min_y;

		mm->ptMinTrackSize.x = min_x;
		mm->ptMinTrackSize.y = min_y;

		return -1;
	}


	bool Window::set_window_prop ( std::string const &name, std::string const &vals )
	{
		if ( name == "size" ) {

			int x, y;
			std::istringstream ss(vals);
			ss >> x >> y;

			SetWindowPos ( widget, NULL, 0, 0, x, y, SWP_NOMOVE | SWP_NOZORDER );

			cur_x = x;
			cur_y = y;

		} else if ( name == "resizable" ) {

			DWORD style = GetWindowLong ( widget, GWL_STYLE );
			if ( vals == "0" ) style &= ~(WS_SIZEBOX | WS_MAXIMIZEBOX);
			else style |= (WS_SIZEBOX | WS_MAXIMIZEBOX);
			SetWindowLong ( widget, GWL_STYLE, style );

			SendMessage ( widget, WM_NCPAINT, 1, 0 );

		} else if ( name == "position" ) {

			if ( vals == "center" ) centre();
			else {
				std::pair<int, int> pos = xgui::semantic_cast< std::pair<int, int> >(vals);
				setPos(pos);
			}

		} else if ( name == "border" ) {

			DWORD style = GetWindowLong ( widget, GWL_STYLE );
			if ( vals == "0" ) style &= ~WS_CAPTION;
			else style |= WS_CAPTION;
			SetWindowLong ( widget, GWL_STYLE, style );

			SetWindowPos ( widget, NULL, 0, 0, cur_x + 1, cur_y + 1, SWP_NOMOVE | SWP_NOZORDER );
			SetWindowPos ( widget, NULL, 0, 0, cur_x, cur_y, SWP_NOMOVE | SWP_NOZORDER );

		} else if ( name == "status-text" ) {

			if ( vals.empty() ) {
				if ( hStatus ) {
					DestroyWindow ( hStatus );
					hStatus = NULL;
				}
			} else {
				if ( !hStatus ) {
					hStatus = CreateWindowEx ( WS_EX_CONTROLPARENT,
						STATUSCLASSNAME, vals.c_str(), WS_VISIBLE | WS_CHILD | SBARS_SIZEGRIP,
						CW_USEDEFAULT, CW_USEDEFAULT,
						CW_USEDEFAULT, CW_USEDEFAULT, widget, NULL,
						xgui::Master::Instance()->getImpl()->hInstance, NULL );

					RECT rc;
					GetWindowRect ( hStatus, &rc );
					int sbh = rc.bottom - rc.top;
					min_y += sbh;

					SetWindowPos ( widget, NULL, 0, 0, cur_x, cur_y + sbh, SWP_NOMOVE | SWP_NOZORDER );

				} else
					SetWindowText ( hStatus, vals.c_str() );
			}

		} else if ( name == "text" ) {
			
			set_widget_prop ( "text", vals );

		} else
			return false;

		return true;
	}


	bool Window::get_window_prop ( std::string const &name, std::string &vals )
	{
		if ( name == "size" ) {

			int x, y;
			RECT rc;
			GetWindowRect( widget, &rc );
			x = rc.right - rc.left;
			y = rc.bottom - rc.top;
			std::ostringstream ss;
			ss << x << " " << y;
			vals = ss.str();

		} else if ( name == "resizable" ) {

			DWORD style = GetWindowLong ( widget, GWL_STYLE );
			if ( (style & WS_SIZEBOX) == WS_SIZEBOX ) vals = "1";
			else vals = "0";

		} else if ( name == "position" ) {

			RECT rc;
			GetWindowRect ( widget, &rc );
			std::pair<int, int> pos;
			pos.first = rc.left;
			pos.second = rc.top;
			vals = xgui::semantic_cast<std::string>(pos);

		} else if ( name == "border" ) {

			DWORD style = GetWindowLong ( widget, GWL_STYLE );
			if ( (style & WS_CAPTION) == WS_CAPTION ) vals = "1";
			else vals = "0";

		} else if ( name == "status-text" ) {

			vals = "";
			if ( hStatus ) {
				int len = GetWindowTextLength ( hStatus );
				char * pstr = (char*)malloc(len + 1);
				GetWindowText ( hStatus, pstr, len + 1 );
				vals = pstr;
				free(pstr);
			}

		} else if ( name == "text" ) {
			
			get_widget_prop ( "text", vals );

		} else
			return false;

		return true;
	}


	void Window::centre()
	{
		if ( !shown && !must_center ) {
			must_center = true;
			return;
		}

		RECT rc;
		int x, y;

		HDC hdc = GetDC(NULL);
		int cx = GetDeviceCaps(hdc, HORZRES);
		int cy = GetDeviceCaps(hdc, VERTRES);
		ReleaseDC(NULL, hdc);

		GetWindowRect ( widget, &rc );

		x = (cx - (rc.right - rc.left)) / 2;
		y = (cy - (rc.bottom - rc.top)) / 2;

		setPos(std::make_pair(x, y));
	}


	bool Window::linkEvent( std::string const &name )
	{
		if ( name == "onclose" ) {
			h_events["onclose"] = OnClose;
		} else if ( name == "onresize" ) {
			h_events["onresize"] = OnResize;
		} else if ( name == "ongetminmaxinfo" ) {
			h_events["ongetminmaxinfo"] = OnGetMinMaxInfo;
		} else
			return Widget::linkEvent(name);

		return true;
	}


	void Window::giveSize ( std::pair<int, int> size )
	{
		xgui::Window * this_window = dynamic_cast<xgui::Window *>(this_widget);

		cur_x = size.first;
		cur_y = size.second;

		if ( hStatus ) {
			RECT rc;
			GetWindowRect ( hStatus, &rc );
			size.second -= rc.bottom - rc.top;
		}

		if ( this_window->child_ ) this_window->child_->getImpl()->giveSize(size);
	}


	void Window::recalcLayout()
	{
		xgui::Window * this_window = dynamic_cast<xgui::Window *>(this_widget);

		if ( !this_window->child_ ) return;

		this_window->child_->recalcLayout();

		min_x = this_window->child_->getImpl()->min_x;
		min_y = this_window->child_->getImpl()->min_y;

		// setta la dimensione minima uguale a quella del root widget
		if ( hStatus ) {
			RECT rc;
			GetWindowRect ( hStatus, &rc );
			min_y += rc.bottom - rc.top;
		}

		min_x = max ( min_x, 100 );
	}

}
