//
// C++ Implementation: menu_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "menu_impl.h"
#include "menu.h"
#include "callback.h"
#include "image.h"
#include "image_impl.h"

static void int_item_id_destroy_notify( std::string * str )
{
	delete str;
}

static void int_item_icon_destroy_notify( xgui::Image * icon )
{
	icon->unref();
}

namespace xguimpl
{

	Menu::Menu ( GtkWidget * real_w ) : Widget( real_w ) {}

	Menu::Menu ( xgui::Window * parent, bool visible )
	: Widget ( gtk_menu_bar_new() )
	{
		if(!visible)
			gtk_widget_hide(widget);
		else
			gtk_widget_show(widget);
	}

	Menu::~Menu() {}

	bool Menu::linkEvent(std::string const & name) 
	{
		if (name == "onselect")
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

	void Menu::addItem( std::string const &parent_id, std::string const &id, std::string const &text, xgui::Image * icon, std::string const &type )
	{
		static std::string checkgroup_parent = "";
		static GSList * temporary_checkgroup = 0;
	
		std::string real_name = text;
		std::replace(real_name.begin(), real_name.end(), '&', '_');
		
		
		GtkMenuItem * mnu_item = 0;
		
		if ( (type == "radio_begin") ) {
			temporary_checkgroup = 0;
			checkgroup_parent = parent_id;
			return;
		}
		else if ( (type == "radio_end") ) {
			temporary_checkgroup = 0;
			checkgroup_parent = "";
			return;
		}
		else if ( !checkgroup_parent.empty() ) {
			if ( parent_id != checkgroup_parent ) return;
			mnu_item = GTK_MENU_ITEM(gtk_radio_menu_item_new_with_mnemonic(temporary_checkgroup, real_name.c_str()));
			temporary_checkgroup = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(mnu_item));
		}
		else if (type == "check")
			mnu_item = GTK_MENU_ITEM(gtk_check_menu_item_new_with_mnemonic(real_name.c_str()));
		else if (type == "separator")
			mnu_item = GTK_MENU_ITEM(gtk_separator_menu_item_new());
		else {
			if (icon == 0)
				mnu_item = GTK_MENU_ITEM(gtk_menu_item_new_with_mnemonic(real_name.c_str()));
			else {
				mnu_item = GTK_MENU_ITEM(gtk_image_menu_item_new_with_mnemonic(real_name.c_str()));
				icon->ref();
				gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(mnu_item), GTK_WIDGET(icon->getImpl()->getImage()) );
				g_object_set_data_full(G_OBJECT(mnu_item), "xgui-mnuitem-icon", (void*)icon, (GDestroyNotify)int_item_icon_destroy_notify);
			}
		}
	
		g_object_set_data_full(G_OBJECT(mnu_item), "xgui-mnuitem-id", (void*)new std::string(id), (GDestroyNotify)int_item_id_destroy_notify);
		g_signal_connect ( G_OBJECT(mnu_item), "activate", G_CALLBACK(OnItemSelect), this );
	
