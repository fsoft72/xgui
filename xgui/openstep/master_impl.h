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

#include "master.h"
#include "widget.h"
#include "openstep/widget_impl.h"

#ifndef _XGUI_MASTER_IMPL_H_
#define _XGUI_MASTER_IMPL_H_

namespace xguimpl
{
	struct Master
	{
		bool threaded;
		xgui::Master * master_;

		Master ( xgui::Master * master );
		~Master();

		int run();
		bool quit();
		void registerIdleCallback ( xgui::Callback * cbk, xgui::Widget * wid );

		void initThreads();
		void quitThreads();

		void guiLock();
		void guiUnlock();

		void destroyWidget(xgui::Widget * w);

		xgui::StrVector openFileDialog(xgui::Window * parent, xgui::FileExtensionsVector const &filter, std::string const &title, std::string const &path, bool multiselection);
		std::string saveFileDialog(xgui::Window * parent, xgui::FileExtensionsVector const &filter, std::string const &title, std::string const &path, std::string const &file_name);
		int messageDialog(xgui::Window * parent, const std::string & title, const std::string & text, const std::string & d_type, const std::string & d_buttons );
	};
}

#endif
