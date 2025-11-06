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
		gtk_box_pack_start( GTK_BOX(widget), img_widget, TRUE, TRUE, 0 );
	
		g_object_ref( G_OBJECT( scroll_widget ) );
		g_object_ref_sink( G_OBJECT( scroll_widget ) );
	}

	ImageView::ImageView ( GtkWidget * real_w ) 
	: Widget (gtk_box_new(GTK_ORIENTATION_VERTICAL, 0)), scroll_widget(gtk_scrolled_window_new(NULL, NULL)), img_widget(real_w) 
	{
		init();
	}

	ImageView::ImageView ( xgui::Container * parent ) 
	: Widget (gtk_box_new(GTK_ORIENTATION_VERTICAL, 0)), scroll_widget(gtk_scrolled_window_new(NULL, NULL)), img_widget(gtk_image_new_from_file(NULL))
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
		
		gtk_image_set_from_pixbuf( GTK_IMAGE(img_widget), gdk_pixbuf_scale_simple(this_imgview->image_->getImpl()->getPixbuf(), x, y, GDK_INTERP_BILINEAR ) );
	}

	void ImageView::setImage(xgui::Image * img)
	{
		if (img)
			gtk_image_set_from_pixbuf( GTK_IMAGE(img_widget), img->getImpl()->getPixbuf() );
		else
			gtk_image_set_from_file( GTK_IMAGE(img_widget), NULL);
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
		GtkAlign halign = (align.first == 0.0f) ? GTK_ALIGN_START : (align.first == 1.0f) ? GTK_ALIGN_END : GTK_ALIGN_CENTER;
		GtkAlign valign = (align.second == 0.0f) ? GTK_ALIGN_START : (align.second == 1.0f) ? GTK_ALIGN_END : GTK_ALIGN_CENTER;
		gtk_widget_set_halign(img_widget, halign);
		gtk_widget_set_valign(img_widget, valign);
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
					gtk_container_remove( GTK_CONTAINER(widget), img_widget );
					gtk_scrolled_window_add_with_viewport ( GTK_SCROLLED_WINDOW(scroll_widget), img_widget );
					gtk_box_pack_start( GTK_BOX(widget), scroll_widget, TRUE, TRUE, 0 );
					g_object_unref(img_widget);
	
					setImage(this_imgview->image_);
					gtk_widget_set_size_request ( GTK_WIDGET(widget), this_imgview->view_width_, this_imgview->view_height_ );
				}
			}
			else {
				if (this_imgview->scrolled_ == true) {
					this_imgview->scrolled_ = false;
					g_object_ref(img_widget);
					gtk_container_remove( GTK_CONTAINER(gtk_bin_get_child(GTK_BIN(scroll_widget))), img_widget );
					gtk_container_remove( GTK_CONTAINER(widget), scroll_widget );
					gtk_container_remove( GTK_CONTAINER(scroll_widget), GTK_WIDGET(gtk_bin_get_child(GTK_BIN(scroll_widget))) );
					gtk_box_pack_start( GTK_BOX(widget), img_widget, TRUE, TRUE, 0 );
					g_object_unref(img_widget);
	
					Resize(this_imgview->view_width_, this_imgview->view_height_);
				}
			}
			return true;
		}

		return false;
	}
}
