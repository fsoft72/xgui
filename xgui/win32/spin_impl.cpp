//
// C++ Implementation: spin_impl
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

static HWND MkSpinWidget ( xgui::Container * parent )
{
	return CreateWindowEx(WS_EX_CLIENTEDGE | WS_EX_NOPARENTNOTIFY,
		"EDIT", "", WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL | ES_NUMBER,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, parent->getImpl()->widget,
		NULL, MAIN_HINSTANCE, NULL);
}


namespace xguimpl
{
	Spin::Spin ( HWND real_w ) : Widget ( real_w )
	{
		LONG origWndProc = GetWindowLong ( widget, GWL_WNDPROC );
		SetProp ( widget, "xgui_orig_wnd_proc", (HANDLE)origWndProc );
		SetWindowLong ( widget, GWL_WNDPROC, (LONG) Master::subProc );

		h_events["onresize"] = OnResize;
	}

	Spin::Spin ( xgui::Container * parent, int min, int max ) :
		Widget ( MkSpinWidget ( parent ) )
	{
		LONG origWndProc = GetWindowLong ( widget, GWL_WNDPROC );
		SetProp ( widget, "xgui_orig_wnd_proc", (HANDLE)origWndProc );
		SetWindowLong ( widget, GWL_WNDPROC, (LONG) Master::subProc );

		SendMessage ( widget, WM_SETFONT, (WPARAM) GetStockObject ( DEFAULT_GUI_FONT ), MAKELPARAM ( TRUE, 0 ) );

		DWORD style = GetClassLong ( widget, GCL_STYLE );
		SetClassLong ( widget, GCL_STYLE, style | CS_DBLCLKS );

		style = WS_VISIBLE | WS_CHILD | UDS_ALIGNRIGHT |
			UDS_ARROWKEYS | UDS_SETBUDDYINT | UDS_NOTHOUSANDS;
	
		_hSpin = CreateUpDownControl ( style, CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT, parent->getImpl()->widget, 100, MAIN_HINSTANCE, widget,
			max, min, min );
	
		// calc min size
		HDC hdc = GetDC ( widget );
		SIZE sz;
		GetTextExtentPoint32 ( hdc, "0000    ", 8, &sz );
		ReleaseDC ( widget, hdc );
		min_x = sz.cx + 6;
		min_y = sz.cy + 6;

		h_events["onresize"] = OnResize;
	}


	Spin::~Spin()
	{
		if ( _hSpin ) DestroyWindow ( _hSpin );
	}

	bool Spin::linkEvent ( std::string const &name )
	{
		if ( name == "onsubmit" )
			h_events["onsubmit"] = OnSubmit;
	
		else if (name == "onchange") {
			this_widget->get ( "value", _last_val );
			SendMessage ( widget, EM_GETSEL, (WPARAM)&_last_pos, 0 );
			h_events["onchange"] = OnChange;
	
		} else if (name == "onresize")
			return true;
		else
			return Widget::linkEvent(name);
	
		return true;
	}

	char const * Spin::bindWMCOMMAND( WORD command_id )
	{
		switch ( command_id )
		{
			case EN_CHANGE: return "onchange";
		}
	
		return "";
	}

	int Spin::getValue() { return SendMessage ( _hSpin, UDM_GETPOS32, 0, NULL ); }
	void Spin::setValue ( int pos ) { SendMessage ( _hSpin, UDM_SETPOS32, 0, pos ); }
	void Spin::setRange ( int min, int max ) { SendMessage ( _hSpin, UDM_SETRANGE32, min, max ); }

	std::string Spin::getAlignment()
	{
		std::string vals;

		DWORD style = GetWindowLong ( widget, GWL_STYLE );
		if ( (style & ES_RIGHT) == ES_RIGHT ) vals = "right";
		else if ( (style & ES_CENTER) == ES_CENTER ) vals = "center";
		else vals = "left";

		return vals;
	}

	void Spin::setAlignment(std::string const &align)
	{
		DWORD style = GetWindowLong ( widget, GWL_STYLE );
		style &= ~(ES_CENTER | ES_RIGHT | ES_LEFT);
		if ( align == "right" ) style |= ES_RIGHT;
		else if ( align == "center" ) style |= ES_CENTER;
		else style |= ES_LEFT;
		SetWindowLong ( widget, GWL_STYLE, style );
	}


	BOOL Spin::OnResize ( xgui::Spin * s, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		hWnd; msg; wParam; lParam;

		s->getImpl()->h_events.erase("onresize");
		SendMessage ( dynamic_cast<xguimpl::Spin *>(s->getImpl())->_hSpin, UDM_SETBUDDY,
			(WPARAM)s->getImpl()->widget, 0 );
		s->getImpl()->h_events["onresize"] = OnResize;
	
		return FALSE;
	}

	int Spin::OnSubmit ( xgui::Spin * s, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		hWnd; msg; wParam; lParam;

		xgui::TextCallback * cb = dynamic_cast<xgui::TextCallback *>(s->getEvent("onsubmit"));
	
		if ( cb ) {
	
			std::string txt;
			s->get ( "value", txt );
	
			return cb->call( s, txt );
	
		}
	
		return 0;
	}

	int Spin::OnChange ( xgui::Spin * spin, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		hWnd; msg; wParam; lParam;

		Spin * s = dynamic_cast<Spin *>(spin->getImpl());

		xgui::TextCallback * cb = dynamic_cast<xgui::TextCallback *>(spin->getEvent("onchange"));
	
		std::string new_val;
		spin->get ( "value", new_val );
	
	#ifdef DEBUG
		std::cout << "value: " << new_val << std::endl;
	#endif
	
		bool retval = xgui::EVT_PROPAGATE;
	
		if ( cb ) retval = cb->call ( dynamic_cast<xgui::Widget *>(s), new_val ) == 0 ? false : true;
	
		SendMessage ( s->widget, EM_GETSEL, (WPARAM)&s->_last_pos, 0 );
		s->_last_pos -= new_val.size() - s->_last_val.size();
	
		if (retval == xgui::EVT_PROPAGATE) {
	
			s->_last_val = new_val;
	
		} else {
	
			s->h_events.erase("onchange");
	
			spin->set ( "value", s->_last_val );
			SendMessage ( s->widget, EM_SETSEL, s->_last_pos, s->_last_pos );
		
			s->h_events["onchange"] = OnChange;
	
		}
	
		return (int)retval;
	}

}
