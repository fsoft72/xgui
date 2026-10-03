// Regression test: GtkApplication lifecycle (application id, nested Run/Quit, Quit from a callback).
#include <cstdio>
#include <cstdlib>
#include <string>

#include "master.h"
#include "window.h"
#include "gtk/master_impl.h"

static int g_stage = 0;
static int g_failures = 0;

/** Ends the nested loop. */
static gboolean inner_quit(gpointer)
{
	g_stage = 2;
	xgui::Master::Quit();
	return G_SOURCE_REMOVE;
}

/** Runs a nested loop, then quits the outer one from inside the callback. */
static gboolean outer_step(gpointer)
{
	g_stage = 1;
	g_idle_add(inner_quit, NULL);
	xgui::Master::Run();
	if (g_stage != 2) {
		std::fprintf(stderr, "nested Run returned at stage %d\n", g_stage);
		++g_failures;
	}
	g_stage = 3;
	xgui::Master::Quit();
	return G_SOURCE_REMOVE;
}

/** Fails the test if the loops do not end. */
static gboolean watchdog(gpointer)
{
	std::fprintf(stderr, "Run did not return, stage %d\n", g_stage);
	std::exit(1);
	return G_SOURCE_REMOVE;
}

/** Returns the id of the application of the current Master. */
static std::string current_app_id()
{
	return g_application_get_application_id(G_APPLICATION(xgui::Master::Instance()->getImpl()->app));
}

int main()
{
	std::string default_id = xguimpl::Master::defaultApplicationId();
	if (!g_application_id_is_valid(default_id.c_str()) || default_id.find("org.xgui.") != 0) {
		std::fprintf(stderr, "invalid default application id '%s'\n", default_id.c_str());
		return 1;
	}

	xgui::Master::SetApplicationId("org.example.XguiLifecycle");
	if (current_app_id() != "org.example.XguiLifecycle") {
		std::fprintf(stderr, "application id is '%s'\n", current_app_id().c_str());
		return 1;
	}

	xgui::Window * win = xgui::Master::CreateWindow();
	GtkWindow * gtk_win = GTK_WINDOW(win->getImpl()->widget);
	if (gtk_window_get_application(gtk_win) != xgui::Master::Instance()->getImpl()->app) {
		std::fprintf(stderr, "window is not attached to the application\n");
		return 1;
	}

	g_timeout_add_seconds(10, watchdog, NULL);
	g_idle_add(outer_step, NULL);
	xgui::Master::Run();
	if (g_stage != 3) {
		std::fprintf(stderr, "outer Run returned at stage %d\n", g_stage);
		++g_failures;
	}

	// An invalid id falls back to the default one
	xgui::Master::SetApplicationId("not a valid id");
	if (current_app_id() != default_id) {
		std::fprintf(stderr, "invalid id gave '%s', expected '%s'\n", current_app_id().c_str(), default_id.c_str());
		++g_failures;
	}
	return g_failures ? 1 : 0;
}
