//
// C++ Interface: frame_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_FRAME_IMPL_H_
#define _XGUI_FRAME_IMPL_H_

#include "widget_impl.h"

namespace xguimpl
{
	struct Frame : public Widget
	{
		xgui::Widget * child;

		xgui::Frame * this_frame;

		//methods
		DLLEXPORT Frame ( xgui::Container * parent, const std::string & label );
		DLLEXPORT Frame ( HWND real_w );
		DLLEXPORT virtual ~Frame();

		DLLEXPORT virtual void setAccessor ( xgui::Widget * w );

		DLLEXPORT bool linkEvent ( std::string const &name );

		DLLEXPORT bool addChild ( xgui::Widget * w );
		DLLEXPORT void delChild ( xgui::Widget * w );

		DLLEXPORT void giveSize ( std::pair<int, int> size );
		DLLEXPORT void recalcLayout();

		DLLEXPORT bool get_frame_prop ( std::string const &name, std::string &vals );
		DLLEXPORT bool set_frame_prop ( std::string const &name, std::string const &vals );

		DLLEXPORT HWND getInternalAsParent();

		DLLEXPORT xgui::Widget * getChild();

		DLLEXPORT static int OnMove ( xgui::Frame * f, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		DLLEXPORT static int OnResize ( xgui::Frame * f, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	};
}

#endif
