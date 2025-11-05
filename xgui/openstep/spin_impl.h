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

#ifndef _XGUI_SPIN_IMPL_H_
#define _XGUI_SPIN_IMPL_H_

#include "widget_impl.h"

namespace xguimpl
{
	struct Spin : public Widget
	{
		int min_, max_;

		//methods
		Spin ( OpenStepWidget * real_w );
		Spin ( xgui::Container * parent, int min, int max );
		virtual ~Spin();

		bool linkEvent ( std::string const &name );

		int getValue();
		void setValue ( int pos );
		void setRange ( int min, int max );

		std::string getAlignment();
		void setAlignment(std::string const &align);

	};
}

#endif
