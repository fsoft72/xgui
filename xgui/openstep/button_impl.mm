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

#include "button_impl.h"
#include "button.h"
#include "callback.h"
#include "image.h"
#include "image_impl.h"

#include "container.h"

#include "openstep_object.h"

@interface XGuiOSButton : NSButton <XGui>
{
	xguimpl::Widget * owner;
}

+ (id)Create:(xgui::Container*)parent withText:(char const*)text;

- (xguimpl::Widget*)getOwner;
- (void)setOwner:(xguimpl::Widget*)w;
- (void)die;

- (void)onClick;
@end

@implementation XGuiOSButton
+ (id)Create:(xgui::Container*)parent withText:(char const*)text
{
	NSRect rect;
	rect.origin.x = rect.origin.y = 0;
	rect.size.width = 1;
	rect.size.height = 1;

	XGuiOSButton * b = [[XGuiOSButton alloc] initWithFrame:rect];

    [b setButtonType:NSMomentaryPushInButton];
    [b setTitle: [NSString stringWithUTF8String:text]];

	[b setTarget: self];
	[b setAction: @selector(onClick)];

    return b;
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
	xguimpl::Widget::OnDestroy(owner);
}

- (void)onClick
{
	xguimpl::Button::OnClick(dynamic_cast<xguimpl::Button*>(owner));
}
@end


namespace xguimpl
{
	Button::Button ( OpenStepWidget * real_w ) : Widget ( real_w ) {}
	Button::Button ( xgui::Container * parent, std::string const &text, xgui::Image * image, bool toggle_button )
	: Widget ( new OpenStepWidget([XGuiOSButton Create:parent withText:text.c_str()]) )
	{
		if (image) {
			image->ref();
			set_widget_prop("tooltip-text", text);
		}

		// Make the button calculate its proper size based on the title
		[(NSButton*)widget->o sizeToFit];

		// Now get the actual size after sizeToFit
		NSRect frame = [(NSButton*)widget->o frame];
		min_x = frame.size.width > 10 ? frame.size.width : 80;  // Reasonable minimum
		min_y = frame.size.height > 10 ? frame.size.height : 24; // Standard button height
	}

	Button::~Button() {}

	void Button::setAccessor(xgui::Widget * w)
	{
		xgui::Button * downcasted_widget = dynamic_cast<xgui::Button*>(w);
		ASSERT(downcasted_widget);
		this_button = downcasted_widget;

		Widget::setAccessor(w);
	}

	bool Button::linkEvent( std::string const &name )
	{
		if ( name == "onclick" ) {
			return true;
		}
	
		return Widget::linkEvent(name);
	}

	bool Button::get_button_prop ( std::string const &name, std::string &vals )
	{
		if ( name == "text" ) {
			vals = "";
			return true;
		}
		else if ( name == "pressed" ) {
			vals = "0";
			return true;
		}
		else if ( name == "alignment" ) {
			vals = joinAlignment(0, 0);
			return true;
		}

		return false;
	}

	bool Button::set_button_prop ( std::string const &name, const std::string &vals )
	{
		if ( name == "text" ) {
			if ( this_button->icon_ == 0 ) {}
				
			else
				set_widget_prop("tooltip-text", vals);
			return true;
		}
		else if ( name == "pressed" ) {
			return true;
		}
		else if ( name == "alignment" ) {
			std::pair<float, float> align = splitAlignment(vals);
			return true;
		}

		return false;
	}

	void Button::setIcon ( xgui::Image * img )
	{
		if (img) {
		}
	}

	int Button::OnClick ( Button * btn )
	{
		xgui::Callback  * cb =  btn->this_widget->getEvent("onclick");
		if ( cb ) 
			cb->call( btn->this_widget );
		return 0;
	}
}


