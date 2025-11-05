//
// C++ Interface: spin
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_SPIN_H_
#define _XGUI_SPIN_H_

#include "xguifwd.h"
#include "widget.h"

namespace xguimpl { struct Spin; }

namespace xgui
{
	struct SpinClass : public xgui::WidgetClass
	{
		DLLEXPORT SpinClass();
		DLLEXPORT std::string const &className();
		DLLEXPORT bool isInstanceable();
		DLLEXPORT xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);
	};

	class Spin : public xgui::Widget
	{
		private:
			static void Spin_set(xgui::Object * o, std::string const &name, std::string const &val);
			static void Spin_get(xgui::Object * o, std::string const &name, std::string &dest);

		protected:
			int min_, max_;
			xguimpl::Spin * spin_impl_;

			DLLEXPORT Spin(xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::Spin * widget);
			DLLEXPORT Spin(xgui::Container * parent, int min, int max);
			DLLEXPORT void init();
			DLLEXPORT virtual ~Spin();

		friend class Master;
		friend struct xguimpl::Spin;
		friend struct SpinClass;
	};
}

#endif
