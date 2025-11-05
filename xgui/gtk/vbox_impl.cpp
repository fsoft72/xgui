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
	VBox::VBox(xgui::Container * parent, int space, bool border) : Widget(gtk_vbox_new(FALSE, space)), this_box(0)
	{
		gtk_container_set_border_width ( GTK_CONTAINER ( widget ), border );
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
		gtk_box_pack_start ( GTK_BOX ( widget ), GTK_WIDGET ( w->getImpl()->widget ), 0, 0, 0 );
	}

	void VBox::setExpand(xgui::Widget * w, float expand)
	{
		int expand_flag = ((expand == 0.0) ? 0 : 1);

		gtk_box_set_child_packing ( GTK_BOX ( widget ), GTK_WIDGET ( w->getImpl()->widget ), expand_flag, expand_flag, 0, GTK_PACK_START );
	}
}
