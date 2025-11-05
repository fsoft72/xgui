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

#ifndef _XGUI_HBOX_H_
#define _XGUI_HBOX_H_

#include "container.h"

namespace xguimpl { struct HBox; }

namespace xgui
{
	struct HBoxClass : public xgui::ContainerClass
	{
		DLLEXPORT HBoxClass();
		DLLEXPORT std::string const &className();
		DLLEXPORT bool isInstanceable();
		DLLEXPORT xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);

		DLLEXPORT virtual bool canContain(xgui::ClassInfo * c);
	};

	class HBox : public xgui::Container
	{
		protected:
			std::map<xgui::Widget *, float> expansions_;
			xguimpl::HBox * hbox_impl_;

			DLLEXPORT HBox(xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::HBox * widget);
			DLLEXPORT HBox(xgui::Container * parent, int space, bool border);
			DLLEXPORT virtual ~HBox();

			DLLEXPORT virtual void addChild(xgui::Widget * w);

		public:
			DLLEXPORT int setExpand(xgui::Widget * w, float expand);
			DLLEXPORT float getExpand(xgui::Widget * w);

			DLLEXPORT std::string dump();

		friend class Master;
		friend struct xguimpl::HBox;
	};
}


#endif
