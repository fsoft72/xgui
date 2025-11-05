//
// C++ Interface: space
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
	struct SpaceClass : public xgui::WidgetClass
	{
		SpaceClass();
		std::string const &className();
		bool isInstanceable();
		xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);
	};

	class Space : public xgui::Widget
	{
		protected:
			Space ( xgui::Container * parent, int min_width, int min_height );
			virtual ~Space();
	};
}

%extend xgui::Space {
	%pythoncode {
		def __del__(self):
			if self.valid: self.delPyOwner(self)
	}
}


