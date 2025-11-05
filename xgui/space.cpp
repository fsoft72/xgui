//
// C++ Implementation: space
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
#	include "space.h"
#	include "gtk/space_impl.h"
#	include "master.h"
#	include "container.h"
#endif

namespace xgui
{
	xgui::SpaceClass space_class_info;

	SpaceClass::SpaceClass() : WidgetClass() { }

	std::string const &SpaceClass::className() { static std::string const c_name = "xgui::Space"; return c_name; }

	bool SpaceClass::isInstanceable() { return true; }

	xgui::Object * SpaceClass::create(xgui::Object * parent, xgui::StringMap &properties)
	{
		xgui::StringMap::iterator prop_i;
		xgui::Container * wparent = dynamic_cast<xgui::Container*>( parent );
		xgui::Space * w = 0;

		if (!parent) {
			XGUI_ERROR("Cannot create a Space without parent");
			return 0;
		}

		if (!wparent) {
			XGUI_ERROR("Cannot create a Space as a child of a " << parent->className());
			return 0;
		}

		w = xgui::Master::CreateSpace(wparent);

		return w;
	}
}

namespace xgui
{
	void Space::init()
	{
		space_impl_ = dynamic_cast<xguimpl::Space *>(impl_);
		ASSERT(space_impl_);
	}

	Space::Space ( xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::Space * widget )
	: Widget ( parent, cinfo, widget )
	{
		init();
	}

	Space::Space ( xgui::Container * parent, int min_width, int min_height )
	: Widget ( parent, &space_class_info, new xguimpl::Space ( parent, min_width, min_height ) )
	{
		impl_->setAccessor(this);
		post_construction();
		init();
	}

	Space::~Space() 
	{
	}
}
