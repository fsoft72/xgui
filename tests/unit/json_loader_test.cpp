// Regression test: malformed JSON UI descriptions must never throw out of LoadJson.
#include <cstdio>
#include <fstream>
#include <string>

#include "master.h"
#include "object.h"

/** Writes the given JSON text to a file inside dir and returns its path. */
static std::string write_json(std::string const & dir, char const * name, char const * text)
{
	std::string path = dir + "/" + name;
	std::ofstream out(path.c_str());
	out << text;
	return path;
}

/** Loads the file and returns 0 if LoadJson did not throw, 1 otherwise. */
static int load_must_not_throw(std::string const & path, char const * label)
{
	try {
		xgui::Master::LoadJson(path, 0);
	}
	catch (std::exception const & e) {
		std::fprintf(stderr, "%s: LoadJson threw: %s\n", label, e.what());
		return 1;
	}
	catch (...) {
		std::fprintf(stderr, "%s: LoadJson threw an unknown exception\n", label);
		return 1;
	}
	return 0;
}

int main(int argc, char ** argv)
{
	std::string dir = argc > 1 ? argv[1] : ".";
	xgui::Master::InitDynamicClasses();

	int failures = 0;
	failures += load_must_not_throw(write_json(dir, "type_number.json", "{\"type\": 5}"), "non-string type");
	failures += load_must_not_throw(write_json(dir, "type_array.json", "{\"type\": [\"window\"]}"), "array type");
	failures += load_must_not_throw(write_json(dir, "child_bad_type.json",
		"{\"type\": \"window\", \"children\": [{\"type\": {\"a\": 1}}]}"), "child with object type");
	failures += load_must_not_throw(write_json(dir, "top_array.json", "[1, 2, 3]"), "top level array");
	return failures ? 1 : 0;
}
