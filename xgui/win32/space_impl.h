//
// C++ Interface: space_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_SPACE_IMPL_H_
#define _XGUI_SPACE_IMPL_H_

#include "widget_impl.h"

namespace xguimpl
{
	struct Space : public Widget
	{
		//methods
		DLLEXPORT Space ( HWND real_w );
		DLLEXPORT Space ( xgui::Container * parent, int min_width, int min_height );
		DLLEXPORT virtual ~Space();

		DLLEXPORT static HWND MkSpace ( xgui::Container * parent );
	};
}

#endif
