//
// C++ Interface: hbox_impl
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

namespace xgui { class HBox;  }

namespace xguimpl
{
	struct HBox : public Widget
	{
		xgui::HBox * this_box;

		HBox(xgui::Container * parent, int space, bool border);
		HBox(GtkWidget * real_w);

		virtual ~HBox();

		void addChild(xgui::Widget * w);
		void setExpand(xgui::Widget * w, float expand);

		virtual void setAccessor(xgui::Widget * w);
	};
}

#endif
