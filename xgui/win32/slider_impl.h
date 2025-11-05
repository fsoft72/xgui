//
// C++ Interface: slider_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_SLIDER_IMPL_H_
#define _XGUI_SLIDER_IMPL_H_

#include "widget_impl.h"

namespace xguimpl
{
	struct Slider : public Widget
	{
		//methods
		DLLEXPORT Slider ( HWND real_w );
		DLLEXPORT Slider ( xgui::Container * parent, int min, int max, bool vertical );
		DLLEXPORT virtual ~Slider();

		DLLEXPORT static HWND MkSlider ( xgui::Container * parent, bool vertical );

		DLLEXPORT bool linkEvent ( std::string const &name );

		DLLEXPORT bool get_slider_prop ( std::string const &name, std::string &vals );
		DLLEXPORT bool set_slider_prop ( std::string const &name, const std::string &vals );

		DLLEXPORT int getPos();
		DLLEXPORT void setPos ( int pos );

		DLLEXPORT static int OnChange ( xgui::Slider * sld, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	};
}

#endif
