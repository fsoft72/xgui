// Regression test: JSON numbers must reach string properties as written ("200", not "200.000000").
#include <cstdio>
#include <fstream>
#include <string>

#include "master.h"
#include "window.h"
#include "container.h"

/** Loads a window with a label whose text is the given JSON number and returns the resulting text. */
static std::string label_text_for(std::string const & dir, char const * json_number)
{
	std::string path = dir + "/number.json";
	std::ofstream(path.c_str()) << "{\"type\": \"window\", \"children\": [{\"type\": \"label\", \"id\": \"lbl\", \"text\": " << json_number << "}]}";

	xgui::Object * o = xgui::Master::LoadJson(path, 0);
	xgui::Container * win = dynamic_cast<xgui::Container *>(o);
	if (!win) return "<load failed>";

	xgui::Widget * label = win->findChild("lbl");
	std::string text = "<label not found>";
	if (label) label->get("text", text);
	return text;
}

/** Compares the label text with the expected string; returns 1 on mismatch. */
static int expect_text(std::string const & dir, char const * json_number, char const * expected)
{
	std::string text = label_text_for(dir, json_number);
	if (text != expected) {
		std::fprintf(stderr, "JSON number %s became '%s', expected '%s'\n", json_number, text.c_str(), expected);
		return 1;
	}
	return 0;
}

int main(int argc, char ** argv)
{
	std::string dir = argc > 1 ? argv[1] : ".";
	xgui::Master::InitDynamicClasses();

	int failures = 0;
	failures += expect_text(dir, "200", "200");
	failures += expect_text(dir, "-7", "-7");
	failures += expect_text(dir, "9007199254740993", "9007199254740993");
	failures += expect_text(dir, "1.5", "1.5");
	return failures ? 1 : 0;
}
