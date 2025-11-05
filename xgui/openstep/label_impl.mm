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

#include "label_impl.h"
#include "label.h"

#include "openstep_object.h"

static xguimpl::OpenStepWidget * MkLabelWidget ( std::string const &text )
{
	xguimpl::OpenStepWidget * widget = 0;
	return widget;
}

namespace xguimpl
{
	Label::Label ( OpenStepWidget * real_w ) : Widget ( real_w ) { }
	Label::Label ( xgui::Container * parent, std::string const &text ) : Widget ( MkLabelWidget ( text ) ) { }
	Label::~Label() { }

	bool Label::set_label_prop ( std::string const &name, std::string const &vals )
	{
		if ( name == "text" ) {
			return true;
		}
		else if ( name == "alignment" ) {
			std::pair<float, float> align = splitAlignment(vals);
			return true;
		}
		return false;
	}


	bool Label::get_label_prop ( std::string const &name, std::string &vals )
	{
		if ( name == "text" ) {
			return true;
		}
		else if ( name == "alignment" ) {
			float x, y;
			vals = joinAlignment(x, y);
			return true;
		}

		return false;
	}
}
