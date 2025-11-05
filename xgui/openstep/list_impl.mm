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

#include "list_impl.h"
#include "list.h"
#include "callback.h"
#include "model.h"
#include "image.h"
#include "image_impl.h"

namespace xguimpl
{
	List::List ( OpenStepWidget * real_w ) 
	: View ( real_w ) { }

	List::List ( xgui::Container * parent )
	: View ( 0 )
	{

	}

	List::~List() 
	{

	}

	bool List::linkEvent( std::string const &name )
	{
		if ( (name == "onselect") )
			return true;
		else if ( name == "onheaderclick" )
			return true;
		else if ( name == "onrefresh" )
			return true;
		else if ( name == "oncheck" )
			return true;
	
		return Widget::linkEvent(name);
	}

	void List::setAccessor(xgui::Widget * w)
	{
		xgui::List * downcasted_widget = dynamic_cast<xgui::List*>(w);
		ASSERT(downcasted_widget);
		this_list = downcasted_widget;

		Widget::setAccessor(w);
	}
	
	bool List::get_list_prop ( std::string const &name, std::string &vals )
	{
		if ( name == "search-enabled" ) {

		} else if ( name == "visible-headers" ) {

		} else if ( name == "has-icons" ) {

		} else if ( name == "has-checkboxes" ) {

		} else
			return false;
	
		return true;
	}

	bool List::set_list_prop ( std::string const &name, const std::string &vals ) 
	{
		if ( name == "search-enabled" ) {

		} else if ( name == "visible-headers" ) {

		} else if ( name == "has-icons" ) {
			
		} else if ( name == "has-checkboxes" ) {

		} else
			return false;
	
		return true;
	}

	void List::insertItem ( xgui::Model * parent, int child_pos, xgui::Model * child )
	{
		if ( (!this_list->getModel()) || (!this_list->num_columns_) ) return;
	
		xgui::Callback * refresh_cb = this_list->getEvent("onrefresh");
		if (refresh_cb) refresh_cb->call( this_list );
	}
	
	void List::removeItem ( xgui::Model * parent, int child_pos, xgui::Model * child ) 
	{
		if ( (!this_list->getModel()) || (!this_list->num_columns_) ) return;
	
		xgui::Callback * refresh_cb = this_list->getEvent("onrefresh");
		if (refresh_cb) refresh_cb->call( dynamic_cast<xgui::Widget*>(this_list) );
	}

	void List::refresh() 
	{
		int &num_columns = this_list->num_columns_;
		xgui::Model * model = this_list->getModel();
		xgui::StyleManager * stylem = this_list->styleman_;

		if ( (!model) || (!num_columns) ) return;
	
		xgui::Callback * refresh_cb = this_list->getEvent("onrefresh");
		if (refresh_cb) refresh_cb->call( this_list );
	}

	void List::addColumn ( int cpos, const std::string & cid, const std::string & cname, int cwidth, const std::string & calign ) 
	{	

	}

	bool List::delColumn ( const std::string & column_name )
	{
		return false;
	}

	void List::sort ( std::string const & column_name, xgui::TextPairCallback * compare_cb )
	{
	}

	bool List::isSelected(xgui::Model * model, int pos) 
	{
		xgui::Model * child = model->getChild(pos);
	}

	void List::select(xgui::Model * parent, int pos, bool state) 
	{
		xgui::Model * child = parent->getChild(pos);
	}

	void List::selectAll(xgui::Model * model, bool state) 
	{

	}

	bool List::isChecked(xgui::Model * parent, int pos)
	{
		xgui::Model * child = parent->getChild(pos);

		return false;
	}

	void List::check(xgui::Model * parent, int pos, bool state)
	{

	}

	void List::checkAll(xgui::Model * parent, bool state)
	{

	}
}

