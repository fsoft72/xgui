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

#ifndef _XGUI_VIEW_IMPL_H_
#define _XGUI_VIEW_IMPL_H_

#include "widget_impl.h"

namespace xgui { class Model; }

namespace xguimpl
{
	struct View : public Widget
	{
		std::set<xgui::Model *> selset_;
		//methods
		View ( OpenStepWidget * real_w );
		virtual ~View();

		virtual void insertItem ( xgui::Model * parent, int child_pos, xgui::Model * child ) = 0;
		virtual void removeItem ( xgui::Model * parent, int child_pos, xgui::Model * child ) = 0;
		virtual void refresh() = 0;

		virtual bool isSelected(xgui::Model * model, int pos) = 0;
		virtual void select(xgui::Model * model, int pos, bool state) = 0;
		virtual void selectAll(xgui::Model * model, bool state) = 0;

		void addSelection ( xgui::Model * model );
		void delSelection ( xgui::Model * model );
		void clearSelectionsCache();
		bool isSelectionCached( xgui::Model * model );
	};
}

#endif
