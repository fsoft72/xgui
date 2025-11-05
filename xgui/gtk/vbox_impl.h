//
// C++ Interface: vbox_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_HBOX_IMPL_H_
#define _XGUI_HBOX_IMPL_H_

#include "widget_impl.h"

namespace xgui { class VBox;  }

namespace xguimpl
{
	struct VBox : public Widget
	{
		xgui::VBox * this_box;

 		VBox(xgui::Container * parent, int space, bool border);
		VBox(GtkWidget * real_w);

		virtual ~VBox();	

		void addChild(xgui::Widget * w);
		void setExpand(xgui::Widget * w, float expand);

		virtual void setAccessor(xgui::Widget * w);
	};
}

#endif
