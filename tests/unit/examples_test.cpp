// Smoke test: every JSON example must load into an object.
#include <cstdio>
#include <dirent.h>
#include <string>
#include <vector>

#include "master.h"
#include "object.h"

int main(int argc, char ** argv)
{
	std::string dir = std::string(argc > 2 ? argv[2] : ".") + "/examples";
	xgui::Master::InitDynamicClasses();

	DIR * d = opendir(dir.c_str());
	if (!d) {
		std::fprintf(stderr, "cannot open %s\n", dir.c_str());
		return 1;
	}

	int failures = 0;
	int loaded = 0;
	for (dirent * e = readdir(d); e != NULL; e = readdir(d)) {
		std::string name = e->d_name;
		if (name.size() < 5 || name.substr(name.size() - 5) != ".json") continue;

		xgui::Object * o = xgui::Master::LoadJson(dir + "/" + name, 0);
		if (!o) {
			std::fprintf(stderr, "%s did not load\n", name.c_str());
			++failures;
		}
		else
			++loaded;
	}
	closedir(d);

	if (loaded == 0) {
		std::fprintf(stderr, "no example loaded\n");
		return 1;
	}
	return failures ? 1 : 0;
}
