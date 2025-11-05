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
	};
}

#endif
