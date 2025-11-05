//
// C++ Interface: window_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_WINDOW_IMPL_H_
#define _XGUI_WINDOW_IMPL_H_

#include "widget_impl.h"

namespace xgui { class Window; class Image; }

namespace xguimpl
{
	struct Window : public Widget
	{
		xgui::Window * this_window;
		GtkWidget * vbox;
		GtkWidget * status;

		Window(xgui::Window * parent = 0);
		Window(GtkWidget * real_w);

		virtual ~Window();	

		bool get_window_prop ( std::string const &name, std::string &vals );
		bool set_window_prop ( std::string const &name, std::string const &vals );

		void show();
	
		int modalRun();
		void modalReturn(int response);
	
		void reorderChildren();
		bool addChild(xgui::Widget * w);
		void delChild(xgui::Widget * w);
	
		void maximize();
		void minimize();
		void restore();

		virtual bool linkEvent(std::string const &name);
		virtual void setAccessor(xgui::Widget * w);

		void setIcon(xgui::Image * icon);

		//Internal Event Handlers
		static int OnClose ( GtkWidget * w, GdkEvent * e, Window * xw );
	};
}

#endif
