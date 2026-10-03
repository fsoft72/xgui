//
// C++ Implementation: combobox_impl
//
// Description: Combobox on GtkDropDown (not editable) or GtkEntry plus a list popover (editable)
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

#include <cstring>
#include <vector>

/** Data key of the list view inside the popover of an editable combobox. */
static char const * const LIST_KEY = "xgui-combobox-list";

/** Returns the first string of a model node, "" when it has none. */
static std::string int_node_text ( xgui::Model * node )
{
	return (node && node->numberOfStrings()) ? node->getString(0) : std::string();
}

/** Returns the first popover below a widget, or NULL. */
static GtkWidget * int_find_popover ( GtkWidget * w )
{
	if ( GTK_IS_POPOVER(w) ) return w;
	for ( GtkWidget * c = gtk_widget_get_first_child(w); c != NULL; c = gtk_widget_get_next_sibling(c) ) {
		GtkWidget * found = int_find_popover(c);
		if ( found ) return found;
	}
	return NULL;
}

/* Items of the editable list: a label showing the string */

static void int_item_setup ( GtkSignalListItemFactory *, GObject * item, gpointer )
{
	GtkWidget * label = gtk_label_new(NULL);
	gtk_label_set_xalign(GTK_LABEL(label), 0.0f);
	gtk_list_item_set_child(GTK_LIST_ITEM(item), label);
}

static void int_item_bind ( GtkSignalListItemFactory *, GObject * item, gpointer )
{
	GtkStringObject * str = GTK_STRING_OBJECT(gtk_list_item_get_item(GTK_LIST_ITEM(item)));
	gtk_label_set_text(GTK_LABEL(gtk_list_item_get_child(GTK_LIST_ITEM(item))), gtk_string_object_get_string(str));
}

namespace xguimpl
{
	Combobox::Combobox ( GtkWidget * real_w )
	: View ( gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ), items(gtk_string_list_new(NULL)), dropdown(0), entry(0), menu_button(0),
	  list_selection(0), editable_selected(-1), updating(false), this_cbox(0) { }

	Combobox::Combobox ( xgui::Container * parent, bool editable )
	: View ( gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ), items(gtk_string_list_new(NULL)), dropdown(0), entry(0), menu_button(0),
	  list_selection(0), editable_selected(-1), updating(false), this_cbox(0)
	{
		build(editable);
	}

	Combobox::~Combobox()
	{
		if ( GTK_IS_WIDGET(widget) ) unbuild();
		g_object_unref ( items );
	}

	void Combobox::setAccessor(xgui::Widget * w)
	{
		xgui::Combobox * downcasted_widget = dynamic_cast<xgui::Combobox*>(w);
		ASSERT(downcasted_widget);
		this_cbox = downcasted_widget;

		Widget::setAccessor(w);
	}

	GtkWidget * Combobox::eventWidget() { return entry ? entry : (dropdown ? dropdown : widget); }

