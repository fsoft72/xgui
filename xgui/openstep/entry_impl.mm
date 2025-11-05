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

#include "entry.h"
#include "entry_impl.h"
#include "callback.h"

namespace xguimpl
{
	Entry::Entry ( OpenStepWidget * real_w ) : Widget ( real_w ), this_entry(0) {}

	Entry::Entry ( xgui::Container * parent, std::string const &text, int maxlen, bool editable, bool password_mode ) 
	: Widget ( 0 ), this_entry(0)
	{}

	Entry::~Entry() { }

	void Entry::setAccessor(xgui::Widget * w)
	{
		xgui::Entry * downcasted_widget = dynamic_cast<xgui::Entry*>(w);
		ASSERT(downcasted_widget);
		this_entry = downcasted_widget;

		Widget::setAccessor(w);
	}

	bool Entry::linkEvent( std::string const &name )
	{
		if ( name == "onsubmit" ) {
			return true;
		}
		else if (name == "onchange") {
			return true;
		}
	
		return Widget::linkEvent(name);
	}


	bool Entry::set_entry_prop ( std::string const &name, std::string const &vals )
	{
		if ( name == "text" ) {
			return true;
		}
		else if ( name == "editable" ) {
			return true;
		}
		else if ( name == "password-mode" ) {
			return true;
		}
		else if ( name == "alignment" ) {
			float align = 0.5f;
			if (vals == "left") align = 0.0f;
			else if (vals == "right") align = 1.0f;
	
			return true;
		}
		else if ( name == "maxlength" ) {
			unsigned int maxlen = xgui::semantic_cast<unsigned int>(vals);
			return true;
		}

		return false;
	}


	bool Entry::get_entry_prop ( std::string const &name, std::string &vals )
	{
		if ( name == "text" ) {
			return true;
		}
		else if ( name == "editable" ) {
			vals = xgui::semantic_cast<std::string>(1);
			return true;
		}
		else if ( name == "password-mode" ) {
			return true;
		}
		else if ( name == "alignment" ) {
			float align = 0.5f;
	
			if (align == 1.0f) vals = "right";
			else if (align == 0.0f) vals = "left";
			else vals = "center";
	
			return true;
		}
		else if ( name == "maxlength" ) {
			vals = xgui::semantic_cast<std::string>(0);
			return true;
		}

		return false;
	}

	void Entry::insertText(std::string const &text, int pos)
	{

	}
	
	void Entry::prependText(std::string const &txt)
	{
	}
	
	
	void Entry::appendText(std::string const &txt)
	{
	}
}
