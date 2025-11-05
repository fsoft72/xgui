//
// C++ Implementation: entry_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "entry.h"
#include "entry_impl.h"
#include "callback.h"

static GtkWidget * MkEntryWidget(std::string const &text, int maxlen, bool editable, bool password_mode)
{
	GtkWidget * widget;

	if ( maxlen == 0 )
		widget = gtk_entry_new ();
	else
		widget = gtk_entry_new_with_max_length ( maxlen );

	gtk_entry_set_text ( GTK_ENTRY ( widget ), text.c_str() );
	gtk_entry_set_editable ( GTK_ENTRY ( widget ), editable );
	gtk_entry_set_visibility ( GTK_ENTRY ( widget ), ! password_mode );

	return widget;
}

namespace xguimpl
{
	Entry::Entry ( GtkWidget * real_w ) : Widget ( real_w ), this_entry(0) {}

	Entry::Entry ( xgui::Container * parent, std::string const &text, int maxlen, bool editable, bool password_mode ) 
	: Widget ( MkEntryWidget ( text, maxlen, editable, password_mode ) ), this_entry(0)
	{}

	Entry::~Entry() { }

	void Entry::setAccessor(xgui::Widget * w)
	{
		xgui::Entry * downcasted_widget = dynamic_cast<xgui::Entry*>(w);
		ASSERT(downcasted_widget);
		this_entry = downcasted_widget;

		Widget::setAccessor(w);
	}

	bool Entry::linkEvent( std::string const &name )
	{
		if ( name == "onsubmit" ) {
			g_signal_connect ( G_OBJECT ( widget ), "activate", G_CALLBACK ( OnSubmit ), this );
			return true;
		}
		else if (name == "onchange") {
			g_signal_connect ( G_OBJECT ( widget ), "insert-text", G_CALLBACK ( OnTextInsert ), this );
			g_signal_connect ( G_OBJECT ( widget ), "delete-text", G_CALLBACK ( OnTextDelete ), this );
			return true;
		}
		/*else if (name == "oncursormove") {  * DISABLED cause not available on win32 *
			g_signal_connect ( G_OBJECT ( widget ), "move-cursor", G_CALLBACK ( int_event_cursormove ), this );
			return true;
		}*/
	
		return Widget::linkEvent(name);
	}


	bool Entry::set_entry_prop ( std::string const &name, std::string const &vals )
	{
		if ( name == "text" ) {
			gtk_entry_set_text ( GTK_ENTRY ( widget ), vals.c_str() );
			return true;
		}
		else if ( name == "editable" ) {
			gtk_entry_set_editable( GTK_ENTRY(widget), (vals == "1" ? true : false) );
			return true;
		}
		else if ( name == "password-mode" ) {
			gtk_entry_set_visibility ( GTK_ENTRY(widget), (vals == "1" ? false : true) );
			return true;
		}
		else if ( name == "alignment" ) {
			float align = 0.5f;
			if (vals == "left") align = 0.0f;
			else if (vals == "right") align = 1.0f;
	
			gtk_entry_set_alignment( GTK_ENTRY(widget), align );
			return true;
		}
		else if ( name == "maxlength" ) {
			unsigned int maxlen = xgui::semantic_cast<unsigned int>(vals);
			gtk_entry_set_max_length( GTK_ENTRY(widget), maxlen );
			return true;
		}

		return false;
	}


	bool Entry::get_entry_prop ( std::string const &name, std::string &vals )
	{
		if ( name == "text" ) {
			vals = gtk_entry_get_text(GTK_ENTRY(widget));
			return true;
		}
		else if ( name == "editable" ) {
			gboolean edit;
			g_object_get( G_OBJECT(widget), "editable", (GValue*)&edit, NULL );
			vals = xgui::semantic_cast<std::string>(edit);
			return true;
		}
		else if ( name == "password-mode" ) {
			vals = ( gtk_entry_get_visibility( GTK_ENTRY(widget) ) ? "0" : "1" );
			return true;
		}
		else if ( name == "alignment" ) {
			float align = gtk_entry_get_alignment( GTK_ENTRY(widget) );
	
			if (align == 1.0f) vals = "right";
			else if (align == 0.0f) vals = "left";
			else vals = "center";
	
			return true;
		}
		else if ( name == "maxlength" ) {
			vals = xgui::semantic_cast<std::string>(gtk_entry_get_max_length( GTK_ENTRY(widget) ) );
			return true;
		}

		return false;
	}

