//
// C++ Interface: list
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_DNDLIST_H_
#define _XGUI_DNDLIST_H_

#include "xguifwd.h"
#include "list.h"

namespace xgui
{
	class DndList : public xgui::List
	{
		protected:
			DLLEXPORT void init();
			DLLEXPORT virtual ~DndList();

		public:
			DLLEXPORT DndList(xgui::Container * parent);

		//friend class Master;
		//friend struct xguimpl::List;
	};
}

#endif
