//
// C++ Implementation: window_impl
//
// Description:
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "window_impl.h"
#include "master_impl.h"
#include "window.h"
#include "callback.h"
#include "image.h"
#include "image_impl.h"
#include "menu.h"

/** Creates the GTK window, transient for the modal parent if there is one, and attaches it to the application. */
static GtkWidget * MkWindow(xgui::Window * modal_parent)
{
	GtkWidget * widget = gtk_window_new();

	if (modal_parent)
		gtk_window_set_transient_for ( GTK_WINDOW ( widget ), GTK_WINDOW ( modal_parent->getImpl()->widget ) );

	GtkApplication * app = xguimpl::application();
	if (app) gtk_window_set_application ( GTK_WINDOW ( widget ), app );

	return widget;
}

namespace xguimpl
{
	Window::Window(xgui::Window * parent)
	: Widget(MkWindow(parent)), this_window(0), vbox(0), status(0), modal_loop(0), modal_response(-1), pointer_x(0), pointer_y(0)
	{
		vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
		gtk_window_set_child( GTK_WINDOW(widget), vbox );

		GtkEventController * motion = gtk_event_controller_motion_new();
		g_signal_connect ( motion, "motion", G_CALLBACK ( OnPointerMotion ), this );
		gtk_widget_add_controller ( widget, motion );
	}

	Window::Window(GtkWidget * real_w)
	: Widget(real_w), this_window(0), vbox(0), status(0), modal_loop(0), modal_response(-1), pointer_x(0), pointer_y(0) { }

	Window::~Window()
	{
		if (modal_loop) g_main_loop_quit(modal_loop);
	}

	void Window::setAccessor(xgui::Widget * w)
	{
		xgui::Window * downcasted_widget = dynamic_cast<xgui::Window*>(w);
		ASSERT(downcasted_widget);
		this_window = downcasted_widget;

		Widget::setAccessor(w);
	}

	bool Window::get_window_prop ( std::string const &name, std::string &vals )
	{
		if ( name == "size" ) {
			int x = gtk_widget_get_width ( widget );
			int y = gtk_widget_get_height ( widget );
			// Before the window is shown it has no size yet: report the requested one
			if ( (x == 0) && (y == 0) ) gtk_window_get_default_size ( GTK_WINDOW ( widget ), &x, &y );
			vals = xgui::semantic_cast<std::string>(std::make_pair(x, y));
			return true;
		}
		else if ( name == "text" ) {
			const char * title = gtk_window_get_title ( GTK_WINDOW ( widget ) );
			vals = title ? title : "";
			return true;
		}
		else if ( name == "position" ) {
			// GTK 4 cannot report the position of a window
			return false;
		}
		else if ( name == "resizable" ) {
			vals = gtk_window_get_resizable( GTK_WINDOW(widget) ) ? "1" : "0";
			return true;
		}
		else if ( name == "border" ) {
			vals = gtk_window_get_decorated( GTK_WINDOW(widget) ) ? "1" : "0";
			return true;
		}
		else if ( name == "status-text" ) {
			vals = status ? gtk_label_get_text( GTK_LABEL(status) ) : "";
			return true;
		}

		return false;
	}

	bool Window::set_window_prop ( std::string const &name, std::string const &vals )
	{
		if ( name == "size" ) {
			std::pair<int, int> size = xgui::semantic_cast< std::pair<int, int> >(vals);
			gtk_widget_set_size_request( GTK_WIDGET(widget), size.first, size.second );
			gtk_window_set_default_size( GTK_WINDOW(widget), size.first, size.second );
			return true;
		}
		else if ( name == "text" ) {
			gtk_window_set_title ( GTK_WINDOW ( widget ), vals.c_str() );
			return true;
		}
		else if ( name == "position" ) {
			// GTK 4 lets the window manager place windows: "center" is what it does already
			if ( vals != "center" ) XGUI_ERROR("Window position '" << vals << "' is not supported by the GTK 4 backend");
			return false;
		}
		else if ( name == "resizable" ) {
			gtk_window_set_resizable( GTK_WINDOW(widget), ( vals == "1" ? 1 : 0 ) );
			return true;
		}
		else if ( name == "border" ) {
			gtk_window_set_decorated( GTK_WINDOW(widget), ( vals == "1" ? 1 : 0 ) );
			return true;
		}
		else if ( name == "status-text" ) {
			if (!vals.empty()) {
				if (!status) {
					status = gtk_label_new("");
					gtk_label_set_xalign( GTK_LABEL(status), 0.0f );
					gtk_widget_add_css_class( status, "statusbar" );
					gtk_box_append( GTK_BOX(vbox), status );
					reorderChildren();
				}
				gtk_label_set_text( GTK_LABEL(status), vals.c_str() );
			}
			else {
				if(status) {
					gtk_box_remove( GTK_BOX(vbox), status );
					status = 0;
				}
			}
			return true;
		}

		return false;
	}

