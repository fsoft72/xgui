//
// C++ Implementation: calendar_impl
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
	Calendar::Calendar ( HWND real_w ) : Widget ( real_w )
	{
		LONG origWndProc = GetWindowLong ( widget, GWL_WNDPROC );
		SetProp ( widget, "xgui_orig_wnd_proc", (HANDLE)origWndProc );
		SetWindowLong ( widget, GWL_WNDPROC, (LONG) Master::subProc );

		DWORD style = GetClassLong ( widget, GCL_STYLE );
		SetClassLong ( widget, GCL_STYLE, style | CS_DBLCLKS );
	}

	Calendar::Calendar ( xgui::Container * parent ) :
		Widget ( MkCalendar( parent ) )
	{
		LONG origWndProc = GetWindowLong ( widget, GWL_WNDPROC );
		SetProp ( widget, "xgui_orig_wnd_proc", reinterpret_cast<HANDLE>(origWndProc) );
		SetWindowLong ( widget, GWL_WNDPROC, (LONG)Master::subProc );

		DWORD style = GetClassLong ( widget, GCL_STYLE );
		SetClassLong ( widget, GCL_STYLE, style | CS_DBLCLKS );

		SendMessage ( widget, WM_SETFONT, (WPARAM) GetStockObject ( DEFAULT_GUI_FONT ), MAKELPARAM ( TRUE, 0 ) );

		RECT rc;
		MonthCal_GetMinReqRect(widget, &rc);

		min_x = rc.right - rc.left;
		min_y = rc.bottom - rc.top;
	}


	Calendar::~Calendar()
	{
	}


	HWND Calendar::MkCalendar ( xgui::Container * parent )
	{
		return CreateWindowEx( WS_EX_NOPARENTNOTIFY, MONTHCAL_CLASS, "",
			WS_TABSTOP | WS_CHILD | WS_VISIBLE,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, (HWND)parent->getImpl()->widget,
			NULL, MAIN_HINSTANCE, NULL );
	}


	char const * Calendar::bindWMNOTIFY ( WORD command_id )
	{
		switch ( command_id ) 
		{
			case MCN_SELCHANGE: return "onselect";
		}

		return "";
	}


	bool Calendar::linkEvent ( std::string const &name )
	{
		if (name == "onselect")
			h_events["onselect"] = OnSelect;

		else
			return Widget::linkEvent ( name );

		return true;
	}


	bool Calendar::set_calendar_prop ( std::string const &name, std::string const &vals )
	{
		SYSTEMTIME st;
		MonthCal_GetCurSel(widget, &st);

		if (name == "month") {
			int month = xgui::semantic_cast<int>(vals);
			st.wMonth = (WORD)month;
			MonthCal_SetCurSel(widget, &st);
		}
		else if (name == "day") {
			int day = xgui::semantic_cast<int>(vals);
			st.wDay = (WORD)day;
			MonthCal_SetCurSel(widget, &st);
		}
		else if (name == "year") {
			int year = xgui::semantic_cast<int>(vals);
			st.wYear = (WORD)year;
			MonthCal_SetCurSel(widget, &st);
		}
		else if (name == "date") {
			std::string::size_type year_end, month_end;
			
			if ((year_end = vals.find('-')) == std::string::npos) return true;
			if ((month_end = vals.find('-', year_end)) == std::string::npos) return true;

			std::string year = vals.substr(0, year_end);
			std::string month = vals.substr(year_end+1, month_end);
			std::string day = vals.substr(month_end+1, std::string::npos);
			
			int d = xgui::semantic_cast<int>(day);
			int m = xgui::semantic_cast<int>(month);
			int y = xgui::semantic_cast<int>(year);

			st.wDay = (WORD)d;
			st.wMonth = (WORD)m;
			st.wYear = (WORD)y;
			MonthCal_SetCurSel(widget, &st);
		} else
			return false;

		return true;
	}


	bool Calendar::get_calendar_prop ( std::string const &name, std::string &vals )
	{
		SYSTEMTIME st;
		MonthCal_GetCurSel(widget, &st);

		if (name == "month") {
			vals = xgui::semantic_cast<std::string>(st.wMonth);
		}
		else if (name == "day") {
			vals = xgui::semantic_cast<std::string>(st.wDay);
		}
		else if (name == "year") {
			vals = xgui::semantic_cast<std::string>(st.wYear);
		}
		else if (name == "date") {
			std::string year = xgui::semantic_cast<std::string>(st.wYear);
			std::string month = xgui::semantic_cast<std::string>(st.wMonth);
			std::string day = xgui::semantic_cast<std::string>(st.wDay);

			vals = year + "-" + month + "-" + day;
		} else
			return false;

		return true;
	}

	
	int Calendar::OnSelect ( xgui::Calendar * cal, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		hWnd; msg; wParam; lParam;

		std::string date;
		cal->get("date", date);

		xgui::Callback * base_cb = cal->getEvent("onselect");
		if (!base_cb) return 0;

		xgui::TextCallback  * cb =  dynamic_cast<xgui::TextCallback *>(base_cb);
		if (!cb) {
			DMESSAGE ( "onselect event of xgui::Calendar expected a TextCallback" );
			return 0;
		}

		if ( cb ) cb->call ( dynamic_cast<xgui::Widget *>(cal), date );

		return 0;
	}
}
