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

#include "widget_impl.h"
#include "callback.h"
#include "container.h"
#include "master.h"
#include "master_impl.h"

#include "openstep_object.h"

namespace xguimpl
{
	Widget::Widget(OpenStepWidget * real_w) 
	: this_widget(0), widget(real_w), min_x(1), min_y(1) 
	{
		ASSERT([widget->o conformsToProtocol:@protocol(XGui)])
		[widget->o setOwner:this];
	}

	Widget::~Widget() 
	{
		delete widget; 
	}

	void Widget::setAccessor(xgui::Widget * w)
	{
 		this_widget = w;
	}

	void Widget::redraw()
	{

	}

	std::pair<int, int> Widget::getMousePos()
	{
		std::pair<int, int> pos;
		return pos;
	}

	std::pair<float, float> Widget::splitAlignment(std::string const &align)
	{
		float valign_v = 0.5f, halign_v = 0.5f;
		std::string::size_type p = align.find('-');
		if (p != std::string::npos) {
			std::string valign = align.substr(0, p);
			std::string halign = align.substr(++p, std::string::npos);
	
			if (valign == "bottom") valign_v = 1.0f;
			else if (valign == "top") valign_v = 0.0f;
	
			if (halign == "right") halign_v = 1.0f;
			else if (halign == "left") halign_v = 0.0f;
	
		}
		else {
			if (align == "top") valign_v = 0.0f;
			else if (align == "left") halign_v = 0.0f;
			else if (align == "right") halign_v = 1.0f;
			else if (align == "bottom") valign_v = 1.0f;
		}
	
		return std::make_pair(halign_v, valign_v);
	}
	
	std::string Widget::joinAlignment(float x, float y)
	{
		std::string alignment = "center";
	
		if ( (x == 0.5f) || (y == 0.5f) ) {
			if (x == 0.0f) alignment = "left";
			else if (x == 1.0f) alignment = "right";
			else if (y == 0.0f) alignment = "top";
			else if (y == 1.0f) alignment = "bottom";
		}
		else {
			std::string valign, halign;
	
			if (y == 1.0f) valign = "bottom";
			else if (y == 0.0f) valign = "top";
		
			if (x == 1.0f) halign = "right";
			else if (x == 0.0f) halign = "left";
	
			alignment = valign + "-" + halign;
		}
	
		return alignment;
	}

	bool Widget::get_widget_prop ( std::string const &name, std::string &vals )
	{
		if ( name == "tooltip-text" ) {
			vals = tip_text;
			return true;
		}
		else if ( name == "visible" ) {
			vals = "1";
			return true;
		}
		else if ( name == "width" ) {
			vals =  xgui::semantic_cast<std::string>(0);
			return true;
		}
		else if ( name == "height" ) {
			vals =  xgui::semantic_cast<std::string>(0);
			return true;
		}
		else if ( name == "enabled" ) {
			vals = xgui::semantic_cast<std::string>(0);
			return true;
		}
		else if ( name == "focused" ) {
 			vals = xgui::semantic_cast<std::string>(0);
			return true;
		}

		return false;
	}

	bool Widget::set_widget_prop ( std::string const &name, std::string const &vals )
	{
		if ( name == "tooltip-text" ) {
			tip_text = vals;
			return true;
		}	
		else if ( name == "visible" ) {
			return true;
		}
		else if ( name == "enabled" ) {
			return true;
		}
		else if ( name == "focused" ) {
			return true;
		}

		return false;
	}

	bool Widget::linkEvent(std::string const &name)
	{
		if ( name == "onfocus" ) {
			return true;
		}
		else if ( name == "onblur" ) {
			return true;
		}
		else if ( name == "onmouseover" ) {
			return true;
		}
		else if ( name == "onmouseout" ) {
			return true;
		}
		else if ( name == "onmousemove" ) {
			return true;
		}
		else if ( name == "onmousedown" ) {
			return true;
		}
		else if ( name == "onmouseup" ) {
			return true;
		}
		else if ( name == "onshow" ) {
			return true;
		}
		else if ( name == "onhide" ) {
			return true;
		}
		else if ( (name == "onenable") || (name == "ondisable") ) {
			return true;
		}
		else if ( name == "onresize" )
			return true;
		else if ( name == "onmove" )
			return true;
		else if ( name == "onkeydown" )
			return true;
		else if ( name == "onkeyup" )
			return true;
		else if ( name == "onkeypress" )
			return true;
		else if ( name == "onmouseclick" )
			return true;
		else if ( name == "onmousedblclick" )
			return true;
		else if ( name == "ondestroy" )
			return true;
		
		return false;
	}

	void Widget::recalcLayout() 
	{

	}

	int Widget::OnDestroy ( Widget * xw )
	{
		int rv = 0;

		xgui::Callback * cb = xw->this_widget->getEvent("ondestroy");
		if ( cb ) rv = !cb->call( xw->this_widget );
        
        /*DMESSAGE("OnDestroy: " << xw->this_widget);*/

		delete xw->this_widget;
	
		return rv;
	}
}
