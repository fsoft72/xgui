//
// C++ Interface: view_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_VIEW_IMPL_H_
#define _XGUI_VIEW_IMPL_H_

#include "widget_impl.h"

namespace xgui { class Model; }

namespace xguimpl
{
	struct View : public Widget
	{
		std::set<xgui::Model *> selset_;
		//methods
		View ( GtkWidget * real_w );
		virtual ~View();

		virtual void insertItem ( xgui::Model * parent, int child_pos, xgui::Model * child ) = 0;
		virtual void removeItem ( xgui::Model * parent, int child_pos, xgui::Model * child ) = 0;
		virtual void refresh() = 0;

		virtual bool isSelected(xgui::Model * model, int pos) = 0;
		virtual void select(xgui::Model * model, int pos, bool state) = 0;
		virtual void selectAll(xgui::Model * model, bool state) = 0;

		void addSelection ( xgui::Model * model );
		void delSelection ( xgui::Model * model );
		void clearSelectionsCache();
		bool isSelectionCached( xgui::Model * model );
	};
}

#endif
