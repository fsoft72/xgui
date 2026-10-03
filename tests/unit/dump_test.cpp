// Regression test: dump() must escape attribute values so the output stays well formed.
#include <cstdio>
#include <string>

#include "master.h"
#include "window.h"

int main()
{
	xgui::Window * win = xgui::Master::CreateWindow();
	win->set("text", "a<b & \"c\" > d");

	std::string out = win->dump();
	if (out.find("a&lt;b &amp; &quot;c&quot; &gt; d") == std::string::npos) {
		std::fprintf(stderr, "attribute not escaped: %s\n", out.c_str());
		return 1;
	}
	return 0;
}
