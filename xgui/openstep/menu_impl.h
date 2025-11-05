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

#ifndef _XGUI_BUTTON_IMPL_H_
#define _XGUI_BUTTON_IMPL_H_

#include "widget_impl.h"

namespace xgui { class Menu; class Image; }

namespace xguimpl
{
	struct Menu : public Widget
	{
		xgui::Menu * this_menu;

		Menu ( OpenStepWidget * real_w );
		Menu ( xgui::Window * parent, bool visible );
		virtual ~Menu();

		virtual void setAccessor(xgui::Widget * w);
		bool linkEvent( std::string const &name );

		void addItem(std::string const &parent_id, std::string const &id, std::string const &text, xgui::Image * icon = 0, std::string const & type = "" );
		void delItem(std::string const &id);

		void popupMenu(std::string const &id);

		void enableItem(std::string const &id);
		void disableItem(std::string const &id);
		bool isItemEnabled(std::string const &id);
		
		void checkItem(std::string const &id);
		void uncheckItem(std::string const &id);
		bool isItemChecked(std::string const &id);
		
		void setItemText(std::string const &id, std::string const &text);
		std::string getItemText(std::string const &id);
	};
}

#endif
