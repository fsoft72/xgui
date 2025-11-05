//
// C++ Implementation: slider_impl
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
	Slider::Slider ( HWND real_w ) : Widget ( real_w )
	{
		LONG origWndProc = GetWindowLong ( widget, GWL_WNDPROC );
		SetProp ( widget, "xgui_orig_wnd_proc", (HANDLE)origWndProc );
		SetWindowLong ( widget, GWL_WNDPROC, (LONG) Master::subProc );

		DWORD style = GetClassLong ( widget, GCL_STYLE );
		SetClassLong ( widget, GCL_STYLE, style | CS_DBLCLKS );
	}

	Slider::Slider ( xgui::Container * parent, int min, int max, bool vertical ) :
		Widget ( MkSlider ( parent, vertical ) )
	{
		LONG origWndProc = GetWindowLong ( widget, GWL_WNDPROC );
		SetProp ( widget, "xgui_orig_wnd_proc", reinterpret_cast<HANDLE>(origWndProc) );
		SetWindowLong ( widget, GWL_WNDPROC, (LONG)Master::subProc );

		DWORD style = GetClassLong ( widget, GCL_STYLE );
		SetClassLong ( widget, GCL_STYLE, style | CS_DBLCLKS );

		SendMessage ( widget, WM_SETFONT, (WPARAM) GetStockObject ( DEFAULT_GUI_FONT ), MAKELPARAM ( TRUE, 0 ) );

		min_x = 32;
		min_y = 32;

		SendMessage ( widget, TBM_SETRANGEMIN, FALSE, min );
		SendMessage ( widget, TBM_SETRANGEMAX, FALSE, max );
		SendMessage ( widget, TBM_SETPOS, FALSE, min );
	}


	Slider::~Slider()
	{
	}


	HWND Slider::MkSlider ( xgui::Container * parent, bool vertical )
	{
		return CreateWindowEx( WS_EX_NOPARENTNOTIFY, TRACKBAR_CLASS, "",
			WS_TABSTOP | WS_CHILD | WS_VISIBLE | (vertical ? TBS_VERT : 0),
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, (HWND)parent->getImpl()->widget,
			NULL, MAIN_HINSTANCE, NULL );
	}


	bool Slider::linkEvent ( std::string const &name )
	{
		if ( name == "onchange" )
			h_events["onchange"] = OnChange;

		else
			return Widget::linkEvent(name);

		return true;
	}


	bool Slider::set_slider_prop ( std::string const &name, std::string const &vals )
	{
		if ( name == "pos" ) {

			setPos ( xgui::semantic_cast<int>(vals) );

		} else if ( name == "min" ) {

			int min = xgui::semantic_cast<int>(vals);
			SendMessage ( widget, TBM_SETRANGEMIN, TRUE, min );

		} else if ( name == "max" ) {

			int max = xgui::semantic_cast<int>(vals);
			SendMessage ( widget, TBM_SETRANGEMAX, TRUE, max );

		} else
			return false;

		return true;
	}


	bool Slider::get_slider_prop ( std::string const &name, std::string &vals )
	{
		if ( name == "pos" ) {
			
			vals = xgui::semantic_cast<std::string>(getPos());
		
		} else if ( name == "min" ) {

			int min = SendMessage ( widget, TBM_GETRANGEMIN, 0, 0 );
			vals = xgui::semantic_cast<std::string>(min);

		} else if ( name == "max" ) {

			int max = SendMessage ( widget, TBM_GETRANGEMAX, 0, 0 );
			vals = xgui::semantic_cast<std::string>(max);

		} else
			return false;

		return true;
	}


	void Slider::setPos ( int pos )
	{
		SendMessage ( widget, TBM_SETPOS, TRUE, pos );
	}


	int Slider::getPos()
	{
		return SendMessage ( widget, TBM_GETPOS, 0, 0 );
	}


	int Slider::OnChange ( xgui::Slider * sld, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		hWnd; msg; wParam; lParam;

		xgui::TextCallback  * cb =  dynamic_cast<xgui::TextCallback *>(sld->getEvent("onchange"));
		std::string pos = xgui::semantic_cast<std::string>(sld->getPos());
		if ( cb ) cb->call ( dynamic_cast<xgui::Widget *>(sld), pos );
		
		return 0;
	}
}
