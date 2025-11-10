//
// C++ Interface: spin_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_SPIN_IMPL_H_
#define _XGUI_SPIN_IMPL_H_

#include "widget_impl.h"

namespace xguimpl
{
	struct Spin : public Widget
	{
		int min_, max_;

		//methods
		Spin ( GtkWidget * real_w );
		Spin ( xgui::Container * parent, int min, int max );
		virtual ~Spin();

		bool linkEvent ( std::string const &name );

		int getValue();
		void setValue ( int pos );
		void setRange ( int min, int max );

		std::string getAlignment();
		void setAlignment(std::string const &align);

		static void OnValueChanged ( GtkSpinButton * w, Spin * e );
		static int OnSubmit ( GtkWidget * w, Spin * e );
		static void OnTextInsert ( GtkWidget * editable, gchar * new_text, gint new_text_length, gint * position, Spin * e );
		static void OnTextDelete ( GtkEditable *editable, gint start_pos, gint end_pos, Spin * e );
	};
}

#endif
