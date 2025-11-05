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

#include "menu_impl.h"
#include "menu.h"
#include "callback.h"
#include "image.h"
#include "image_impl.h"

namespace xguimpl
{

	Menu::Menu ( OpenStepWidget * real_w ) : Widget( real_w ) {}

	Menu::Menu ( xgui::Window * parent, bool visible )
	: Widget ( 0 )
	{

	}

	Menu::~Menu() {}

	bool Menu::linkEvent(std::string const & name) 
	{
		if (name == "onselect")
			return true;
		
		return Widget::linkEvent(name);
	}

	void Menu::setAccessor(xgui::Widget * w)
	{
		xgui::Menu * downcasted_widget = dynamic_cast<xgui::Menu*>(w);
		ASSERT(downcasted_widget);
		this_menu = downcasted_widget;

		Widget::setAccessor(w);
	}

	void Menu::addItem( std::string const &parent_id, std::string const &id, std::string const &text, xgui::Image * icon, std::string const &type )
	{

	}
	
	void Menu::delItem( std::string const &id)
	{

	}
	
	void Menu::popupMenu(std::string const &id)
	{

	}
	
	void Menu::enableItem(std::string const &id)
	{

	}
	
	void Menu::disableItem(std::string const &id)
	{

	}
	
	bool Menu::isItemEnabled(std::string const &id)
	{

	}
	
	void Menu::checkItem(std::string const &id)
	{

	}
	
	void Menu::uncheckItem(std::string const &id)
	{

	}
	
	bool Menu::isItemChecked(std::string const &id)
	{

	}
	
	void Menu::setItemText(std::string const &id, std::string const &text)
	{

	}
	
	std::string Menu::getItemText(std::string const &id)
	{
		return "";
	}
}
