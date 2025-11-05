/* * 
 * Copyright (C) 2007,2008 by 
 * 	Alessandro Molina <alessandro.molina@gmail.com>
 *  
 * This file is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 * 
 * This file is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 * General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License
 * along with this file; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *  
 * */

#include "imageview.h"
#include "imageview_impl.h"
#include "image.h"
#include "image_impl.h"

namespace xguimpl
{
 	void ImageView::init()
	{

	}

	ImageView::ImageView ( OpenStepWidget * real_w ) 
	: Widget (0), scroll_widget(0), img_widget(real_w) 
	{
		init();
	}

	ImageView::ImageView ( xgui::Container * parent ) 
	: Widget (0), scroll_widget(0), img_widget(0)
	{
		init();
	}

	ImageView::~ImageView() 
	{

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
	}

	void ImageView::setImage(xgui::Image * img)
	{

	}

	bool ImageView::get_imageview_prop ( std::string const &name, std::string &vals )
	{
		if ( name == "alignment" ) {
			float x, y;
			vals = joinAlignment(x, y);
			return true;
		}

		return false;
	}

	bool ImageView::set_imageview_prop ( std::string const &name, const std::string &vals )
	{
		if ( name == "alignment" ) {
			std::pair<float, float> align = splitAlignment(vals);
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
				}
			}
			else {
				if (this_imgview->scrolled_ == true) {
					this_imgview->scrolled_ = false;
					Resize(this_imgview->view_width_, this_imgview->view_height_);
				}
			}
			return true;
		}

		return false;
	}
}
