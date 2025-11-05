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

#include "precomp.h"

namespace xguimpl
{
	Menu::Menu ( HMENU real_m ) : Widget ( (HWND)real_m ), _last_id(0)
	{
		h_events["onselect"] = OnSelect;
	}

	Menu::Menu ( xgui::Window * parent, bool visible ) :
		Widget ( (HWND)CreateMenu() ), _last_id(0), _visible(visible)
	{
		parent;

		h_events["onselect"] = OnSelect;
	}


	Menu::~Menu()
	{
	}


	void Menu::addItem ( std::string const &parent_id, std::string const &id, std::string const &text,
        xgui::Image * icon, std::string const & type )
	{
		HMENU hMenu;
		bool bRadio = false;
		static bool is_first = false;

		if ( type == "radio_begin" ) {
			_radio_parent = parent_id;
			is_first = true;
			return;
		} else if ( type == "radio_end" ) {
			_radio_parent = "";
			is_first = false;
			return;
		}

		if ( !_radio_parent.empty() ) {
			if ( parent_id == _radio_parent ) bRadio = true;
			else return;
		}
		
		if ( parent_id.empty() ) hMenu = (HMENU)widget;
		else {

			int parent_pid = _menu_map.get_id ( parent_id );

			HMENU hParent = _parent_map[parent_id];
			MENUITEMINFO mii;
			memset(&mii, 0, sizeof(mii));
			mii.cbSize = sizeof(mii);
			mii.fMask = MIIM_SUBMENU;
			GetMenuItemInfo ( hParent, parent_pid, FALSE, &mii );

			if ( mii.hSubMenu ) {
				hMenu = mii.hSubMenu;
			} else {
				mii.fMask = MIIM_SUBMENU;
				mii.hSubMenu = hMenu = CreatePopupMenu();
				SetMenuItemInfo ( hParent, parent_pid, FALSE, &mii );
			}
				
		}

		int internal_id = _last_id++;

		MENUITEMINFO mii;
		memset(&mii, 0, sizeof(mii));
		mii.cbSize = sizeof(mii);

		mii.fMask = MIIM_FTYPE | MIIM_ID | MIIM_DATA | MIIM_STRING | (icon ? MIIM_BITMAP : 0);
		if ( type == "separator" )
			mii.fType = MFT_SEPARATOR;
		else
			mii.fType = MFT_STRING;
		if ( bRadio ) {
			mii.fType |= MFT_RADIOCHECK;
			mii.dwItemData = 'R';
		} else if ( type == "check" )
			mii.dwItemData = 'C';

		mii.dwTypeData = (LPSTR)text.c_str();
		mii.wID = internal_id;
		mii.cch = text.length();
		if ( icon ) {
			HBITMAP hbmp = icon->getImpl()->getButtonBitmap();
			mii.hbmpItem = hbmp;
			//DeleteObject ( hbmp );
		}

		InsertMenuItem ( hMenu, (UINT)-1, TRUE, &mii );

		DrawMenuBar ( this_widget->getParent()->getImpl()->widget );

		_parent_map[id] = hMenu;
		_menu_map.set ( internal_id, id );

		if ( is_first && !_radio_parent.empty() && (type != "radio_begin") ) {
			checkItem ( id );
			is_first = false;
		}
	}


	void Menu::delItem( std::string const &id )
	{
		HMENU hMenu = _parent_map[id];
		int mid = _menu_map.get_id ( id );

		MENUITEMINFO mii;
		memset(&mii, 0, sizeof(mii));
		mii.cbSize = sizeof(mii);
		mii.fMask = MIIM_SUBMENU;
		GetMenuItemInfo ( hMenu, mid, FALSE, &mii );

		HMENU hSub = mii.hSubMenu;
		if ( hSub ) {
			
			int count = GetMenuItemCount ( hSub );
			MENUITEMINFO mii2;
			memset(&mii2, 0, sizeof(mii2));
			mii2.cbSize = sizeof(mii2);
			mii2.fMask = MIIM_ID;
			for ( int i = 0; i < count; ++i ) {
				GetMenuItemInfo ( hSub, i, TRUE, &mii2 );
				std::string id = _menu_map.get_string ( mii2.wID );
				delItem ( id );
			}

		}

		DeleteMenu ( hMenu, mid, MF_BYCOMMAND );
		_menu_map.remove ( id );
		_parent_map.erase ( id );
	}


