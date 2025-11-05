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

namespace xgui { class ImageView; class Image; }

namespace xguimpl
{
	struct ImageView : public Widget
	{
		OpenStepWidget * scroll_widget;
		OpenStepWidget * img_widget;
		xgui::ImageView * this_imgview;

		//methods
		ImageView ( OpenStepWidget * real_w );
		ImageView ( xgui::Container * parent );
 		void init();
		virtual ~ImageView();	

		virtual void setAccessor(xgui::Widget * w);
		void Resize(int x, int y);

		bool get_imageview_prop ( std::string const &name, std::string &vals );
		bool set_imageview_prop ( std::string const &name, const std::string &vals );

		void setImage ( xgui::Image * img );
	};
}

#endif
