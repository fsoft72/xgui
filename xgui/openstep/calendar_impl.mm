/* * 
 * Copyright (C) 2007,2008 by 
 * 	Alessandro Molina <alessandro.molina@gmail.com>
 *  
 * This file is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 * 
 * This file is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 * General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License
 * along with this file; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *  
 * */

#include "calendar_impl.h"
#include "callback.h"

namespace xguimpl
{
	Calendar::Calendar ( OpenStepWidget * real_w ) : Widget ( real_w ) {}
	Calendar::Calendar ( xgui::Container * parent ) : Widget ( 0 ) { }
	Calendar::~Calendar() {}

	bool Calendar::linkEvent( std::string const &name )
	{
		if ( name == "onselect" ) {
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
		int val = 0;
		return val+1;
	}
	
	int Calendar::getDay()
	{
		int val = 1;
		return val;
	}
	
	int Calendar::getYear()
	{
		int val = 2007;
		return val;
	}
	
	void Calendar::setMonth(int val)
	{

	}
	
	void Calendar::setDay(int val)
	{

	}
	
	void Calendar::setYear(int val)
	{

	}
}
