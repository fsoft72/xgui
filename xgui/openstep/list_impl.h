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

#ifndef _XGUI_LIST_IMPL_H_
#define _XGUI_LIST_IMPL_H_

#include "style.h"
#include "list.h"
#include "view_impl.h"

namespace xgui { class List; }

namespace xguimpl
{
	struct List : public View
	{
		xgui::List * this_list;

		//methods
		List ( OpenStepWidget * real_w );
		List ( xgui::Container * parent );
		virtual ~List();
		virtual void setAccessor(xgui::Widget * w);


		bool get_list_prop ( std::string const &name, std::string &vals );
		bool set_list_prop ( std::string const &name, const std::string &vals );
		bool linkEvent( std::string const &name );

		virtual void insertItem ( xgui::Model * parent, int child_pos, xgui::Model * child );
		virtual void removeItem ( xgui::Model * parent, int child_pos, xgui::Model * child );
		virtual void refresh();

		bool isSelected(xgui::Model * parent, int pos);
		void select(xgui::Model * parent, int pos, bool state);
		void selectAll(xgui::Model * parent, bool state);

		bool isChecked(xgui::Model * parent, int pos);
		void check(xgui::Model * parent, int pos, bool state);
		void checkAll(xgui::Model * parent, bool state);

		void addColumn ( int cpos, const std::string & cid, const std::string & cname, int cwidth, const std::string & calign );
		bool delColumn ( const std::string & id );
		void sort ( const std::string & column_id, xgui::TextPairCallback * callback );

	};
}

#endif
