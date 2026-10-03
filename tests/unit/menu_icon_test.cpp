// Regression test: menu items with an icon must keep working (text access) without GtkImageMenuItem.
#include <cstdio>
#include <string>

#include "master.h"
#include "window.h"
#include "menu.h"
#include "image.h"

int main(int argc, char ** argv)
{
	std::string icon_path = std::string(argc > 2 ? argv[2] : ".") + "/output/icon.png";

	xgui::Window * win = xgui::Master::CreateWindow();
	xgui::Menu * menu = xgui::Master::CreateMenu(win);
	xgui::Image * icon = xgui::Master::LoadImage(icon_path);
	if (!icon) {
		std::fprintf(stderr, "cannot load %s\n", icon_path.c_str());
		return 1;
	}

	menu->addItem("", "file", "&File");
	menu->addItem("file", "open", "&Open", icon);
	menu->addItem("file", "plain", "&Plain");

	if (menu->getItemText("plain") != "Plain") {
		std::fprintf(stderr, "plain item text is '%s'\n", menu->getItemText("plain").c_str());
		return 1;
	}

	menu->setItemText("open", "Opened");
	if (menu->getItemText("open") != "Opened") {
		std::fprintf(stderr, "icon item text is '%s', expected 'Opened'\n", menu->getItemText("open").c_str());
		return 1;
	}
	return 0;
}
