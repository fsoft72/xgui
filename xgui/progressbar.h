//
// C++ Interface: progressbar
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_PROGRESSBAR_H_
#define _XGUI_PROGRESSBAR_H_

#include "xguifwd.h"
#include "widget.h"

namespace xguimpl { struct Progressbar; }

namespace xgui
{
	struct ProgressbarClass : public xgui::WidgetClass
	{
		DLLEXPORT ProgressbarClass();
		DLLEXPORT std::string const &className();
		DLLEXPORT bool isInstanceable();
		DLLEXPORT xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);
	};

	class Progressbar : public xgui::Widget
	{
		private:
			static void Progressbar_set(xgui::Object * o, std::string const &name, std::string const &val);
			static void Progressbar_get(xgui::Object * o, std::string const &name, std::string &dest);

		protected:
			int min_, max_, pos_;
			xguimpl::Progressbar * pbar_impl_;

			DLLEXPORT Progressbar(xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::Progressbar * widget);
			DLLEXPORT Progressbar(xgui::Container * parent, int min, int max);
			DLLEXPORT void init();
			DLLEXPORT virtual ~Progressbar();

		public:
			DLLEXPORT int getPos ();
			DLLEXPORT int setPos ( int pos );

		friend class Master;
		friend struct xguimpl::Progressbar;
		friend struct ProgressbarClass;	
	};
}

#endif
