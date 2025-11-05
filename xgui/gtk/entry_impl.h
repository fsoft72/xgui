//
// C++ Interface: entry_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_ENTRY_IMPL_H_
#define _XGUI_ENTRY_IMPL_H_

#include "widget_impl.h"

namespace xgui { class Entry; }

namespace xguimpl
{
	struct Entry : public Widget
	{
		xgui::Entry * this_entry;

		//methods
		Entry ( GtkWidget * real_w );
		Entry ( xgui::Container * parent, std::string const &text, int maxlen, bool editable, bool password_mode );
		virtual ~Entry();

		virtual void setAccessor(xgui::Widget * w);
		bool linkEvent( std::string const &name );

		bool get_entry_prop ( std::string const &name, std::string &vals );
		bool set_entry_prop ( std::string const &name, const std::string &vals );

		void appendText ( const std::string & text );
		void prependText ( const std::string & text );
		void insertText ( const std::string & text, int pos );

		static int OnSubmit ( GtkWidget * w, Entry * e );
		static void OnTextInsert ( GtkWidget * editable, gchar * new_text, gint new_text_length, gint * position, Entry * e );
		static void OnTextDelete ( GtkEditable *editable, gint start_pos, gint end_pos, Entry * e );
	};
}

#endif
