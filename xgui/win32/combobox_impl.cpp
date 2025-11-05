//
// C++ Implementation: combobox_impl
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
	Combobox::Combobox ( HWND real_w ) : View ( real_w ),
		_maxlen(0), _last_pos(0)
	{
		LONG origWndProc = GetWindowLong ( widget, GWL_WNDPROC );
		SetProp ( widget, "xgui_orig_wnd_proc", (HANDLE)origWndProc );
		SetWindowLong ( widget, GWL_WNDPROC, (LONG)Master::subProc );

		LONG style = GetClassLong ( widget, GCL_STYLE );
		SetClassLong ( widget, GCL_STYLE, style | CS_DBLCLKS );

		h_events["onselect"] = OnSelect;
	}

	Combobox::Combobox ( xgui::Container * parent, bool editable ) :
		View ( MkCombobox ( parent, editable, 0 ) ), _maxlen(0), _last_pos(0)
	{
		RECT rc;
		GetClientRect ( widget, &rc );
		min_x = 16;
		min_y = rc.bottom;

		h_events["onselect"] = OnSelect;
	}


	Combobox::~Combobox()
	{
	}


	HWND Combobox::MkCombobox ( xgui::Container * parent, bool editable, Combobox * cbox )
	{
		std::string txt = "";

		RECT rc;
		rc.bottom = rc.left = rc.right = rc.top = CW_USEDEFAULT;

		int cx = 0, cy = 0, maxlen = 0;

		if (cbox) {
			cx = cbox->cur_x;
			cy = cbox->cur_y;
			maxlen = cbox->_maxlen;

			GetWindowRect(cbox->widget, &rc);
			ScreenToClient((HWND)parent->getImpl()->widget, (LPPOINT)&rc);

			txt = cbox->getEntryText();

			DestroyWindow(cbox->widget);
		}

		HWND retval = CreateWindowEx( WS_EX_NOPARENTNOTIFY, "COMBOBOX", "",
			WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CBS_HASSTRINGS | WS_VSCROLL |
			(editable ? CBS_DROPDOWN : CBS_DROPDOWNLIST),
			rc.left, rc.top, cx, cy + 100, (HWND)parent->getImpl()->widget, NULL,
			MAIN_HINSTANCE, NULL );

		if ( !txt.empty() ) SetWindowText ( retval, txt.c_str() );

		LONG origWndProc = GetWindowLong ( retval, GWL_WNDPROC );
		SetProp ( retval, "xgui_orig_wnd_proc", (HANDLE)origWndProc );
		SetWindowLong ( retval, GWL_WNDPROC, (LONG)Master::subProc );

		LONG style = GetClassLong ( retval, GCL_STYLE );
		SetClassLong ( retval, GCL_STYLE, style | CS_DBLCLKS );

		SendMessage ( retval, WM_SETFONT, (WPARAM) GetStockObject ( DEFAULT_GUI_FONT ), MAKELPARAM ( TRUE, 0 ) );

		SendMessage ( retval, CB_LIMITTEXT, (WPARAM)maxlen, 0 );

		return retval;
	}


	bool Combobox::linkEvent ( std::string const &name )
	{
		if ( name == "onselect" )
			return true;

		else if ( name == "onsubmit" ) {
			h_events["onsubmit"] = OnSubmit;

		} else if ( name == "onchange" ) {
			this_widget->get ( "text", _last_text );
			SendMessage ( widget, CB_GETEDITSEL, (WPARAM)&_last_pos, 0 );
			h_events["onchange"] = OnChange;

		} else
			return Widget::linkEvent(name);

		return true;
	}


	char const * Combobox::bindWMCOMMAND( WORD command_id )
	{
		switch ( command_id )
		{
			case CBN_SELCHANGE: return "onselect";
			case CBN_EDITCHANGE: return "onchange";
		}

		return "";
	}


	void Combobox::setEntryText ( std::string const &text )
	{
		Widget::set_widget_prop ( "text", text );
	}


	std::string Combobox::getEntryText()
	{
		std::string text;
		Widget::get_widget_prop ( "text", text );
		return text;
	}


	void Combobox::setSelected ( int num )
	{
		SendMessage ( widget, CB_SETCURSEL, num, 0 );
		OnSelect ( this_cbox, widget, 0, 0, 0 );
	}


	int Combobox::getSelected()
	{
		return SendMessage ( widget, CB_GETCURSEL, 0, 0 );
	}


	std::string Combobox::getSelectedItemText()
	{
		return getEntryText();
	}


	void Combobox::setMaxLength ( int len )
	{
		_maxlen = len;
		SendMessage ( widget, CB_LIMITTEXT, (WPARAM)_maxlen, 0 );
	}


	int Combobox::getMaxLength()
	{
		return _maxlen;
	}


	void Combobox::setAsEditable ( bool iseditable )
	{
		MASTER_INSTANCE->unregisterWidget ( this );
		widget = MkCombobox ( this_widget->getParent(), iseditable, this );
		MASTER_INSTANCE->registerWidget ( this );
		refresh();
	}


	void Combobox::refresh()
	{
		if ( !this_cbox->model_ ) return;

		h_events.erase("onselect");
		
		SendMessage ( widget, CB_RESETCONTENT, 0, 0 );

		for ( xgui::Model::iterator iter = this_cbox->model_->begin();
			iter != this_cbox->model_->end(); ++iter )
		{
			xgui::Model * child = dynamic_cast<xgui::Model*>(*iter);
			SendMessage ( widget, CB_ADDSTRING, 0, (LPARAM)child->getString(0).c_str() );
		}

		h_events["onselect"] = OnSelect;
	}


	void Combobox::insertItem ( xgui::Model * parent, int child_pos, xgui::Model * child )
	{
		parent;

		h_events.erase("onselect");
		SendMessage ( widget, CB_INSERTSTRING, child_pos, (LPARAM)child->getString(0).c_str() );
		h_events["onselect"] = OnSelect;
	}


	void Combobox::removeItem ( xgui::Model * parent, int child_pos, xgui::Model * child )
	{
		parent; child;

		h_events.erase("onselect");
		SendMessage ( widget, CB_DELETESTRING, child_pos, 0 );
		h_events["onselect"] = OnSelect;
	}


	bool Combobox::isSelected ( xgui::Model * parent, int child_pos )
	{
		int idx = SendMessage ( widget, CB_GETCURSEL, 0, 0 );
		return (idx == child_pos);
	}


	void Combobox::select ( xgui::Model * parent, int child_pos, bool state )
	{
		if ( state )
			SendMessage ( widget, CB_SETCURSEL, child_pos, 0 );
		else {
			int idx = SendMessage ( widget, CB_GETCURSEL, 0, 0 );
			if ( idx == child_pos )
				SendMessage ( widget, CB_SETCURSEL, -1, 0 );
		}
	}


	void Combobox::selectAll ( xgui::Model * parent, bool state )
	{
		if ( !state )
			SendMessage ( widget, CB_SETCURSEL, -1, 0 );
	}


	void Combobox::giveSize ( std::pair<int, int> size )
	{
		cur_x = size.first;
		cur_y = size.second;

		SetWindowPos ( widget, NULL, 0, 0, cur_x, cur_y + 100, SWP_NOMOVE | SWP_NOZORDER );
	}


	void Combobox::appendText ( const std::string & text )
	{
		std::string txt = getEntryText();
		SetWindowText ( widget, (txt + text).c_str() );
	}


	void Combobox::prependText ( const std::string & text )
	{
		std::string txt = getEntryText();
		SetWindowText ( widget, (text + txt).c_str() );
	}


	void Combobox::insertText ( const std::string & text, int pos )
	{
		std::string txt = getEntryText();
		txt.insert ( pos, text );
		SetWindowText ( widget, txt.c_str() );
	}


	void Combobox::popup()
	{
		SendMessage ( widget, CB_SHOWDROPDOWN, FALSE, 0 );
	}


	void Combobox::popdown()
	{
		SendMessage ( widget, CB_SHOWDROPDOWN, TRUE, 0 );
	}


	int Combobox::OnChange ( xgui::Combobox * cbox, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		hWnd; msg; wParam; lParam;

		Combobox * c = dynamic_cast<Combobox *>(cbox->getImpl());

		xgui::TextCallback * cb = dynamic_cast<xgui::TextCallback *>(cbox->getEvent("onchange"));

		std::string new_txt = c->getEntryText();

		DMESSAGE ( "text: " << new_txt );

		bool ret = xgui::EVT_PROPAGATE;

		if ( cb ) ret = cb->call ( dynamic_cast<xgui::Widget *>(c), new_txt ) == 0 ? false : true;

		SendMessage ( c->widget, CB_GETEDITSEL, (WPARAM)&c->_last_pos, 0 );
		c->_last_pos -= new_txt.size() - c->_last_text.size();

		if (ret == xgui::EVT_PROPAGATE) {

			c->_last_text = new_txt;

		} else {

			c->h_events.erase("onchange");

			c->setEntryText ( c->_last_text );
			SendMessage ( c->widget, CB_SETEDITSEL, 0, MAKELPARAM(c->_last_pos, c->_last_pos) );
		
			c->h_events["onchange"] = OnChange;

		}

		return ret == xgui::EVT_BLOCK ? TRUE : FALSE;
	}


	int Combobox::OnSelect ( xgui::Combobox * cbox, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		hWnd; msg; wParam; lParam;

		Combobox * c = dynamic_cast<Combobox *>(cbox->getImpl());

		int selection = SendMessage ( c->widget, CB_GETCURSEL, 0, 0 );

		int len = SendMessage ( c->widget, CB_GETLBTEXTLEN, selection, 0 );
		char * s = (char *)malloc(len + 1);
		SendMessage ( c->widget, CB_GETLBTEXT, (WPARAM)selection, (LPARAM)s );
		std::string txt(s);
		free(s);

		c->_last_text = txt;
		c->_last_pos = 0;

		xgui::TextStatusCallback * cb = dynamic_cast<xgui::TextStatusCallback *>(cbox->getEvent("onselect"));
		if ( cb ) cb->call( cbox, txt, selection );

		return 0;
	}


	int Combobox::OnSubmit ( xgui::Combobox * cbox, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		hWnd; msg; wParam; lParam;

		Combobox * c = dynamic_cast<Combobox *>(cbox->getImpl());

		std::string txt = c->getEntryText();

		xgui::TextCallback * cb = dynamic_cast<xgui::TextCallback *>(cbox->getEvent("onsubmit"));
		if ( cb ) return cb->call ( cbox, txt );

		return 0;
	}


	void Combobox::setAccessor ( xgui::Widget * w )
	{
		this_cbox = dynamic_cast<xgui::Combobox *>(w);
		ASSERT(this_cbox);

		Widget::setAccessor ( w );
	}
}
