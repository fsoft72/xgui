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

#include "tree_impl.h"
#include "tree.h"
#include "model.h"
#include "image.h"
#include "image_impl.h"
#include "style.h"

namespace xguimpl
{
	Tree::Tree ( OpenStepWidget * real_w ) 
	: View (0)
	{
	}

	Tree::Tree ( xgui::Container * parent )
	: View(0)
	{
	}

	Tree::~Tree() 
	{ 
	}

	bool Tree::linkEvent( std::string const &name )
	{
		if ( (name == "onselect") )
			return true;
		else if ( name == "onrefresh" )
			return true;
		else if ( name == "oncheck" )
			return true;
		else if ( name == "onexpand" ) {
			return true;
		}
	
		return Widget::linkEvent(name);
	}

	void Tree::setAccessor(xgui::Widget * w)
	{
		xgui::Tree * downcasted_widget = dynamic_cast<xgui::Tree*>(w);
		ASSERT(downcasted_widget);
		this_tree = downcasted_widget;

		Widget::setAccessor(w);
	}

	bool Tree::set_tree_prop ( std::string const &name, std::string const &val )
	{
		if ( name == "search-enabled" ) {

		} else if ( name == "has-icons" ) {
			bool should_put_icons = ((val == "1") ? true : false);
		} else if ( name == "has-checkboxes" ) {
			bool should_put_checks = ((val == "1") ? true : false);
		} else
			return false;

		return true;
	}

	bool Tree::get_tree_prop ( std::string const &name, std::string &dest )
	{
		if ( name == "search-enabled" ) {

		} else if ( name == "has-icons" ) {

		} else if ( name == "has-checkboxes" ) {

		} else
			return false;

		return true;
	}

	void Tree::insertItem ( xgui::Model * parent, int child_pos, xgui::Model * child )
	{
		if ( (!this_tree->getModel()) ) return;

		xgui::Callback * refresh_cb = this_tree->getEvent("onrefresh");
		if (refresh_cb) refresh_cb->call( this_tree );
	
	}

	void Tree::removeItem ( xgui::Model * parent, int child_pos, xgui::Model * child ) 
	{
		if ( (!this_tree->getModel()) ) return;
	
		xgui::Callback * refresh_cb = this_tree->getEvent("onrefresh");
		if (refresh_cb) refresh_cb->call( this_tree );
	
	}

	void Tree::refresh() 
	{
		xgui::Model * model = this_tree->getModel();
		if ( (!model) ) return;
	
		xgui::Callback * refresh_cb = this_tree->getEvent("onrefresh");
		if (refresh_cb) refresh_cb->call( this_tree );
	
	}

	bool Tree::isSelected(xgui::Model * model, int pos) 
	{
	}

	void Tree::select(xgui::Model * parent, int pos, bool state) 
	{
	}

	void Tree::selectAll(xgui::Model * model, bool state) 
	{
	}

	bool Tree::isChecked(xgui::Model * parent, int pos)
	{
	}

	void Tree::check(xgui::Model * parent, int pos, bool state)
	{
	}

	void Tree::checkAll(xgui::Model * parent, bool state)
	{
	}

	void Tree::expand ( xgui::Model * node )
	{
		if (!node) return;
	}

	void Tree::collapse ( xgui::Model * node )
	{
		if (!node) return;
	}

	void Tree::sort ( xgui::Model * node, xgui::TextPairCallback * callback ) {}

}
