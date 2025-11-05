//
// C++ Implementation: view_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "view_impl.h"
#include "model.h"

namespace xguimpl
{
	View::View ( GtkWidget * real_w ) : Widget ( real_w ) {}
	View::~View() {}

	void View::addSelection ( xgui::Model * model )
	{
		DMESSAGE("Adding selection " << model->getString(0));
		selset_.insert ( model );
	}
	
	
	void View::delSelection ( xgui::Model * model )
	{
		DMESSAGE("Removing selection " << model->getString(0));
		selset_.erase ( model );
	}
	
	
	bool View::isSelectionCached ( xgui::Model * model )
	{
		std::set<xgui::Model *>::iterator iter = selset_.find ( model );
		return iter != selset_.end();
	}
	
	void View::clearSelectionsCache()
	{
		selset_.clear();
	}
}
