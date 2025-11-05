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

#include "window_impl.h"
#include "window.h"
#include "callback.h"
#include "image.h"
#include "image_impl.h"
#include "menu.h"

#include "openstep_object.h"

@interface XGuiOSWindow : NSWindow <XGui>
{
	xguimpl::Widget * owner;
}

+ (id)Create;

- (BOOL)windowShouldClose:(id)window;

- (xguimpl::Widget*)getOwner;
- (void)setOwner:(xguimpl::Widget*)w;
- (void)die;

- (void)dealloc;
@end

@implementation XGuiOSWindow
+ (id)Create
{
	NSRect rect;
	rect.origin.x = rect.origin.y = 0;
	rect.size.width = 640;
	rect.size.height = 480;

	unsigned mask = NSTitledWindowMask|NSClosableWindowMask|NSMiniaturizableWindowMask|NSResizableWindowMask;

	return [[XGuiOSWindow alloc] initWithContentRect:rect styleMask:mask backing:NSBackingStoreBuffered defer:NO];
}

- (xguimpl::Widget*)getOwner
{
	return owner;
}

- (void)setOwner:(xguimpl::Widget*)w
{
	owner = w;
}

- (BOOL)windowShouldClose:(id)win
{
	return xguimpl::Window::OnClose(dynamic_cast<xguimpl::Window*>(owner));
}

- (void)die
{
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];

	NSView * root = [self contentView];
	NSArray * children = [root subviews];

	while ([children count]) {
		[[children lastObject] die];
		children = [root subviews];
	}

	[pool release];

	xguimpl::Widget::OnDestroy(owner);
	owner = NULL;
}

- (void)dealloc
{
	if (owner)
		[self die];
	[super dealloc];
}
@end

namespace xguimpl
{
	Window::Window(xgui::Window * parent)
	: Widget(new OpenStepWidget([XGuiOSWindow Create])), this_window(0)
	{
	}

	Window::Window(OpenStepWidget * real_w) : Widget(real_w), this_window(0) { }

	Window::~Window() { }

	void Window::setAccessor(xgui::Widget * w)
	{
		xgui::Window * downcasted_widget = dynamic_cast<xgui::Window*>(w);
		ASSERT(downcasted_widget);
		this_window = downcasted_widget;

		Widget::setAccessor(w);
	}

	bool Window::get_window_prop ( std::string const &name, std::string &vals )
	{
		if ( name == "size" ) {
			return true;
		}
		else if ( name == "text" ) {
			return true;
		}
		else if ( name == "position" ) {
			return true;
		}
		else if ( name == "resizable" ) {

			return true;
		}
		else if ( name == "border" ) {

			return true;
		}
		else if ( name == "status-text" ) {
			vals = "";
			return true;
		}

		return false;
	}

	bool Window::set_window_prop ( std::string const &name, std::string const &vals )
	{
		if ( name == "size" ) {
			return true;
		}
		else if ( name == "text" ) {

			return true;
		}
		else if ( name == "position" ) {
			return true;
		}
		else if ( name == "resizable" ) {

			return true;
		}
		else if ( name == "border" ) {

			return true;
		}
		else if ( name == "status-text" ) {
			return true;
		}

		return false;
	}

	void Window::show()
	{
		recalcLayout();
		[widget->o makeKeyAndOrderFront:nil];
	}

	void Window::recalcLayout()
	{
		xgui::Window * this_window = dynamic_cast<xgui::Window *>(this_widget);

		if ( !this_window->child_ ) return;

		this_window->child_->recalcLayout();

		min_x = this_window->child_->getImpl()->min_x;
		min_y = this_window->child_->getImpl()->min_y;

		min_x = min_x > 100 ? min_x : 100;

		[widget->o setMinSize:NSMakeSize(min_x, min_y)];
	}

	int Window::modalRun() 
	{
	}

	void Window::modalReturn(int response) 
	{
	}


	bool Window::addChild(xgui::Widget * w) 
	{
		bool done = false;
		if (!this_window->child_) {
			this_window->child_ = w;
			id sub_view = w->getImpl()->widget->o;

		    [[widget->o contentView] addSubview:sub_view];
			NSRect fsize = [[widget->o contentView] frame];

			[sub_view setFrameOrigin:fsize.origin];
			[sub_view setFrameSize:fsize.size];
		    [sub_view setNeedsDisplay:YES];
			[sub_view setAutoresizingMask:NSViewWidthSizable|NSViewHeightSizable];

			done = true;
		}

		return done;
	}

	void Window::delChild(xgui::Widget * w)
	{
		if (this_window->child_ == w) this_window->child_ = 0;
	}

	void Window::maximize()
	{

	}
	
	void Window::minimize()
	{

	}
	
	void Window::restore()
	{
	}

	bool Window::linkEvent(std::string const &name)
	{
		if ( name == "onclose" ) {
			return true;
		}
	
		return Widget::linkEvent(name);
	}

	void Window::setIcon(xgui::Image * img)
	{
	}

	/* Event Handlers */

	int Window::OnClose ( Window * xw )
	{
		xgui::Callback * cb =  xw->this_widget->getEvent( "onclose" );
		if ( cb ) return !cb->call( xw->this_widget );
		return 0;
	}
}
