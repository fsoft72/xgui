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

namespace xguimpl
{
	Master::Master(xgui::Master * master)
	: app_tooltips(0), master_(master)
	{
		DMESSAGE("XGUI Initialized");
		int argc = 0;
		char **argv = 0;
	
		gtk_init ( &argc, &argv );
	
		app_tooltips = gtk_tooltips_new ();
		gtk_tooltips_enable ( GTK_TOOLTIPS ( app_tooltips ) );
	}

	Master::~Master()
	{
		gtk_main_quit();
	}

	void Master::initThreads()
	{
		threaded = true;
		g_thread_init(0);
		gdk_threads_init();
	}
	
	void Master::guiLock()
	{
		if (threaded)
			gdk_threads_enter();
	}
	
	void Master::guiUnlock()
	{
		if (threaded) {
			gdk_flush();
			gdk_threads_leave();
		}
	}
	
	void Master::quitThreads()
	{
	
	}

	int Master::run()
	{
		++master_->loop_level_;
		gtk_main ();
		return 0;
	}
	
	bool Master::quit()
	{
		if (--master_->loop_level_ <= 0) {
			return true;
		}
		else {
			gtk_main_quit();
			return false;
		}
	}

	void Master::destroyWidget(xgui::Widget * w)
	{
		gtk_widget_destroy( GTK_WIDGET(w->getImpl()->widget) );
	}

	void Master::registerIdleCallback ( xgui::Callback * cbk, xgui::Widget * wid )
	{
		g_idle_add_full(G_PRIORITY_DEFAULT_IDLE, IdleCallbackWrapper, new IdleCallbackParameter(cbk, wid), IdleParameterDestructor );
	}
	
	xgui::StrVector Master::openFileDialog(xgui::Window * parent, xgui::FileExtensionsVector const &filter, std::string const &title, std::string const &path, bool multiselection)
	{
		std::vector<GtkFileFilter*> gtk_filters;
		for(std::vector<xgui::FileExtension>::const_iterator i = filter.begin(); i != filter.end(); ++i) {
			GtkFileFilter * ff = gtk_file_filter_new();
			gtk_file_filter_set_name(ff, i->name.c_str());
			gtk_file_filter_add_pattern(ff, i->extension.c_str());
			gtk_filters.push_back(ff);
		}
	
		GtkWidget *dialog = gtk_file_chooser_dialog_new ( title.c_str(), GTK_WINDOW(parent->getImpl()->widget),
		                                                  GTK_FILE_CHOOSER_ACTION_OPEN,	GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
		                                                  GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL );
	
		gtk_file_chooser_set_select_multiple( GTK_FILE_CHOOSER(dialog), multiselection );
	
		for (std::vector<GtkFileFilter*>::iterator i = gtk_filters.begin(); i != gtk_filters.end(); ++i)
			gtk_file_chooser_add_filter (GTK_FILE_CHOOSER(dialog), *i );
	
		if (!path.empty())
			gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), path.c_str());
	
		xgui::StrVector ret_file_names;
		if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
			GSList * files =  gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(dialog));
			for (GSList * file = files; file != 0; file = file->next) {
				ret_file_names.push_back( (char*)file->data );
				g_free(file->data);
			}
			g_slist_free(files);
		}
	
		gtk_widget_destroy(dialog);
		return ret_file_names;
	}
	
	std::string Master::saveFileDialog(xgui::Window * parent, xgui::FileExtensionsVector const &filter, std::string const &title, std::string const &path, std::string const &file_name)
	{
		std::vector<GtkFileFilter*> gtk_filters;
		for(std::vector<xgui::FileExtension>::const_iterator i = filter.begin(); i != filter.end(); ++i) {
			GtkFileFilter * ff = gtk_file_filter_new();
			gtk_file_filter_set_name(ff, i->name.c_str());
			gtk_file_filter_add_pattern(ff, i->extension.c_str());
			gtk_filters.push_back(ff);
		}
	
		GtkWidget *dialog = gtk_file_chooser_dialog_new ( title.c_str(), GTK_WINDOW(parent->getImpl()->widget), 
		                                                  GTK_FILE_CHOOSER_ACTION_SAVE,	GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
		                                                  GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT, NULL );
	
		for (std::vector<GtkFileFilter*>::iterator i = gtk_filters.begin(); i != gtk_filters.end(); ++i)
			gtk_file_chooser_add_filter (GTK_FILE_CHOOSER(dialog), *i );
	
		if (!path.empty())
			gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), path.c_str());
	
		if (!file_name.empty())
			gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), file_name.c_str());
	
		std::string ret_file_name;
		if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
			char * fname = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
			ret_file_name = fname;
			g_free(fname);
		}
	
		gtk_widget_destroy(dialog);
		return ret_file_name;
	}
	
	int Master::messageDialog(xgui::Window * parent, const std::string & title, const std::string & text, const std::string & d_type, const std::string & d_buttons )
	{
		std::string type = d_type;
		std::transform(type.begin(), type.end(), type.begin(), tolower);
	
		std::string buttons = d_buttons;
		std::transform(buttons.begin(), buttons.end(), buttons.begin(), tolower);
	
		GtkMessageType t;
		if ( type.find ( "warn" ) == 0 ) t = GTK_MESSAGE_WARNING;
		else if ( type.find ( "question" ) == 0 ) t = GTK_MESSAGE_QUESTION;
		else if ( type.find ( "err" ) == 0 ) t = GTK_MESSAGE_ERROR;
		else t = GTK_MESSAGE_INFO;
	
		GtkButtonsType b;
		if ( buttons == "ok-cancel" ) b = GTK_BUTTONS_OK_CANCEL;
		else if ( buttons == "yes-no" ) b = GTK_BUTTONS_YES_NO;
		else if ( buttons == "yes-no-cancel" ) b = GTK_BUTTONS_YES_NO;
		else b = GTK_BUTTONS_OK;
	
		GtkWidget * mbox = gtk_message_dialog_new ( GTK_WINDOW(parent->getImpl()->widget), GtkDialogFlags(GTK_DIALOG_MODAL |
		                                            GTK_DIALOG_DESTROY_WITH_PARENT), t, b, text.c_str(), 0 );
		gtk_window_set_title( GTK_WINDOW(mbox), title.c_str() );
	
		if ( buttons == "yes-no-cancel" ) gtk_dialog_add_buttons ( GTK_DIALOG(mbox), GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, 0);
	
		int retval = gtk_dialog_run( GTK_DIALOG(mbox) );
	
		gtk_widget_destroy(mbox);
	
		if ( (retval == GTK_RESPONSE_CANCEL) || (retval == GTK_RESPONSE_CLOSE) ) return 0;
		else if ( (retval == GTK_RESPONSE_OK) || (retval == GTK_RESPONSE_YES) || (retval == GTK_RESPONSE_APPLY) ) return 1;
		else if ( (retval == GTK_RESPONSE_NO) ) return 2;
		else return -1;
	}
}
