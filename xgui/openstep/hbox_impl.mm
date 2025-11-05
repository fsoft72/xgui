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

#include "hbox_impl.h"
#include "hbox.h"

#include "openstep_object.h"

@interface XGuiOSHBox : NSBox <XGui>
{
	xguimpl::Widget * owner;
}

+ (id)Create;

- (xguimpl::Widget*)getOwner;
- (void)setOwner:(xguimpl::Widget*)w;
- (void)die;

- (void)resizeWithOldSuperviewSize:(NSSize)oldBoundsSize;
@end

@implementation XGuiOSHBox
+ (id)Create
{
	NSRect rect;
	rect.origin.x = rect.origin.y = 0;
	rect.size.width = 1;
	rect.size.height = 1;

	XGuiOSHBox * box = [[XGuiOSHBox alloc] initWithFrame:rect];
	[box setAutoresizingMask:NSViewWidthSizable|NSViewHeightSizable];
	[box setTitlePosition:NSNoTitle];
	[box setBorderType:NSNoBorder];
	[box setAutoresizesSubviews:NO];

	return box;
}

- (xguimpl::Widget*)getOwner
{
	return owner;
}

- (void)setOwner:(xguimpl::Widget*)w
{
	owner = w;
}

- (void)die
{
	[self removeFromSuperviewWithoutNeedingDisplay];

	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	NSArray * children = [[self contentView] subviews];

	while ([children count]) {
		id o = [children lastObject];
		if ([o conformsToProtocol:@protocol(XGui)])
			[o die];
		else
			[o removeFromSuperviewWithoutNeedingDisplay];
		children = [[self contentView] subviews];
	}

	[pool release];

	xguimpl::Widget::OnDestroy(owner);
	owner = NULL;
}

- (void)resizeWithOldSuperviewSize:(NSSize)oldBoundsSize
{
	[[self window] disableFlushWindow];
	[super resizeWithOldSuperviewSize:oldBoundsSize];

	NSRect full_rect = [[self contentView] frame];
	xguimpl::HBox * downcasted_widget = dynamic_cast<xguimpl::HBox*>(owner);
	downcasted_widget->giveSize(std::make_pair(full_rect.size.width, full_rect.size.height));

	[[self window] enableFlushWindow];
}
@end


namespace xguimpl
{
	HBox::HBox(xgui::Container * parent, int space, bool border)
	: Widget(new OpenStepWidget([XGuiOSHBox Create])), this_box(0), space_(space)
	{
		if (border)
			[widget->o setBorderType:NSLineBorder];
	}

	HBox::HBox(OpenStepWidget * real_w) : Widget(real_w), this_box(0), space_(0) { }

	HBox::~HBox() { }

	void HBox::setAccessor(xgui::Widget * w)
	{
		xgui::HBox * downcasted_widget = dynamic_cast<xgui::HBox*>(w);
		ASSERT(downcasted_widget);
		this_box = downcasted_widget;

		Widget::setAccessor(w);
	}

	void HBox::addChild(xgui::Widget * w)
	{
		NSRect full_rect = [[widget->o contentView] frame];
		[widget->o addSubview:w->getImpl()->widget->o];
		this->giveSize(std::make_pair(full_rect.size.width, full_rect.size.height));
	}

	void HBox::giveSize ( std::pair<float, float> size )
	{
		xgui::HBox * hb = dynamic_cast<xgui::HBox *>(this_widget);

		int cur_x = static_cast<int>(size.first);
		int cur_y = static_cast<int>(size.second);

		int w = cur_x - space_ * 2;
		int h = cur_y - space_ * 2;

		int slot_x = 0;  //somma delle dimensioni minime dei figli resizabili
		double px;

		if ( hb && !hb->children_.empty() ) {

			for ( xgui::WidgetsVector::iterator wid_iter = hb->children_.begin();
				wid_iter != hb->children_.end(); ++wid_iter )
			{
				xgui::Widget * wid = *wid_iter;
				Widget * wimpl = wid->getImpl();

				int min_x = wimpl->min_x;

				if (!wimpl->expand)
					w -= min_x;
				else 
					slot_x += min_x;
			}

			if ( slot_x == 0 )
				px = 1;
			else
				px = (double)w / (double)slot_x;

			int child_x = space_;
			int child_y = space_;

			for ( xgui::WidgetsVector::iterator wid_iter = hb->children_.begin();
				wid_iter != hb->children_.end(); ++wid_iter )
			{
				xgui::Widget * wid = *wid_iter;
				Widget * wimpl = wid->getImpl();

				int min_x = wimpl->min_x;

				if ( wimpl->expand )
					min_x = int(min_x * px);

				NSRect rect;
				rect.origin.x = child_x;
				rect.origin.y = child_y;
				rect.size.width = min_x;
				rect.size.height = h;

				[wimpl->widget->o setFrame:rect];
				[wimpl->widget->o resizeWithOldSuperviewSize:NSMakeSize(w, h)];
			    [wimpl->widget->o setNeedsDisplay:YES];				

				child_x += min_x;
			}

		}
	}

	void HBox::recalcLayout()
	{
		xgui::HBox * hb = dynamic_cast<xgui::HBox *>(this_widget);
		if ( !hb ) {
			min_x = space_ * 2;
			min_y = space_ * 2;
			return;
		}

		min_x = min_y = 0;

		for ( xgui::WidgetsVector::iterator wid_iter = hb->children_.begin();
			wid_iter != hb->children_.end(); ++wid_iter )
		{
			xgui::Widget * child = *wid_iter;
			Widget * childimpl = child->getImpl();

			child->recalcLayout();

			min_x += childimpl->min_x;
			min_y = min_y > childimpl->min_y ? min_y : childimpl->min_y;
		}

		min_x += space_ * 2;
		min_y += space_ * 2;
	}

	void HBox::setExpand(xgui::Widget * w, float expand)
	{
		w->getImpl()->expand = ((expand == 0.0) ? false : true);

		NSRect full_rect = [[widget->o contentView] frame];
		this->giveSize(std::make_pair(full_rect.size.width, full_rect.size.height));
	}
}

