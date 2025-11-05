//
// C++ Interface: vbox
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
	struct VBoxClass : public xgui::ContainerClass
	{
		VBoxClass();
		std::string const &className();
		bool isInstanceable();
		xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);

		virtual bool canContain(xgui::ClassInfo * c);
	};

	class VBox : public xgui::Container
	{
		private:
			VBox(xgui::Container * parent, int space, bool border);
			virtual ~VBox();

		public:
			void setExpand(xgui::Widget * w, float expand);
			float getExpand(xgui::Widget * w);
	};
}

%extend xgui::VBox {
	%pythoncode {
		def __del__(self):
			if self.valid: self.delPyOwner(self)
	}
}

