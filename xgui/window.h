//
// C++ Interface: window
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_WINDOW_H_
#define _XGUI_WINDOW_H_

#include "container.h"

namespace xguimpl { struct Window; }

namespace xgui
{
	class Menu;
	class Image;

	struct WindowClass : public xgui::ContainerClass
	{
		DLLEXPORT WindowClass();
		DLLEXPORT std::string const &className();
		DLLEXPORT bool isInstanceable();
		DLLEXPORT xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);

		DLLEXPORT virtual bool canBeContainedIn(xgui::ClassInfo * c);
	};

	class Window : public xgui::Container
	{
		private:
			Window &operator = ( const Window & w );

		protected:
			xguimpl::Window * window_impl_;
			bool modal_;
			xgui::Menu * menu_;
			xgui::Widget * child_;
			xgui::Image * icon_;

			DLLEXPORT Window(xgui::Window * parent, xgui::ClassInfo * cinfo, xguimpl::Window * widget);
			DLLEXPORT Window(xgui::Window * parent);
			DLLEXPORT void init();

			DLLEXPORT virtual ~Window();

			DLLEXPORT virtual void addChild(xgui::Widget * w);
			DLLEXPORT virtual void delChild(xgui::Widget * w);

		public:
			DLLEXPORT int show();
			DLLEXPORT int modalRun();
			DLLEXPORT bool isModal();
			DLLEXPORT int modalReturn(int response);

			DLLEXPORT xgui::Widget * getRootChild();
			DLLEXPORT xgui::Menu * getMenu();

			DLLEXPORT int maximize();
			DLLEXPORT int minimize();
			DLLEXPORT int restore();

			DLLEXPORT int setIcon ( Image * img );
			DLLEXPORT xgui::Image * getIcon();

		friend class Master;
		friend struct xguimpl::Window;
	};
}

#endif
