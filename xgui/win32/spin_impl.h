//
// C++ Interface: spin_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_SPIN_IMPL_H_
#define _XGUI_SPIN_IMPL_H_

#include "widget_impl.h"

namespace xguimpl
{
	struct Spin : public Widget
	{
		int min_, max_;
		HWND _hSpin;
		std::string _alignment;
		std::string _last_val;
		int _last_pos;

		//methods
		DLLEXPORT Spin ( HWND real_w );
		DLLEXPORT Spin ( xgui::Container * parent, int min, int max );
		DLLEXPORT virtual ~Spin();

		DLLEXPORT virtual char const * bindWMCOMMAND( WORD command_id );
		DLLEXPORT bool linkEvent ( std::string const &name );

		DLLEXPORT int getValue();
		DLLEXPORT void setValue ( int pos );
		DLLEXPORT void setRange ( int min, int max );

		DLLEXPORT std::string getAlignment();
		DLLEXPORT void setAlignment(std::string const &align);

		DLLEXPORT static int OnChange ( xgui::Spin * s, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		DLLEXPORT static BOOL OnResize ( xgui::Spin * s, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		DLLEXPORT static BOOL OnMove ( xgui::Spin * s, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		DLLEXPORT static int OnSubmit ( xgui::Spin * s, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	};
}

#endif
