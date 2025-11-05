//
// C++ Interface: hbox
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
	struct HBoxClass : public xgui::ContainerClass
	{
		HBoxClass();
		std::string const &className();
		bool isInstanceable();
		xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);

		virtual bool canContain(xgui::ClassInfo * c);
	};

	class HBox : public xgui::Container
	{
		protected:
			HBox(xgui::Container * parent, int space, bool border);
			virtual ~HBox();

		public:
			void setExpand(xgui::Widget * w, float expand);
			float getExpand(xgui::Widget * w);
	};
}

%extend xgui::HBox {
	%pythoncode {
		def __del__(self):
			if self.valid: self.delPyOwner(self)
	}
}

