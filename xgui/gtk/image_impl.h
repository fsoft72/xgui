//
// C++ Interface: image_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_IMAGE_IMPL_H_
#define _XGUI_IMAGE_IMPL_H_

#include "base.h"
#include <gtk/gtk.h>

namespace xgui { class Image; }

namespace xguimpl
{
	struct Image
	{
		xgui::Image *this_image;
		GdkTexture *image;

		Image(std::string const &path);
		virtual ~Image();

		/** Creates a new widget showing the image at its natural size. */
		GtkWidget *getImage();
		GdkTexture *getTexture();
		void setAccessor(xgui::Image * accessor);
		bool isValid();
	};
}

#endif
