//
// C++ Interface: entry
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_ENTRY_H_
#define _XGUI_ENTRY_H_

#include "xguifwd.h"
#include "widget.h"

namespace xguimpl { struct Entry; }

namespace xgui
{
	struct EntryClass : public xgui::WidgetClass
	{
		DLLEXPORT EntryClass();
		DLLEXPORT std::string const &className();
		DLLEXPORT bool isInstanceable();
		DLLEXPORT xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);
	};


	class Entry : public xgui::Widget
	{
		protected:
			xguimpl::Entry * entry_impl_;

			DLLEXPORT Entry ( xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::Entry * widget );
			DLLEXPORT Entry ( xgui::Container * parent, std::string const &text, int maxlen, bool editable, bool password_mode );
			DLLEXPORT void init();

			DLLEXPORT virtual ~Entry();

		public:
			DLLEXPORT int appendText ( const std::string & text );
			DLLEXPORT int prependText ( const std::string & text );
			DLLEXPORT int insertText ( const std::string & text, int pos );

		friend class Master;
		friend struct xguimpl::Entry;
	};
}

#endif
