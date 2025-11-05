//
// C++ Interface: widget
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_WIDGET_H_
#define _XGUI_WIDGET_H_

#include "xguifwd.h"
#include "object.h"

namespace xguimpl { struct Widget; }

namespace xgui
{
	struct WidgetClass : public xgui::ObjectClass
	{
		DLLEXPORT WidgetClass();
		DLLEXPORT bool isWidget();
		DLLEXPORT std::string const &className();

		DLLEXPORT virtual bool canContain(xgui::ClassInfo * c);
		DLLEXPORT virtual bool canBeContainedIn(xgui::ClassInfo * c);
	};

	class Widget : public xgui::Object
	{
		private:
			Widget & operator = ( const Widget & );

		protected:
			xguimpl::Widget * impl_;
			xgui::Container * parent_;
			xgui::CallbackMap events_;

			DLLEXPORT Widget(xgui::Container * parent, xgui::ClassInfo * cinfo = 0, xguimpl::Widget * i = 0);
			DLLEXPORT virtual ~Widget();

			DLLEXPORT void post_construction();

		public:
			DLLEXPORT xguimpl::Widget * getImpl() { return impl_; }

			DLLEXPORT int setAsFloating();
			DLLEXPORT xgui::Container * getParent();
			DLLEXPORT xgui::Window * getContext();
			DLLEXPORT std::string getPath();

			DLLEXPORT bool linkEvent( std::string const &name, xgui::Callback * cb );
			DLLEXPORT xgui::Callback * getEvent( std::string const &name );
			DLLEXPORT bool hasEvent ( std::string const &name );

			DLLEXPORT int recalcLayout();

			DLLEXPORT std::pair<int, int> getMousePos();
			DLLEXPORT int redraw();

		friend struct xguimpl::Widget;
	};
}

#endif
