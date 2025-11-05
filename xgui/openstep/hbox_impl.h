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

#ifndef _XGUI_HBOX_IMPL_H_
#define _XGUI_HBOX_IMPL_H_

#include "widget_impl.h"

namespace xgui { class HBox;  }

namespace xguimpl
{
	struct HBox : public Widget
	{
		xgui::HBox * this_box;

		HBox(xgui::Container * parent, int space, bool border);
		HBox(OpenStepWidget * real_w);

		virtual ~HBox();

		void addChild(xgui::Widget * w);
		void setExpand(xgui::Widget * w, float expand);

		virtual void setAccessor(xgui::Widget * w);

		/* Cocoa Related things */
		int space_;
		void giveSize ( std::pair<float, float> size );
		void recalcLayout();
	};
}

#endif
