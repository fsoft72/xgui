//
// C++ Interface: checkbox_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef _XGUI_CHECKBOX_IMPL_H_
#define _XGUI_CHECKBOX_IMPL_H_

#include "widget_impl.h"

namespace xgui { class Checkbox; }

namespace xguimpl
{
	struct Checkbox : public Widget
	{
		xgui::Checkbox * this_checkbox;
		std::string value;

		//methods
		Checkbox ( GtkWidget * real_w );
		Checkbox ( xgui::Container * parent, std::string const &text );
		virtual ~Checkbox();	

		virtual void setAccessor(xgui::Widget * w);
		bool linkEvent( std::string const &name );

		bool get_checkbox_prop ( std::string const &name, std::string &vals );
		bool set_checkbox_prop ( std::string const &name, const std::string &vals );

		static void OnClick ( GtkWidget * w, Checkbox * btn );
	};
}

#endif