	void Combobox::build(bool editable)
	{
		if ( !editable ) {
			dropdown = gtk_drop_down_new ( G_LIST_MODEL ( g_object_ref ( items ) ), NULL );
			// Unlike GTK 3, a GtkDropDown always selects an item (the first one) when it has items
			gtk_widget_set_hexpand ( dropdown, TRUE );
			g_signal_connect ( dropdown, "notify::selected", G_CALLBACK ( OnSelect ), this );
			g_signal_connect ( dropdown, "activate", G_CALLBACK ( OnSubmit ), this );
			gtk_box_append ( GTK_BOX(widget), dropdown );
			return;
		}

		entry = gtk_entry_new();
		gtk_widget_set_hexpand ( entry, TRUE );
		GtkEditable * delegate = gtk_editable_get_delegate ( GTK_EDITABLE ( entry ) );
		g_signal_connect ( delegate, "insert-text", G_CALLBACK ( OnTextInsert ), this );
		g_signal_connect ( delegate, "delete-text", G_CALLBACK ( OnTextDelete ), this );
		g_signal_connect ( entry, "activate", G_CALLBACK ( OnSubmit ), this );

		list_selection = gtk_single_selection_new ( G_LIST_MODEL ( g_object_ref ( items ) ) );
		gtk_single_selection_set_autoselect ( list_selection, FALSE );
		gtk_single_selection_set_can_unselect ( list_selection, TRUE );
		GtkListItemFactory * factory = gtk_signal_list_item_factory_new();
		g_signal_connect ( factory, "setup", G_CALLBACK ( int_item_setup ), NULL );
		g_signal_connect ( factory, "bind", G_CALLBACK ( int_item_bind ), NULL );
		GtkWidget * list = gtk_list_view_new ( GTK_SELECTION_MODEL ( g_object_ref ( list_selection ) ), factory );
		gtk_list_view_set_single_click_activate ( GTK_LIST_VIEW(list), TRUE );
		g_signal_connect ( list, "activate", G_CALLBACK ( OnListActivate ), this );

		GtkWidget * scrolled = gtk_scrolled_window_new();
		gtk_scrolled_window_set_policy ( GTK_SCROLLED_WINDOW(scrolled), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC );
		gtk_scrolled_window_set_propagate_natural_height ( GTK_SCROLLED_WINDOW(scrolled), TRUE );
		gtk_scrolled_window_set_max_content_height ( GTK_SCROLLED_WINDOW(scrolled), 300 );
		gtk_scrolled_window_set_child ( GTK_SCROLLED_WINDOW(scrolled), list );

		GtkWidget * popover = gtk_popover_new();
		gtk_popover_set_child ( GTK_POPOVER(popover), scrolled );
		menu_button = gtk_menu_button_new();
		gtk_menu_button_set_popover ( GTK_MENU_BUTTON(menu_button), popover );
		g_object_set_data ( G_OBJECT(menu_button), LIST_KEY, list );

		gtk_widget_add_css_class ( widget, "linked" );
		gtk_box_append ( GTK_BOX(widget), entry );
		gtk_box_append ( GTK_BOX(widget), menu_button );
	}

	void Combobox::unbuild()
	{
		if ( dropdown ) {
			g_signal_handlers_disconnect_by_data ( dropdown, this );
			gtk_box_remove ( GTK_BOX(widget), dropdown );
			dropdown = 0;
		}
		if ( entry ) {
			g_signal_handlers_disconnect_by_data ( gtk_editable_get_delegate ( GTK_EDITABLE ( entry ) ), this );
			g_signal_handlers_disconnect_by_data ( entry, this );
			gtk_box_remove ( GTK_BOX(widget), entry );
			entry = 0;
		}
		if ( menu_button ) {
			gtk_menu_button_popdown ( GTK_MENU_BUTTON(menu_button) );
			GtkWidget * list = (GtkWidget *)g_object_get_data ( G_OBJECT(menu_button), LIST_KEY );
			if ( list ) g_signal_handlers_disconnect_by_data ( list, this );
			gtk_box_remove ( GTK_BOX(widget), menu_button );
			menu_button = 0;
		}
		if ( list_selection ) {
			g_object_unref ( list_selection );
			list_selection = 0;
		}
		gtk_widget_remove_css_class ( widget, "linked" );
		editable_selected = -1;
	}

	bool Combobox::linkEvent( std::string const &name )
	{
		// The handlers are connected when the widgets are built, so that setAsEditable() keeps them
		if ( (name == "onchange") || (name == "onrefresh") || (name == "onsubmit") )
			return true;

		return Widget::linkEvent(name);
	}

	std::string Combobox::itemText(int index)
	{
		if ( index < 0 || (guint)index >= g_list_model_get_n_items ( G_LIST_MODEL(items) ) ) return "";
		const char * text = gtk_string_list_get_string ( items, index );
		return text ? text : "";
	}

	void Combobox::notifyRefresh()
	{
		xgui::Callback * refresh_cb = this_cbox->getEvent("onrefresh");
		if (refresh_cb) refresh_cb->call( this_cbox );
	}

	bool Combobox::notifyChange(std::string const &text)
	{
		xgui::Callback * base_cb = this_cbox->getEvent("onchange");
		if (!base_cb) return true;

		xgui::TextCallback * cb = dynamic_cast<xgui::TextCallback*>(base_cb);
		if (!cb) {
			DMESSAGE("onchange event of xgui::Combobox expected a TextCallback");
			return true;
		}
		return cb->call( this_cbox, text );
	}

