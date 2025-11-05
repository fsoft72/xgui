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

#include "image_impl.h"
#include "image.h"

namespace xguimpl
{
	Image::Image(std::string const &path)
	: this_image(0), image(0)
	{

	}

	Image::~Image() {  }

	void Image::setAccessor(xgui::Image * accessor) 
	{ 
		ASSERT(accessor);
		this_image = accessor;

		if (image != NULL) {
			this_image->width_ = 0;
			this_image->height_ = 0;
		}
	}

	bool Image::isValid() { return image != NULL; }
}
