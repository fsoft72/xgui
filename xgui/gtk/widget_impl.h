//
// C++ Interface: widget_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_WIDGET_IMPL_H_
#define _XGUI_WIDGET_IMPL_H_

#include <gtk/gtk.h>
#include "widget.h"

namespace xguimpl
{
	struct Widget
	{
		xgui::Widget * this_widget;
		GtkWidget * widget;
		
		std::string tip_text;
		int last_x, last_y;
		int last_w, last_h;

		Widget(GtkWidget * real_w);
		virtual ~Widget();

		bool isMoving(int x, int y);
		bool isResizing(int w, int h);
		std::pair<float, float> splitAlignment(std::string const &align);
		std::string joinAlignment(float x, float y);
		void redraw();
		std::pair<int, int> getMousePos();

		bool get_widget_prop ( std::string const &name, std::string &vals );
		bool set_widget_prop ( std::string const &name, std::string const &vals );

		virtual bool linkEvent(std::string const &name);
		virtual void recalcLayout();
		virtual void setAccessor(xgui::Widget * w);

		static int OnFocus ( GtkWidget * w, GdkEventFocus * event, Widget * xw );
		static int OnBlur ( GtkWidget * w, GdkEventFocus * event, Widget * xw );
		static int OnMouseOver ( GtkWidget * w, GdkEventCrossing *event, Widget * xw );
		static int OnMouseOut ( GtkWidget * w, GdkEventCrossing *event, Widget * xw );
		static int OnMouseDown ( GtkWidget * w, GdkEventButton * event, Widget * xw );
		static int OnMouseUp ( GtkWidget * w, GdkEventButton * event, Widget * xw );
		static int OnGdkEvent ( GtkWidget *widget, GdkEvent *event, Widget * xw );
		static int OnMouseMove ( GtkWidget * w, GdkEventMotion * event, Widget * xw );
		static int OnShow ( GtkWidget * w, Widget * xw );
		static int OnHide ( GtkWidget * w, Widget * xw );
		static int OnDestroy ( GtkWidget * w, Widget * xw );
		static int OnEnableDisable ( GtkWidget *w, GtkStateType state, Widget * xw );
	};
}

#endif