	void Menu::popupMenu(std::string const &id)
	{
		HMENU hMenu = _parent_map[id];
		int mid = _menu_map.get_id ( id );

		MENUITEMINFO mii;
		memset(&mii, 0, sizeof(mii));
		mii.cbSize = sizeof(mii);
		mii.fMask = MIIM_SUBMENU;
		GetMenuItemInfo ( hMenu, mid, FALSE, &mii );

		HMENU hPopupMenu = mii.hSubMenu;
		if ( !hPopupMenu ) return;

		POINT pt;
		GetCursorPos ( &pt );

		HWND hWnd = this_widget->getParent()->getImpl()->widget;

		SetForegroundWindow ( hWnd );
		TrackPopupMenu ( hPopupMenu, 0, pt.x, pt.y, 0, hWnd, NULL );
		PostMessage ( hWnd, WM_NULL, 0, 0 );
	}


	void Menu::enableItem(std::string const &id)
	{
		HMENU hMenu = _parent_map[id];
		int mid = _menu_map.get_id ( id );

		EnableMenuItem ( hMenu, mid, MF_ENABLED | MF_BYCOMMAND );
	}


	void Menu::disableItem(std::string const &id)
	{
		HMENU hMenu = _parent_map[id];
		int mid = _menu_map.get_id ( id );

		EnableMenuItem ( hMenu, mid, MF_GRAYED | MF_BYCOMMAND );
	}


	bool Menu::isItemEnabled(std::string const &id)
	{
		HMENU hMenu = _parent_map[id];
		int mid = _menu_map.get_id ( id );

		MENUITEMINFO mii;
		memset(&mii, 0, sizeof(mii));
		mii.cbSize = sizeof(mii);
		mii.fMask = MIIM_STATE;
		GetMenuItemInfo ( hMenu, mid, FALSE, &mii );

		return (mii.fState & MFS_GRAYED) != MFS_GRAYED;
	}


	void Menu::checkItem(std::string const &id)
	{
		HMENU hMenu = _parent_map[id];
		int mid = _menu_map.get_id ( id );

		CheckMenuItem ( hMenu, mid, MF_CHECKED | MF_BYCOMMAND );
	}


	void Menu::uncheckItem(std::string const &id)
	{
		HMENU hMenu = _parent_map[id];
		int mid = _menu_map.get_id ( id );

		CheckMenuItem ( hMenu, mid, MF_UNCHECKED | MF_BYCOMMAND );
	}


	bool Menu::isItemChecked(std::string const &id)
	{
		HMENU hMenu = _parent_map[id];
		int mid = _menu_map.get_id ( id );

		MENUITEMINFO mii;
		memset(&mii, 0, sizeof(mii));
		mii.cbSize = sizeof(mii);
		mii.fMask = MIIM_STATE;
		GetMenuItemInfo ( hMenu, mid, FALSE, &mii );

		return (mii.fState & MFS_CHECKED) == MFS_CHECKED;
	}


	void Menu::setItemText(std::string const &id, std::string const &text)
	{
		HMENU hMenu = _parent_map[id];
		int mid = _menu_map.get_id ( id );

		MENUITEMINFO mii;
		memset(&mii, 0, sizeof(mii));
		mii.cbSize = sizeof(mii);
		mii.fMask = MIIM_STRING;
		mii.dwTypeData = (LPSTR)text.c_str();
		mii.cch = text.length();

		SetMenuItemInfo ( hMenu, mid, FALSE, &mii );
	}


	std::string Menu::getItemText(std::string const &id)
	{
		HMENU hMenu = _parent_map[id];
		int mid = _menu_map.get_id ( id );

		char buf[1024] = {0};

		MENUITEMINFO mii;
		memset(&mii, 0, sizeof(mii));
		mii.cbSize = sizeof(mii);
		mii.fMask = MIIM_STRING;
		mii.dwTypeData = (LPSTR)buf;
		mii.cch = sizeof(buf) - 1;

		GetMenuItemInfo ( hMenu, mid, FALSE, &mii );

		return mii.dwTypeData;
	}


