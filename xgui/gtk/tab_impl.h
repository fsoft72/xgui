//
// C++ Interface: tab_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_TAB_IMPL_H_
#define _XGUI_TAB_IMPL_H_

#include "widget_impl.h"

namespace xgui { class Container; }

namespace xguimpl
{
	struct Tab : public Widget
	{
		xgui::Tab * this_tab;

		//methods
		Tab ( xgui::Container * parent, const std::string & alignment );
		Tab ( GtkWidget * real_w );
		virtual ~Tab();

		virtual void setAccessor ( xgui::Widget * w );

		bool linkEvent ( std::string const &name );

		bool addChild ( xgui::Widget * w );
		void delChild ( xgui::Widget * w );

		bool get_tab_prop ( std::string const &name, std::string &vals );
		bool set_tab_prop ( std::string const &name, std::string const &vals );

		void setPageIcon ( xgui::Widget * w, xgui::Image * img );
		void setPageLabel ( xgui::Widget * w, const std::string & label );
	
		static void OnSelect(GtkNotebook * widget, GtkWidget *page, int page_n, Tab * w);
	};
}

#endif
