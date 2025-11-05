//
// C++ Implementation: list
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "precomp.h"

#ifndef WIN32
#	include "dnd_list.h"
#	include "gtk/dndlist_impl.h"
#	include "master.h"
#	include "dynamic.h"
#endif

namespace xgui
{
	extern xgui::ListClass list_class_info;

	void DndList::init()
	{
		list_impl_ = dynamic_cast<xguimpl::DndList *>(impl_);
		ASSERT(list_impl_);
	}

	DndList::DndList(xgui::Container * parent)
	: List ( parent, &list_class_info, new xguimpl::DndList(parent) )
	{
		impl_->setAccessor(this);
		post_construction();
		init();
	}

	DndList::~DndList() 
	{
	}
}
