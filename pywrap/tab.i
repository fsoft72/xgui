//
// C++ Interface: tab
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
	struct TabClass : public xgui::ContainerClass
	{
		TabClass();
		std::string const &className();
		bool isInstanceable();
		xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);

		virtual bool canContain(xgui::ClassInfo * c);
	};

	class Tab : public xgui::Container
	{
		protected:
			Tab(xgui::Container * parent, const std::string & alignment);
			virtual ~Tab();

		public:
			void setPageIcon(xgui::Widget *, xgui::Image * icon);
			void setPageLabel(xgui::Widget *, std::string const &str);

			xgui::Image * getPageIcon(xgui::Widget * w);
			std::string const &getPageLabel(xgui::Widget * w);
	};
}

%extend xgui::Tab {
	%pythoncode {
		def __del__(self):
			if self.valid: self.delPyOwner(self)
	}
}
