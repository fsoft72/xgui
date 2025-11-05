//
// C++ Interface: button_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_CHECKBOX_IMPL_H_
#define _XGUI_CHECKBOX_IMPL_H_

#include "widget_impl.h"

namespace xgui { class Window; }

namespace xguimpl
{
	struct Checkbox : public Widget
	{
		//methods
		DLLEXPORT Checkbox ( HWND real_w );
		DLLEXPORT Checkbox ( xgui::Container * parent, std::string const &text );
		DLLEXPORT virtual ~Checkbox();

		DLLEXPORT bool linkEvent ( std::string const &name );
		DLLEXPORT char const * bindWMCOMMAND ( WORD command_id );

		DLLEXPORT bool get_checkbox_prop ( std::string const &name, std::string &vals );
		DLLEXPORT bool set_checkbox_prop ( std::string const &name, const std::string &vals );

		DLLEXPORT static int OnClick ( xgui::Checkbox * c, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	};
}

#endif
