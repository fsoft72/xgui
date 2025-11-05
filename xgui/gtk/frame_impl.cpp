//
// C++ Implementation: frame_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "frame_impl.h"
#include "frame.h"

namespace xguimpl
{
	Frame::Frame ( GtkWidget * real_w ) : Widget ( real_w ), child(0) {}
	Frame::Frame ( xgui::Container * parent, std::string const &label ) : Widget (gtk_frame_new(label.c_str()) ), child(0) {}
	Frame::~Frame() {}

	bool Frame::addChild(xgui::Widget * wid)
	{
		if (child) return false;
	
		child = wid;
		gtk_container_add ( GTK_CONTAINER(widget), GTK_WIDGET(wid->getImpl()->widget) );
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
	
			gtk_frame_get_label_align ( GTK_FRAME(widget), &halign_v, &valign_v );
	
			if (valign_v == 1.0f) valign = "bottom";
			else if (valign_v == 0.0f) valign == "top";
	
			if (halign_v == 1.0f) halign = "right";
			else if (halign_v == 0.0f) halign = "left";
		} else if ( name == "text" ) {
			vals = gtk_frame_get_label(GTK_FRAME(widget));
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
	
				gtk_frame_set_label_align ( GTK_FRAME(widget), halign_v, valign_v );
			}
		} else if ( name == "text" ) {
			gtk_frame_set_label(GTK_FRAME(widget), vals.c_str());
		} else
			return false;

		return true;
	}
}
