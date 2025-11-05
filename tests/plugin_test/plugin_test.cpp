#include "../../xgui/xgui.h"

#define CLASS_NAME		"PluginLabel"

using namespace xgui;


struct PluginLabelClass : public xgui::LabelClass
{
	xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);
	std::string const &className();
};

static PluginLabelClass class_info;


class PluginLabel : public xgui::Label
{
public:
	PluginLabel(xgui::Container * parent, const std::string & text) : xgui::Label(parent, &class_info, text)
	{
		post_construction();
	}

	~PluginLabel()
	{
	}



};


xgui::Object * PluginLabelClass::create(xgui::Object * parent, xgui::StringMap &properties)
{
	return new PluginLabel(dynamic_cast<xgui::Container *>(parent), "John");
}

std::string const &PluginLabelClass::className()
{
	static std::string name ( CLASS_NAME );
	return name;
}


extern "C"
void xgui_module_init()
{
	Master::RegisterClass ( &class_info );
}


extern "C"
void xgui_module_close()
{
	xgui::Master::UnregisterClass ( CLASS_NAME );
}

