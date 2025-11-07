#include "../../xgui/xgui.h"

#include <fstream>
#include <iostream>

int main(int argc, char* argv[])
{
	xgui::Master::InitDynamicClasses();

#ifdef WIN32
	xgui::Master::LoadLibrary("plugin_test.dll");
#else
	xgui::Master::LoadLibrary("./plugin_test.so");
#endif

	// Default filenames
	std::string gui_file = "gui.json";
	std::string style_file = "style.json";
	std::string model_file = "model.json";

	// If a filename is provided as an argument, use it for the main GUI
	if (argc > 1) {
		gui_file = argv[1];
		std::cout << "Loading GUI from: " << gui_file << std::endl;
	}

	// Load JSON files instead of XML
	xgui::StyleManager * shared_sm = dynamic_cast<xgui::StyleManager*>(xgui::Master::LoadJson(style_file));
	if (!shared_sm) {
		std::cerr << "Failed to load style manager from: " << style_file << std::endl;
	}

	xgui::Model * shared_model = dynamic_cast<xgui::Model*>(xgui::Master::LoadJson(model_file));
	if (!shared_model) {
		std::cerr << "Failed to load model from: " << model_file << std::endl;
	}

	xgui::Window * w = dynamic_cast<xgui::Window*>(xgui::Master::LoadJson(gui_file));
	if (!w) {
		std::cerr << "Failed to load window from: " << gui_file << std::endl;
		return 1;
	}

	w->show();
	xgui::Master::Run();
	return 0;
}
