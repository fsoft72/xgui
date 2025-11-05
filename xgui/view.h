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
		DLLEXPORT ViewClass();
		DLLEXPORT std::string const &className();
	};

	class Model;

	class View : public xgui::Widget
	{
		private:
			View & operator = ( const View & );

		protected:
			xguimpl::View * view_impl_;
			xgui::Model * model_;

			DLLEXPORT View(xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::View * widget);
			DLLEXPORT virtual ~View();

			//View Interaction Protocol
			DLLEXPORT void insertItem ( xgui::Model * parent, int child_pos, xgui::Model * child );
			DLLEXPORT void removeItem ( xgui::Model * parent, int child_pos, xgui::Model * child );

		public:
			DLLEXPORT int setModel ( xgui::Model * model );
			DLLEXPORT xgui::Model * getModel();
			
			DLLEXPORT int refresh();
			
			DLLEXPORT bool isSelected(xgui::Model * parent, int child_pos);
			DLLEXPORT int select(xgui::Model * parent, int child_pos, bool state);
			DLLEXPORT int selectAll(xgui::Model * parent, bool state);

		friend class Model;
	};

}

#endif
