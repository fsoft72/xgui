//
// C++ Interface: master_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "master.h"
#include "widget.h"
#include "gtk/widget_impl.h"

#ifndef _XGUI_MASTER_IMPL_H_
#define _XGUI_MASTER_IMPL_H_

namespace xguimpl
{
	struct Master
	{
		bool threaded;
		GtkTooltip * app_tooltips;
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
