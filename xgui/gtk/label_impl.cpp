//
// C++ Implementation: label_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "label_impl.h"
#include "label.h"

static GtkWidget * MkLabelWidget ( std::string const &text )
{
	GtkWidget * widget = gtk_label_new ( "-" );
	gtk_label_set_line_wrap ( GTK_LABEL ( widget ), 1 );
	gtk_label_set_markup ( GTK_LABEL ( widget ), text.c_str() );

	return widget;
}

namespace xguimpl
{
	Label::Label ( GtkWidget * real_w ) : Widget ( real_w ) { }
	Label::Label ( xgui::Container * parent, std::string const &text ) : Widget ( MkLabelWidget ( text ) ) { }
	Label::~Label() { }

	bool Label::set_label_prop ( std::string const &name, std::string const &vals )
	{
		if ( name == "text" ) {
			gtk_label_set_markup ( GTK_LABEL ( widget ), vals.c_str() );
			return true;
		}
		else if ( name == "alignment" ) {
			std::pair<float, float> align = splitAlignment(vals);
			gtk_misc_set_alignment ( GTK_MISC(widget), align.first, align.second );
			return true;
		}
		return false;
	}


	bool Label::get_label_prop ( std::string const &name, std::string &vals )
	{
		if ( name == "text" ) {
			vals = gtk_label_get_text( GTK_LABEL( widget ) );
			return true;
		}
		else if ( name == "alignment" ) {
			float x, y;
			gtk_misc_get_alignment ( GTK_MISC(widget), &x, &y);
			vals = joinAlignment(x, y);
			return true;
		}

		return false;
	}
}
