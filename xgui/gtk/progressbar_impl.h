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
		Progressbar ( GtkWidget * real_w );
		Progressbar ( xgui::Container * parent, int min, int max );
		virtual ~Progressbar();

		bool linkEvent ( std::string const &name );
		void setState ( int min, int max, int pos );

		static void OnChange ( GtkWidget * w, Progressbar * pbar );
	};
}

#endif