	int Menu::OnSelect ( xgui::Menu * m, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		hWnd; msg; lParam;

		Menu * mnu = dynamic_cast<Menu *>(m->getImpl());

		xgui::TextStatusCallback  * cb = dynamic_cast<xgui::TextStatusCallback *>(m->getEvent("onselect"));

		int id = LOWORD(wParam);
		std::string menu_id = mnu->_menu_map.get_string ( id );

		HMENU hMenu = mnu->_parent_map[menu_id];
		int status = 1;

		MENUITEMINFO mii;
		memset(&mii, 0, sizeof(mii));
		mii.cbSize = sizeof(mii);
		mii.fMask = MIIM_DATA | MIIM_STATE;
		GetMenuItemInfo ( hMenu, id, FALSE, &mii );

		if ( mii.dwItemData == 'C' ) {

			if ( (mii.fState & MFS_CHECKED) == MFS_CHECKED ) {
				mii.fState &= ~MFS_CHECKED;
				status = 0;
			} else
				mii.fState |= MFS_CHECKED;

			SetMenuItemInfo ( hMenu, id, FALSE, &mii );

		} else if ( mii.dwItemData == 'R' ) {

			if ( (mii.fState & MFS_CHECKED) == MFS_CHECKED ) return 0;
			mii.fState = MFS_CHECKED;
			mii.fMask = MIIM_STATE;
			SetMenuItemInfo ( hMenu, id, FALSE, &mii );

			//get menu item position
			int pos, count = GetMenuItemCount ( hMenu );
			for ( pos = 0; pos < count; ++pos )
				if ( GetMenuItemID ( hMenu, pos ) == (UINT)id ) break;
			if ( pos == count ) return 0;

			//send uncheck to the currently checked radio item (if any)
			MENUITEMINFO mii2;
			memset(&mii2, 0, sizeof(mii2));
			mii2.cbSize = sizeof(mii2);
			mii2.fMask = MIIM_DATA | MIIM_STATE | MIIM_ID;

			for ( int i = pos - 1; i >= 0; --i ) {
				
				GetMenuItemInfo ( hMenu, i, TRUE, &mii2 );
				if ( mii2.dwItemData != 'R' ) break;

				if ( (mii2.fState & MFS_CHECKED) == MFS_CHECKED ) {

					mii2.fState &= ~MFS_CHECKED;
					mii2.fMask = MIIM_STATE;
					SetMenuItemInfo ( hMenu, i, TRUE, &mii2 );
					mii2.fMask = MIIM_DATA | MIIM_STATE | MIIM_ID;

					if ( cb )
						cb->call ( dynamic_cast<xgui::Widget *>(mnu), mnu->_menu_map.get_string ( mii2.wID ), 0 );

				}

			}

			for ( int i = pos + 1; i < count; ++i ) {

				GetMenuItemInfo ( hMenu, i, TRUE, &mii2 );
				if ( mii2.dwItemData != 'R' ) break;

				if ( (mii2.fState & MFS_CHECKED) == MFS_CHECKED ) {

					mii2.fState &= ~MFS_CHECKED;
					mii2.fMask = MIIM_STATE;
					SetMenuItemInfo ( hMenu, i, TRUE, &mii2 );
					mii2.fMask = MIIM_DATA | MIIM_STATE | MIIM_ID;

					if ( cb )
						cb->call ( dynamic_cast<xgui::Widget *>(mnu), mnu->_menu_map.get_string ( mii2.wID ), 0 );

				}

			}

		}

		if ( cb )
			cb->call ( dynamic_cast<xgui::Widget *>(mnu), menu_id, status );

		return 0;
	}


	bool Menu::linkEvent( std::string const &name )
	{
		if ( name == "onselect" )
			return true;
		else
			return Widget::linkEvent(name);
	}
}
