//
// C++ Implementation: master_impl
//
// Description:
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "master_impl.h"
#include "callback.h"
#include "window.h"

#include <algorithm>
#include <cctype>
#include <cerrno>

struct IdleCallbackParameter
{
	IdleCallbackParameter(xgui::Callback * c, xgui::Widget * w) : cbk(c), wid(w) {}

	xgui::Callback * cbk;
	xgui::Widget * wid;
};

static int IdleCallbackWrapper( void * cb_info )
{
	IdleCallbackParameter * cb_p = (IdleCallbackParameter*)cb_info;

	cb_p->cbk->call(cb_p->wid);

	return FALSE;
}

static void IdleParameterDestructor( void * cb_info )
{
	IdleCallbackParameter * cb_p = (IdleCallbackParameter*)cb_info;
	delete cb_p;
}

/** Serializes the callers of GuiLock()/GuiUnlock(); GTK itself is not thread safe, see registerIdleCallback(). */
static GRecMutex gui_mutex;

/** The application of the current Master, used by the windows to attach themselves. */
static GtkApplication * current_app = NULL;

/** Returns the GTK window to use as transient parent of a dialog, or NULL when there is no parent. */
static GtkWindow * int_transient_parent ( xgui::Window * parent )
{
	if ( !parent ) return NULL;
	return GTK_WINDOW(parent->getImpl()->widget);
}

/** The application needs an activate handler, otherwise GIO warns on g_application_run(). */
static void int_on_activate ( GApplication *, gpointer ) {}

/** Returns true when the error only reports that the user dismissed or cancelled a dialog. */
static bool int_is_dismissed ( GError * error )
{
	return g_error_matches(error, GTK_DIALOG_ERROR, GTK_DIALOG_ERROR_DISMISSED)
	    || g_error_matches(error, GTK_DIALOG_ERROR, GTK_DIALOG_ERROR_CANCELLED)
	    || g_error_matches(error, G_IO_ERROR, G_IO_ERROR_CANCELLED);
}

/** Reports a dialog error unless it only means that the dialog was dismissed, then frees it. */
static void int_report_dialog_error ( GError * error, char const * dialog )
{
	if ( !error ) return;
	if ( !int_is_dismissed(error) ) XGUI_ERROR(dialog << " failed: " << error->message);
	g_error_free(error);
}

/** Builds the GtkFileFilter list of a file dialog from the xgui filters, or NULL when there are none. */
static GListModel * int_file_filters ( xgui::FileExtensionsVector const &filter )
{
	if ( filter.empty() ) return NULL;

	GListStore * filters = g_list_store_new(GTK_TYPE_FILE_FILTER);
	for (xgui::FileExtensionsVector::const_iterator i = filter.begin(); i != filter.end(); ++i) {
		GtkFileFilter * ff = gtk_file_filter_new();
		gtk_file_filter_set_name(ff, i->name.c_str());
		gtk_file_filter_add_pattern(ff, i->extension.c_str());
		g_list_store_append(filters, ff);
		g_object_unref(ff);
	}
	return G_LIST_MODEL(filters);
}

/** Creates a file dialog with title, filters and initial folder. */
static GtkFileDialog * int_file_dialog ( xgui::FileExtensionsVector const &filter, std::string const &title, std::string const &path )
{
	GtkFileDialog * dialog = gtk_file_dialog_new();
	gtk_file_dialog_set_title(dialog, title.c_str());
	gtk_file_dialog_set_modal(dialog, TRUE);

	GListModel * filters = int_file_filters(filter);
	if (filters) {
		gtk_file_dialog_set_filters(dialog, filters);
		g_object_unref(filters);
	}

	if (!path.empty()) {
		GFile * folder = g_file_new_for_path(path.c_str());
		gtk_file_dialog_set_initial_folder(dialog, folder);
		g_object_unref(folder);
	}
	return dialog;
}

/** State shared between a synchronous dialog call and its async completion callback. */
struct DialogWait
{
	GMainLoop * loop;
	GFile * file;
	GListModel * files;
	int button;
};

/** Returns the local path of a GFile (empty when it has none). */
static std::string int_file_path ( GFile * file )
{
	char * path = g_file_get_path(file);
	std::string result = path ? path : "";
	g_free(path);
	return result;
}

static void int_on_open_done ( GObject * source, GAsyncResult * result, gpointer data )
{
	DialogWait * wait = (DialogWait*)data;
	GError * error = NULL;
	wait->file = gtk_file_dialog_open_finish(GTK_FILE_DIALOG(source), result, &error);
	int_report_dialog_error(error, "OpenFileDialog");
	g_main_loop_quit(wait->loop);
}

