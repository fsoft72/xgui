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

#include "checkbox_impl.h"
#include "checkbox.h"
#include "callback.h"

namespace xguimpl
{
	Checkbox::Checkbox ( OpenStepWidget * real_w ) : Widget ( real_w ) {}
	Checkbox::Checkbox ( xgui::Container * parent, std::string const &text )
	: Widget ( 0 ) 
	{
	}

	Checkbox::~Checkbox() {}

	void Checkbox::setAccessor(xgui::Widget * w)
	{
		xgui::Checkbox * downcasted_widget = dynamic_cast<xgui::Checkbox*>(w);
		ASSERT(downcasted_widget);
		this_checkbox = downcasted_widget;

		Widget::setAccessor(w);
	}

	bool Checkbox::linkEvent( std::string const &name )
	{
		if ( name == "onclick" ) {
			return true;
		}
	
		return Widget::linkEvent(name);
	}

	bool Checkbox::get_checkbox_prop ( std::string const &name, std::string &vals )
	{
		if ( name == "text" ) {
			vals = "";
			return true;
		}
		else if ( name == "value" ) {
			vals = value;
			return true;
		}
		else if ( name == "checked" ) {
			vals = "0";
			return true;
		}
		else if ( name == "alignment" ) {
			std::string  halign = "center";
			float valign_v, halign_v;

			if (halign_v == 1.0f) halign = "right";
			else if (halign_v == 0.0f) halign = "left";

			vals = halign;

			return true;
		}

		return false;
	}

	bool Checkbox::set_checkbox_prop ( std::string const &name, const std::string &vals )
	{
		if ( name == "text" ) {
			return true;
		}
		else if ( name == "value" ) {
			value = vals;
			if ( value.empty() ) value = "on";
			return true;
		}
		else if ( name == "checked" ) {
			return true;
		}
		else if ( name == "alignment" ) {
			
			float  halign_v = 0.5f;
			
			std::string halign = vals;

			if (halign == "right") halign_v = 1.0f;
			else if (halign == "left") halign_v = 0.0f;

	
			return true;
		}


		return false;
	}
}


