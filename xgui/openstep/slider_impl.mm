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

#include "slider_impl.h"
#include "callback.h"

#include "openstep_object.h"

static xguimpl::OpenStepWidget * MkSliderWidget( int min, int max, bool vertical )
{
	xguimpl::OpenStepWidget * widget = 0;
	return widget;
}

namespace xguimpl
{
	Slider::Slider ( OpenStepWidget * real_w ) : Widget ( real_w ), min_(0), max_(0) {}

	Slider::Slider ( xgui::Container * parent, int min, int max, bool vertical )
	: Widget ( MkSliderWidget(min, max, vertical ) ), min_(min), max_(max)
	{ }

	Slider::~Slider() {}

	bool Slider::linkEvent( std::string const &name )
	{
		if ( name == "onchange" ) {
			return true;
		}
	
		return Widget::linkEvent(name);
	}

	bool Slider::get_slider_prop ( std::string const &name, std::string &vals )
	{
		if ( name == "pos" ) {
			vals = xgui::semantic_cast<std::string> (getPos());
			return true;
		}
		else if ( name == "min" ) {
			vals = xgui::semantic_cast<std::string> (min_);
			return true;
		}
		else if ( name == "max" ) {
			vals = xgui::semantic_cast<std::string> (max_);
			return true;
		}

		return false;
	}

	bool Slider::set_slider_prop ( std::string const &name, const std::string &vals )
	{
		if ( name == "pos" ) {
			setPos ( xgui::semantic_cast<int> ( vals ) );
			return true;
		}
		else if ( name == "min" ) {
			min_ = xgui::semantic_cast<int> ( vals );
			return true;
		}
		else if ( name == "max" ) {
			max_ = xgui::semantic_cast<int> ( vals );
			return true;
		}

		return false;
	}


	void Slider::setPos (int pos)
	{
	
	}
	
	int Slider::getPos()
	{
		return 0;
	}
}
