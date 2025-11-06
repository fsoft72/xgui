//
// C++ Implementation: checkbox_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "checkbox_impl.h"
#include "checkbox.h"
#include "callback.h"

static GtkWidget * MkCheckboxWidget(std::string const &text)
{
	return gtk_check_button_new_with_label ( text.c_str() );
}


namespace xguimpl
{
	Checkbox::Checkbox ( GtkWidget * real_w ) : Widget ( real_w ) {}
	Checkbox::Checkbox ( xgui::Container * parent, std::string const &text )
	: Widget ( MkCheckboxWidget(text) ) 
	{
	}

	Checkbox::~Checkbox() {}

	void Checkbox::setAccessor(xgui::Widget * w)
	{
		xgui::Checkbox * downcasted_widget = dynamic_cast<xgui::Checkbox*>(w);
		ASSERT(downcasted_widget);
		this_checkbox = downcasted_widget;

		Widget::setAccessor(w);
	}

	bool Checkbox::linkEvent( std::string const &name )
	{
		if ( name == "onclick" ) {
			g_signal_connect ( G_OBJECT ( widget ), "clicked", G_CALLBACK ( OnClick ), this );
			return true;
		}
	
		return Widget::linkEvent(name);
	}

	bool Checkbox::get_checkbox_prop ( std::string const &name, std::string &vals )
	{
		if ( name == "text" ) {
			vals = gtk_button_get_label(GTK_BUTTON(widget));
			return true;
		}
		else if ( name == "value" ) {
			vals = value;
			return true;
		}
		else if ( name == "checked" ) {
			
			vals = (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(widget) ) ? "1" : "0");
			return true;
		}
		else if ( name == "alignment" ) {
			std::string  halign = "center";
			float valign_v, halign_v;

			gtk_button_get_alignment ( GTK_BUTTON(widget), &halign_v, &valign_v );

			if (halign_v == 1.0f) halign = "right";
			else if (halign_v == 0.0f) halign = "left";

			vals = halign;

			return true;
		}

		return false;
	}

	bool Checkbox::set_checkbox_prop ( std::string const &name, const std::string &vals )
	{
		if ( name == "text" ) {
			gtk_button_set_label( GTK_BUTTON(widget), vals.c_str() );
			return true;
		}
		else if ( name == "value" ) {
			value = vals;
			if ( value.empty() ) value = "on";
			return true;
		}
		else if ( name == "checked" ) {
			gtk_toggle_button_set_active ( GTK_TOGGLE_BUTTON(widget), (vals == "1" ? true : false) );
			return true;
		}
		else if ( name == "alignment" ) {
			
			float  halign_v = 0.5f;
			
			std::string halign = vals;

			if (halign == "right") halign_v = 1.0f;
			else if (halign == "left") halign_v = 0.0f;

			gtk_button_set_alignment ( GTK_BUTTON(widget), halign_v, 0.5f );
			
			return true;
		}


		return false;
	}

	void Checkbox::OnClick ( GtkWidget * w, Checkbox * cbox )
	{
		xgui::Callback * base_cb = cbox->this_widget->getEvent("onclick");
		if (!base_cb) return;

		xgui::TextCallback  * cb =  dynamic_cast<xgui::TextCallback*>(base_cb);
		if (!cb) {
			DMESSAGE ( "onclick event of xgui::Checkbox expected a TextCallback" );
			return;
		}

		std::string value = (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(w) ) ? cbox->value : "");
		cb->call( cbox->this_widget, value );
	}
}


