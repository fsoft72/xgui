//
// C++ Interface: slider
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
	struct SliderClass : public xgui::WidgetClass
	{
		SliderClass();
		std::string const &className();
		bool isInstanceable();
		xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);
	};

	class Slider : public xgui::Widget
	{
		protected:
			Slider ( xgui::Container * parent, int min, int max, bool vertical );
			virtual ~Slider();

		public:
			int getPos();
			void setPos ( int pos );
	};
}

%extend xgui::Slider {
	%pythoncode {
		def __del__(self):
			if self.valid: self.delPyOwner(self)
	}
}