	int Combobox::notifySubmit()
	{
		xgui::Callback * base_cb = this_cbox->getEvent("onsubmit");
		if (!base_cb) return 0;

		xgui::TextCallback * cb = dynamic_cast<xgui::TextCallback *>(base_cb);
		if (!cb) {
			DMESSAGE("onsubmit event of xgui::Combobox expected a TextCallback");
			return 0;
		}

		return cb->call ( this_cbox, entry ? getEntryText() : getSelectedItemText() );
	}

	void Combobox::refresh()
	{
		xgui::Model * model = this_cbox->getModel();
		if (!model) return;

		std::vector<std::string> texts;
		for ( xgui::Model::iterator iter = model->begin(); iter != model->end(); ++iter )
			texts.push_back ( int_node_text ( dynamic_cast<xgui::Model*>(*iter) ) );
		std::vector<const char *> strings;
		for ( std::vector<std::string>::iterator t = texts.begin(); t != texts.end(); ++t )
			strings.push_back ( t->c_str() );
		strings.push_back ( NULL );

		int selected = getSelected();
		updating = true;
		gtk_string_list_splice ( items, 0, g_list_model_get_n_items ( G_LIST_MODEL(items) ), strings.data() );
		setSelected ( (selected < (int)texts.size()) ? selected : -1 );
		updating = false;

		notifyRefresh();
	}

	void Combobox::insertItem ( xgui::Model * parent, int child_pos, xgui::Model * child )
	{
		xgui::Model * model = this_cbox->getModel();
		if ( !model || (parent != model) ) return;

		guint n = g_list_model_get_n_items ( G_LIST_MODEL(items) );
		guint pos = (child_pos < 0 || (guint)child_pos > n) ? n : (guint)child_pos;
		std::string text = int_node_text ( child );
		const char * additions[] = { text.c_str(), NULL };

		int selected = getSelected();
		updating = true;
		gtk_string_list_splice ( items, pos, 0, additions );
		// The selected item stays selected when rows are inserted before it
		setSelected ( (selected >= 0 && (guint)selected >= pos) ? selected + 1 : selected );
		updating = false;

		notifyRefresh();
	}

	void Combobox::removeItem ( xgui::Model * parent, int child_pos, xgui::Model * child )
	{
		xgui::Model * model = this_cbox->getModel();
		if ( !model || (parent != model) ) return;
		if ( child_pos < 0 || (guint)child_pos >= g_list_model_get_n_items ( G_LIST_MODEL(items) ) ) return;

		int selected = getSelected();
		updating = true;
		gtk_string_list_remove ( items, child_pos );
		if ( selected == child_pos ) setSelected ( -1 );
		else setSelected ( selected > child_pos ? selected - 1 : selected );
		updating = false;

		notifyRefresh();
	}

	void Combobox::appendText(std::string const &text)
	{
		if ( entry ) setEntryText ( getEntryText() + text );
	}

	void Combobox::prependText(std::string const &text)
	{
		if ( entry ) setEntryText ( text + getEntryText() );
	}

	void Combobox::insertText(std::string const &text, unsigned int pos)
	{
		if ( !entry ) return;
		std::string str = getEntryText();
		if ( (pos > 0) && (pos < str.size()) )
			str.insert(pos, text);
		setEntryText ( str );
	}

	void Combobox::setEntryText(std::string const &text) { if (entry) gtk_editable_set_text ( GTK_EDITABLE(entry), text.c_str() ); }
	std::string Combobox::getEntryText() { return entry ? gtk_editable_get_text( GTK_EDITABLE( entry ) ) : ""; }

	void Combobox::setSelected(int num)
	{
		guint n = g_list_model_get_n_items ( G_LIST_MODEL(items) );
		bool valid = (num >= 0) && ((guint)num < n);

		if ( dropdown ) {
			gtk_drop_down_set_selected ( GTK_DROP_DOWN(dropdown), valid ? (guint)num : GTK_INVALID_LIST_POSITION );
		}
		else if ( entry ) {
			if ( valid && (num != editable_selected) ) {
				// Choosing an item by code does not ask onchange, as for the drop down
				bool was_updating = updating;
				updating = true;
				setEntryText ( itemText(num) );
				updating = was_updating;
			}
			editable_selected = valid ? num : -1;
		}
	}

