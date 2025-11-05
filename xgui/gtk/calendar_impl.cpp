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

#include "calendar_impl.h"
#include "callback.h"

namespace xguimpl
{
	Calendar::Calendar ( GtkWidget * real_w ) : Widget ( real_w ) {}
	Calendar::Calendar ( xgui::Container * parent ) : Widget ( gtk_calendar_new() ) { }
	Calendar::~Calendar() {}

	bool Calendar::linkEvent( std::string const &name )
	{
		if ( name == "onselect" ) {
			gtk_signal_connect ( GTK_OBJECT ( widget ), "day-selected", G_CALLBACK ( OnSelect ), this );
			return true;
		}
	
		return Widget::linkEvent(name);
	}

	bool Calendar::get_calendar_prop ( std::string const &name, std::string &vals )
	{
		if (name == "month") {
			vals = xgui::semantic_cast<std::string>(getMonth());
			return true;
		}
		else if (name == "day") {
			vals = xgui::semantic_cast<std::string>(getDay());
			return true;
		}
		else if (name == "year") {
			vals = xgui::semantic_cast<std::string>(getYear());
			return true;
		}
		else if (name == "date") {
			std::string year = xgui::semantic_cast<std::string>(getYear());
			std::string month = xgui::semantic_cast<std::string>(getMonth());
			std::string day = xgui::semantic_cast<std::string>(getDay());
	
			vals = year + "-" + month + "-" + day;
			return true;
		}

		return false;
	}

	bool Calendar::set_calendar_prop ( std::string const &name, const std::string &vals )
	{
		if (name == "month") {
			setMonth(xgui::semantic_cast<int>(vals));
			return true;
		}
		else if (name == "day") {
			setDay(xgui::semantic_cast<int>(vals));
			return true;
		}
		else if (name == "year") {
			setYear(xgui::semantic_cast<int>(vals));
			return true;
		}
		else if (name == "date") {
			std::string::size_type year_end, month_end;
			
			if ((year_end = vals.find('-')) == std::string::npos) return true;
			if ((month_end = vals.find('-', year_end)) == std::string::npos) return true;
	
			std::string year = vals.substr(0, year_end);
			std::string month = vals.substr(year_end+1, month_end);
			std::string day = vals.substr(month_end+1, std::string::npos);
			
			setDay(xgui::semantic_cast<int>(day));
			setMonth(xgui::semantic_cast<int>(month));
			setYear(xgui::semantic_cast<int>(year));
			return true;
		}

		return false;
	}
	
	int Calendar::getMonth()
	{
		int val;
		g_object_get(G_OBJECT(widget), "month", &val, NULL);
		return val+1;
	}
	
	int Calendar::getDay()
	{
		int val;
		g_object_get(G_OBJECT(widget), "day", &val, NULL);
		return val;
	}
	
	int Calendar::getYear()
	{
		int val;
		g_object_get(G_OBJECT(widget), "year", &val, NULL);
		return val;
	}
	
	void Calendar::setMonth(int val)
	{
		g_object_set(G_OBJECT(widget), "month", val-1, NULL);
	}
	
	void Calendar::setDay(int val)
	{
		g_object_set(G_OBJECT(widget), "day", val, NULL);
	}
	
	void Calendar::setYear(int val)
	{
		g_object_set(G_OBJECT(widget), "year", val, NULL);
	}

	void Calendar::OnSelect ( GtkWidget * w, Calendar * cal )
	{
		xgui::Callback  * cb = cal->this_widget->getEvent("onclick");
		if ( cb ) 
			cb->call( cal->this_widget );
	}
}
