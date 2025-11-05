//
// C++ Implementation: entry_impl
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
	Entry::Entry ( HWND real_w ) : Widget ( real_w ),
		_maxlen(0), _editable(true), _password_mode(false), _alignment("left"),
		_last_pos(0)
	{
		LONG origWndProc = GetWindowLong ( widget, GWL_WNDPROC );
		SetProp ( widget, "xgui_orig_wnd_proc", (HANDLE)origWndProc );
		SetWindowLong ( widget, GWL_WNDPROC, (LONG) Master::subProc );

		DWORD style = GetClassLong ( widget, GCL_STYLE );
		SetClassLong ( widget, GCL_STYLE, style | CS_DBLCLKS );
	}

	Entry::Entry ( xgui::Container * parent, std::string const &text, int maxlen,
		bool editable, bool password_mode ) :
		Widget ( MkEntry ( parent, text, maxlen, editable, password_mode, "left", 0 ) ),
		_maxlen(maxlen), _editable(editable), _password_mode(password_mode), _alignment("left"),
		_last_pos(0)
	{
		LONG origWndProc = GetWindowLong ( widget, GWL_WNDPROC );
		SetProp ( widget, "xgui_orig_wnd_proc", (HANDLE)origWndProc );
		SetWindowLong ( widget, GWL_WNDPROC, (LONG) Master::subProc );

		const char * str = text.c_str();
		if ( text.size() < 4 ) str = "OOOO";

		HDC hdc = GetDC ( widget );
		SIZE sz;
		GetTextExtentPoint32 ( hdc, str, strlen(str), &sz );
		ReleaseDC ( widget, hdc );
		min_x = sz.cx + 6;
		min_y = sz.cy + 6;
	}


	Entry::~Entry()
	{
	}


	HWND Entry::MkEntry ( xgui::Container * parent, std::string const &text, int maxlen,
		bool editable, bool password_mode, const std::string & alignment, Entry * entry )
	{
		DWORD style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | ES_AUTOHSCROLL;

		if ( password_mode ) style |= ES_PASSWORD;
		if ( !editable ) style |= ES_READONLY;

		if (alignment == "center") style |= ES_CENTER;
		else if (alignment == "right") style |= ES_RIGHT;
		else style |= ES_LEFT;

		std::string txt = text;

		RECT rc;
		rc.bottom = rc.left = rc.right = rc.top = CW_USEDEFAULT;

		if (entry) {
			GetWindowRect ( entry->widget, &rc );
			rc.right -= rc.left;
			rc.bottom -= rc.top;
			ScreenToClient ( (HWND)parent->getImpl()->widget, (LPPOINT)&rc );

			entry->get_entry_prop ( "text", txt );

			DestroyWindow ( entry->widget );
		}

		HWND widget = CreateWindowEx(WS_EX_CLIENTEDGE | WS_EX_NOPARENTNOTIFY,
			"EDIT", txt.c_str(), style,
			rc.left, rc.top, rc.right, rc.bottom, (HWND)parent->getImpl()->widget,
			NULL, MAIN_HINSTANCE, NULL);

		SendMessage ( widget, WM_SETFONT, (WPARAM) GetStockObject ( DEFAULT_GUI_FONT ), MAKELPARAM ( TRUE, 0 ) );

		SendMessage ( widget, EM_SETLIMITTEXT, (WPARAM)maxlen, 0 );

		style = GetClassLong ( widget, GCL_STYLE );
		SetClassLong ( widget, GCL_STYLE, style | CS_DBLCLKS );

		return widget;
	}


	bool Entry::linkEvent ( std::string const &name )
	{
		if ( name == "onsubmit" )
			h_events["onsubmit"] = OnSubmit;

		else if (name == "onchange") {
			get_widget_prop ( "text", _last_text );
			SendMessage ( widget, EM_GETSEL, (WPARAM)&_last_pos, 0 );
			h_events["onchange"] = OnChange;

		} else
			return Widget::linkEvent(name);

		return true;
	}


	char const * Entry::bindWMCOMMAND ( WORD command_id )
	{
		switch ( command_id )
		{
			case EN_CHANGE: return "onchange";
		}

		return "";
	}


	bool Entry::set_entry_prop ( std::string const &name, std::string const &vals )
	{
		if ( name == "password-mode" ) {

			_password_mode = (vals == "1");
			
			MASTER_INSTANCE->unregisterWidget ( this );
			widget = MkEntry ( this_widget->getParent(), "", _maxlen, _editable, _password_mode, _alignment, this );
			MASTER_INSTANCE->registerWidget ( this );

		} else if ( name == "alignment" ) {

			_alignment = vals;
			
			MASTER_INSTANCE->unregisterWidget ( this );
			widget = MkEntry ( this_widget->getParent(), "", _maxlen, _editable, _password_mode, _alignment, this );
			MASTER_INSTANCE->registerWidget ( this );

		} else if ( name == "editable" ) {

			_editable = (vals == "1");

			MASTER_INSTANCE->unregisterWidget ( this );
			widget = MkEntry ( this_widget->getParent(), "", _maxlen, _editable, _password_mode, _alignment, this );
			MASTER_INSTANCE->registerWidget ( this );

		} else if ( name == "maxlength" ) {

			_maxlen = xgui::semantic_cast<int>(vals);
			SendMessage ( widget, EM_SETLIMITTEXT, (WPARAM)_maxlen, 0 );

		} else if ( name == "text" ) {
			
			set_widget_prop ( "text", vals );

		} else
			return false;

		return true;
	}


	bool Entry::get_entry_prop ( std::string const &name, std::string &vals )
	{
		if ( name == "text" ) {

			if (h_events.find("onchange") != h_events.end()) vals = _last_text;
			else get_widget_prop ( "text", vals );

		} else if ( name == "editable" ) {

			vals = _editable ? "1" : "0";

		} else if ( name == "password-mode" ) {

			vals = _password_mode ? "1" : "0";

		} else if ( name == "alignment" ) {

			vals = _alignment;

		} else if ( name == "maxlength" ) {

			vals = xgui::semantic_cast<std::string>(_maxlen);

		} else
			return false;

		return true;
	}

	void Entry::appendText ( const std::string & text )
	{
		std::string txt;
		get_entry_prop ( "text", txt );
		SetWindowText ( widget, (txt + text).c_str() );
	}


	void Entry::prependText ( const std::string & text )
	{
		std::string txt;
		get_entry_prop ( "text", txt );
		SetWindowText ( widget, (text + txt).c_str() );
	}


	void Entry::insertText ( const std::string & text, int pos )
	{
		std::string txt;
		get_entry_prop ( "text", txt );
		txt.insert ( pos, text );
		SetWindowText ( widget, txt.c_str() );
	}


	int Entry::OnSubmit ( xgui::Entry * e, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		hWnd; msg; wParam; lParam;

		xgui::TextCallback * cb = dynamic_cast<xgui::TextCallback *>(e->getEvent("onsubmit"));

		if ( cb ) {

			std::string txt;
			e->get ( "text", txt );

			return cb->call( dynamic_cast<xgui::Widget *>(e), txt );

		}

		return 0;
	}


	int Entry::OnChange ( xgui::Entry * we, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		hWnd; msg; wParam; lParam;

		Entry * e = dynamic_cast<Entry *>(we->getImpl());

		xgui::TextCallback * cb = dynamic_cast<xgui::TextCallback *>(we->getEvent("onchange"));

		std::string new_txt;
		e->get_widget_prop ( "text", new_txt );

		DMESSAGE ( "text: " << new_txt );

		bool retval = xgui::EVT_PROPAGATE;

		if ( cb ) retval = cb->call ( dynamic_cast<xgui::Widget *>(e), new_txt ) == 0 ? false : true;

		SendMessage ( e->widget, EM_GETSEL, (WPARAM)&e->_last_pos, 0 );
		e->_last_pos -= new_txt.size() - e->_last_text.size();

		if (retval == xgui::EVT_PROPAGATE) {

			e->_last_text = new_txt;

		} else {

			e->h_events.erase("onchange");

			e->set_entry_prop ( "text", e->_last_text );
			SendMessage ( e->widget, EM_SETSEL, e->_last_pos, e->_last_pos );
		
			e->h_events["onchange"] = Entry::OnChange;

		}

		return (int)retval;
	}
}
