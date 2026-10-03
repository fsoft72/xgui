//
// C++ Implementation: menu_impl
//
// Description: menu bar built on a GMenu model and a GSimpleActionGroup
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "menu_impl.h"
#include "window_impl.h"
#include "menu.h"
#include "window.h"
#include "callback.h"
#include "image.h"
#include "image_impl.h"

/** Custom GMenu attribute holding the xgui id of an item. */
static char const * const XGUI_ID_ATTRIBUTE = "xgui-id";

/** Counter used to give every menu its own action prefix. */
static int menu_counter = 0;

/** Converts a label with '&' mnemonics into the GMenu syntax ('_'). */
static std::string int_mnemonic_label ( std::string const &text )
{
	std::string label;
	for (std::string::const_iterator c = text.begin(); c != text.end(); ++c) {
		if (*c == '_') label += "__";
		else if (*c == '&') label += '_';
		else label += *c;
	}
	return label;
}

/** Escapes a plain text label so that GMenu does not take '_' as a mnemonic. */
static std::string int_plain_label ( std::string const &text )
{
	std::string label;
	for (std::string::const_iterator c = text.begin(); c != text.end(); ++c) {
		if (*c == '_') label += "__";
		else label += *c;
	}
	return label;
}

/** Returns the text shown for a GMenu label (mnemonic markers removed). */
static std::string int_display_text ( std::string const &label )
{
	std::string text;
	for (std::string::size_type i = 0; i < label.size(); ++i) {
		if (label[i] != '_') text += label[i];
		else if ( (i + 1 < label.size()) && (label[i + 1] == '_') ) { text += '_'; ++i; }
	}
	return text;
}

/** Creates the menu bar widget on a new root model. */
static GtkWidget * int_new_menu_bar ()
{
	GMenu * root = g_menu_new();
	GtkWidget * bar = gtk_popover_menu_bar_new_from_model ( G_MENU_MODEL(root) );
	g_object_unref ( root );
	return bar;
}

namespace xguimpl
{

	Menu::Menu ( GtkWidget * real_w )
	: Widget( real_w ), this_menu(0), root(0), actions(0), action_counter(0), radio_action(0), window_widget(0), popup(0) {}

	Menu::Menu ( xgui::Window * parent, bool visible )
	: Widget ( int_new_menu_bar() ), this_menu(0), root(0), actions(0), action_counter(0), radio_action(0), window_widget(0), popup(0)
	{
		root = G_MENU ( gtk_popover_menu_bar_get_menu_model ( GTK_POPOVER_MENU_BAR(widget) ) );
		g_object_ref ( root );

		actions = g_simple_action_group_new();
		prefix = "xguimenu" + xgui::semantic_cast<std::string>(++menu_counter);
		if (parent) {
			window_widget = parent->getImpl()->widget;
			gtk_widget_insert_action_group ( window_widget, prefix.c_str(), G_ACTION_GROUP(actions) );
		}

		gtk_widget_set_visible ( widget, visible );
	}

	Menu::~Menu()
	{
		if (popup) gtk_widget_unparent(popup);

		for (MenuItemMap::iterator i = items_map.begin(); i != items_map.end(); ++i)
			if (i->second.icon) i->second.icon->unref();

		if (actions) {
			gchar ** names = g_action_group_list_actions ( G_ACTION_GROUP(actions) );
			for (gchar ** n = names; n && *n; ++n) {
				GAction * action = g_action_map_lookup_action ( G_ACTION_MAP(actions), *n );
				g_signal_handlers_disconnect_by_data ( action, this );
			}
			g_strfreev ( names );

			if (window_widget && GTK_IS_WIDGET(window_widget))
				gtk_widget_insert_action_group ( window_widget, prefix.c_str(), NULL );
			g_object_unref ( actions );
		}

		for (std::vector<GMenu *>::iterator m = owned_menus.begin(); m != owned_menus.end(); ++m)
			g_object_unref ( *m );
		if (root) g_object_unref ( root );
	}

	bool Menu::linkEvent(std::string const & name)
	{
		// Applications link "onclick"; "onselect" is accepted as the name the GTK 3 backend looked up
		if ( (name == "onclick") || (name == "onselect") )
			return true;

		return Widget::linkEvent(name);
	}

