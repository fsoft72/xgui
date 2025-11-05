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

#ifndef _XGUI_VBOX_IMPL_H_
#define _XGUI_VBOX_IMPL_H_

#include "widget_impl.h"

namespace xguimpl
{
	struct VBox : public Widget
	{
		int space;

		//methods
		DLLEXPORT VBox ( xgui::Container * parent, int space, bool border );
		DLLEXPORT VBox ( HWND real_w );
		DLLEXPORT virtual ~VBox();

		DLLEXPORT bool addChild ( xgui::Widget * w );
		DLLEXPORT void delChild ( xgui::Widget * w );

		DLLEXPORT void giveSize ( std::pair<int, int> size );
		DLLEXPORT void recalcLayout();

		DLLEXPORT void setExpand ( xgui::Widget * w, float expand );
	};
}

#endif
