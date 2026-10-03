//
// C++ Implementation: vbox_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "vbox_impl.h"
#include "vbox.h"

namespace xguimpl
{
	VBox::VBox(xgui::Container * parent, int space, bool border) : Widget(gtk_box_new(GTK_ORIENTATION_VERTICAL, space)), this_box(0)
	{
		// GTK 4 has no container border: a margin gives the same spacing
		int margin = border ? 1 : 0;
		gtk_widget_set_margin_start ( widget, margin );
		gtk_widget_set_margin_end ( widget, margin );
		gtk_widget_set_margin_top ( widget, margin );
		gtk_widget_set_margin_bottom ( widget, margin );
	}

	VBox::VBox(GtkWidget * real_w) : Widget(real_w), this_box(0) { }

	VBox::~VBox() { }

	void VBox::setAccessor(xgui::Widget * w)
	{
		xgui::VBox * downcasted_widget = dynamic_cast<xgui::VBox*>(w);
		ASSERT(downcasted_widget);
		this_box = downcasted_widget;

		Widget::setAccessor(w);
	}

	void VBox::addChild(xgui::Widget * w)
	{
		gtk_box_append ( GTK_BOX ( widget ), GTK_WIDGET ( w->getImpl()->widget ) );
	}

	void VBox::setExpand(xgui::Widget * w, float expand)
	{
		int expand_flag = ((expand == 0.0) ? 0 : 1);

		// Expanding along the box orientation, filling is the default alignment of GTK 4 widgets
		gtk_widget_set_vexpand ( GTK_WIDGET ( w->getImpl()->widget ), expand_flag );
	}
}
