//
// C++ Implementation: image_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "image_impl.h"
#include "image.h"

namespace xguimpl
{
	Image::Image(std::string const &path)
	: this_image(0), image(0)
	{
		image = gdk_pixbuf_new_from_file ( path.c_str(), NULL );
	}

	Image::~Image() { if (image) g_object_unref(image); }

	void Image::setAccessor(xgui::Image * accessor) 
	{ 
		ASSERT(accessor);
		this_image = accessor;

		if (image != NULL) {
			this_image->width_ = gdk_pixbuf_get_width ( image );
			this_image->height_ = gdk_pixbuf_get_height ( image );
		}
	}

	bool Image::isValid() { return image != NULL; }

	GtkImage * Image::getImage()
	{
		return (GtkImage*)gtk_image_new_from_pixbuf(image);
	}
	
	GdkPixbuf * Image::getPixbuf()
	{
		return image;
	}

}
