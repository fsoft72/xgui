//
// C++ Interface: calendar
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_CALENDAR_H_
#define _XGUI_CALENDAR_H_

#include "xguifwd.h"
#include "widget.h"

namespace xguimpl { struct Calendar; }

namespace xgui
{
	struct CalendarClass : public xgui::WidgetClass
	{
		DLLEXPORT CalendarClass();
		DLLEXPORT std::string const &className();
		DLLEXPORT bool isInstanceable();
		DLLEXPORT xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);
	};

	class Calendar : public xgui::Widget
	{
		protected:
			xguimpl::Calendar * calendar_impl_;

			DLLEXPORT Calendar(xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::Calendar * widget);
			DLLEXPORT Calendar(xgui::Container * parent);
			DLLEXPORT void init();

			DLLEXPORT virtual ~Calendar();

		friend class Master;
		friend struct xguimpl::Calendar;
	};
}

#endif
