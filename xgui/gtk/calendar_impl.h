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
		Calendar ( GtkWidget * real_w );
		Calendar ( xgui::Container * parent );
		virtual ~Calendar();

		bool linkEvent ( std::string const &name );

		bool get_calendar_prop ( std::string const &name, std::string &vals );
		bool set_calendar_prop ( std::string const &name, const std::string &vals );

		int getMonth();
		int getDay();
		int getYear();

		void setMonth(int);
		void setDay(int);
		void setYear(int);

		static void OnSelect ( GtkWidget * w, Calendar * cal );
	};
}

#endif
