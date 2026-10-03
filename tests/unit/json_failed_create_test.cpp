// Regression test: when a JSON node cannot be created, its children must not be created
// (they would become orphan top-level widgets).
#include <cstdio>
#include <fstream>
#include <string>

#include <gtk/gtk.h>

#include "master.h"

/** Returns the number of GTK top-level windows. */
static unsigned int count_toplevels()
{
	GList * toplevels = gtk_window_list_toplevels();
	unsigned int n = g_list_length(toplevels);
	g_list_free(toplevels);
	return n;
}

int main(int argc, char ** argv)
{
	std::string path = std::string(argc > 1 ? argv[1] : ".") + "/failed_create.json";
	std::ofstream(path.c_str()) << "{\"type\": \"button\", \"text\": \"x\", \"children\": [{\"type\": \"window\"}]}";

	xgui::Master::InitDynamicClasses();
	unsigned int before = count_toplevels();

	// A button without a parent cannot be created
	xgui::Object * o = xgui::Master::LoadJson(path, 0);

	unsigned int after = count_toplevels();
	if (o != 0) {
		std::fprintf(stderr, "LoadJson returned an object for a node that cannot be created\n");
		return 1;
	}
	if (after != before) {
		std::fprintf(stderr, "children of a failed node created %u orphan window(s)\n", after - before);
		return 1;
	}
	return 0;
}
