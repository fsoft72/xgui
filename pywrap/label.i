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

namespace xgui
{
	struct LabelClass : public xgui::WidgetClass
	{
		  LabelClass();
		  std::string const &className();
		  bool isInstanceable();
		  xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);
	};

	class Label : public xgui::Widget
	{
		protected:
			Label ( xgui::Container * parent, std::string const &text );
			virtual ~Label();
	};
}

%extend xgui::Label {
	%pythoncode {
		def __del__(self):
			if self.valid: self.delPyOwner(self)
	}
}

