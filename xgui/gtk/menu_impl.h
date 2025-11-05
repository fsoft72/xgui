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

#ifndef _XGUI_BUTTON_IMPL_H_
#define _XGUI_BUTTON_IMPL_H_

#include "widget_impl.h"

namespace xgui { class Menu; class Image; }

namespace xguimpl
{
	struct Menu : public Widget
	{
		typedef std::map<std::string, GtkMenuItem *> MenuItemMap;
		xgui::Menu * this_menu;
		MenuItemMap items_map;

		Menu ( GtkWidget * real_w );
		Menu ( xgui::Window * parent, bool visible );
		virtual ~Menu();

		std::pair<MenuItemMap::iterator, GtkMenuItem *> findMenuItem(std::string const &name);

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

		static void OnItemSelect(GtkMenuItem *item, Menu * w);
	};
}

#endif
