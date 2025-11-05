//
// C++ Interface: menu
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_MENU_H_
#define _XGUI_MENU_H_

#include "xguifwd.h"
#include "widget.h"
#include "window.h"

namespace xguimpl { struct Menu; struct MenuItem; }

namespace xgui
{
	class Image;

	struct MenuClass : public xgui::WidgetClass
	{
		MenuClass();
		std::string const &className();
		bool isInstanceable();
		xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);

		virtual bool canContain(xgui::ClassInfo * c);
		virtual bool canBeContainedIn(xgui::ClassInfo * c);
	};

	class Menu : public xgui::Widget
	{
		private:
			Menu & operator = ( const Menu & b );

		protected:
			xguimpl::Menu * menu_impl_;
			xguimpl::MenuItem * root_;

			Menu(xgui::Window * parent, xgui::ClassInfo * cinfo, xguimpl::Menu * widget);
			Menu(xgui::Window * parent, bool visible);
			virtual ~Menu();

		public:	
			int addItem(std::string const &parent_id, std::string const &id, std::string const &text, xgui::Image * icon = 0, std::string const & type = "" );
			int delItem(std::string const &id);

			int popupMenu(std::string const &id);

			int enableItem(std::string const &id);
			int disableItem(std::string const &id);
			bool isItemEnabled(std::string const &id);
			
			int checkItem(std::string const &id);
			int uncheckItem(std::string const &id);
			bool isItemChecked(std::string const &id);
			
			int setItemText(std::string const &id, std::string const &text);
			std::string getItemText(std::string const &id);

			DLLEXPORT virtual std::string dump();
		
		friend class Master;
		friend struct xguimpl::Menu;
	};
}

#endif
