//
// C++ Interface: spin
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
	struct SpinClass : public xgui::WidgetClass
	{
		SpinClass();
		std::string const &className();
		bool isInstanceable();
		xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);
	};

	class Spin : public xgui::Widget
	{
		protected:
			Spin(xgui::Container * parent, int min, int max);
			virtual ~Spin();
	};
}

%extend xgui::Spin {
	%pythoncode {
		def __del__(self):
			if self.valid: self.delPyOwner(self)
	}
}

