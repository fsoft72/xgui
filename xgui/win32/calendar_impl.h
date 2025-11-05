//
// C++ Interface: calendar_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_CALENDAR_IMPL_H_
#define _XGUI_CALENDAR_IMPL_H_

#include "widget_impl.h"

namespace xguimpl
{
	struct Calendar : public Widget
	{
		//methods
		DLLEXPORT Calendar ( HWND real_w );
		DLLEXPORT Calendar ( xgui::Container * parent );
		DLLEXPORT virtual ~Calendar();

		DLLEXPORT static HWND MkCalendar ( xgui::Container * parent );

		DLLEXPORT char const * bindWMNOTIFY ( WORD command_id );
		DLLEXPORT bool linkEvent ( std::string const &name );

		DLLEXPORT bool get_calendar_prop ( std::string const &name, std::string &vals );
		DLLEXPORT bool set_calendar_prop ( std::string const &name, const std::string &vals );

		DLLEXPORT static int OnSelect ( xgui::Calendar * cal, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	};
}

#endif
