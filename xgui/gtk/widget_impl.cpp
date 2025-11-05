//
// C++ Implementation: widget_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "widget_impl.h"
#include "callback.h"
#include "container.h"
#include "master.h"
#include "master_impl.h"

namespace xguimpl
{
	Widget::Widget(GtkWidget * real_w) 
	: this_widget(0), widget(real_w), last_x(0), last_y(0), last_w(0), last_h(0) 
	{
		if (!(GTK_WIDGET_NO_WINDOW(widget))) {
			if (widget->window != NULL) {
				int emask = gdk_window_get_events(widget->window);
				gdk_window_set_events(widget->window, (GdkEventMask)(emask | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK | GDK_ENTER_NOTIFY_MASK | GDK_LEAVE_NOTIFY_MASK) );
			}
		}

		gtk_signal_connect ( GTK_OBJECT ( widget ), "destroy", G_CALLBACK ( OnDestroy ), this );
		gtk_signal_connect ( GTK_OBJECT ( widget ), "event", G_CALLBACK ( OnGdkEvent ), this );
	}

	Widget::~Widget() {}

	void Widget::setAccessor(xgui::Widget * w)
	{
 		this_widget = w;
	}

	bool Widget::isMoving(int x, int y)
	{
		if ((x != last_x) || (y != last_y)) {
			last_x = x;
			last_y = y;
			return true;
		}
		return false;
	}
	
	bool Widget::isResizing(int w, int h)
	{
		if ((w != last_w) || (h != last_h)) {
			last_w = w;
			last_h = h;
			return true;
		}
		return false;
	}

	void Widget::redraw()
	{
		gtk_widget_queue_draw(widget);
	}

	std::pair<int, int> Widget::getMousePos()
	{
		std::pair<int, int> pos;
		gtk_widget_get_pointer(widget, &pos.first, &pos.second);
		return pos;
	}

	std::pair<float, float> Widget::splitAlignment(std::string const &align)
	{
		float valign_v = 0.5f, halign_v = 0.5f;
		std::string::size_type p = align.find('-');
		if (p != std::string::npos) {
			std::string valign = align.substr(0, p);
			std::string halign = align.substr(++p, std::string::npos);
	
			if (valign == "bottom") valign_v = 1.0f;
			else if (valign == "top") valign_v = 0.0f;
	
			if (halign == "right") halign_v = 1.0f;
			else if (halign == "left") halign_v = 0.0f;
	
		}
		else {
			if (align == "top") valign_v = 0.0f;
			else if (align == "left") halign_v = 0.0f;
			else if (align == "right") halign_v = 1.0f;
			else if (align == "bottom") valign_v = 1.0f;
		}
	
		return std::make_pair(halign_v, valign_v);
	}
	
	std::string Widget::joinAlignment(float x, float y)
	{
		std::string alignment = "center";
	
		if ( (x == 0.5f) || (y == 0.5f) ) {
			if (x == 0.0f) alignment = "left";
			else if (x == 1.0f) alignment = "right";
			else if (y == 0.0f) alignment = "top";
			else if (y == 1.0f) alignment = "bottom";
		}
		else {
			std::string valign, halign;
	
			if (y == 1.0f) valign = "bottom";
			else if (y == 0.0f) valign = "top";
		
			if (x == 1.0f) halign = "right";
			else if (x == 0.0f) halign = "left";
	
			alignment = valign + "-" + halign;
		}
	
		return alignment;
	}

	bool Widget::get_widget_prop ( std::string const &name, std::string &vals )
	{
		if ( name == "tooltip-text" ) {
			vals = tip_text;
			return true;
		}
		else if ( name == "visible" ) {
			if (GTK_WIDGET_VISIBLE(widget))
				vals = "1";
			else
				vals = "0";
			return true;
		}
		else if ( name == "width" ) {
			vals =  xgui::semantic_cast<std::string>(widget->allocation.width);
			return true;
		}
		else if ( name == "height" ) {
			vals =  xgui::semantic_cast<std::string>(widget->allocation.height);
			return true;
		}
		else if ( name == "enabled" ) {
			vals = xgui::semantic_cast<std::string>(GTK_WIDGET_IS_SENSITIVE(widget));
			return true;
		}
		else if ( name == "focused" ) {
			gboolean val;
			g_object_get( G_OBJECT(widget), "has-focus", &val, NULL);
 			vals = xgui::semantic_cast<std::string>(val);
			return true;
		}

		return false;
	}

