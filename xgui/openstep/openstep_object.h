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

#ifndef _XGUI_WIDGET_IMPL_OPENSTEP_H_
#define _XGUI_WIDGET_IMPL_OPENSTEP_H_

#import <AppKit/AppKit.h>

namespace xguimpl
{
	struct Widget;

	struct OpenStepObject
	{
		id o;

		OpenStepObject(id obj);
		~OpenStepObject();
	};
}

@protocol XGui
- (xguimpl::Widget*)getOwner;
- (void)setOwner:(xguimpl::Widget*)w;
- (void)die;
@end

#endif
