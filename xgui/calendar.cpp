//
// C++ Implementation: calendar
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

#ifndef WIN32
#	include "calendar.h"
#	include "gtk/calendar_impl.h"
#	include "master.h"
#	include "image.h"
#	include "container.h"
#endif

static void calendar_set(xgui::Object * o, std::string const &name, std::string const &val)
{
	xgui::Calendar * cal = dynamic_cast<xgui::Calendar*>(o);
	xguimpl::Calendar * impl = dynamic_cast<xguimpl::Calendar*>(cal->getImpl());

	impl->set_calendar_prop(name, val);
}

static void calendar_get(xgui::Object * o, std::string const &name, std::string &dest)
{
	xgui::Calendar * cal = dynamic_cast<xgui::Calendar*>(o);
	xguimpl::Calendar * impl = dynamic_cast<xguimpl::Calendar*>(cal->getImpl());

	impl->get_calendar_prop(name, dest);
}

namespace xgui
{
	xgui::CalendarClass calendar_class_info;

	CalendarClass::CalendarClass() : WidgetClass() 
	{
		registerProperty("day", calendar_set, calendar_get, false);
		registerProperty("month", calendar_set, calendar_get, false);
		registerProperty("year", calendar_set, calendar_get, false);
		registerProperty("date", calendar_set, calendar_get, true);
	}

	std::string const &CalendarClass::className() { static std::string const c_name = "xgui::Calendar"; return c_name; }

	bool CalendarClass::isInstanceable() { return true; }

	xgui::Object * CalendarClass::create(xgui::Object * parent, xgui::StringMap &properties)
	{
		xgui::StringMap::iterator prop_i;
		xgui::Container * wparent = dynamic_cast<xgui::Container*>( parent );
		xgui::Calendar * w = 0;

		if (!parent) {
			XGUI_ERROR("Cannot create a Calendar without parent");
			return 0;
		}

		if (!wparent) {
			XGUI_ERROR("Cannot create a Calendar as a child of a " << parent->className());
			return 0;
		}

		w = xgui::Master::CreateCalendar(wparent);

		return w;
	}
}

namespace xgui
{
	void Calendar::init()
	{
		calendar_impl_ = dynamic_cast<xguimpl::Calendar *>(impl_);
		ASSERT(calendar_impl_);
	}

	Calendar::Calendar(xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::Calendar * widget)
	: Widget ( parent, cinfo, widget )
	{
		init();
	}

	Calendar::Calendar(xgui::Container * parent)
	: Widget ( parent, &calendar_class_info, new xguimpl::Calendar(parent) )
	{
		impl_->setAccessor(this);
		post_construction();
		init();
	}

	Calendar::~Calendar() 
	{
	}
}
