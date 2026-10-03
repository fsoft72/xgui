// Regression test: dialogs must work without a parent window (common from Python).
#include <cstdio>
#include <string>

#include <gtk/gtk.h>

#include "master.h"

/** Answers the first open dialog with OK / ACCEPT / CANCEL as appropriate. */
static gboolean answer_dialog(gpointer)
{
	GList * toplevels = gtk_window_list_toplevels();
	for (GList * l = toplevels; l != NULL; l = l->next) {
		if (GTK_IS_DIALOG(l->data)) {
			bool is_message = GTK_IS_MESSAGE_DIALOG(l->data);
			gtk_dialog_response(GTK_DIALOG(l->data), is_message ? GTK_RESPONSE_OK : GTK_RESPONSE_CANCEL);
			break;
		}
	}
	g_list_free(toplevels);
	return G_SOURCE_REMOVE;
}

int main()
{
	// Make sure the Master (and GTK) is initialized
	xgui::Master::Instance();

	g_timeout_add(200, answer_dialog, NULL);
	int rc = xgui::Master::MessageDialog(0, "Title", "100% sure %s %n", "info", "ok");
	if (rc != 1) {
		std::fprintf(stderr, "MessageDialog without parent returned %d\n", rc);
		return 1;
	}

	xgui::FileExtensionsVector filters;
	g_timeout_add(200, answer_dialog, NULL);
	xgui::StrVector files = xgui::Master::OpenFileDialog(0, filters, "Open", "", false);
	if (!files.empty()) return 1;

	g_timeout_add(200, answer_dialog, NULL);
	std::string file = xgui::Master::SaveFileDialog(0, filters, "Save", "", "out.txt");
	if (!file.empty()) return 1;
	return 0;
}
