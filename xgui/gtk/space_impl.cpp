//
// C++ Implementation: space_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "space_impl.h"
#include "space.h"

namespace xguimpl
{
	Space::Space ( GtkWidget * real_w ) : Widget ( real_w ) { }
	Space::Space ( xgui::Container * parent, int min_width, int min_height ) : Widget ( gtk_fixed_new() )
	{
		gtk_widget_set_size_request ( widget, min_width, min_height );
	}

	Space::~Space() { }
}
