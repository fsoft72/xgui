//
// C++ Interface: button
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_BUTTON_H_
#define _XGUI_BUTTON_H_

#include "xguifwd.h"
#include "widget.h"

namespace xguimpl { struct Button; }

namespace xgui
{
	struct ButtonClass : public xgui::WidgetClass
	{
		DLLEXPORT ButtonClass();
		DLLEXPORT std::string const &className();
		DLLEXPORT bool isInstanceable();

		DLLEXPORT xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);
	};

	class Button : public xgui::Widget
	{
		protected:
			xguimpl::Button * button_impl_;
			xgui::Image * icon_;

			DLLEXPORT Button(xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::Button * widget);
			DLLEXPORT Button(xgui::Container * parent, std::string const &text, xgui::Image * image, bool toggle_button);
			DLLEXPORT void init();
			DLLEXPORT virtual ~Button();

		public:
			DLLEXPORT int setIcon ( Image * img );
			DLLEXPORT xgui::Image * getIcon();

		friend class Master;
		friend struct xguimpl::Button;
	};
}

#endif
