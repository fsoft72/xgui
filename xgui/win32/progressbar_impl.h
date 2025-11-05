//
// C++ Interface: progressbar_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_PROGRESSBAR_IMPL_H_
#define _XGUI_PROGRESSBAR_IMPL_H_

#include "widget_impl.h"

namespace xguimpl
{
	struct Progressbar : public Widget
	{
		//methods
		DLLEXPORT Progressbar ( HWND real_w );
		DLLEXPORT Progressbar ( xgui::Container * parent, int min, int max );
		DLLEXPORT virtual ~Progressbar();

		DLLEXPORT bool linkEvent ( std::string const &name );
		DLLEXPORT void setState ( int min, int max, int pos );

		DLLEXPORT static void OnChange ( HWND w, Progressbar * pbar );
	};
}

#endif
