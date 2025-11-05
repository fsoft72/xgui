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

namespace xgui
{
	struct CheckboxClass : public xgui::WidgetClass
	{
		CheckboxClass();
		std::string const &className();
		bool isInstanceable();
		xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);
	};

	class Checkbox : public xgui::Widget
	{
		protected:
			Checkbox(xgui::Container * parent, std::string const &text);
			virtual ~Checkbox();

		public:
			std::string const &getValue();
			void setValue(std::string const &);
	};
}

%extend xgui::Checkbox {
	%pythoncode {
		def __del__(self):
			if self.valid: self.delPyOwner(self)
	}
}