static void int_on_open_multiple_done ( GObject * source, GAsyncResult * result, gpointer data )
{
	DialogWait * wait = (DialogWait*)data;
	GError * error = NULL;
	wait->files = gtk_file_dialog_open_multiple_finish(GTK_FILE_DIALOG(source), result, &error);
	int_report_dialog_error(error, "OpenFileDialog");
	g_main_loop_quit(wait->loop);
}

static void int_on_save_done ( GObject * source, GAsyncResult * result, gpointer data )
{
	DialogWait * wait = (DialogWait*)data;
	GError * error = NULL;
	wait->file = gtk_file_dialog_save_finish(GTK_FILE_DIALOG(source), result, &error);
	int_report_dialog_error(error, "SaveFileDialog");
	g_main_loop_quit(wait->loop);
}

static void int_on_alert_done ( GObject * source, GAsyncResult * result, gpointer data )
{
	DialogWait * wait = (DialogWait*)data;
	GError * error = NULL;
	wait->button = gtk_alert_dialog_choose_finish(GTK_ALERT_DIALOG(source), result, &error);
	int_report_dialog_error(error, "MessageDialog");
	g_main_loop_quit(wait->loop);
}

namespace xguimpl
{
	void runNestedLoop ( GMainLoop * loop )
	{
		g_main_loop_run(loop);
	}

	GtkApplication * application() { return current_app; }

	std::string Master::defaultApplicationId()
	{
		const char * prgname = g_get_prgname();
#ifdef __GLIBC__
		if (!prgname) prgname = program_invocation_short_name;
#endif
		std::string name = (prgname && *prgname) ? prgname : "app";

		for (std::string::iterator c = name.begin(); c != name.end(); ++c)
			if (!std::isalnum((unsigned char)*c) && *c != '_') *c = '_';
		if (std::isdigit((unsigned char)name[0])) name = "_" + name;

		return "org.xgui." + name;
	}

	Master::Master(xgui::Master * master)
	: threaded(false), master_(master), app(0), registered(false), app_running(false)
	{
		DMESSAGE("XGUI Initialized");

		std::string app_id = xgui::Master::application_id_;
		if (app_id.empty()) app_id = defaultApplicationId();
		if (!g_application_id_is_valid(app_id.c_str())) {
			XGUI_ERROR("Invalid application id '" << app_id << "', using the default one");
			app_id = defaultApplicationId();
		}

		GApplicationFlags flags = xgui::Master::application_unique_ ? G_APPLICATION_DEFAULT_FLAGS : G_APPLICATION_NON_UNIQUE;
		app = gtk_application_new(app_id.c_str(), flags);
		g_signal_connect(app, "activate", G_CALLBACK(int_on_activate), NULL);

		// Registering emits "startup", which initializes GTK, so windows can be attached right away
		GError * error = NULL;
		registered = g_application_register(G_APPLICATION(app), NULL, &error);
		if (!registered) {
			XGUI_ERROR("Cannot register application '" << app_id << "': " << (error ? error->message : "unknown error"));
			if (error) g_error_free(error);
			gtk_init();
		}
		else if (g_application_get_is_remote(G_APPLICATION(app)))
			XGUI_ERROR("Application '" << app_id << "' is already running, its windows belong to the other instance");

		// The application lives until Quit(), not until its last window is closed
		g_application_hold(G_APPLICATION(app));
		current_app = registered ? app : NULL;
	}

	Master::~Master()
	{
		for (std::vector<GMainLoop *>::iterator l = loops.begin(); l != loops.end(); ++l)
			g_main_loop_quit(*l);
		loops.clear();

		if (current_app == app) current_app = NULL;
		g_application_release(G_APPLICATION(app));
		g_application_quit(G_APPLICATION(app));
		g_object_unref(app);
	}

	void Master::initThreads()
	{
		threaded = true;
	}

	void Master::guiLock()
	{
		if (threaded)
			g_rec_mutex_lock(&gui_mutex);
	}

	void Master::guiUnlock()
	{
		if (threaded) {
			gdk_display_flush(gdk_display_get_default());
			g_rec_mutex_unlock(&gui_mutex);
		}
	}

	void Master::quitThreads()
	{

	}

	int Master::run()
	{
		++master_->loop_level_;

		if (!app_running) {
			app_running = true;
			// Quit() deletes this object while the application runs: keep a reference and do not touch members afterwards
			GApplication * running_app = G_APPLICATION(g_object_ref(app));
			g_application_run(running_app, 0, NULL);
			g_object_unref(running_app);
			return 0;
		}

		GMainLoop * loop = g_main_loop_new(NULL, FALSE);
		loops.push_back(loop);
		runNestedLoop(loop);
		g_main_loop_unref(loop);
		return 0;
	}

	bool Master::quit()
	{
		if (--master_->loop_level_ <= 0) return true;

		if (!loops.empty()) {
			g_main_loop_quit(loops.back());
			loops.pop_back();
		}
		return false;
	}

