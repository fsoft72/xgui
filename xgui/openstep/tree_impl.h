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

#ifndef _XGUI_TREE_IMPL_H_
#define _XGUI_TREE_IMPL_H_

#include "widget_impl.h"
#include "view_impl.h"
#include "callback.h"

namespace xgui { class Window; }

namespace xguimpl
{
	struct Tree : public View
	{
		xgui::Tree * this_tree;

		//methods
		Tree ( OpenStepWidget * real_w );
		Tree ( xgui::Container * parent );
		virtual ~Tree();

		virtual void setAccessor ( xgui::Widget * w );

		bool linkEvent( std::string const &name );

		bool set_tree_prop ( std::string const &name, std::string const &val );
		bool get_tree_prop ( std::string const &name, std::string &dest );

		void refresh();
		void insertItem ( xgui::Model * parent, int child_pos, xgui::Model * child );
		void removeItem ( xgui::Model * parent, int child_pos, xgui::Model * child );

		void sort ( xgui::Model * node, xgui::TextPairCallback * callback );
		void expand ( xgui::Model * node );
		void collapse ( xgui::Model * node );

		bool isSelected(xgui::Model * parent, int pos);
		void select(xgui::Model * parent, int pos, bool state);
		void selectAll(xgui::Model * parent, bool state);

		bool isChecked(xgui::Model * parent, int pos);
		void check(xgui::Model * parent, int pos, bool state);
		void checkAll(xgui::Model * parent, bool state);
	};
}

#endif