	void Menu::setAccessor(xgui::Widget * w)
	{
		xgui::Menu * downcasted_widget = dynamic_cast<xgui::Menu*>(w);
		ASSERT(downcasted_widget);
		this_menu = downcasted_widget;

		Widget::setAccessor(w);
	}

	GSimpleAction * Menu::newAction ( GVariantType const * parameter, GVariant * state )
	{
		std::string name = "item" + xgui::semantic_cast<std::string>(++action_counter);
		GSimpleAction * action = state ? g_simple_action_new_stateful ( name.c_str(), parameter, state )
		                               : g_simple_action_new ( name.c_str(), parameter );
		g_action_map_add_action ( G_ACTION_MAP(actions), G_ACTION(action) );
		g_object_unref ( action );
		return action;
	}

	GMenuItem * Menu::buildItem ( std::string const &id, MenuItemInfo const &info )
	{
		GMenuItem * item = g_menu_item_new ( info.label.c_str(), NULL );
		g_menu_item_set_attribute ( item, XGUI_ID_ATTRIBUTE, "s", id.c_str() );

		if (info.submenu)
			g_menu_item_set_submenu ( item, G_MENU_MODEL(info.submenu) );
		else if (info.action) {
			std::string action = prefix + "." + g_action_get_name ( G_ACTION(info.action) );
			if (info.type == "radio")
				g_menu_item_set_action_and_target_value ( item, action.c_str(), g_variant_new_string ( id.c_str() ) );
			else
				g_menu_item_set_action_and_target_value ( item, action.c_str(), NULL );
		}

		// GTK 4 popover menus usually do not draw the icon next to the label
		if (info.icon)
			g_menu_item_set_icon ( item, G_ICON(info.icon->getImpl()->getTexture()) );

		return item;
	}

	int Menu::findPosition ( std::string const &id, MenuItemInfo const &info )
	{
		GMenuModel * model = G_MENU_MODEL(info.container);
		int n = g_menu_model_get_n_items ( model );
		for (int i = 0; i < n; ++i) {
			gchar * item_id = NULL;
			if ( !g_menu_model_get_item_attribute ( model, i, XGUI_ID_ATTRIBUTE, "s", &item_id ) ) continue;
			bool found = (id == item_id);
			g_free ( item_id );
			if (found) return i;
		}
		return -1;
	}

	void Menu::rebuildItem ( std::string const &id )
	{
		MenuItemMap::iterator e = items_map.find(id);
		if (e == items_map.end()) return;

		int pos = findPosition ( id, e->second );
		if (pos < 0) return;

		GMenuItem * item = buildItem ( id, e->second );
		g_menu_remove ( e->second.container, pos );
		g_menu_insert_item ( e->second.container, pos, item );
		g_object_unref ( item );
	}

	void Menu::addItem( std::string const &parent_id, std::string const &id, std::string const &text, xgui::Image * icon, std::string const &type )
	{
		if ( type == "radio_begin" ) {
			radio_parent = parent_id;
			radio_action = newAction ( G_VARIANT_TYPE_STRING, g_variant_new_string ( "" ) );
			g_signal_connect ( radio_action, "activate", G_CALLBACK ( OnRadioActivate ), this );
			return;
		}
		if ( type == "radio_end" ) {
			radio_parent.clear();
			radio_action = 0;
			return;
		}
		// While a radio group is open only its own items are accepted, as with GTK 3
		if ( radio_action && (parent_id != radio_parent) ) return;

		// The item is appended to the root menu, or to the current section of the submenu of its parent
		GMenu * container = root;
		MenuItemMap::iterator parent = items_map.end();
		if ( !parent_id.empty() ) {
			parent = items_map.find(parent_id);
			if ( parent == items_map.end() ) return;
			if ( !parent->second.submenu ) {
				GMenu * submenu = g_menu_new();
				GMenu * section = g_menu_new();
				g_menu_append_section ( submenu, NULL, G_MENU_MODEL(section) );
				owned_menus.push_back ( submenu );
				owned_menus.push_back ( section );
				parent->second.submenu = submenu;
				parent->second.section = section;
				rebuildItem ( parent_id );
			}
			container = parent->second.section;
		}

		MenuItemInfo info;
		info.container = container;
		info.type = radio_action ? "radio" : type;

		if ( info.type == "separator" ) {
			// A separator starts a new section of the parent submenu
			if ( parent != items_map.end() ) {
				GMenu * section = g_menu_new();
				g_menu_append_section ( parent->second.submenu, NULL, G_MENU_MODEL(section) );
				owned_menus.push_back ( section );
				parent->second.section = section;
				info.container = section;
			}
			items_map[id] = info;
			return;
		}

		info.label = int_mnemonic_label ( text );
		if ( icon ) {
			icon->ref();
			info.icon = icon;
		}

		if ( parent_id.empty() ) {
			// A menu bar only shows submenus: top level items get theirs right away
			info.submenu = g_menu_new();
			info.section = g_menu_new();
			g_menu_append_section ( info.submenu, NULL, G_MENU_MODEL(info.section) );
			owned_menus.push_back ( info.submenu );
			owned_menus.push_back ( info.section );
		}
		else if ( info.type == "radio" ) {
			info.action = radio_action;
			// The first item of a group is the selected one
			GVariant * state = g_action_get_state ( G_ACTION(radio_action) );
			bool first = !*g_variant_get_string ( state, NULL );
			g_variant_unref ( state );
			if ( first ) g_simple_action_set_state ( radio_action, g_variant_new_string ( id.c_str() ) );
		}
		else {
			GVariant * state = (info.type == "check") ? g_variant_new_boolean ( FALSE ) : NULL;
			info.action = newAction ( NULL, state );
			action_ids[g_action_get_name ( G_ACTION(info.action) )] = id;
			g_signal_connect ( info.action, "activate", G_CALLBACK ( OnActionActivate ), this );
		}

		items_map[id] = info;
		GMenuItem * item = buildItem ( id, info );
		g_menu_append_item ( container, item );
		g_object_unref ( item );
	}

