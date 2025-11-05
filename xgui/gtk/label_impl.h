//
// C++ Interface: label_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_LABEL_IMPL_H_
#define _XGUI_LABEL_IMPL_H_

#include "widget_impl.h"

namespace xguimpl
{
	struct Label : public Widget
	{
		//methods
		Label( GtkWidget * real_w );
		Label ( xgui::Container * parent, std::string const &text );
		virtual ~Label();

		bool get_label_prop ( std::string const &name, std::string &vals );
		bool set_label_prop ( std::string const &name, const std::string &vals );
	};
}

#endif
