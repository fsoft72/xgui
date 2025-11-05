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

namespace xgui
{
	struct ButtonClass : public xgui::WidgetClass
	{
		ButtonClass();
		std::string const &className();
		bool isInstanceable();

		xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);
	};

	class Button : public xgui::Widget
	{
		protected:
			Button(xgui::Container * parent, std::string const &text, xgui::Image * image, bool toggle_button);
			virtual ~Button();

		public:
			void setIcon ( Image * img );
			xgui::Image * getIcon();
	};
}

%extend xgui::Button {
	%pythoncode {
		def __del__(self):
			if self.valid: self.delPyOwner(self)
	}
}
