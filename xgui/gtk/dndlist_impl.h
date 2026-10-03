//
// C++ Interface: list_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_DNDLIST_IMPL_H_
#define _XGUI_DNDLIST_IMPL_H_

#include "style.h"
#include "dnd_list.h"
#include "list_impl.h"

namespace xgui { class DndList; }

namespace xguimpl
{
	struct DndList : public List
	{
		//methods
		DndList ( xgui::Container * parent );

		/** The widget the drag source and the drop target are attached to. */
		GtkWidget * dragWidget();
		/** Gets the string dragged out of the list (first string of the selected row); false if nothing is selected. */
		bool dragData ( std::string &data );
		/** Removes the dragged row after a move. */
		void dragDelete ();
		/** Appends a dropped string as a new row; false if the list has no model. */
		bool dropData ( std::string const &data );
	};
}

#endif