	void Menu::delItem( std::string const &id )
	{
		MenuItemMap::iterator e = items_map.find(id);
		if (e == items_map.end()) return;

		int pos = (e->second.type == "separator") ? -1 : findPosition ( id, e->second );
		if (pos >= 0) g_menu_remove ( e->second.container, pos );

		if ( e->second.action && (e->second.type != "radio") ) {
			char const * name = g_action_get_name ( G_ACTION(e->second.action) );
			action_ids.erase ( name );
			g_action_map_remove_action ( G_ACTION_MAP(actions), name );
		}
		if (e->second.icon) e->second.icon->unref();
		items_map.erase(e);
	}

	void Menu::popupMenu(std::string const &id)
	{
		MenuItemMap::iterator e = items_map.find(id);
		if ( (e == items_map.end()) || !e->second.submenu || !window_widget ) return;

		if (popup) gtk_widget_unparent ( popup );

		// The popover is placed at the last pointer position seen inside the window
		GtkWidget * anchor = gtk_window_get_child ( GTK_WINDOW(window_widget) );
		if (!anchor) return;
		Window * window_impl = dynamic_cast<Window *>(this_menu->getParent()->getImpl());
		graphene_point_t in = GRAPHENE_POINT_INIT ( 0, 0 ), out;
		if (window_impl) in = GRAPHENE_POINT_INIT ( (float)window_impl->pointer_x, (float)window_impl->pointer_y );
		if ( !gtk_widget_compute_point ( window_widget, anchor, &in, &out ) ) out = in;

		popup = gtk_popover_menu_new_from_model ( G_MENU_MODEL(e->second.submenu) );
		gtk_widget_set_parent ( popup, anchor );
		GdkRectangle where = { (int)out.x, (int)out.y, 1, 1 };
		gtk_popover_set_pointing_to ( GTK_POPOVER(popup), &where );
		gtk_popover_set_has_arrow ( GTK_POPOVER(popup), FALSE );
		g_signal_connect ( popup, "closed", G_CALLBACK ( OnPopupClosed ), this );
		gtk_popover_popup ( GTK_POPOVER(popup) );
	}

	std::string Menu::actionName(std::string const &id)
	{
		MenuItemMap::iterator e = items_map.find(id);
		if ( (e == items_map.end()) || !e->second.action ) return "";
		return g_action_get_name ( G_ACTION(e->second.action) );
	}

	void Menu::enableItem(std::string const &id)
	{
		MenuItemMap::iterator e = items_map.find(id);
		if ( (e != items_map.end()) && e->second.action )
			g_simple_action_set_enabled ( e->second.action, TRUE );
	}

	void Menu::disableItem(std::string const &id)
	{
		MenuItemMap::iterator e = items_map.find(id);
		if ( (e != items_map.end()) && e->second.action )
			g_simple_action_set_enabled ( e->second.action, FALSE );
	}

