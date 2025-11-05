//
// C++ Interface: widget_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_WIDGET_IMPL_H_
#define _XGUI_WIDGET_IMPL_H_

#include <windows.h>

namespace xgui { class Widget; }

namespace xguimpl
{
	struct Widget
	{
		xgui::Widget * this_widget;
		HWND widget;

		xgui::StrVoidMap  h_events;

		bool	redrawing;
		bool	in_redraw;

		int		min_x;
		int		min_y;
		int		cur_x;
		int		cur_y;
		int		pos_x;
		int		pos_y;

		bool	h_resize;
		bool	v_resize;

		bool	mouse_in;

		std::string	last_key_pressed;
		std::string	tip_text;

		void (*proxy_notify)(void *, xgui::Widget*);

		//methods
		DLLEXPORT Widget ( HWND real_w );
		DLLEXPORT ~Widget();

		DLLEXPORT virtual void setAccessor ( xgui::Widget * w );
		DLLEXPORT inline xgui::Widget * getAccessor() { return this_widget; }

		DLLEXPORT virtual bool linkEvent ( std::string const &name );
		DLLEXPORT virtual void recalcLayout();

		DLLEXPORT bool isMouseInWindow();

		DLLEXPORT virtual HWND getInternalAsParent();

		DLLEXPORT virtual void giveSize(std::pair<int, int> size);

		DLLEXPORT void setPos(std::pair<int, int> pos);

		DLLEXPORT std::pair<int, int> getMousePos();
		DLLEXPORT void redraw();

		DLLEXPORT virtual char const * bindWMCOMMAND ( WORD command_id );
		DLLEXPORT virtual char const * bindWMNOTIFY ( LPNMHDR nmhdr );

		DLLEXPORT bool get_widget_prop ( std::string const &name, std::string &vals );
		DLLEXPORT bool set_widget_prop ( std::string const &name, const std::string &vals );

		DLLEXPORT void set_tooltip_text ( HWND hWnd, const char * text );

		DLLEXPORT static int OnFocus ( xgui::Widget * w, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		DLLEXPORT static int OnBlur ( xgui::Widget * w, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		DLLEXPORT static int OnMouseDown ( xgui::Widget * w, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		DLLEXPORT static int OnMouseUp ( xgui::Widget * w, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		DLLEXPORT static int OnMouseDblClick ( xgui::Widget * w, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		DLLEXPORT static int OnMouseMove ( xgui::Widget * w, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		DLLEXPORT static int OnMouseOut ( xgui::Widget * w, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		DLLEXPORT static int OnHide ( xgui::Widget * w, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		DLLEXPORT static int OnShow ( xgui::Widget * w, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		DLLEXPORT static int OnEnable ( xgui::Widget * w, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		DLLEXPORT static int OnDisable ( xgui::Widget * w, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		DLLEXPORT static int OnResize ( xgui::Widget * w, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		DLLEXPORT static int OnMove ( xgui::Widget * w, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		DLLEXPORT static int OnKeyDown ( xgui::Widget * w, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		DLLEXPORT static int OnKeyUp ( xgui::Widget * w, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		DLLEXPORT static int OnDestroy ( xgui::Widget * w, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	};
}

#endif
