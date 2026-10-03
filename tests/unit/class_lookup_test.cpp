// Test: class lookup by name and by tag stays consistent across register/unregister.
#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

#include "master.h"
#include "class_info.h"

/** Prints the message and returns 1 when the condition is false. */
static int expect(bool condition, char const * message)
{
	if (condition) return 0;
	std::fprintf(stderr, "%s\n", message);
	return 1;
}

int main()
{
	xgui::Master::InitDynamicClasses();

	int failures = 0;
	xgui::ClassInfo * label = xgui::Master::FindClass("xgui::Label");
	failures += expect(label != 0, "FindClass(xgui::Label) not found");
	failures += expect(label && xgui::Master::FindClassByTag(label->tag()) == label, "FindClassByTag does not return the same class");
	failures += expect(xgui::Master::FindClass("no::Such") == 0, "unknown class name found");
	failures += expect(xgui::Master::FindClassByTag("no-such-tag") == 0, "unknown tag found");

	std::vector<std::string> names = xgui::Master::availableClasses();
	failures += expect(std::find(names.begin(), names.end(), "xgui::Label") != names.end(), "availableClasses misses xgui::Label");
	std::vector<std::string> sorted = names;
	std::sort(sorted.begin(), sorted.end());
	failures += expect(std::unique(sorted.begin(), sorted.end()) == sorted.end(), "availableClasses has duplicates");

	if (label) {
		std::string tag = label->tag();
		xgui::Master::UnregisterClass("xgui::Label");
		failures += expect(xgui::Master::FindClass("xgui::Label") == 0, "class still found by name after unregister");
		failures += expect(xgui::Master::FindClassByTag(tag) == 0, "class still found by tag after unregister");

		xgui::Master::RegisterClass(label);
		failures += expect(xgui::Master::FindClass("xgui::Label") == label, "class not found by name after register");
		failures += expect(xgui::Master::FindClassByTag(tag) == label, "class not found by tag after register");
	}
	return failures ? 1 : 0;
}
