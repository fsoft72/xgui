//
// C++ Implementation: imageview_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "imageview.h"
#include "imageview_impl.h"
#include "image.h"
#include "image_impl.h"

namespace xguimpl
{
 	void ImageView::init()
	{
		gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll_widget), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
		gtk_widget_set_hexpand( img_widget, TRUE );
		gtk_widget_set_vexpand( img_widget, TRUE );
		gtk_box_append( GTK_BOX(widget), img_widget );
	
		// The scrolled window is detached while the view is not scrolled: keep it alive
		g_object_ref_sink( G_OBJECT( scroll_widget ) );
	}

	ImageView::ImageView ( GtkWidget * real_w ) 
	: Widget (gtk_box_new(GTK_ORIENTATION_VERTICAL, 0)), scroll_widget(gtk_scrolled_window_new()), img_widget(real_w) 
	{
		init();
	}

	ImageView::ImageView ( xgui::Container * parent ) 
	: Widget (gtk_box_new(GTK_ORIENTATION_VERTICAL, 0)), scroll_widget(gtk_scrolled_window_new()), img_widget(gtk_picture_new())
	{
		init();
	}

	ImageView::~ImageView() 
	{
		g_object_unref(scroll_widget);
	}

	void ImageView::setAccessor(xgui::Widget * w)
	{
		xgui::ImageView * downcasted_widget = dynamic_cast<xgui::ImageView*>(w);
		ASSERT(downcasted_widget);
		this_imgview = downcasted_widget;

		Widget::setAccessor(w);
	}

	void ImageView::Resize(int x, int y)
	{
		this_imgview->view_width_ = x;
		this_imgview->view_height_ = y;

		// The picture is stretched to the requested view size
		gtk_picture_set_can_shrink( GTK_PICTURE(img_widget), TRUE );
		gtk_picture_set_content_fit( GTK_PICTURE(img_widget), GTK_CONTENT_FIT_FILL );
		gtk_widget_set_size_request( img_widget, x, y );
	}

	void ImageView::setImage(xgui::Image * img)
	{
		gtk_picture_set_paintable( GTK_PICTURE(img_widget), img ? GDK_PAINTABLE(img->getImpl()->getTexture()) : NULL );
	}

	bool ImageView::get_imageview_prop ( std::string const &name, std::string &vals )
	{
		if ( name == "alignment" ) {
			float x, y;
			// gtk_misc deprecated - use default alignment
		x = 0.5f;
		y = 0.5f;
			vals = joinAlignment(x, y);
			return true;
		}

		return false;
	}

	bool ImageView::set_imageview_prop ( std::string const &name, const std::string &vals )
	{
		if ( name == "alignment" ) {
			std::pair<float, float> align = splitAlignment(vals);
			// gtk_misc deprecated - use halign/valign
			gtk_widget_set_halign(img_widget, toGtkAlign(align.first));
			gtk_widget_set_valign(img_widget, toGtkAlign(align.second));
			return true;
		}
		else if ( name == "view-size" ) {
			if (this_imgview->image_ && (this_imgview->scrolled_ == false)) {
				std::pair<int, int> size = xgui::semantic_cast< std::pair<int, int> >(vals);
				Resize(size.first, size.second);
			}
			return true;
		}
		else if ( name == "scrolled" ) {
			if ( vals == "1" ) {
				if (this_imgview->scrolled_ == false) {
					this_imgview->scrolled_ = true;
					g_object_ref(img_widget);
					gtk_box_remove( GTK_BOX(widget), img_widget );
					// Natural size inside the scrolled window, so that it can be scrolled
					gtk_picture_set_can_shrink( GTK_PICTURE(img_widget), FALSE );
					gtk_widget_set_size_request( img_widget, -1, -1 );
					gtk_scrolled_window_set_child( GTK_SCROLLED_WINDOW(scroll_widget), img_widget );
					gtk_widget_set_hexpand( scroll_widget, TRUE );
					gtk_widget_set_vexpand( scroll_widget, TRUE );
					gtk_box_append( GTK_BOX(widget), scroll_widget );
					g_object_unref(img_widget);
	
					setImage(this_imgview->image_);
					gtk_widget_set_size_request ( GTK_WIDGET(widget), this_imgview->view_width_, this_imgview->view_height_ );
				}
			}
			else {
				if (this_imgview->scrolled_ == true) {
					this_imgview->scrolled_ = false;
					g_object_ref(img_widget);
					gtk_scrolled_window_set_child( GTK_SCROLLED_WINDOW(scroll_widget), NULL );
					gtk_box_remove( GTK_BOX(widget), scroll_widget );
					gtk_box_append( GTK_BOX(widget), img_widget );
					g_object_unref(img_widget);
	
					Resize(this_imgview->view_width_, this_imgview->view_height_);
				}
			}
			return true;
		}

		return false;
	}
}
