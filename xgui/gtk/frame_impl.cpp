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
		gtk_frame_set_child ( GTK_FRAME(widget), GTK_WIDGET(wid->getImpl()->widget) );
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
			// GTK 4 frames only have a horizontal label alignment
			float halign_v = gtk_frame_get_label_align ( GTK_FRAME(widget) );
			vals = joinAlignment(halign_v, 0.5f);
		} else if ( name == "text" ) {
			const char * label = gtk_frame_get_label(GTK_FRAME(widget));
			vals = label ? label : "";
		} else
			return false;
	
		return true;
	}

	bool Frame::set_frame_prop ( std::string const &name, std::string const &vals )
	{
		if ( name == "alignment" ) {
			// GTK 4 frames only have a horizontal label alignment
			gtk_frame_set_label_align ( GTK_FRAME(widget), splitAlignment(vals).first );
		} else if ( name == "text" ) {
			gtk_frame_set_label(GTK_FRAME(widget), vals.c_str());
		} else
			return false;

		return true;
	}
}
