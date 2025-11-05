//
// C++ Interface: button_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef _XGUI_BUTTON_IMPL_H_
#define _XGUI_BUTTON_IMPL_H_

#include "widget_impl.h"

namespace xgui { class Button; class Image; }

namespace xguimpl
{
	struct Button : public Widget
	{
		xgui::Button * this_button;

		//methods
		Button ( GtkWidget * real_w );
		Button ( xgui::Container * parent, std::string const &text, xgui::Image * image, bool toggle_button );
		virtual ~Button();	

		virtual void setAccessor(xgui::Widget * w);
		bool linkEvent( std::string const &name );

		bool get_button_prop ( std::string const &name, std::string &vals );
		bool set_button_prop ( std::string const &name, const std::string &vals );

		void setIcon ( xgui::Image * img );

		static 	int OnClick ( GtkWidget * w, Button * btn );
	};
}

#endif
