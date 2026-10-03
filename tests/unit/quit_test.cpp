// Regression test: after Master::Quit() the singleton must not dangle; a later call re-creates it.
#include <malloc.h>
#include <cstdio>

#include "master.h"
#include "window.h"

int main()
{
	// Fill freed memory with a pattern so that a use after free is not silently harmless
	mallopt(M_PERTURB, 165);

	xgui::Master::Instance();
	xgui::Master::Quit();

	xgui::Window * win = xgui::Master::CreateWindow();
	if (!win) {
		std::fprintf(stderr, "CreateWindow after Quit returned NULL\n");
		return 1;
	}
	return 0;
}
