//
// C++ Interface: entry_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_ENTRY_IMPL_H_
#define _XGUI_ENTRY_IMPL_H_

#include "widget_impl.h"

namespace xguimpl
{
	struct Entry : public Widget
	{
		int _maxlen;
		bool _editable;
		bool _password_mode;
		std::string _alignment;
		std::string _last_text;
		int _last_pos;

		//methods
		DLLEXPORT Entry ( HWND real_w );
		DLLEXPORT Entry ( xgui::Container * parent, std::string const &text, int maxlen, bool editable, bool password_mode );
		DLLEXPORT virtual ~Entry();

		DLLEXPORT static HWND MkEntry ( xgui::Container * parent, std::string const &text, int maxlen,
			bool editable, bool password_mode, const std::string & alignment, Entry * entry );

		DLLEXPORT bool linkEvent ( std::string const &name );
		DLLEXPORT char const * bindWMCOMMAND ( WORD command_id );

		DLLEXPORT bool get_entry_prop ( std::string const &name, std::string &vals );
		DLLEXPORT bool set_entry_prop ( std::string const &name, const std::string &vals );

		DLLEXPORT void appendText ( const std::string & text );
		DLLEXPORT void prependText ( const std::string & text );
		DLLEXPORT void insertText ( const std::string & text, int pos );

		DLLEXPORT static int OnSubmit ( xgui::Entry * e, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		DLLEXPORT static int OnChange ( xgui::Entry * e, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	};
}

#endif