	void Window::show()
	{
		gtk_window_present ( GTK_WINDOW ( widget ) );
	}

	int Window::modalRun()
	{
		if ( !this_window->isModal() ) return -1;
		if ( modal_loop ) return -1;

		modal_response = -1;
		modal_loop = g_main_loop_new(NULL, FALSE);
		gulong close_id = g_signal_connect ( G_OBJECT ( widget ), "close-request", G_CALLBACK ( OnModalClose ), this );

		gtk_window_set_modal ( GTK_WINDOW( widget ), TRUE );
		gtk_window_present ( GTK_WINDOW( widget ) );
		runNestedLoop ( modal_loop );

		g_main_loop_unref ( modal_loop );
		modal_loop = 0;
		g_signal_handler_disconnect ( widget, close_id );
		gtk_window_set_modal ( GTK_WINDOW( widget ), FALSE );

		return modal_response;
	}

	void Window::modalReturn(int response)
	{
		if ( !this_window->isModal() || !modal_loop ) return;
		modal_response = response;
		g_main_loop_quit ( modal_loop );
	}

	void Window::reorderChildren()
	{
		GtkWidget * order[3] = {
			this_window->menu_ ? this_window->menu_->getImpl()->widget : NULL,
			this_window->child_ ? this_window->child_->getImpl()->widget : NULL,
			status
		};

		GtkWidget * previous = NULL;
		for (int i = 0; i < 3; ++i) {
			if ( !order[i] ) continue;
			gtk_box_reorder_child_after( GTK_BOX(vbox), order[i], previous );
			previous = order[i];
		}
	}

	bool Window::addChild(xgui::Widget * w)
	{
		bool done = false;
		GtkWidget * child = w->getImpl()->widget;

		if ((GTK_IS_POPOVER_MENU_BAR(child)) && (!this_window->menu_)) {
			this_window->menu_ = reinterpret_cast<xgui::Menu*>(w);
			gtk_box_append( GTK_BOX(vbox), child );
			done = true;
		}
		else if (!this_window->child_) {
			this_window->child_ = w;
			gtk_widget_set_hexpand( child, TRUE );
			gtk_widget_set_vexpand( child, TRUE );
			gtk_box_append( GTK_BOX(vbox), child );
			done = true;
		}

		reorderChildren();
		return done;
	}

	void Window::delChild(xgui::Widget * w)
	{
		if (this_window->child_ == w) this_window->child_ = 0;
 		else if (this_window->menu_ == w) this_window->menu_ = 0;
	}

	void Window::maximize()
	{
		gtk_window_maximize( GTK_WINDOW(widget) );
	}

	void Window::minimize()
	{
		gtk_window_minimize( GTK_WINDOW(widget) );
	}

	void Window::restore()
	{
		gtk_window_unmaximize( GTK_WINDOW(widget) );
		gtk_window_unminimize( GTK_WINDOW(widget) );
	}

	bool Window::linkEvent(std::string const &name)
	{
		if ( name == "onclose" ) {
			g_signal_connect ( G_OBJECT ( widget ), "close-request", G_CALLBACK ( OnClose ), this );
			return true;
		}

		return Widget::linkEvent(name);
	}

	gboolean Window::OnClose ( GtkWindow * w, Window * xw )
	{
		xgui::Callback * cb =  xw->this_widget->getEvent( "onclose" );
		if ( cb ) return !cb->call( xw->this_widget );
		return FALSE;
	}

	gboolean Window::OnModalClose ( GtkWindow * w, Window * xw )
	{
		// Like gtk_dialog_run(): closing a modal window ends modalRun() with -1 but keeps the window
		xw->modal_response = -1;
		if ( xw->modal_loop ) g_main_loop_quit ( xw->modal_loop );
		return TRUE;
	}

	void Window::OnPointerMotion ( GtkEventControllerMotion * ctrl, double x, double y, Window * xw )
	{
		xw->pointer_x = x;
		xw->pointer_y = y;
	}

	void Window::setIcon(xgui::Image * img)
	{
		// GTK 4 windows only take a themed icon name, an image file cannot be used as window icon
		DMESSAGE("Window icons from images are not supported by the GTK 4 backend");
	}
}