		GtkMenuShell * parent = 0;
		if (parent_id.empty()) {
			parent = GTK_MENU_SHELL(widget);
		}
		else {
			GtkMenuItem * parent_item = items_map[parent_id];
			if (parent_item == 0) {
				gtk_widget_destroy(GTK_WIDGET(mnu_item));
				return;
			}
			parent = GTK_MENU_SHELL( gtk_menu_item_get_submenu(parent_item) );
			if (!parent) {
				parent = GTK_MENU_SHELL(gtk_menu_new());
				gtk_menu_item_set_submenu(parent_item, GTK_WIDGET(parent));
			}
		}
		items_map[id] = mnu_item;
		gtk_menu_shell_append( parent, GTK_WIDGET(mnu_item) );
		gtk_widget_show( GTK_WIDGET(mnu_item) );
	}
	
	std::pair<Menu::MenuItemMap::iterator, GtkMenuItem *> Menu::findMenuItem(std::string const &name)
	{
		MenuItemMap::iterator e = items_map.find(name);
		if (e == items_map.end()) 
			return std::make_pair(items_map.end(), (GtkMenuItem*)0);
		else
			return std::make_pair(e, e->second);
	}
	
	void Menu::delItem( std::string const &id)
	{
		std::pair<MenuItemMap::iterator, GtkMenuItem *> e = findMenuItem(id);
		items_map.erase(e.first);
		gtk_widget_destroy(GTK_WIDGET(e.second));
	}
	
	void Menu::popupMenu(std::string const &id)
	{
		std::pair<MenuItemMap::iterator, GtkMenuItem *> e = findMenuItem(id);
		if (e.second) {
			GtkMenu * mnu = GTK_MENU(gtk_menu_item_get_submenu(e.second));
			if (mnu) gtk_menu_popup ( mnu, NULL, NULL, NULL, NULL, 0, gtk_get_current_event_time());
		}
	}
	
	void Menu::enableItem(std::string const &id)
	{
		std::pair<MenuItemMap::iterator, GtkMenuItem *> e = findMenuItem(id);
		if (e.second)
			gtk_widget_set_sensitive ( GTK_WIDGET(e.second), TRUE );
	}
	
	void Menu::disableItem(std::string const &id)
	{
		std::pair<MenuItemMap::iterator, GtkMenuItem *> e = findMenuItem(id);
		if (e.second)
			gtk_widget_set_sensitive ( GTK_WIDGET(e.second), FALSE );
	}
	
	bool Menu::isItemEnabled(std::string const &id)
	{
		std::pair<MenuItemMap::iterator, GtkMenuItem *> e = findMenuItem(id);
		if (e.second)
			return gtk_widget_is_sensitive( GTK_WIDGET(e.second) );
		else
			return false;
	}
	
	void Menu::checkItem(std::string const &id)
	{
		std::pair<MenuItemMap::iterator, GtkMenuItem *> e = findMenuItem(id);
		if ( (e.second) && ( (GTK_IS_CHECK_MENU_ITEM(e.second)) || (GTK_IS_RADIO_MENU_ITEM(e.second)) ) )
			gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM(e.second), TRUE);
	}
	
	void Menu::uncheckItem(std::string const &id)
	{
		std::pair<MenuItemMap::iterator, GtkMenuItem *> e = findMenuItem(id);
		if ( (e.second) && ( (GTK_IS_CHECK_MENU_ITEM(e.second)) || (GTK_IS_RADIO_MENU_ITEM(e.second)) ) )
			gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM(e.second), FALSE);
	}
	
	bool Menu::isItemChecked(std::string const &id)
	{
		std::pair<MenuItemMap::iterator, GtkMenuItem *> e = findMenuItem(id);
		if ( (e.second) && ( (GTK_IS_CHECK_MENU_ITEM(e.second)) || (GTK_IS_RADIO_MENU_ITEM(e.second)) ) )
			return gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(e.second) );
		else
			return false;
	}
	
	void Menu::setItemText(std::string const &id, std::string const &text)
	{
		std::pair<MenuItemMap::iterator, GtkMenuItem *> e = findMenuItem(id);
		if (e.second) {
			GtkLabel * lbl = GTK_LABEL( gtk_bin_get_child(GTK_BIN(e.second)) );
			gtk_label_set_text(lbl, text.c_str());
		}
	}
	
	std::string Menu::getItemText(std::string const &id)
	{
		std::pair<MenuItemMap::iterator, GtkMenuItem *> e = findMenuItem(id);
		if (e.second) {
			GtkLabel * lbl = GTK_LABEL( gtk_bin_get_child(GTK_BIN(e.second)) );
			return gtk_label_get_text(lbl);
		}
		else
			return "";
	}

	void Menu::OnItemSelect(GtkMenuItem *item, Menu * w)
	{
		std::string &item_name = *(std::string*)g_object_get_data(G_OBJECT(item), "xgui-mnuitem-id");
		int item_state = 1;
	
		if ( (gtk_check_menu_item_get_type() == G_OBJECT_TYPE(item)) || (gtk_radio_menu_item_get_type() == G_OBJECT_TYPE(item)) )
			item_state = gtk_check_menu_item_get_active ( GTK_CHECK_MENU_ITEM(item) );
	
		if ( !(gtk_menu_item_get_submenu(item)) ) {
			xgui::Callback * base_cb = w->this_menu->getEvent("onselect");
			if (!base_cb) return;
			
			xgui::TextStatusCallback * cb = dynamic_cast<xgui::TextStatusCallback *>(base_cb);
			if (!cb) {
				DMESSAGE("onselect event of xgui::Menu expected a TextStatusCallback");
				return;
			}
	
			cb->call ( w->this_menu, item_name, item_state );
		}
	}
}
