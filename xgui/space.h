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

#ifndef _XGUI_SPACE_H_
#define _XGUI_SPACE_H_

#include "xguifwd.h"
#include "widget.h"

namespace xguimpl { struct Space; }

namespace xgui
{
	struct SpaceClass : public xgui::WidgetClass
	{
		DLLEXPORT SpaceClass();
		DLLEXPORT std::string const &className();
		DLLEXPORT bool isInstanceable();
		DLLEXPORT xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);
	};

	class Space : public xgui::Widget
	{
		protected:
			xguimpl::Space * space_impl_;

			DLLEXPORT Space ( xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::Space * widget );
			DLLEXPORT Space ( xgui::Container * parent, int min_width, int min_height );
			DLLEXPORT void init();

			DLLEXPORT virtual ~Space();

		friend class Master;
		friend struct xguimpl::Space;
	};
}

#endif

