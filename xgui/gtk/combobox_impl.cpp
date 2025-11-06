//
// C++ Implementation: combobox_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "combobox_impl.h"
#include "combobox.h"
#include "callback.h"
#include "model.h"


namespace xguimpl
{
	Combobox::Combobox ( GtkWidget * real_w ) 
	: View ( gtk_event_box_new() ), real_widget(real_w), entry(0), this_cbox(0) { }

	Combobox::Combobox ( xgui::Container * parent, bool editable )
	: View ( gtk_event_box_new() ), real_widget(0), entry(0), this_cbox(0)
	{
		real_widget = ((!editable) ? gtk_combo_box_text_new() : gtk_combo_box_text_new_with_entry());
		gtk_container_add( GTK_CONTAINER(widget), real_widget );
		gtk_event_box_set_visible_window ( GTK_EVENT_BOX(widget), 0 );

		g_signal_connect ( G_OBJECT(real_widget), "event", G_CALLBACK(Widget::OnGdkEvent), this );

		if (editable)
			entry = GTK_ENTRY(gtk_bin_get_child(GTK_BIN(real_widget)));
	}

	Combobox::~Combobox() { }

	void Combobox::setAccessor(xgui::Widget * w)
	{
		xgui::Combobox * downcasted_widget = dynamic_cast<xgui::Combobox*>(w);
		ASSERT(downcasted_widget);
		this_cbox = downcasted_widget;

		Widget::setAccessor(w);
	}
	

	bool Combobox::linkEvent( std::string const &name )
	{	
		if ( name == "onselect" )
			return true;
		else if ( name == "onrefresh" )
			return true;
		else if ( name == "onsubmit" ) {
			g_signal_connect ( G_OBJECT ( real_widget ), "activate", G_CALLBACK ( OnSubmit ), this );
			return true;
		}
		else if ( (name == "onchange") && (entry) ) {
			g_signal_connect(G_OBJECT(entry), "insert-text", G_CALLBACK ( OnTextInsert ), this);
			g_signal_connect(G_OBJECT(entry), "delete-text", G_CALLBACK ( OnTextDelete ), this);
			return true;
		}
		
		return Widget::linkEvent(name);
	}

	void Combobox::refresh()
	{
		xgui::Model * model = this_cbox->getModel();
		if (!model) return;
	
		g_signal_handlers_disconnect_by_func( real_widget, (void*)OnSelect, this);
		gtk_list_store_clear( GTK_LIST_STORE(gtk_combo_box_get_model( GTK_COMBO_BOX(real_widget) )) );

		if ( gtk_widget_get_realized ( real_widget ) ) {
			gdk_window_freeze_updates ( gtk_widget_get_parent_window ( real_widget ) );
		}

		/*int selected = -1;*/
		int sel_pos = 0;
		for ( xgui::Model::iterator iter = model->begin(); iter != model->end(); ++iter )
		{
			xgui::Model * child = dynamic_cast<xgui::Model*>(*iter);

			gtk_combo_box_text_append_text ( GTK_COMBO_BOX_TEXT(real_widget), child->getString(0).c_str() );
			/*if ( this_cbox->isSelected(child) )
				selected = sel_pos;*/
			++sel_pos;
		}

		/*if (selected != -1)
			gtk_combo_box_set_active ( GTK_COMBO_BOX(real_widget), selected );*/

		if ( gtk_widget_get_realized ( real_widget ) ) {
			gdk_window_thaw_updates ( gtk_widget_get_parent_window ( real_widget ) );
		}
	
		xgui::Callback * refresh_cb = this_cbox->getEvent("onrefresh");
		if (refresh_cb) refresh_cb->call( this_cbox );
	
		g_signal_connect ( G_OBJECT ( real_widget ), "changed", G_CALLBACK ( OnSelect ), this );
	}
	
