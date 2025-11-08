//
// C++ Interface: container
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
	struct ContainerClass : public xgui::WidgetClass
	{
		ContainerClass();
		std::string const &className();
		bool isContainer();

		virtual bool canContain(xgui::ClassInfo * c);
		virtual bool canBeContainedIn(xgui::ClassInfo * c);
	};

	class Container : public xgui::Widget
	{
		protected:
			Container( xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::Widget * widget = 0);
			virtual ~Container();

		public:
			xgui::Widget * findChild(std::string const &child_name);
			xgui::Widget * findDirectChild(std::string const &child_name);
			xgui::Widget * getChild(int pos);
			unsigned int size();
	};
}

%clear xgui::Widget *;

