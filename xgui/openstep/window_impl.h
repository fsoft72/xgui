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

#ifndef _XGUI_WINDOW_IMPL_H_
#define _XGUI_WINDOW_IMPL_H_

#include "widget_impl.h"

namespace xgui { class Window; class Image; }

namespace xguimpl
{
	struct Window : public Widget
	{
		xgui::Window * this_window;

		Window(xgui::Window * parent = 0);
		Window(OpenStepWidget * real_w);

		virtual ~Window();	

		bool get_window_prop ( std::string const &name, std::string &vals );
		bool set_window_prop ( std::string const &name, std::string const &vals );

		void show();
	
		int modalRun();
		void modalReturn(int response);
	
		bool addChild(xgui::Widget * w);
		void delChild(xgui::Widget * w);
	
		void maximize();
		void minimize();
		void restore();

		virtual bool linkEvent(std::string const &name);
		virtual void setAccessor(xgui::Widget * w);

		void setIcon(xgui::Image * icon);

		/* Cocoa Implementation Related Function */
		static int OnClose ( Window * xw );
		void recalcLayout();
	};
}

#endif