	void Combobox::insertItem ( xgui::Model * parent, int child_pos, xgui::Model * child )
	{
		xgui::Model * model = this_cbox->getModel();
		if(!model) return;

		g_signal_handlers_disconnect_by_func( real_widget, (void*)OnSelect, this);

		gtk_combo_box_text_insert_text( GTK_COMBO_BOX_TEXT(real_widget), child_pos, child->getString(0).c_str() );

		xgui::Callback * refresh_cb = this_cbox->getEvent("onrefresh");
		if (refresh_cb) refresh_cb->call( this_cbox );

		g_signal_connect ( G_OBJECT ( real_widget ), "changed", G_CALLBACK ( OnSelect ), this );
	}
	
	void Combobox::removeItem ( xgui::Model * parent, int child_pos, xgui::Model * child )
	{
		xgui::Model * model = this_cbox->getModel();
		if(!model) return;

		g_signal_handlers_disconnect_by_func( real_widget, (void*)OnSelect, this);

		gtk_combo_box_text_remove( GTK_COMBO_BOX_TEXT(real_widget), child_pos );

		xgui::Callback * refresh_cb = this_cbox->getEvent("onrefresh");
		if (refresh_cb) refresh_cb->call( this_cbox );

		g_signal_connect ( G_OBJECT ( real_widget ), "changed", G_CALLBACK ( OnSelect ), this );
	}

	void Combobox::appendText(std::string const &text)
	{
		// gtk_entry_append_text removed in GTK 3
		std::string str = gtk_entry_get_text( entry );
		str += text;
		gtk_entry_set_text( entry, str.c_str() );
	}

	void Combobox::prependText(std::string const &text)
	{
		// gtk_entry_prepend_text removed in GTK 3
		std::string str = gtk_entry_get_text( entry );
		str = text + str;
		gtk_entry_set_text( entry, str.c_str() );
	}
	void Combobox::insertText(std::string const &text, unsigned int pos)
	{
		std::string str = gtk_entry_get_text( entry );
		if ( (pos > 0) && (pos < str.size()) )
			str.insert(pos, text);
		gtk_entry_set_text( entry, str.c_str() );
	}

	void Combobox::setEntryText(std::string const &text) { gtk_entry_set_text ( entry, text.c_str() ); }
	std::string Combobox::getEntryText() { return gtk_entry_get_text( GTK_ENTRY( gtk_bin_get_child(GTK_BIN(real_widget)) ) ); }

