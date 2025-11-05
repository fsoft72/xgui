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

#include "toolbar_impl.h"
#include "callback.h"
#include "image.h"
#include "image_impl.h"
#include "master_impl.h"

namespace xguimpl
{
	Toolbar::Toolbar ( OpenStepWidget * real_w ) : Widget( real_w ) {}
	Toolbar::Toolbar ( xgui::Container * parent ) : Widget ( 0 ) { }
	Toolbar::~Toolbar() { }

	bool Toolbar::linkEvent(std::string const & name) 
	{
		if ( name == "onclick" )
			return true;
		
		return Widget::linkEvent(name);
	}

	void Toolbar::setAccessor(xgui::Widget * w)
	{
		xgui::Toolbar * downcasted_widget = dynamic_cast<xgui::Toolbar*>(w);
		ASSERT(downcasted_widget);
		this_tb = downcasted_widget;

		Widget::setAccessor(w);
	}

	void Toolbar::setStyle(std::string const &vals)
	{

	}

	std::string Toolbar::getStyle()
	{
		std::string vals = "both";
		return vals;
	}

	void Toolbar::addSeparator() 
	{ 

	}

	void Toolbar::addButton ( const xgui::TbButtonInfo & btn )
	{
	}
	
	void Toolbar::addToggleButton ( const xgui::TbButtonInfo & btn )
	{
	}
	
	void Toolbar::addRadioButtons ( std::vector<xgui::TbButtonInfo> const & checkgroup )
	{
	}

	bool Toolbar::removeButton ( const std::string & name )
	{
	}
	
	void Toolbar::enableButton ( const std::string & name )
	{
	}
	
	void Toolbar::disableButton ( const std::string & name )
	{
	}
	
	void Toolbar::checkButton ( const std::string & name )
	{
	}
	
	void Toolbar::uncheckButton ( const std::string & name )
	{
	}
	
	bool Toolbar::isButtonChecked ( const std::string & name )
	{
	}

}
