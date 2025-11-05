//
// C++ Interface: label_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_LABEL_IMPL_H_
#define _XGUI_LABEL_IMPL_H_

#include "widget_impl.h"

namespace xguimpl
{
	struct Label : public Widget
	{
		//methods
		DLLEXPORT Label( HWND real_w );
		DLLEXPORT Label ( xgui::Container * parent, std::string const &text );
		DLLEXPORT virtual ~Label();

		DLLEXPORT static HWND MkLabel ( xgui::Container * parent, std::string const &text );

		DLLEXPORT bool get_label_prop ( std::string const &name, std::string &vals );
		DLLEXPORT bool set_label_prop ( std::string const &name, const std::string &vals );
	};
}

#endif
