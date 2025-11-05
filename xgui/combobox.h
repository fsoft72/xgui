//
// C++ Interface: combobox
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_COMBOBOX_H_
#define _XGUI_COMBOBOX_H_

#include "xguifwd.h"
#include "view.h"

namespace xguimpl { struct Combobox; }

namespace xgui
{
	struct ComboboxClass : public xgui::ViewClass
	{
		DLLEXPORT ComboboxClass();
		DLLEXPORT bool isInstanceable();
		DLLEXPORT std::string const &className();
		DLLEXPORT xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);
	
		DLLEXPORT virtual bool canContain(xgui::ClassInfo * c);
	};

	class Combobox : public xgui::View
	{
		protected:
			bool editable_;
			xguimpl::Combobox * cbox_impl_;

			DLLEXPORT Combobox(xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::Combobox * widget);
			DLLEXPORT Combobox(xgui::Container * parent, bool editable);
			DLLEXPORT void init();

			DLLEXPORT virtual ~Combobox();

		public:
			DLLEXPORT int insertText(std::string const &text, unsigned int pos);
			DLLEXPORT int appendText(std::string const &text);
			DLLEXPORT int prependText(std::string const &text);

			DLLEXPORT int popup();
			DLLEXPORT int popdown();

			DLLEXPORT bool isEditable();
			DLLEXPORT int setAsEditable(bool editable);

		friend class Master;
		friend struct xguimpl::Combobox;
	};
}

#endif
