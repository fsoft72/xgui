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

#include "master_impl.h"
#include "callback.h"

#import <Foundation/Foundation.h>

#include "openstep_object.h"

namespace xguimpl
{
	struct OpenStepMaster
	{
		static NSApplication * app;
		static NSAutoreleasePool * pool;

		static void InitApp() { OpenStepMaster::app = [NSApplication sharedApplication]; }
		static void InitPool() { OpenStepMaster::pool = [[NSAutoreleasePool alloc] init]; }
		static void ReleasePool() { [OpenStepMaster::pool release]; }
	};

	NSAutoreleasePool * OpenStepMaster::pool = 0;
	NSApplication * OpenStepMaster::app = 0;

	Master::Master(xgui::Master * master)
	: master_(master)
	{
		OpenStepMaster::InitPool();
		OpenStepMaster::InitApp();

		DMESSAGE("XGUI Initialized");
	}

	Master::~Master()
	{
		OpenStepMaster::ReleasePool();
	}

	void Master::initThreads()
	{
		threaded = true;
	}
	
	void Master::guiLock()
	{

	}
	
	void Master::guiUnlock()
	{

	}
	
	void Master::quitThreads()
	{
	
	}

	int Master::run()
	{
		[NSApp run];
		return 1;
	}
	
	bool Master::quit()
	{
		[NSApp stop:NSApp];
		return true;
	}

	void Master::destroyWidget(xgui::Widget * w)
	{
		id o = w->getImpl()->widget->o;
		NSWindow * window = NULL;

		ASSERT([o conformsToProtocol:@protocol(XGui)])

		if ( [o respondsToSelector:@selector(window)] )
			window = [o window];
		
		[o die];
		[o release];

		if (window) {
			[window disableFlushWindow];

			NSRect full_rect = [window frame];
			NSRect tmp_rect = full_rect;
			tmp_rect.size.width += 1;
			[window setFrame:tmp_rect display:YES];
			[window setFrame:full_rect display:YES];

			[window enableFlushWindow];
		}

	}

	void Master::registerIdleCallback ( xgui::Callback * cbk, xgui::Widget * wid )
	{

	}
	
	xgui::StrVector Master::openFileDialog(xgui::Window * parent, xgui::FileExtensionsVector const &filter, std::string const &title, std::string const &path, bool multiselection)
	{
		xgui::StrVector ret_file_names;
		return ret_file_names;
	}
	
	std::string Master::saveFileDialog(xgui::Window * parent, xgui::FileExtensionsVector const &filter, std::string const &title, std::string const &path, std::string const &file_name)
	{
		std::string ret_file_name;
		return ret_file_name;
	}
	
	int Master::messageDialog(xgui::Window * parent, const std::string & title, const std::string & text, const std::string & d_type, const std::string & d_buttons )
	{
		return -1;
	}
}
