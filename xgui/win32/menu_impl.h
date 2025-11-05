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
#include "idmap.h"

namespace xgui { class Window; }

namespace xguimpl
{
	struct Menu : public Widget
	{
		std::map<std::string, HMENU> _parent_map;
		IdMap _menu_map;
		unsigned int _last_id;
		std::string _radio_parent;
		bool _visible;

		//methods
		DLLEXPORT Menu ( HMENU real_m );
		DLLEXPORT Menu ( xgui::Window * parent, bool visible );
		virtual ~Menu();

		DLLEXPORT void addItem ( std::string const &parent_id, std::string const &id, std::string const &text, xgui::Image * icon, std::string const & type );
		DLLEXPORT void delItem ( std::string const &id );

		DLLEXPORT void popupMenu ( std::string const &id );

		DLLEXPORT void enableItem ( std::string const &id );
		DLLEXPORT void disableItem ( std::string const &id );
		DLLEXPORT bool isItemEnabled ( std::string const &id );

		DLLEXPORT void checkItem ( std::string const &id );
		DLLEXPORT void uncheckItem ( std::string const &id );
		DLLEXPORT bool isItemChecked ( std::string const &id );

		DLLEXPORT void setItemText ( std::string const &id, std::string const &text );
		DLLEXPORT std::string getItemText ( std::string const &id );

		DLLEXPORT bool linkEvent( std::string const &name );

		DLLEXPORT static int OnSelect ( xgui::Menu * mnu, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	};
}

#endif
