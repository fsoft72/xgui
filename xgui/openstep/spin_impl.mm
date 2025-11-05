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

#include "spin_impl.h"
#include "callback.h"

namespace xguimpl
{
	Spin::Spin ( OpenStepWidget * real_w ) : Widget ( real_w ), min_(0), max_(0) {}

	Spin::Spin ( xgui::Container * parent, int min, int max )
	: Widget (0), min_(min), max_(max)
	{ }

	Spin::~Spin() {}

	bool Spin::linkEvent( std::string const &name )
	{
		if ( name == "onsubmit" ) {
			return true;
		}
		else if (name == "onchange") {
			return true;
		}

		return Widget::linkEvent(name);
	}

	int Spin::getValue()
	{
		return 0;
	}

	void Spin::setValue ( int pos )
	{

	}

	void Spin::setRange ( int min, int max )
	{

	}

	std::string Spin::getAlignment()
	{
		std::string vals;
		float align = 0.5f;

		if (align == 1.0f) vals = "right";
		else if (align == 0.0f) vals = "left";
		else vals = "center";

		return vals;
	}

	void Spin::setAlignment(std::string const &vals)
	{
		float align = 0.5f;
		if (vals == "left") align = 0.0f;
		else if (vals == "right") align = 1.0f;

	}
}