	bool Widget::set_widget_prop ( std::string const &name, std::string const &vals )
	{
		if ( name == "tooltip-text" ) {
			tip_text = vals;
			gtk_tooltips_set_tip( GTK_TOOLTIPS(xgui::Master::Instance()->getImpl()->app_tooltips), widget, vals.c_str(), vals.c_str());
			return true;
		}	
		else if ( name == "visible" ) {
			if ( vals == "1" )
				gtk_widget_show_all(widget);
			else if ( vals == "0" )
				gtk_widget_hide(widget);
			return true;
		}
		else if ( name == "enabled" ) {
			int val = xgui::semantic_cast<int>(vals);
			gtk_widget_set_sensitive ( widget, val );
			return true;
		}
		else if ( name == "focused" ) {
			gboolean val = xgui::semantic_cast<gboolean>(vals);
			g_object_set( G_OBJECT(widget), "has-focus", &val, NULL);
			return true;
		}

		return false;
	}

	bool Widget::linkEvent(std::string const &name)
	{
		if ( name == "onfocus" ) {
			gtk_signal_connect ( GTK_OBJECT ( widget ), "focus-in-event", G_CALLBACK ( OnFocus ), this );
			return true;
		}
		else if ( name == "onblur" ) {
			gtk_signal_connect ( GTK_OBJECT ( widget ), "focus-out-event", G_CALLBACK ( OnBlur ), this );
			return true;
		}
		else if ( name == "onmouseover" ) {
			gtk_signal_connect ( GTK_OBJECT ( widget ), "enter-notify-event", G_CALLBACK ( OnMouseOver ), this );
			return true;
		}
		else if ( name == "onmouseout" ) {
			gtk_signal_connect ( GTK_OBJECT ( widget ), "leave-notify-event", G_CALLBACK ( OnMouseOut ), this );
			return true;
		}
		else if ( name == "onmousemove" ) {
			gtk_signal_connect ( GTK_OBJECT ( widget ), "motion-notify-event", G_CALLBACK ( OnMouseMove ), this );
			return true;
		}
		else if ( name == "onmousedown" ) {
			gtk_signal_connect ( GTK_OBJECT ( widget ), "button-press-event", G_CALLBACK ( OnMouseDown ), this );
			return true;
		}
		else if ( name == "onmouseup" ) {
			gtk_signal_connect ( GTK_OBJECT ( widget ), "button-release-event", G_CALLBACK ( OnMouseUp ), this );
			return true;
		}
		else if ( name == "onshow" ) {
			gtk_signal_connect ( GTK_OBJECT ( widget ), "show", G_CALLBACK ( OnShow ), this );
			return true;
		}
		else if ( name == "onhide" ) {
			gtk_signal_connect ( GTK_OBJECT ( widget ), "hide", G_CALLBACK ( OnHide ), this );
			return true;
		}
		else if ( (name == "onenable") || (name == "ondisable") ) {
			gtk_signal_connect ( GTK_OBJECT ( widget ), "state-changed", G_CALLBACK ( OnEnableDisable ), this );
			return true;
		}
		else if ( name == "onresize" )
			return true;
		else if ( name == "onmove" )
			return true;
		else if ( name == "onkeydown" )
			return true;
		else if ( name == "onkeyup" )
			return true;
		else if ( name == "onkeypress" )
			return true;
		else if ( name == "onmouseclick" )
			return true;
		else if ( name == "onmousedblclick" )
			return true;
		else if ( name == "ondestroy" )
			return true;
		
		return false;
	}

	void Widget::recalcLayout() {}

	int Widget::OnFocus ( GtkWidget * w, GdkEventFocus * event, Widget * xw )
	{
		xgui::Callback * cb = xw->this_widget->getEvent("onfocus");
		if ( cb ) return !cb->call( xw->this_widget );
	
		return 0;
	}
	
	int Widget::OnBlur ( GtkWidget * w, GdkEventFocus * event, Widget * xw )
	{
		xgui::Callback * cb = xw->this_widget->getEvent("onblur");
		if ( cb ) return !cb->call( xw->this_widget );
	
		return 0;
	}	
	
	int Widget::OnMouseOver ( GtkWidget * w, GdkEventCrossing *event, Widget * xw )
	{
		xgui::Callback * cb = xw->this_widget->getEvent("onmouseover");
		if ( cb ) return !cb->call( xw->this_widget );
	
		return 0;
	}
	
	int Widget::OnMouseOut ( GtkWidget * w, GdkEventCrossing *event, Widget * xw )
	{
		xgui::Callback * cb = xw->this_widget->getEvent("onmouseout");
		if ( cb ) return !cb->call( xw->this_widget );
	
		return 0;
	}
	
