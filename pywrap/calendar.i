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

namespace xgui
{
	struct CalendarClass : public xgui::WidgetClass
	{
		CalendarClass();
		std::string const &className();
		bool isInstanceable();
		xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);
	};

	class Calendar : public xgui::Widget
	{
		protected:
			Calendar(xgui::Container * parent);
			virtual ~Calendar();
	};
}

%extend xgui::Calendar {
	%pythoncode {
		def __del__(self):
			if self.valid: self.delPyOwner(self)
	}
}

