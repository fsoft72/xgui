//
// C++ Implementation: spin_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "spin_impl.h"
#include "callback.h"

namespace xguimpl
{
	Spin::Spin ( GtkWidget * real_w ) : Widget ( real_w ), min_(0), max_(0) {}

	Spin::Spin ( xgui::Container * parent, int min, int max )
	: Widget ( gtk_spin_button_new_with_range ( (double) min, (double) max, 1 ) ), min_(min), max_(max)
	{ }

	Spin::~Spin() {}

	bool Spin::linkEvent( std::string const &name )
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

		return Widget::linkEvent(name);
	}

	int Spin::getValue()
	{
		return (int)gtk_spin_button_get_value ( GTK_SPIN_BUTTON (widget) );
	}

	void Spin::setValue ( int pos )
	{
		gtk_spin_button_set_value ( GTK_SPIN_BUTTON (widget), pos );
	}

	void Spin::setRange ( int min, int max )
	{
		gtk_spin_button_set_range ( GTK_SPIN_BUTTON (widget), min, max );
	}

	std::string Spin::getAlignment()
	{
		std::string vals;
		float align;
		// GTK3: Use xalign property instead of deprecated gtk_entry_get_alignment
		g_object_get(G_OBJECT(widget), "xalign", &align, NULL);

		if (align == 1.0f) vals = "right";
		else if (align == 0.0f) vals = "left";
		else vals = "center";

		return vals;
	}

	void Spin::setAlignment(std::string const &vals)
	{
		float align = 0.5f;
		if (vals == "left") align = 0.0f;
		else if (vals == "right") align = 1.0f;

		// GTK3: Use xalign property instead of deprecated gtk_entry_set_alignment
		g_object_set(G_OBJECT(widget), "xalign", align, NULL);
	}

	int Spin::OnSubmit ( GtkWidget * w, Spin * e )
	{
		xgui::Callback * base_cb = e->this_widget->getEvent("onsubmit");
		if (!base_cb) return 0;
		
		xgui::TextCallback * cb = dynamic_cast<xgui::TextCallback *>(base_cb);
		if (!cb) {
			DMESSAGE("onsubmit event of xgui::Spin expected a TextCallback");
			return 0;
		}
	
		std::string buf = gtk_entry_get_text ( GTK_ENTRY ( e->widget ) );
		return cb->call( e->this_widget, buf);
	}

	void Spin::OnTextInsert ( GtkWidget * editable, gchar * new_text, gint new_text_length, gint * position, Spin * e )
	{
		if (!new_text_length) return;
	
		g_signal_handlers_block_by_func ( G_OBJECT ( editable ), (void*)OnTextInsert, e );
		bool text_approved = true;
	
		std::string text_after_change = gtk_entry_get_text( GTK_ENTRY(editable) );
		text_after_change.insert(*position, text_after_change);
	
		xgui::Callback * base_cb = e->this_widget->getEvent("onchange");
		if (base_cb) {
			xgui::TextCallback * cb = dynamic_cast<xgui::TextCallback *>(base_cb);
			if (!cb)
				DMESSAGE("onchange event of xgui::Spin expected a TextCallback");
			else
				text_approved = cb->call ( e->this_widget, text_after_change );
		}
	
		if ( text_approved ) 
			gtk_editable_insert_text ( GTK_EDITABLE ( editable ) , new_text, new_text_length, position);
	
	
		g_signal_handlers_unblock_by_func ( G_OBJECT ( editable ), (void*)OnTextInsert, e );
		g_signal_stop_emission_by_name ( G_OBJECT ( editable ), "insert-text" );
	}

	void Spin::OnTextDelete ( GtkEditable *editable, gint start_pos, gint end_pos, Spin * e )
	{
		g_signal_handlers_block_by_func ( G_OBJECT ( editable ), (void*)OnTextDelete, e );
		bool deletion_approved = true;
	
		std::string text_after_change = gtk_entry_get_text( GTK_ENTRY(editable) );
		text_after_change.erase(start_pos, end_pos - start_pos);
	
		xgui::TextCallback * cb = dynamic_cast<xgui::TextCallback *>(e->this_widget->getEvent("onchange"));
		if ( cb ) 
			deletion_approved = cb->call ( e->this_widget, text_after_change );
		if ( deletion_approved ) 
			gtk_editable_delete_text( GTK_EDITABLE(editable), start_pos, end_pos );
		
		g_signal_handlers_unblock_by_func ( G_OBJECT ( editable ), (void*)OnTextDelete, e );
		g_signal_stop_emission_by_name ( G_OBJECT ( editable ), "delete-text" );
	}
}