	int Widget::OnMouseDown ( GtkWidget * w, GdkEventButton * event, Widget * xw )
	{
		std::string button;
		int status = 0;
	
		switch (event->button)
		{
			case 1: button = "left"; break;
			case 2: button = "middle"; break;
			case 3: button = "right"; break;
		}
	
		if (event->state & GDK_BUTTON1_MASK) status |= xgui::mouseLeftMask;
		if (event->state & GDK_BUTTON2_MASK) status |= xgui::mouseMiddleMask;
		if (event->state & GDK_BUTTON3_MASK) status |= xgui::mouseRightMask;
	
		if (event->state & GDK_CONTROL_MASK) status |= xgui::keyControlMask;
		if (event->state & GDK_SHIFT_MASK) status |= xgui::keyShiftMask;
		if (event->state & GDK_MOD1_MASK) status |= xgui::keyAltMask;
	
	
		xgui::TextStatusCallback * cb = dynamic_cast<xgui::TextStatusCallback*>(xw->this_widget->getEvent ( "onmousedown" ));	
		if ( cb ) return !cb->call( xw->this_widget, button, status);
	
		return 0;
	}
	
	int Widget::OnMouseUp ( GtkWidget * w, GdkEventButton * event, Widget * xw )
	{
		std::string button;
		int status = 0;
	
		switch (event->button)
		{
			case 1: button = "left"; break;
			case 2: button = "middle"; break;
			case 3: button = "right"; break;
		}
	
		if (event->state & GDK_BUTTON1_MASK) status |= xgui::mouseLeftMask;
		if (event->state & GDK_BUTTON2_MASK) status |= xgui::mouseMiddleMask;
		if (event->state & GDK_BUTTON3_MASK) status |= xgui::mouseRightMask;
	
		if (event->state & GDK_CONTROL_MASK) status |= xgui::keyControlMask;
		if (event->state & GDK_SHIFT_MASK) status |= xgui::keyShiftMask;
		if (event->state & GDK_MOD1_MASK) status |= xgui::keyAltMask;
	
	
		xgui::TextStatusCallback * cb = dynamic_cast<xgui::TextStatusCallback*>(xw->this_widget->getEvent ( "onmouseup" ));	
		if ( cb ) return !cb->call( xw->this_widget, button, status);
	
		return 0;
	}
	
