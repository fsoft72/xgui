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
		GError * error = NULL;
		image = gdk_texture_new_from_filename ( path.c_str(), &error );
		if ( error ) {
			DMESSAGE("Cannot load image " << path << ": " << error->message);
			g_error_free ( error );
		}
	}

	Image::~Image() { if (image) g_object_unref(image); }

	void Image::setAccessor(xgui::Image * accessor) 
	{ 
		ASSERT(accessor);
		this_image = accessor;

		if (image != NULL) {
			this_image->width_ = gdk_texture_get_width ( image );
			this_image->height_ = gdk_texture_get_height ( image );
		}
	}

	bool Image::isValid() { return image != NULL; }

	GtkWidget * Image::getImage()
	{
		// A GtkImage would be scaled to the icon size: a picture keeps the pixel size of the file
		GtkWidget * picture = gtk_picture_new_for_paintable ( GDK_PAINTABLE ( image ) );
		gtk_picture_set_can_shrink ( GTK_PICTURE ( picture ), FALSE );
		return picture;
	}
	
	GdkTexture * Image::getTexture()
	{
		return image;
	}

}