	int Combobox::getSelected()
	{
		if ( dropdown ) {
			guint selected = gtk_drop_down_get_selected ( GTK_DROP_DOWN(dropdown) );
			return (selected == GTK_INVALID_LIST_POSITION) ? -1 : (int)selected;
		}
		return editable_selected;
	}

	std::string Combobox::getSelectedItemText() { return itemText ( getSelected() ); }

	void Combobox::setMaxLength(int len) { if (entry) gtk_entry_set_max_length(GTK_ENTRY(entry), len); }
	int Combobox::getMaxLength() { return entry ? gtk_entry_get_max_length(GTK_ENTRY(entry)) : 0; }

	void Combobox::setAsEditable(bool iseditable)
	{
		if ( iseditable == (entry != 0) ) return;
		unbuild();
		build(iseditable);
	}

	void Combobox::popup()
	{
		if ( menu_button ) gtk_menu_button_popup ( GTK_MENU_BUTTON(menu_button) );
		else if ( dropdown ) g_signal_emit_by_name ( dropdown, "activate" );
	}

	void Combobox::popdown()
	{
		if ( menu_button ) gtk_menu_button_popdown ( GTK_MENU_BUTTON(menu_button) );
		else if ( dropdown ) {
			GtkWidget * popover = int_find_popover ( dropdown );
			if ( popover ) gtk_popover_popdown ( GTK_POPOVER(popover) );
		}
	}

	bool Combobox::isSelected(xgui::Model * model, int pos)
	{
		return getSelected() == pos;
	}

	void Combobox::select(xgui::Model * parent, int pos, bool state)
	{
		if (state)
			setSelected ( pos );
		else if (isSelected(parent, pos))
			setSelected ( -1 );
	}

	void Combobox::selectAll(xgui::Model * model, bool state)
	{
		if (!state)
			setSelected ( -1 );
	}

	void Combobox::OnSelect ( GObject * w, GParamSpec * pspec, Combobox * cbox )
	{
		if ( cbox->updating ) return;
		cbox->notifyChange ( cbox->getSelectedItemText() );
	}

	void Combobox::OnSubmit ( GtkWidget * w, Combobox * e )
	{
		e->notifySubmit();
	}

	void Combobox::OnListActivate ( GtkListView * list, guint position, Combobox * cbox )
	{
		// The chosen item becomes the text of the entry; onchange can still reject it
		cbox->setEntryText ( cbox->itemText(position) );
		if ( cbox->getEntryText() == cbox->itemText(position) ) cbox->editable_selected = position;
		gtk_menu_button_popdown ( GTK_MENU_BUTTON(cbox->menu_button) );
	}

	void Combobox::OnTextInsert ( GtkEditable * editable, gchar * new_text, gint new_text_length, gint * position, Combobox * e )
	{
		if ( !new_text_length || e->updating ) return;
		if ( new_text_length < 0 ) new_text_length = std::strlen ( new_text );

		std::string text_after_change = gtk_editable_get_text( editable );
		text_after_change.insert(*position, new_text, new_text_length);

		g_signal_handlers_block_by_func ( G_OBJECT ( editable ), (void*)OnTextInsert, e );
		if ( e->notifyChange ( text_after_change ) ) {
			gtk_editable_insert_text ( editable, new_text, new_text_length, position );
			e->editable_selected = -1;
		}
		g_signal_handlers_unblock_by_func ( G_OBJECT ( editable ), (void*)OnTextInsert, e );
		g_signal_stop_emission_by_name ( G_OBJECT ( editable ), "insert-text" );
	}

	void Combobox::OnTextDelete ( GtkEditable *editable, gint start_pos, gint end_pos, Combobox * e )
	{
		if ( e->updating ) return;

		std::string text_after_change = gtk_editable_get_text( editable );
		if ( end_pos < 0 ) end_pos = text_after_change.size();
		text_after_change.erase(start_pos, end_pos - start_pos);

		g_signal_handlers_block_by_func ( G_OBJECT ( editable ), (void*)OnTextDelete, e );
		if ( e->notifyChange ( text_after_change ) ) {
			gtk_editable_delete_text( editable, start_pos, end_pos );
			e->editable_selected = -1;
		}
		g_signal_handlers_unblock_by_func ( G_OBJECT ( editable ), (void*)OnTextDelete, e );
		g_signal_stop_emission_by_name ( G_OBJECT ( editable ), "delete-text" );
	}
}
