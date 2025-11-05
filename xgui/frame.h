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

#ifndef _XGUI_FRAME_H_
#define _XGUI_FRAME_H_

#include "xguifwd.h"
#include "container.h"

namespace xguimpl { struct Frame; }

namespace xgui
{
	struct FrameClass : public xgui::ContainerClass
	{
		DLLEXPORT FrameClass();
		DLLEXPORT std::string const &className();
		DLLEXPORT bool isInstanceable();
		DLLEXPORT xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);
	};

	class Frame : public xgui::Container
	{
		protected:
			xguimpl::Frame * frame_impl_;

			DLLEXPORT Frame(xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::Frame * widget);
			DLLEXPORT Frame(xgui::Container * parent, const std::string & label);
			DLLEXPORT virtual ~Frame();

			DLLEXPORT virtual void addChild(xgui::Widget * w);
			DLLEXPORT virtual void delChild(xgui::Widget * w);

		public:
			DLLEXPORT xgui::Widget * getRootChild();

		friend class Master;
		friend struct xguimpl::Frame;
	};
}

#endif
