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

#include "progressbar_impl.h"
#include "callback.h"
#include "progressbar.h"

namespace xguimpl
{
	Progressbar::Progressbar ( OpenStepWidget * real_w ) : Widget ( real_w ) {}
	Progressbar::Progressbar ( xgui::Container * parent, int min, int max ) : Widget ( 0 ) { }
	Progressbar::~Progressbar() {}

	bool Progressbar::linkEvent( std::string const &name )
	{
		if ( name == "onchange" ) {
			return true;
		}
	
		return Widget::linkEvent(name);
	}

	void Progressbar::setState (int min, int max, int pos)
	{
		double rel_pos = ((double)( pos - min )) / ((double)( max - min ));
	}
}
