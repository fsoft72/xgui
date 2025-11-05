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

#ifndef _XGUI_TAB_IMPL_H_
#define _XGUI_TAB_IMPL_H_

#include "widget_impl.h"

namespace xgui { class Container; }

namespace xguimpl
{
	struct Tab : public Widget
	{
		xgui::Tab * this_tab;

		//methods
		Tab ( xgui::Container * parent, const std::string & alignment );
		Tab ( OpenStepWidget * real_w );
		virtual ~Tab();

		virtual void setAccessor ( xgui::Widget * w );

		bool linkEvent ( std::string const &name );

		bool addChild ( xgui::Widget * w );
		void delChild ( xgui::Widget * w );

		bool get_tab_prop ( std::string const &name, std::string &vals );
		bool set_tab_prop ( std::string const &name, std::string const &vals );

		void setPageIcon ( xgui::Widget * w, xgui::Image * img );
		void setPageLabel ( xgui::Widget * w, const std::string & label );
	};
}

#endif
