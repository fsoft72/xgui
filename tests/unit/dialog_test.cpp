// Regression test: dialogs must work without a parent window (common from Python),
// and the synchronous wrappers must return the button chosen in the GTK 4 async dialogs.
#include <cstdio>
#include <string>

#include <gtk/gtk.h>

#include "master.h"

/** Button label to press in the next dialog; empty to close the dialog instead. */
static std::string g_answer;

/** Returns the first button below a widget whose label is the given one, or NULL. */
static GtkWidget * find_button(GtkWidget * w, std::string const &label)
{
	if (GTK_IS_BUTTON(w)) {
		const char * l = gtk_button_get_label(GTK_BUTTON(w));
		if (l && label == l) return w;
	}
	for (GtkWidget * c = gtk_widget_get_first_child(w); c != NULL; c = gtk_widget_get_next_sibling(c)) {
		GtkWidget * found = find_button(c, label);
		if (found) return found;
	}
	return NULL;
}

/** Answers the visible dialog window, retrying until it shows up. */
static gboolean answer_dialog(gpointer)
{
	GListModel * toplevels = gtk_window_get_toplevels();
	GtkWidget * dialog = NULL;
	for (guint i = 0; i < g_list_model_get_n_items(toplevels) && !dialog; ++i) {
		GtkWidget * w = GTK_WIDGET(g_list_model_get_item(toplevels, i));
		if (gtk_widget_get_visible(w)) dialog = w;
		g_object_unref(w);
	}
	if (!dialog) return G_SOURCE_CONTINUE;

	GtkWidget * button = g_answer.empty() ? NULL : find_button(dialog, g_answer);
	if (button)
		g_signal_emit_by_name(button, "clicked");
	else
		gtk_window_close(GTK_WINDOW(dialog));
	return G_SOURCE_REMOVE;
}

/** Shows a message dialog, answers it with a button label and checks the returned code. */
static int expect_message(char const * buttons, char const * answer, int expected)
{
	g_answer = answer;
	g_timeout_add(100, answer_dialog, NULL);
	int rc = xgui::Master::MessageDialog(0, "Title", "100% sure %s %n", "info", buttons);
	if (rc == expected) return 0;
	std::fprintf(stderr, "MessageDialog '%s' answered '%s' returned %d, expected %d\n", buttons, answer, rc, expected);
	return 1;
}

int main()
{
	// Make sure the Master (and GTK) is initialized
	xgui::Master::Instance();

	int failures = 0;
	failures += expect_message("ok", "_OK", 1);
	failures += expect_message("ok", "", -1);
	failures += expect_message("ok-cancel", "_Cancel", 0);
	failures += expect_message("yes-no", "_No", 2);
	failures += expect_message("yes-no-cancel", "_Yes", 1);

	xgui::FileExtensionsVector filters;
	filters.push_back(xgui::FileExtension("Text", "*.txt"));
	g_answer.clear();
	g_timeout_add(100, answer_dialog, NULL);
	xgui::StrVector files = xgui::Master::OpenFileDialog(0, filters, "Open", "", false);
	if (!files.empty()) { std::fprintf(stderr, "closed open dialog returned files\n"); ++failures; }

	g_timeout_add(100, answer_dialog, NULL);
	std::string file = xgui::Master::SaveFileDialog(0, filters, "Save", "/tmp", "out.txt");
	if (!file.empty()) { std::fprintf(stderr, "closed save dialog returned '%s'\n", file.c_str()); ++failures; }
	return failures ? 1 : 0;
}
