#include "master.h"
#include "widget.h"
#include "gtk/widget_impl.h"

#ifndef _XGUI_MASTER_IMPL_H_
#define _XGUI_MASTER_IMPL_H_

#include <vector>

namespace xguimpl
{
	/** Runs a nested main loop until g_main_loop_quit() is called on it. */
	void runNestedLoop ( GMainLoop * loop );

	/** Returns the GtkApplication the windows are attached to, or NULL when it could not be registered. */
	GtkApplication * application();

	struct Master
	{
		bool threaded;
		xgui::Master * master_;

		/** The application driving the outermost Run() level. */
		GtkApplication * app;
		/** True once the application has been registered (GTK is initialized either way). */
		bool registered;
		/** True while g_application_run() is on the stack. */
		bool app_running;
		/** Main loops of the nested Run() levels, innermost last. */
		std::vector<GMainLoop *> loops;

		Master ( xgui::Master * master );
		~Master();

		/** Builds the default application id from the program name. */
		static std::string defaultApplicationId();

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
