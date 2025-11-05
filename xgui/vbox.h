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

#ifndef _XGUI_VBOX_H_
#define _XGUI_VBOX_H_

#include "xguifwd.h"
#include "container.h"

namespace xguimpl { struct VBox; }

namespace xgui
{
	struct VBoxClass : public xgui::ContainerClass
	{
		DLLEXPORT VBoxClass();
		DLLEXPORT std::string const &className();
		DLLEXPORT bool isInstanceable();
		DLLEXPORT xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);

		DLLEXPORT virtual bool canContain(xgui::ClassInfo * c);
	};

	class VBox : public xgui::Container
	{
		protected:
			std::map<xgui::Widget *, float> expansions_;
			xguimpl::VBox * vbox_impl_;

			DLLEXPORT VBox(xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::VBox * widget);
			DLLEXPORT VBox(xgui::Container * parent, int space, bool border);
			DLLEXPORT virtual ~VBox();

			DLLEXPORT virtual void addChild(xgui::Widget * w);
	
		public:
			DLLEXPORT int setExpand(xgui::Widget * w, float expand);
			DLLEXPORT float getExpand(xgui::Widget * w);

			DLLEXPORT std::string dump();


		friend class Master;
		friend struct xguimpl::VBox;
	};
}


#endif
