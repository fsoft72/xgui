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
#include "window.h"
#include "callback.h"
#include "image.h"
#include "image_impl.h"
#include "menu.h"

static GtkWidget * MkWindow(xgui::Window * modal_parent)
{
	GtkWidget * widget;

	if (modal_parent) {
		widget = gtk_dialog_new();
		gtk_window_set_transient_for ( GTK_WINDOW ( widget ), GTK_WINDOW ( modal_parent->getImpl()->widget ) );
		// gtk_dialog_set_has_separator removed in GTK 3
	}
	else
		widget = gtk_window_new ( GTK_WINDOW_TOPLEVEL );
	gtk_widget_realize ( widget );

	return widget;
}

namespace xguimpl
{
	Window::Window(xgui::Window * parent)
	: Widget(MkWindow(parent)), this_window(0), vbox(0), status(0)
	{
		if(!parent) {
			vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
			gtk_container_add( GTK_CONTAINER(widget), vbox );
		}
		else
			vbox = gtk_dialog_get_content_area(GTK_DIALOG(widget));
	}

	Window::Window(GtkWidget * real_w) : Widget(real_w), this_window(0), vbox(0), status(0)	{ }

	Window::~Window() { }

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
			int x, y;
			gtk_window_get_size ( GTK_WINDOW ( widget ), &x, &y );
			vals = xgui::semantic_cast<std::string>(std::make_pair(x, y));
			return true;
		}
		else if ( name == "text" ) {
			gchararray title;
			g_object_get(widget, "title", &title, NULL);
			vals = title ? title : "";
			g_free(title);
			return true;
		}
		else if ( name == "position" ) {
			int x,y;
			gtk_window_get_position ( GTK_WINDOW ( widget ), &x, &y );
			vals = xgui::semantic_cast<std::string>(std::make_pair(x, y));
			return true;
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
			vals = "";
			return true;
		}

		return false;
	}

	bool Window::set_window_prop ( std::string const &name, std::string const &vals )
	{
		if ( name == "size" ) {
			std::pair<int, int> size = xgui::semantic_cast< std::pair<int, int> >(vals);
			gtk_widget_set_size_request( GTK_WIDGET(widget), size.first, size.second );
			gtk_window_resize( GTK_WINDOW(widget), size.first, size.second );
			return true;
		}
		else if ( name == "text" ) {
			gtk_window_set_title ( GTK_WINDOW ( widget ), vals.c_str() );
			return true;
		}
		else if ( name == "position" ) {
			if ( vals == "center") 
				gtk_window_set_position( GTK_WINDOW(widget), GTK_WIN_POS_CENTER );
			else {
				std::pair<int, int> pos = xgui::semantic_cast< std::pair<int, int> >(vals);
				gtk_window_move ( GTK_WINDOW ( widget ), pos.first, pos.second );
			}
			return true;
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
					status = gtk_statusbar_new();
					gtk_box_pack_start( GTK_BOX(vbox), status, 0, 1, 0 );
					reorderChildren();
					gtk_widget_show(status);
				}
				gtk_statusbar_push(GTK_STATUSBAR(status), 
						gtk_statusbar_get_context_id(GTK_STATUSBAR(status), "XGUIStatusMessage"), vals.c_str());
			}
			else {
				if(status) {
					gtk_widget_destroy(status);
					status = 0;
				}
			}
			return true;
		}

		return false;
	}

	void Window::show()
	{
		xgui::Menu * mnu = this_window->getMenu();
		bool is_menu_visible = true;
		if(mnu) {
			std::string mnu_vis;
			mnu->get("visible", mnu_vis);
			is_menu_visible = ((mnu_vis == "1") ? true : false);
		}

		gtk_widget_show_all ( widget );
		if (!is_menu_visible)
			mnu->set("visible", "0");
	}

	int Window::modalRun() 
	{
		int dret = -1;

		if (this_window->isModal()) {
			gtk_window_set_modal ( GTK_WINDOW( widget ), TRUE );
			dret = gtk_dialog_run ( GTK_DIALOG ( widget ) );
			gtk_window_set_modal ( GTK_WINDOW( widget ), FALSE );
			if (dret < 0) return -1;
		}
	
		return ( dret );
	}

	void Window::modalReturn(int response) 
	{
		if (this_window->isModal())
			gtk_dialog_response ( GTK_DIALOG ( widget ), response );
	}

	void Window::reorderChildren()
	{
		if (this_window->menu_ && status && this_window->child_) {
			gtk_box_reorder_child(GTK_BOX(vbox), this_window->menu_->getImpl()->widget, 0);
			gtk_box_reorder_child(GTK_BOX(vbox), this_window->child_->getImpl()->widget, 1);
			gtk_box_reorder_child(GTK_BOX(vbox), status, 2);
		}
		else if (this_window->menu_ && this_window->child_) {
			gtk_box_reorder_child(GTK_BOX(vbox), this_window->menu_->getImpl()->widget, 0);
			gtk_box_reorder_child(GTK_BOX(vbox), this_window->child_->getImpl()->widget, 1);
		}
		else if (status && this_window->child_) {
			gtk_box_reorder_child(GTK_BOX(vbox), this_window->child_->getImpl()->widget, 0);
			gtk_box_reorder_child(GTK_BOX(vbox), status, 1);
		}
	}

	bool Window::addChild(xgui::Widget * w) 
	{
		bool done = false;

		if ((GTK_IS_MENU_BAR(w->getImpl()->widget)) && (!this_window->menu_)) {
			this_window->menu_ = reinterpret_cast<xgui::Menu*>(w);
			gtk_box_pack_start( GTK_BOX(vbox), w->getImpl()->widget, 0, 1, 0);
			done = true;
		}
		else if (!this_window->child_) {
			this_window->child_ = w;
			gtk_box_pack_start(GTK_BOX(vbox), w->getImpl()->widget, 1, 1, 0);
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
		gtk_window_iconify( GTK_WINDOW(widget) );
	}
	
	void Window::restore()
	{
		gtk_window_unmaximize( GTK_WINDOW(widget) );
		gtk_window_deiconify( GTK_WINDOW(widget) );
	}

	bool Window::linkEvent(std::string const &name)
	{
		if ( name == "onclose" ) {
			g_signal_connect ( G_OBJECT ( widget ), "delete_event", G_CALLBACK ( OnClose ), this );
			return true;
		}
	
		return Widget::linkEvent(name);
	}

	int Window::OnClose ( GtkWidget * w, GdkEvent * e, Window * xw )
	{
		xgui::Callback * cb =  xw->this_widget->getEvent( "onclose" );
		if ( cb ) return !cb->call( xw->this_widget );
		return 0;
	}

	void Window::setIcon(xgui::Image * img)
	{
		if (img)
			gtk_window_set_icon( GTK_WINDOW(widget), img->getImpl()->getPixbuf() );
		else
			gtk_window_set_icon( GTK_WINDOW(widget), NULL );
	}
}
