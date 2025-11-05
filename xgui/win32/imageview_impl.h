//
// C++ Interface: imageview_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_IMAGEVIEW_IMPL_H_
#define _XGUI_IMAGEVIEW_IMPL_H_

#include "widget_impl.h"

namespace xguimpl
{
	struct ImageView : public Widget
	{
		char h_align;
		char v_align;
		int start_x, start_y;

		xgui::ImageView * this_imageview;

		//methods
		DLLEXPORT ImageView ( HWND real_w );
		DLLEXPORT ImageView ( xgui::Container * parent );
		DLLEXPORT virtual ~ImageView();

		DLLEXPORT virtual void setAccessor ( xgui::Widget * w );

		DLLEXPORT static HWND MkImageView ( xgui::Container * parent );

		DLLEXPORT bool get_imageview_prop ( std::string const &name, std::string &vals );
		DLLEXPORT bool set_imageview_prop ( std::string const &name, const std::string &vals );

		DLLEXPORT void setImage ( xgui::Image * img );

		DLLEXPORT static LRESULT CALLBACK wndProc ( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

		DLLEXPORT void OnPaint ( WPARAM wParam, LPARAM lParam );
		DLLEXPORT void OnHScroll ( WPARAM wParam, LPARAM lParam );
		DLLEXPORT void OnVScroll ( WPARAM wParam, LPARAM lParam );
		DLLEXPORT void OnResize ( WPARAM wParam, LPARAM lParam );

	};
}

#endif
