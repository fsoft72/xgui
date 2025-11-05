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

#include "view_impl.h"
#include "model.h"

namespace xguimpl
{
	View::View ( OpenStepWidget * real_w ) : Widget ( real_w ) {}
	View::~View() {}

	void View::addSelection ( xgui::Model * model )
	{
		DMESSAGE("Adding selection " << model->getString(0));
		selset_.insert ( model );
	}
	
	
	void View::delSelection ( xgui::Model * model )
	{
		DMESSAGE("Removing selection " << model->getString(0));
		selset_.erase ( model );
	}
	
	
	bool View::isSelectionCached ( xgui::Model * model )
	{
		std::set<xgui::Model *>::iterator iter = selset_.find ( model );
		return iter != selset_.end();
	}
	
	void View::clearSelectionsCache()
	{
		selset_.clear();
	}
}
