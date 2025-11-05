//
// C++ Interface: slider
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_SLIDER_H_
#define _XGUI_SLIDER_H_

#include "xguifwd.h"
#include "widget.h"

namespace xguimpl { struct Slider; }

namespace xgui
{
	struct SliderClass : public xgui::WidgetClass
	{
		DLLEXPORT SliderClass();
		DLLEXPORT std::string const &className();
		DLLEXPORT bool isInstanceable();
		DLLEXPORT xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);
	};

	class Slider : public xgui::Widget
	{
		protected:
			xguimpl::Slider * slider_impl_;

			DLLEXPORT Slider ( xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::Slider * widget );
			DLLEXPORT Slider ( xgui::Container * parent, int min, int max, bool vertical );
			DLLEXPORT void init();

			DLLEXPORT virtual ~Slider();

		public:
			DLLEXPORT int getPos();
			DLLEXPORT int setPos ( int pos );

		friend class Master;
		friend struct xguimpl::Slider;
	};
}

#endif
