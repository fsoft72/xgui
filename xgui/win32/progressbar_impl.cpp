//
// C++ Implementation: progressbar_impl
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

static HWND MkPbarWidget ( xgui::Container * parent, int min, int max)
{
	HWND widget = CreateWindowEx( WS_EX_NOPARENTNOTIFY, PROGRESS_CLASS, "",
		WS_TABSTOP | WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, parent->getImpl()->widget, NULL,
		MAIN_HINSTANCE, NULL );

	SendMessage ( widget, WM_SETFONT, (WPARAM) GetStockObject ( DEFAULT_GUI_FONT ), MAKELPARAM ( TRUE, 0 ) );
	SendMessage ( widget, PBM_SETRANGE32, min, max );
	SendMessage ( widget, PBM_SETPOS, min, 0 );

	return widget;
}

namespace xguimpl
{
	Progressbar::Progressbar ( HWND real_w ) 
	: Widget ( real_w ) 
	{
		LONG origWndProc = GetWindowLong ( widget, GWL_WNDPROC );
		SetProp ( widget, "xgui_orig_wnd_proc", reinterpret_cast<HANDLE>(origWndProc) );
		SetWindowLong ( widget, GWL_WNDPROC, (LONG)Master::subProc );

		DWORD style = GetClassLong ( widget, GCL_STYLE );
		SetClassLong ( widget, GCL_STYLE, style | CS_DBLCLKS );
	}

	Progressbar::Progressbar ( xgui::Container * parent, int min, int max ) 
	: Widget ( MkPbarWidget(parent, min, max) ) 
	{ 
		LONG origWndProc = GetWindowLong ( widget, GWL_WNDPROC );
		SetProp ( widget, "xgui_orig_wnd_proc", reinterpret_cast<HANDLE>(origWndProc) );
		SetWindowLong ( widget, GWL_WNDPROC, (LONG)Master::subProc );

		DWORD style = GetClassLong ( widget, GCL_STYLE );
		SetClassLong ( widget, GCL_STYLE, style | CS_DBLCLKS );

		min_x = 32;
		min_y = 32;
	}

	Progressbar::~Progressbar() {}

	bool Progressbar::linkEvent( std::string const &name )
	{
		if ( name == "onchange" ) {
			return true;
		}
	
		return Widget::linkEvent(name);
	}

	void Progressbar::OnChange ( HWND, Progressbar * pbar )
	{
		xgui::Callback * base_cb = pbar->this_widget->getEvent("onchange");
		if (!base_cb) return;
		
		xgui::TextCallback * cb = dynamic_cast<xgui::TextCallback *>(base_cb);
		if (!cb) {
			DMESSAGE("onchange event of xgui::Progressbar expected a TextCallback");
			return;
		}
	
		xgui::Progressbar * this_pbar = dynamic_cast<xgui::Progressbar*>(pbar->this_widget);
		cb->call( pbar->this_widget, xgui::semantic_cast<std::string> (this_pbar->getPos()) );
	}

	void Progressbar::setState (int min, int max, int pos)
	{
		SendMessage ( widget, PBM_SETRANGE32, min, max );
		SendMessage ( widget, PBM_SETPOS, pos, 0 );
		OnChange(widget, this);
	}
}
