//
// C++ Interface: view
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_VIEW_H_
#define _XGUI_VIEW_H_

#include "xguifwd.h"
#include "widget.h"

namespace xguimpl { struct View; }

namespace xgui 
{
	struct ViewClass : public xgui::WidgetClass
	{
		ViewClass();
		std::string const &className();
	};

	class Model;

	class View : public xgui::Widget
	{
		private:
			View(xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::View * widget);
			virtual ~View();

		public:
			void setModel ( xgui::Model * model );
			xgui::Model * getModel();

			void refresh();

			bool isSelected(xgui::Model * parent, int child_pos);
			void select(xgui::Model * parent, int child_pos, bool state);
			void selectAll(xgui::Model * parent, bool state);
	};

}

#endif
