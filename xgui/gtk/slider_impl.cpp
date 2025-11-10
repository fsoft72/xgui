//
// C++ Implementation: slider_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "slider_impl.h"
#include "callback.h"

static GtkWidget * MkSliderWidget( int min, int max, bool vertical )
{
	GtkWidget * widget;

	if ( vertical ) widget = gtk_vscale_new_with_range ( min, max, 1);
	else widget = gtk_hscale_new_with_range (min, max, 1);

	return widget;
}

namespace xguimpl
{
	Slider::Slider ( GtkWidget * real_w ) : Widget ( real_w ), min_(0), max_(0) {}

	Slider::Slider ( xgui::Container * parent, int min, int max, bool vertical )
	: Widget ( MkSliderWidget(min, max, vertical ) ), min_(min), max_(max)
	{ }

	Slider::~Slider() {}

	bool Slider::linkEvent( std::string const &name )
	{
		if ( name == "oninput" ) {
			g_signal_connect ( G_OBJECT ( widget ), "value-changed", G_CALLBACK ( OnChange ), this );
			return true;
		}

		return Widget::linkEvent(name);
	}

	bool Slider::get_slider_prop ( std::string const &name, std::string &vals )
	{
		if ( name == "value" ) {
			vals = xgui::semantic_cast<std::string> (getPos());
			return true;
		}
		else if ( name == "min" ) {
			vals = xgui::semantic_cast<std::string> (min_);
			return true;
		}
		else if ( name == "max" ) {
			vals = xgui::semantic_cast<std::string> (max_);
			return true;
		}

		return false;
	}

	bool Slider::set_slider_prop ( std::string const &name, const std::string &vals )
	{
		if ( name == "value" ) {
			setPos ( xgui::semantic_cast<int> ( vals ) );
			return true;
		}
		else if ( name == "min" ) {
			min_ = xgui::semantic_cast<int> ( vals );
			gtk_range_set_range ( GTK_RANGE (widget), min_, max_ ); 
			return true;
		}
		else if ( name == "max" ) {
			max_ = xgui::semantic_cast<int> ( vals );
			gtk_range_set_range ( GTK_RANGE (widget), min_, max_ ); 
			return true;
		}

		return false;
	}

	void Slider::OnChange ( GtkWidget * w, Slider * pbar )
	{
		xgui::Callback * base_cb = pbar->this_widget->getEvent("oninput");
		if (!base_cb) return;

		xgui::IntCallback * cb = dynamic_cast<xgui::IntCallback *>(base_cb);
		if (!cb) {
			DMESSAGE("oninput event of xgui::Slider expected an IntCallback");
			return;
		}

		cb->call( pbar->this_widget, pbar->getPos() );
	}

	void Slider::setPos (int pos)
	{
		gtk_range_set_value ( GTK_RANGE (widget), pos );
	
		//(int_event_onchangepos(widget, this); 
	}
	
	int Slider::getPos()
	{
		return (int) gtk_range_get_value ( GTK_RANGE (widget) );
	}
}