	void Entry::insertText(std::string const &text, int pos)
	{
		std::string str = gtk_entry_get_text( GTK_ENTRY(widget) );
		if ( (pos > 0) && (((unsigned int)pos) < str.size()) )
			str.insert(pos, text);
		gtk_entry_set_text( GTK_ENTRY(widget), str.c_str() );
	}
	
	void Entry::prependText(std::string const &txt)
	{
		gtk_entry_prepend_text ( GTK_ENTRY(widget), txt.c_str() );
	}
	
	
	void Entry::appendText(std::string const &txt)
	{
		gtk_entry_append_text ( GTK_ENTRY(widget), txt.c_str() );
	}

	int Entry::OnSubmit ( GtkWidget * w, Entry * e )
	{
		xgui::Callback * base_cb = e->this_entry->getEvent("onsubmit");
		if (!base_cb) return 0;
		
		xgui::TextCallback * cb = dynamic_cast<xgui::TextCallback *>(base_cb);
		if (!cb) {
			DMESSAGE("onsubmit event of xgui::Entry expected a TextCallback");
			return 0;
		}
	
		std::string buf = gtk_entry_get_text ( GTK_ENTRY ( e->widget ) );
		return cb->call( e->this_entry, buf);
	}

	void Entry::OnTextInsert ( GtkWidget * editable, gchar * new_text, gint new_text_length, gint * position, Entry * e )
	{
		if (!new_text_length) return;
	
		g_signal_handlers_block_by_func ( G_OBJECT ( editable ), (void*)OnTextInsert, e );
		bool text_approved = true;
	
		std::string text_after_change = gtk_entry_get_text( GTK_ENTRY(editable) );
		text_after_change.insert(*position, text_after_change);
	
		xgui::Callback * base_cb = e->this_entry->getEvent("onchange");
		if (base_cb) {
			xgui::TextCallback * cb = dynamic_cast<xgui::TextCallback *>(base_cb);
			if (!cb)
				DMESSAGE("onchange event of xgui::Entry expected a TextCallback");
			else
				text_approved = cb->call ( e->this_entry, text_after_change );
		}
	
		if ( text_approved ) 
			gtk_editable_insert_text ( GTK_EDITABLE ( editable ) , new_text, new_text_length, position);
	
	
		g_signal_handlers_unblock_by_func ( G_OBJECT ( editable ), (void*)OnTextInsert, e );
		g_signal_emit_stop_by_name ( G_OBJECT ( editable ), "insert-text" );
	}

	void Entry::OnTextDelete ( GtkEditable *editable, gint start_pos, gint end_pos, Entry * e )
	{
		g_signal_handlers_block_by_func ( G_OBJECT ( editable ), (void*)OnTextDelete, e );
		bool deletion_approved = true;
	
		std::string text_after_change = gtk_entry_get_text( GTK_ENTRY(editable) );
		text_after_change.erase(start_pos, end_pos - start_pos);
	
		xgui::TextCallback * cb = dynamic_cast<xgui::TextCallback *>(e->this_entry->getEvent("onchange"));
		if ( cb ) 
			deletion_approved = cb->call ( e->this_entry, text_after_change );
		if ( deletion_approved ) 
			gtk_editable_delete_text( GTK_EDITABLE(editable), start_pos, end_pos );
		
		g_signal_handlers_unblock_by_func ( G_OBJECT ( editable ), (void*)OnTextDelete, e );
		g_signal_emit_stop_by_name ( G_OBJECT ( editable ), "delete-text" );
	}
}
