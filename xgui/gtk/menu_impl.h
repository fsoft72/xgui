//
// C++ Interface: menu_impl
//
// Description:
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_MENU_IMPL_H_
#define _XGUI_MENU_IMPL_H_

#include "widget_impl.h"

#include <map>
#include <vector>

namespace xgui { class Menu; class Image; class Window; }

namespace xguimpl
{
	/** A menu item: a GMenuItem inside a GMenu, driven by an action of the menu action group. */
	struct MenuItemInfo
	{
		MenuItemInfo() : container(0), submenu(0), section(0), action(0), icon(0) {}

		/** The GMenu (root menu or section of a submenu) holding the item. */
		GMenu * container;
		/** Children of the item, NULL until the first child is added. */
		GMenu * submenu;
		/** Section of the submenu new children are appended to. */
		GMenu * section;
		/** Action of a plain or check item, or the group action of a radio item. */
		GSimpleAction * action;
		/** "" (plain), "check", "radio" or "separator". */
		std::string type;
		/** Label with the mnemonic marked by '_'. */
		std::string label;
		xgui::Image * icon;
	};

	struct Menu : public Widget
	{
		typedef std::map<std::string, MenuItemInfo> MenuItemMap;

		xgui::Menu * this_menu;
		MenuItemMap items_map;

		/** Root model shown by the menu bar. */
		GMenu * root;
		/** Actions of the items, inserted in the window under prefix. */
		GSimpleActionGroup * actions;
		std::string prefix;
		int action_counter;
		/** Item id of every plain or check action, by action name. */
		std::map<std::string, std::string> action_ids;
		/** GMenu objects owned by this menu (submenus and sections). */
		std::vector<GMenu *> owned_menus;

		/** Radio group being built between "radio_begin" and "radio_end". */
		std::string radio_parent;
		GSimpleAction * radio_action;

		GtkWidget * window_widget;
		GtkWidget * popup;

		Menu ( GtkWidget * real_w );
		Menu ( xgui::Window * parent, bool visible );
		virtual ~Menu();

		virtual void setAccessor(xgui::Widget * w);
		bool linkEvent( std::string const &name );

		void addItem(std::string const &parent_id, std::string const &id, std::string const &text, xgui::Image * icon = 0, std::string const & type = "" );
		void delItem(std::string const &id);

		void popupMenu(std::string const &id);

		void enableItem(std::string const &id);
		void disableItem(std::string const &id);
		bool isItemEnabled(std::string const &id);

		void checkItem(std::string const &id);
		void uncheckItem(std::string const &id);
		bool isItemChecked(std::string const &id);

		void setItemText(std::string const &id, std::string const &text);
		std::string getItemText(std::string const &id);

		/** Returns the name (without prefix) of the action of an item, or "" when it has none. */
		std::string actionName(std::string const &id);

		/** Creates a new uniquely named action and adds it to the action group. */
		GSimpleAction * newAction(GVariantType const * parameter, GVariant * state);
		/** Builds the immutable GMenuItem describing an item. */
		GMenuItem * buildItem(std::string const &id, MenuItemInfo const &info);
		/** Returns the position of an item in its container, or -1. */
		int findPosition(std::string const &id, MenuItemInfo const &info);
		/** Replaces the GMenuItem of an item after its description changed. */
		void rebuildItem(std::string const &id);
		/** Calls the onselect event for an item. */
		void notifySelect(std::string const &id, int state);

		static void OnActionActivate(GSimpleAction * action, GVariant * parameter, Menu * w);
		static void OnRadioActivate(GSimpleAction * action, GVariant * parameter, Menu * w);
		static void OnPopupClosed(GtkPopover * popover, Menu * w);
	};
}

#endif
