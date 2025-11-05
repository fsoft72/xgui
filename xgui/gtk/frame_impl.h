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

		//methods
		Frame ( xgui::Container * parent, const std::string & label );
		Frame ( GtkWidget * real_w );
		virtual ~Frame();

		bool addChild ( xgui::Widget * w );
		void delChild ( xgui::Widget * w );
		xgui::Widget * getChild();

		bool get_frame_prop ( std::string const &name, std::string &vals );
		bool set_frame_prop ( std::string const &name, std::string const &vals );
	};
}

#endif
