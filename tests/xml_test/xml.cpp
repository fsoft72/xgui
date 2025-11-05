#include "../../xgui/xgui.h"

#include <fstream>

int main()
{
	xgui::Master::InitDynamicClasses();

#ifdef WIN32
	xgui::Master::LoadLibrary("plugin_test.dll");
#else
	xgui::Master::LoadLibrary("./plugin_test.so");
#endif

	std::string dtd = xgui::Master::GenerateDTD();

	std::ofstream f ( "c:\\pippo.dtd" );
	f << dtd;
	f.close();

	xgui::StyleManager * shared_sm = dynamic_cast<xgui::StyleManager*>(xgui::Master::LoadXml("style.xml"));	
	xgui::Model * shared_model = dynamic_cast<xgui::Model*>(xgui::Master::LoadXml("model.xml"));
	xgui::Window * w = dynamic_cast<xgui::Window*>(xgui::Master::LoadXml("gui.xml"));


	w->show();
	xgui::Master::Run();
	return 0;
}
