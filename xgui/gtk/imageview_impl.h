//
// C++ Interface: imageview_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef _XGUI_BUTTON_IMPL_H_
#define _XGUI_BUTTON_IMPL_H_

#include "widget_impl.h"

namespace xgui { class ImageView; class Image; }

namespace xguimpl
{
	struct ImageView : public Widget
	{
		GtkWidget * scroll_widget;
		GtkWidget * img_widget;
		xgui::ImageView * this_imgview;

		//methods
		ImageView ( GtkWidget * real_w );
		ImageView ( xgui::Container * parent );
 		void init();
		virtual ~ImageView();	

		virtual void setAccessor(xgui::Widget * w);
		void Resize(int x, int y);

		bool get_imageview_prop ( std::string const &name, std::string &vals );
		bool set_imageview_prop ( std::string const &name, const std::string &vals );

		void setImage ( xgui::Image * img );
	};
}

#endif
