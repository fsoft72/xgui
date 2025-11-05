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

#include "tab.h"
#include "tab_impl.h"
#include "image.h"
#include "image_impl.h"
#include "callback.h"

namespace xguimpl
{

	Tab::Tab(xgui::Container * parent, const std::string & alignment) : Widget(0), this_tab(0)
	{
	}

	Tab::Tab(OpenStepWidget * real_w) : Widget(real_w), this_tab(0) { }

	Tab::~Tab() { }

	void Tab::setAccessor ( xgui::Widget * w )
	{
		this_tab = dynamic_cast<xgui::Tab *>(w);
		ASSERT(this_tab);

		Widget::setAccessor ( w );
	}

	bool Tab::linkEvent( std::string const &name )
	{
		if ( name == "onselect" ) {

			return true;
		}
	
		return Widget::linkEvent(name);
	}

	void Tab::delChild ( xgui::Widget * child ) { }

	bool Tab::addChild ( xgui::Widget * wid )
	{	
		return true;
	}

	bool Tab::set_tab_prop ( std::string const &name, std::string const &vals )
	{
		if ( name == "selected-num" ) {

			return true;
		}
		else if ( name == "selected-page" ) {
			return true;
		}

		return false;
	}

	bool Tab::get_tab_prop ( std::string const &name, std::string &vals )
	{
		if ( name == "selected-num" ) {
			return true;
		}
		else if ( name == "selected-page" ) {
			return true;
		}
		else if ( name == "page-count" ) {
			return true;
		}

		return false;
	}

	void Tab::setPageIcon ( xgui::Widget * w, xgui::Image * img )
	{
		if (!img) return;

	}

	void Tab::setPageLabel ( xgui::Widget * w, const std::string & label)
	{
	}
}