	bool Menu::isItemEnabled(std::string const &id)
	{
		MenuItemMap::iterator e = items_map.find(id);
		if (e == items_map.end()) return false;
		if (!e->second.action) return true;
		return g_action_get_enabled ( G_ACTION(e->second.action) );
	}

	void Menu::checkItem(std::string const &id)
	{
		MenuItemMap::iterator e = items_map.find(id);
		if (e == items_map.end()) return;

		if (e->second.type == "check")
			g_simple_action_set_state ( e->second.action, g_variant_new_boolean ( TRUE ) );
		else if (e->second.type == "radio")
			g_simple_action_set_state ( e->second.action, g_variant_new_string ( id.c_str() ) );
	}

	void Menu::uncheckItem(std::string const &id)
	{
		// A radio item is unchecked only by checking another item of its group
		MenuItemMap::iterator e = items_map.find(id);
		if ( (e != items_map.end()) && (e->second.type == "check") )
			g_simple_action_set_state ( e->second.action, g_variant_new_boolean ( FALSE ) );
	}

	bool Menu::isItemChecked(std::string const &id)
	{
		MenuItemMap::iterator e = items_map.find(id);
		if (e == items_map.end()) return false;

		GVariant * state = NULL;
		bool checked = false;
		if (e->second.type == "check") {
			state = g_action_get_state ( G_ACTION(e->second.action) );
			checked = g_variant_get_boolean ( state );
		}
		else if (e->second.type == "radio") {
			state = g_action_get_state ( G_ACTION(e->second.action) );
			checked = (id == g_variant_get_string ( state, NULL ));
		}
		if (state) g_variant_unref ( state );
		return checked;
	}

	void Menu::setItemText(std::string const &id, std::string const &text)
	{
		MenuItemMap::iterator e = items_map.find(id);
		if ( (e == items_map.end()) || (e->second.type == "separator") ) return;

		e->second.label = int_plain_label ( text );
		rebuildItem ( id );
	}

	std::string Menu::getItemText(std::string const &id)
	{
		MenuItemMap::iterator e = items_map.find(id);
		if (e == items_map.end()) return "";
		return int_display_text ( e->second.label );
	}

	void Menu::notifySelect(std::string const &id, int state)
	{
		xgui::Callback * base_cb = this_menu->getEvent("onclick");
		if (!base_cb) base_cb = this_menu->getEvent("onselect");
		if (!base_cb) return;

		xgui::TextStatusCallback * cb = dynamic_cast<xgui::TextStatusCallback *>(base_cb);
		if (!cb) {
			DMESSAGE("onclick event of xgui::Menu expected a TextStatusCallback");
			return;
		}

		cb->call ( this_menu, id, state );
	}

	void Menu::OnActionActivate(GSimpleAction * action, GVariant * parameter, Menu * w)
	{
		std::map<std::string, std::string>::iterator e = w->action_ids.find ( g_action_get_name ( G_ACTION(action) ) );
		if (e == w->action_ids.end()) return;
		std::string id = e->second;

		int item_state = 1;
		GVariant * state = g_action_get_state ( G_ACTION(action) );
		if (state) {
			// A check item toggles its state when activated
			item_state = !g_variant_get_boolean ( state );
			g_variant_unref ( state );
			g_simple_action_set_state ( action, g_variant_new_boolean ( item_state ) );
		}

		w->notifySelect ( id, item_state );
	}

	void Menu::OnRadioActivate(GSimpleAction * action, GVariant * parameter, Menu * w)
	{
		if (!parameter) return;

		GVariant * state = g_action_get_state ( G_ACTION(action) );
		std::string previous = g_variant_get_string ( state, NULL );
		g_variant_unref ( state );

		std::string selected = g_variant_get_string ( parameter, NULL );
		g_simple_action_set_state ( action, g_variant_new_string ( selected.c_str() ) );

		// As with GTK 3 radio menu items, the item losing the selection is reported first
		if ( !previous.empty() && (previous != selected) ) w->notifySelect ( previous, 0 );
		w->notifySelect ( selected, 1 );
	}

	void Menu::OnPopupClosed(GtkPopover * popover, Menu * w)
	{
		if (w->popup != GTK_WIDGET(popover)) return;
		gtk_widget_unparent ( w->popup );
		w->popup = 0;
	}
}
