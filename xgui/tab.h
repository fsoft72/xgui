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

#ifndef _XGUI_TAB_H_
#define _XGUI_TAB_H_

#include "xguifwd.h"
#include "container.h"

namespace xguimpl { struct Tab; }

namespace xgui
{
	struct TabClass : public xgui::ContainerClass
	{
		DLLEXPORT TabClass();
		DLLEXPORT std::string const &className();
		DLLEXPORT bool isInstanceable();
		DLLEXPORT xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);

		DLLEXPORT virtual bool canContain(xgui::ClassInfo * c);
	};

	class Tab : public xgui::Container
	{
		protected:
			std::map< xgui::Widget *, std::pair<xgui::Image*, std::string> > child_props_;
			xguimpl::Tab * tab_impl_;

			DLLEXPORT Tab(xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::Tab * widget);
			DLLEXPORT Tab(xgui::Container * parent, const std::string & alignment);
			DLLEXPORT virtual ~Tab();

			DLLEXPORT virtual void addChild(xgui::Widget * w);
			DLLEXPORT virtual void delChild(xgui::Widget * w);

		public:
			DLLEXPORT int setPageIcon(xgui::Widget *, xgui::Image * icon);
			DLLEXPORT int setPageLabel(xgui::Widget *, std::string const &str);

			DLLEXPORT xgui::Image * getPageIcon(xgui::Widget * w);
			DLLEXPORT std::string const &getPageLabel(xgui::Widget * w);

			DLLEXPORT virtual std::string dump();

		friend class Master;
		friend struct xguimpl::Tab;
	};
}

#endif
