//
// C++ Interface: label
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_LABEL_H_
#define _XGUI_LABEL_H_

#include "xguifwd.h"
#include "widget.h"

namespace xguimpl { struct Label; }

namespace xgui
{
	struct LabelClass : public xgui::WidgetClass
	{
		DLLEXPORT LabelClass();
		DLLEXPORT std::string const &className();
		DLLEXPORT bool isInstanceable();
		DLLEXPORT xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);
	};

	class Label : public xgui::Widget
	{
		protected:
			xguimpl::Label * label_impl_;

			DLLEXPORT Label ( xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::Label * widget );
			DLLEXPORT Label ( xgui::Container * parent, xgui::ClassInfo * cinfo, std::string const &text );
			DLLEXPORT Label ( xgui::Container * parent, std::string const &text );
			DLLEXPORT void init();

			DLLEXPORT virtual ~Label();

		friend class Master;
		friend struct xguimpl::Label;
	};
}

#endif

