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

#include "frame_impl.h"
#include "frame.h"

namespace xguimpl
{
	Frame::Frame ( OpenStepWidget * real_w ) : Widget ( real_w ), child(0) {}
	Frame::Frame ( xgui::Container * parent, std::string const &label ) : Widget (0), child(0) {}
	Frame::~Frame() {}

	bool Frame::addChild(xgui::Widget * wid)
	{
		if (child) return false;
	
		child = wid;
		return true;
	}
	
	void Frame::delChild(xgui::Widget * wid)
	{
		if (child == wid)
			child = 0;
	}

	xgui::Widget * Frame::getChild() { return child; }

	bool Frame::get_frame_prop ( std::string const &name, std::string &vals )
	{
		if ( name == "alignment" ) {
			std::string valign = "center", halign = "center";
			float valign_v, halign_v;

			if (valign_v == 1.0f) valign = "bottom";
			else if (valign_v == 0.0f) valign == "top";
	
			if (halign_v == 1.0f) halign = "right";
			else if (halign_v == 0.0f) halign = "left";
		} else if ( name == "text" ) {
			vals = "";
		} else
			return false;
	
		return true;
	}

	bool Frame::set_frame_prop ( std::string const &name, std::string const &vals )
	{
		if ( name == "alignment" ) {
			std::string::size_type p = vals.find('-');
			if (p != std::string::npos) {
				float valign_v = 0, halign_v = 0;
				std::string valign = vals.substr(0, p);
				std::string halign = vals.substr(++p, std::string::npos);
	
				if (valign == "bottom") valign_v = 1.0f;
				else if (valign == "top") valign_v = 0.0f;
	
				if (halign == "right") halign_v = 1.0f;
				else if (halign == "left") halign_v = 0.0f;
			}
		} else if ( name == "text" ) {

		} else
			return false;

		return true;
	}
}
