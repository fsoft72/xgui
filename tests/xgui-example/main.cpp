#include "../../xgui/xgui.h"
#include <iostream>
#include <string>

using namespace xgui;

int OnWindowClose(xgui::Widget * w)
{
	Master::Quit();
	return EVT_PROPAGATE;
}
CppFCallback cb_on_window_close(OnWindowClose);

void printUsage(const char* programName)
{
	std::cout << "XGUI Example Viewer - Load and display JSON GUI definitions\n\n";
	std::cout << "Usage: " << programName << " <json-file>\n\n";
	std::cout << "Arguments:\n";
	std::cout << "  <json-file>    Path to JSON file containing GUI definition\n\n";
	std::cout << "Examples:\n";
	std::cout << "  " << programName << " examples/01-simple-button.json\n";
	std::cout << "  " << programName << " output/gui.json\n";
}

int main(int argc, char* argv[])
{
	// Check if a JSON file was provided
	if (argc < 2) {
		printUsage(argv[0]);
		return 1;
	}

	std::string jsonFile = argv[1];

	// Check if file exists by attempting to open it
	std::ifstream testFile(jsonFile.c_str());
	if (!testFile.good()) {
		std::cerr << "Error: Cannot open file '" << jsonFile << "'\n";
		std::cerr << "Please check that the file exists and is readable.\n\n";
		printUsage(argv[0]);
		return 1;
	}
	testFile.close();

	std::cout << "Loading GUI from: " << jsonFile << std::endl;

	// Initialize dynamic classes (required for JSON loading with plugins)
	Master::InitDynamicClasses();

	// Load the JSON file
	Object* loadedObj = Master::LoadJson(jsonFile, 0);

	if (!loadedObj) {
		std::cerr << "Error: Failed to load JSON file '" << jsonFile << "'\n";
		std::cerr << "Please check that the file contains valid JSON GUI definition.\n";
		return 1;
	}

	std::cout << "Successfully loaded: " << loadedObj->className() << std::endl;

	// Check if it's a Window
	Window* window = dynamic_cast<Window*>(loadedObj);

	if (window) {
		// Link close event to quit the application
		window->linkEvent("onclose", &cb_on_window_close);

		// Show the window
		window->show();

		std::cout << "Window displayed. Close the window to exit.\n";

		// Run the main event loop
		Master::Run();
	} else {
		std::cerr << "Warning: Loaded object is not a Window (" << loadedObj->className() << ")\n";
		std::cerr << "The example viewer requires a top-level window object.\n";
		Master::DestroyObject(loadedObj);
		return 1;
	}

	return 0;
}
