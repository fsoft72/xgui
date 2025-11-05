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
		int min_, max_;

		//methods
		Slider ( GtkWidget * real_w );
		Slider ( xgui::Container * parent, int min, int max, bool vertical );
		virtual ~Slider();

		bool linkEvent ( std::string const &name );

		bool get_slider_prop ( std::string const &name, std::string &vals );
		bool set_slider_prop ( std::string const &name, const std::string &vals );

		int getPos();
		void setPos ( int pos );

		static void OnChange ( GtkWidget * w, Slider * pbar );

	};
}

#endif