	int Widget::OnGdkEvent ( GtkWidget *widget, GdkEvent *event, Widget * xw )
	{
		static unsigned int last_evt_time = 0;
		static int last_event_type = 0;
	
		static Widget * last_pressed_widget = 0;
	
		static unsigned int last_key_pressed = 0;
		static bool last_key_repeating = false;
	
		switch (event->type) {
			case GDK_BUTTON_PRESS:
			case GDK_2BUTTON_PRESS:
			case GDK_3BUTTON_PRESS:
			case GDK_BUTTON_RELEASE:
			{
				xgui::TextStatusCallback * cb = 0;
				GdkEventButton * btn_event = (GdkEventButton*)event;
				std::string button;
				int status = 0;
	
				if ( (last_evt_time == btn_event->time) && (last_event_type == event->type) )
					return 0;
				else {
					last_evt_time = btn_event->time;
					last_event_type = event->type;
				}
			
				switch (btn_event->button)
				{
					case 1: button = "left"; break;
					case 2: button = "middle"; break;
					case 3: button = "right"; break;
				}
			
				if (btn_event->state & GDK_BUTTON1_MASK) status |= xgui::mouseLeftMask;
				if (btn_event->state & GDK_BUTTON2_MASK) status |= xgui::mouseMiddleMask;
				if (btn_event->state & GDK_BUTTON3_MASK) status |= xgui::mouseRightMask;
			
				if (btn_event->state & GDK_CONTROL_MASK) status |= xgui::keyControlMask;
				if (btn_event->state & GDK_SHIFT_MASK) status |= xgui::keyShiftMask;
				if (btn_event->state & GDK_MOD1_MASK) status |= xgui::keyAltMask;
	
				if (btn_event->type == GDK_2BUTTON_PRESS) {
					last_pressed_widget = 0;
					cb = dynamic_cast<xgui::TextStatusCallback *>(xw->this_widget->getEvent("onmousedblclick"));
					if (cb) return !cb->call( xw->this_widget, button, status);
				}
				else if (btn_event->type == GDK_BUTTON_PRESS) {
					last_pressed_widget = xw;
					return 0;
				}
				else if (btn_event->type == GDK_BUTTON_RELEASE) {
					if (last_pressed_widget == xw) {
						last_pressed_widget = 0;
						cb = dynamic_cast<xgui::TextStatusCallback *>(xw->this_widget->getEvent("onmouseclick"));
						if (cb) return !cb->call( xw->this_widget, button, status);
					}
					else 
						last_pressed_widget = 0;
						return 0;
				}
			}
				break;
	
			case GDK_KEY_PRESS:
			case GDK_KEY_RELEASE:
			{
				GdkEventKey * key_event = (GdkEventKey*)event;
				std::string key_name = gdk_keyval_name (key_event->keyval);
				xgui::TextCallback * cb = 0;
	
				if ( (last_evt_time == key_event->time) && (last_event_type == key_event->type) )
					return 0;
				else {
					last_evt_time = key_event->time;
					last_event_type = key_event->type;
				}
				
				if (key_event->type == GDK_KEY_PRESS) {
					if (last_key_pressed != 0) {
						last_key_repeating = true;
						cb = dynamic_cast<xgui::TextCallback *>(xw->this_widget->getEvent("onkeypress"));
					}
					else {
						last_key_pressed = key_event->keyval;
						if (cb = dynamic_cast<xgui::TextCallback *>(xw->this_widget->getEvent("onkeydown")))
							cb->call(xw->this_widget, key_name);
						cb = dynamic_cast<xgui::TextCallback *>(xw->this_widget->getEvent("onkeypress"));
					}
				}
				else if (key_event->type == GDK_KEY_RELEASE) {
					last_key_pressed = 0;
					last_key_repeating = false;
					cb = dynamic_cast<xgui::TextCallback *>(xw->this_widget->getEvent("onkeyup"));
				}
	
				if (cb) return !cb->call( xw->this_widget, key_name );
			}
				break;
	
			case GDK_CONFIGURE:
			{
				GdkEventConfigure * cfg_event = (GdkEventConfigure*)event;
				xgui::TextCallback * cb = 0;
			
				if (xw->isMoving(cfg_event->x, cfg_event->y)) {
					cb = dynamic_cast<xgui::TextCallback *>(xw->this_widget->getEvent("onmove"));
					if (cb) cb->call( xw->this_widget, xgui::semantic_cast<std::string>( std::make_pair( cfg_event->x, cfg_event->y ) ) );
				}
	
				if (xw->isResizing(cfg_event->width, cfg_event->height)) {
					cb = dynamic_cast<xgui::TextCallback *>(xw->this_widget->getEvent("onresize"));
					if (cb) cb->call( xw->this_widget, xgui::semantic_cast<std::string>( std::make_pair( cfg_event->width, cfg_event->height ) ) );
				}
			}
				break;
	
			default:
				break;
		}
	
		return 0;
	}
	
	int Widget::OnMouseMove ( GtkWidget * w, GdkEventMotion * event, Widget * xw )
	{
		xgui::TextCallback * cb = dynamic_cast<xgui::TextCallback*>(xw->this_widget->getEvent ( "onmousemove" ));
		if ( cb ) return !cb->call( xw->this_widget, xgui::semantic_cast<std::string>(std::make_pair(event->x, event->y)));
		return 0;
	}
	
	int Widget::OnShow ( GtkWidget * w, Widget * xw )
	{
		xgui::Callback * cb = dynamic_cast<xgui::Callback*>(xw->this_widget->getEvent ( "onshow" ));
		if ( cb ) return !cb->call( xw->this_widget );
		return 0;
	}
	
	int Widget::OnHide ( GtkWidget * w, Widget * xw )
	{
		xgui::Callback * cb = dynamic_cast<xgui::Callback*>(xw->this_widget->getEvent ( "onhide" ));
		if ( cb ) return !cb->call( xw->this_widget );
		return 0;
	}
	
	int Widget::OnDestroy ( GtkWidget * w, Widget * xw )
	{
		int rv = 0;
	
		xgui::Callback * cb = xw->this_widget->getEvent("ondestroy");
		if ( cb ) rv = !cb->call( xw->this_widget );
		
		delete xw->this_widget;
	
		return rv;
	}
	
	int Widget::OnEnableDisable ( GtkWidget *w, GtkStateType state, Widget * xw )
	{
		xgui::Callback * cb = 0;
	
		if ( state == GTK_STATE_INSENSITIVE )
			cb = xw->this_widget->getEvent("ondisable");
		else if ( !GTK_WIDGET_IS_SENSITIVE(w) )
			cb = xw->this_widget->getEvent("onenable" );
	
		if (cb) return !cb->call(xw->this_widget);
		return 0;
	}

}
