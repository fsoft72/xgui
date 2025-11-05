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

#include "widget.h"

#ifndef _XGUI_CONTAINER_H_
#define _XGUI_CONTAINER_H_

namespace xguimpl { struct Widget; }

namespace xgui
{
	struct ContainerClass : public xgui::WidgetClass
	{
		DLLEXPORT ContainerClass();
		DLLEXPORT std::string const &className();
		DLLEXPORT bool isContainer();

		DLLEXPORT virtual bool canContain(xgui::ClassInfo * c);
		DLLEXPORT virtual bool canBeContainedIn(xgui::ClassInfo * c);
	};

	class Container : public xgui::Widget
	{
		private:
			Container & operator = ( const Container & );

		protected:
			xgui::WidgetsVector children_;

			DLLEXPORT Container( xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::Widget * widget = 0);
			DLLEXPORT virtual ~Container();

			DLLEXPORT virtual void addChild(xgui::Widget *);
			DLLEXPORT virtual void delChild(xgui::Widget *);

		public:
			DLLEXPORT virtual std::string dump();
			DLLEXPORT xgui::Widget * findChild(std::string const &child_name);
			DLLEXPORT xgui::Widget * findDirectChild(std::string const &child_name);
			DLLEXPORT xgui::Widget * getChild(int pos);
			DLLEXPORT unsigned int size();

		friend class Widget;
	};
}

#endif