	void Master::destroyWidget(xgui::Widget * w)
	{
		GtkWidget * widget = GTK_WIDGET(w->getImpl()->widget);

		if (GTK_IS_WINDOW(widget))
			gtk_window_destroy(GTK_WINDOW(widget));
		else
			detachWidget(widget);
	}

	void Master::registerIdleCallback ( xgui::Callback * cbk, xgui::Widget * wid )
	{
		g_idle_add_full(G_PRIORITY_DEFAULT_IDLE, IdleCallbackWrapper, new IdleCallbackParameter(cbk, wid), IdleParameterDestructor );
	}

	xgui::StrVector Master::openFileDialog(xgui::Window * parent, xgui::FileExtensionsVector const &filter, std::string const &title, std::string const &path, bool multiselection)
	{
		GtkFileDialog * dialog = int_file_dialog(filter, title, path);
		DialogWait wait = { g_main_loop_new(NULL, FALSE), NULL, NULL, -1 };

		if (multiselection)
			gtk_file_dialog_open_multiple(dialog, int_transient_parent(parent), NULL, int_on_open_multiple_done, &wait);
		else
			gtk_file_dialog_open(dialog, int_transient_parent(parent), NULL, int_on_open_done, &wait);
		runNestedLoop(wait.loop);

		xgui::StrVector ret_file_names;
		if (wait.file) {
			ret_file_names.push_back(int_file_path(wait.file));
			g_object_unref(wait.file);
		}
		if (wait.files) {
			guint n = g_list_model_get_n_items(wait.files);
			for (guint i = 0; i < n; ++i) {
				GFile * file = G_FILE(g_list_model_get_item(wait.files, i));
				ret_file_names.push_back(int_file_path(file));
				g_object_unref(file);
			}
			g_object_unref(wait.files);
		}

		g_main_loop_unref(wait.loop);
		g_object_unref(dialog);
		return ret_file_names;
	}

	std::string Master::saveFileDialog(xgui::Window * parent, xgui::FileExtensionsVector const &filter, std::string const &title, std::string const &path, std::string const &file_name)
	{
		GtkFileDialog * dialog = int_file_dialog(filter, title, path);
		if (!file_name.empty())
			gtk_file_dialog_set_initial_name(dialog, file_name.c_str());

		DialogWait wait = { g_main_loop_new(NULL, FALSE), NULL, NULL, -1 };
		gtk_file_dialog_save(dialog, int_transient_parent(parent), NULL, int_on_save_done, &wait);
		runNestedLoop(wait.loop);

		std::string ret_file_name;
		if (wait.file) {
			ret_file_name = int_file_path(wait.file);
			g_object_unref(wait.file);
		}

		g_main_loop_unref(wait.loop);
		g_object_unref(dialog);
		return ret_file_name;
	}

	int Master::messageDialog(xgui::Window * parent, const std::string & title, const std::string & text, const std::string & d_type, const std::string & d_buttons )
	{
		std::string buttons = d_buttons;
		std::transform(buttons.begin(), buttons.end(), buttons.begin(), tolower);

		// Labels shown by the dialog and the xgui code returned for each of them
		std::vector<const char *> labels;
		std::vector<int> codes;
		int cancel_index = -1;
		if ( buttons == "ok-cancel" ) {
			labels = { "_Cancel", "_OK" }; codes = { 0, 1 }; cancel_index = 0;
		}
		else if ( buttons == "yes-no" ) {
			labels = { "_No", "_Yes" }; codes = { 2, 1 };
		}
		else if ( buttons == "yes-no-cancel" ) {
			labels = { "_Cancel", "_No", "_Yes" }; codes = { 0, 2, 1 }; cancel_index = 0;
		}
		else {
			labels = { "_OK" }; codes = { 1 };
		}
		labels.push_back(NULL);

		// GtkAlertDialog has no message type: the title is the message, the text its detail
		GtkAlertDialog * dialog = gtk_alert_dialog_new("%s", title.c_str());
		gtk_alert_dialog_set_detail(dialog, text.c_str());
		gtk_alert_dialog_set_buttons(dialog, labels.data());
		gtk_alert_dialog_set_cancel_button(dialog, cancel_index);
		gtk_alert_dialog_set_default_button(dialog, (int)codes.size() - 1);
		gtk_alert_dialog_set_modal(dialog, TRUE);

		DialogWait wait = { g_main_loop_new(NULL, FALSE), NULL, NULL, -1 };
		gtk_alert_dialog_choose(dialog, int_transient_parent(parent), NULL, int_on_alert_done, &wait);
		runNestedLoop(wait.loop);

		g_main_loop_unref(wait.loop);
		g_object_unref(dialog);

		if ( (wait.button < 0) || (wait.button >= (int)codes.size()) ) return -1;
		return codes[wait.button];
	}
}
