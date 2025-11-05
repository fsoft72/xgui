//
// C++ Implementation: progressbar_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "progressbar_impl.h"
#include "callback.h"
#include "progressbar.h"

namespace xguimpl
{
	Progressbar::Progressbar ( GtkWidget * real_w ) : Widget ( real_w ) {}
	Progressbar::Progressbar ( xgui::Container * parent, int min, int max ) : Widget ( gtk_progress_bar_new () ) { }
	Progressbar::~Progressbar() {}

	bool Progressbar::linkEvent( std::string const &name )
	{
		if ( name == "onchange" ) {
			return true;
		}
	
		return Widget::linkEvent(name);
	}

	void Progressbar::OnChange ( GtkWidget * w, Progressbar * pbar )
	{
		xgui::Callback * base_cb = pbar->this_widget->getEvent("onchange");
		if (!base_cb) return;
		
		xgui::TextCallback * cb = dynamic_cast<xgui::TextCallback *>(base_cb);
		if (!cb) {
			DMESSAGE("onchange event of xgui::Progressbar expected a TextCallback");
			return;
		}
	
		xgui::Progressbar * this_pbar = dynamic_cast<xgui::Progressbar*>(pbar->this_widget);
		cb->call( pbar->this_widget, xgui::semantic_cast<std::string> (this_pbar->getPos()) );
	}

	void Progressbar::setState (int min, int max, int pos)
	{
		double rel_pos = ((double)( pos - min )) / ((double)( max - min ));
		gtk_progress_bar_set_fraction ( GTK_PROGRESS_BAR (widget), rel_pos );
		OnChange(widget, this);
	}
}