	void Combobox::setSelected(int num) { gtk_combo_box_set_active(GTK_COMBO_BOX(real_widget), num); }
	int Combobox::getSelected() { return gtk_combo_box_get_active(GTK_COMBO_BOX(real_widget)); }
	std::string Combobox::getSelectedItemText()
	{
		gchar * txt = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(real_widget));
		std::string result = txt ? txt : "";
		g_free(txt);
		return result;
	}

	void Combobox::setMaxLength(int len) { gtk_entry_set_max_length(entry, len); }
	int Combobox::getMaxLength() { return gtk_entry_get_max_length(entry); }

	void Combobox::setAsEditable(bool iseditable)
	{
		bool visible = gtk_widget_get_visible(real_widget);
		g_signal_handlers_disconnect_matched ( G_OBJECT(real_widget), G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, this );
		gtk_widget_destroy(real_widget);

		real_widget = ((!iseditable) ? gtk_combo_box_text_new() : gtk_combo_box_text_new_with_entry());
		if (!iseditable)
			entry = 0;
		else
			entry = GTK_ENTRY(gtk_bin_get_child(GTK_BIN(real_widget)));

		gtk_container_add ( GTK_CONTAINER(widget), real_widget );
		g_signal_connect ( G_OBJECT(real_widget), "event", G_CALLBACK(Widget::OnGdkEvent), this );

		if (visible) gtk_widget_show(real_widget);
		refresh();
	}

	void Combobox::popup() { gtk_combo_box_popup ( GTK_COMBO_BOX(real_widget) ); }
	void Combobox::popdown() { gtk_combo_box_popdown ( GTK_COMBO_BOX(real_widget) ); }

	bool Combobox::isSelected(xgui::Model * model, int pos) 
	{
		return (gtk_combo_box_get_active( GTK_COMBO_BOX(widget) ) == pos );
	}

	void Combobox::select(xgui::Model * parent, int pos, bool state)
	{ 
		if (state)
			gtk_combo_box_set_active( GTK_COMBO_BOX(widget), pos );
		else
			if (isSelected(parent, pos))
				gtk_combo_box_set_active( GTK_COMBO_BOX(widget), -1 );
	}

	void Combobox::selectAll(xgui::Model * model, bool state) 
	{
		if (!state)
			gtk_combo_box_set_active( GTK_COMBO_BOX(widget), -1 );
	}

	int Combobox::OnSelect ( GtkComboBox * w, Combobox *cbox )
	{
		xgui::Callback * base_cb = cbox->this_widget->getEvent("onselect");
		if (!base_cb) return 0;

		int selection = gtk_combo_box_get_active ( GTK_COMBO_BOX( w ) );
		xgui::TextStatusCallback * cb = dynamic_cast<xgui::TextStatusCallback*>(base_cb);
		gchar * text = gtk_combo_box_text_get_active_text ( GTK_COMBO_BOX_TEXT( w ) );
		std::string txt(text ? text : "");
		g_free(text);

		if (!cb) {
			DMESSAGE("onselect event of xgui::Combobox expected a TextStatusCallback");
			return 0;
		}

		cb->call( cbox->this_cbox, txt, selection );

		return 0;
	}

	int Combobox::OnSubmit ( GtkWidget * w, Combobox * e )
	{
		xgui::Callback * base_cb = e->this_cbox->getEvent("onsubmit");
		if (!base_cb) return 0;

		xgui::TextCallback * cb = dynamic_cast<xgui::TextCallback *>(base_cb);
		if (!cb) {
			DMESSAGE("onsubmit event of xgui::Entry expected a TextCallback");
			return 0;
		}

		gchar * text = gtk_combo_box_text_get_active_text ( GTK_COMBO_BOX_TEXT( e->real_widget ) );
		std::string txt(text ? text : "");
		g_free(text);
		int rv = cb->call ( e->this_cbox, txt );

		return rv;
	}

	void Combobox::OnTextInsert ( GtkWidget * editable, gchar * new_text, gint new_text_length, gint * position, Combobox * e )
	{
		if (!new_text_length) return;
	
		g_signal_handlers_block_by_func ( G_OBJECT ( editable ), (void*)OnTextInsert, e );
		bool text_approved = true;
	
		std::string text_after_change = gtk_entry_get_text( GTK_ENTRY(editable) );
		text_after_change.insert(*position, text_after_change);
	
		xgui::Callback * base_cb = e->this_cbox->getEvent("onchange");
		if (base_cb) {
			xgui::TextCallback * cb = dynamic_cast<xgui::TextCallback *>(base_cb);
			if (!cb)
				DMESSAGE("onchange event of xgui::Entry expected a TextCallback");
			else
				text_approved = cb->call ( e->this_cbox, text_after_change );
		}
	
		if ( text_approved ) 
			gtk_editable_insert_text ( GTK_EDITABLE ( editable ) , new_text, new_text_length, position);
	
	
		g_signal_handlers_unblock_by_func ( G_OBJECT ( editable ), (void*)OnTextInsert, e );
		g_signal_stop_emission_by_name ( G_OBJECT ( editable ), "insert-text" );
	}

	void Combobox::OnTextDelete ( GtkEditable *editable, gint start_pos, gint end_pos, Combobox * e )
	{
		g_signal_handlers_block_by_func ( G_OBJECT ( editable ), (void*)OnTextDelete, e );
		bool deletion_approved = true;
	
		std::string text_after_change = gtk_entry_get_text( GTK_ENTRY(editable) );
		text_after_change.erase(start_pos, end_pos - start_pos);
	
		xgui::TextCallback * cb = dynamic_cast<xgui::TextCallback *>(e->this_cbox->getEvent("onchange"));
		if ( cb ) 
			deletion_approved = cb->call ( e->this_cbox, text_after_change );
		if ( deletion_approved ) 
			gtk_editable_delete_text( GTK_EDITABLE(editable), start_pos, end_pos );
		
		g_signal_handlers_unblock_by_func ( G_OBJECT ( editable ), (void*)OnTextDelete, e );
		g_signal_stop_emission_by_name ( G_OBJECT ( editable ), "delete-text" );
	}
}
