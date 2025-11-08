//
// C++ Interface: frame
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

%typemap(out) xgui::Widget * { $result = XGUIPyObjectCreate($1, false); }

namespace xgui
{
	struct FrameClass : public xgui::ContainerClass
	{
		FrameClass();
		std::string const &className();
		bool isInstanceable();
		xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);
	};

	class Frame : public xgui::Container
	{
		protected:
			Frame(xgui::Container * parent, const std::string & label);
			virtual ~Frame();

		public:
			xgui::Widget * getRootChild();
	};
}

%extend xgui::Frame {
	%pythoncode {
		def __del__(self):
			if self.valid: self.delPyOwner(self)
	}
}

%clear xgui::Widget *;

