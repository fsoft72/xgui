//
// C++ Interface: combobox_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_COMBOBOX_IMPL_H_
#define _XGUI_COMBOBOX_IMPL_H_

#include "widget_impl.h"

namespace xgui { class Window; }

namespace xguimpl
{
	struct Combobox : public View
	{
		int _maxlen;
		std::string _last_text;
		int _last_pos;

		xgui::Combobox * this_cbox;

		//methods
		DLLEXPORT Combobox ( HWND real_w );
		DLLEXPORT Combobox ( xgui::Container * parent, bool editable );
		DLLEXPORT virtual ~Combobox();

		DLLEXPORT static HWND MkCombobox ( xgui::Container * parent, bool editable, Combobox * cbox );

		DLLEXPORT virtual void setAccessor ( xgui::Widget * w );

		DLLEXPORT bool linkEvent( std::string const &name );
		DLLEXPORT char const * bindWMCOMMAND( WORD command_id );

		DLLEXPORT void refresh();
		DLLEXPORT void insertItem ( xgui::Model * parent, int child_pos, xgui::Model * child );
		DLLEXPORT void removeItem ( xgui::Model * parent, int child_pos, xgui::Model * child );

		DLLEXPORT bool isSelected ( xgui::Model * parent, int child_pos );
		DLLEXPORT void select ( xgui::Model * parent, int child_pos, bool state );
		DLLEXPORT void selectAll ( xgui::Model * parent, bool state );

		DLLEXPORT void giveSize ( std::pair<int, int> size );

		DLLEXPORT void appendText ( const std::string & text );
		DLLEXPORT void prependText ( const std::string & text );
		DLLEXPORT void insertText ( const std::string & text, int pos );

		DLLEXPORT void setEntryText ( std::string const &text );
		DLLEXPORT std::string getEntryText();

		DLLEXPORT void setSelected ( int num );
		DLLEXPORT int getSelected();
		DLLEXPORT std::string getSelectedItemText();

		DLLEXPORT void setMaxLength ( int len );
		DLLEXPORT int getMaxLength();

		DLLEXPORT void setAsEditable ( bool iseditable );

		DLLEXPORT void popup();
		DLLEXPORT void popdown();

		DLLEXPORT static int OnChange ( xgui::Combobox * c, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		DLLEXPORT static int OnSelect ( xgui::Combobox * c, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		DLLEXPORT static int OnSubmit ( xgui::Combobox * c, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	};
}

#endif
