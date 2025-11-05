//
// C++ Interface: view_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_VIEW_IMPL_H_
#define _XGUI_VIEW_IMPL_H_

#include "widget_impl.h"

namespace xgui { class Model; }

namespace xguimpl
{
	struct View : public Widget
	{
		//methods
		DLLEXPORT View ( HWND real_w );
		DLLEXPORT virtual ~View();

		DLLEXPORT virtual void insertItem ( xgui::Model * parent, int child_pos, xgui::Model * child ) = 0;
		DLLEXPORT virtual void removeItem ( xgui::Model * parent, int child_pos, xgui::Model * child ) = 0;
		DLLEXPORT virtual void refresh() = 0;

		DLLEXPORT virtual bool isSelected ( xgui::Model * parent, int child_pos ) = 0;
		DLLEXPORT virtual void select ( xgui::Model * parent, int child_pos, bool state ) = 0;
		DLLEXPORT virtual void selectAll ( xgui::Model * parent, bool state ) = 0;

	};
}

#endif
