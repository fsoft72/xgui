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

#ifndef _XGUI_BUTTON_IMPL_H_
#define _XGUI_BUTTON_IMPL_H_

#include "widget_impl.h"

namespace xgui { class Window; class Image; }

namespace xguimpl
{
	struct Button : public Widget
	{
		//methods
		DLLEXPORT Button ( HWND real_w );
		DLLEXPORT Button ( xgui::Container * parent, std::string const &text, xgui::Image * image, bool toggle_button );
		DLLEXPORT virtual ~Button();

		DLLEXPORT bool linkEvent ( std::string const &name );
		DLLEXPORT char const * bindWMCOMMAND ( WORD command_id );

		DLLEXPORT bool get_button_prop ( std::string const &name, std::string &vals );
		DLLEXPORT bool set_button_prop ( std::string const &name, const std::string &vals );

		DLLEXPORT void setIcon ( xgui::Image * img );

		DLLEXPORT static int OnClick ( xgui::Button * b, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	};
}

#endif
