//
// C++ Implementation: button_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "button_impl.h"
#include "button.h"
#include "callback.h"
#include "image.h"
#include "image_impl.h"

static GtkWidget * MkButtonWidget(std::string const &text, xgui::Image * img, bool toggle)
{
	GtkWidget * widget;

	if ( (!img) && (!toggle) )
		widget = gtk_button_new_with_label ( text.c_str() );
	else if ( (img) && (!toggle) ) {
		widget = gtk_button_new ();
		gtk_container_add ( GTK_CONTAINER ( widget ), GTK_WIDGET ( img->getImpl()->getImage() ) );
	}
	else if ( (!img) && (toggle) )
		widget = gtk_toggle_button_new_with_label ( text.c_str() );
	else if ( (img) && (toggle) ) {
		widget = gtk_toggle_button_new();
		gtk_container_add ( GTK_CONTAINER ( widget ), GTK_WIDGET ( img->getImpl()->getImage() ) );
	}

	return widget;
}


namespace xguimpl
{
	Button::Button ( GtkWidget * real_w ) : Widget ( real_w ) {}
	Button::Button ( xgui::Container * parent, std::string const &text, xgui::Image * image, bool toggle_button )
	: Widget ( MkButtonWidget(text, image, toggle_button) ) 
	{
		if (image) {
			image->ref();
			set_widget_prop("tooltip-text", text);
		}
	}

	Button::~Button() {}

	void Button::setAccessor(xgui::Widget * w)
	{
		xgui::Button * downcasted_widget = dynamic_cast<xgui::Button*>(w);
		ASSERT(downcasted_widget);
		this_button = downcasted_widget;

		Widget::setAccessor(w);
	}

	bool Button::linkEvent( std::string const &name )
	{
		if ( name == "onclick" ) {
			gtk_signal_connect ( GTK_OBJECT ( widget ), "clicked", G_CALLBACK ( OnClick ), this );
			return true;
		}
	
		return Widget::linkEvent(name);
	}

	bool Button::get_button_prop ( std::string const &name, std::string &vals )
	{
		if ( name == "text" ) {
			vals = gtk_button_get_label(GTK_BUTTON(widget));
			return true;
		}
		else if ( name == "pressed" ) {
			if (!GTK_IS_TOGGLE_BUTTON(widget))
				vals = "0";
			else
				vals = (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(widget) ) ? "1" : "0");
			return true;
		}
		else if ( name == "alignment" ) {
			float x, y;
			gtk_misc_get_alignment ( GTK_MISC( gtk_bin_get_child( GTK_BIN(widget) ) ), &x, &y);
			vals = joinAlignment(x, y);
			return true;
		}

		return false;
	}

	bool Button::set_button_prop ( std::string const &name, const std::string &vals )
	{
		if ( name == "text" ) {
			if ( this_button->icon_ == 0 )
				gtk_button_set_label( GTK_BUTTON(widget), vals.c_str() );
			else
				set_widget_prop("tooltip-text", vals);
			return true;
		}
		else if ( name == "pressed" ) {
			if (GTK_IS_TOGGLE_BUTTON(widget)) 
				gtk_toggle_button_set_active ( GTK_TOGGLE_BUTTON(widget), (vals == "1" ? true : false) );
			return true;
		}
		else if ( name == "alignment" ) {
			std::pair<float, float> align = splitAlignment(vals);
			gtk_misc_set_alignment ( GTK_MISC( gtk_bin_get_child( GTK_BIN(widget) ) ), align.first, align.second );
			return true;
		}

		return false;
	}

	void Button::setIcon ( xgui::Image * img )
	{
		if (img) {
			gtk_container_remove(GTK_CONTAINER(widget), gtk_bin_get_child(GTK_BIN(widget)));
			gtk_container_add ( GTK_CONTAINER ( widget ), GTK_WIDGET ( img->getImpl()->getImage() ) );
			gtk_widget_show(gtk_bin_get_child(GTK_BIN(widget)));
		}
	}

	int Button::OnClick ( GtkWidget * w, Button * btn )
	{
		xgui::Callback  * cb =  btn->this_widget->getEvent("onclick");
		if ( cb ) 
			cb->call( btn->this_widget );
		return 0;
	}
}


