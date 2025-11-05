//
// C++ Interface: checkbox
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_CHECKBOX_H_
#define _XGUI_CHECKBOX_H_

#include "xguifwd.h"
#include "widget.h"

namespace xguimpl { struct Checkbox; }

namespace xgui
{
	struct CheckboxClass : public xgui::WidgetClass
	{
		DLLEXPORT CheckboxClass();
		DLLEXPORT std::string const &className();
		DLLEXPORT bool isInstanceable();
		DLLEXPORT xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);
	};

	class Checkbox : public xgui::Widget
	{
		protected:
			std::string value_;

			xguimpl::Checkbox * checkbox_impl_;

			DLLEXPORT Checkbox(xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::Checkbox * widget);
			DLLEXPORT Checkbox(xgui::Container * parent, std::string const &text);
			DLLEXPORT void init();
			DLLEXPORT virtual ~Checkbox();

		public:
			DLLEXPORT std::string const &getValue();
			DLLEXPORT int setValue(std::string const &);

		friend class Master;
		friend struct xguimpl::Checkbox;
	};
}

#endif
