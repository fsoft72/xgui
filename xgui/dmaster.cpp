//
// C++ Implementation: xml
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "precomp.h"

#ifndef WIN32
#	include <stack>
#	include <algorithm>

#	include "master.h"
#	include "dynamic.h"

#	include "xgui.h"

#	include "json.hpp"

#	include <dlfcn.h>

#	include <fstream>
#endif

static std::string trim(std::string const& source, char const* delims = " \t\r\n") {
  std::string result(source);
  std::string::size_type index = result.find_last_not_of(delims);
  if(index != std::string::npos)
    result.erase(++index);

  index = result.find_first_not_of(delims);
  if(index != std::string::npos)
    result.erase(0, index);
  else
    result.erase();
  return result;
}

namespace xgui
{
	void Master::SaveXml(xgui::Object * root, std::string const &filename)
	{
		std::string dump = root->dump();
		std::ofstream file(filename.c_str());

		file << dump;

		file.close();
	}

	xgui::ClassInfo * Master::FindClass(std::string const &node)
	{
		ClassInfoList::iterator c;
		for(c = Instance()->class_info_.begin(); c != Instance()->class_info_.end(); ++c) {
			if ((*c)->className() == node)
				return *c;
		}

		return 0;
	}

	xgui::ClassInfo * Master::FindClassByTag(std::string const &tag)
	{
		ClassInfoList::iterator c;
		for(c = Instance()->class_info_.begin(); c != Instance()->class_info_.end(); ++c) {
			std::string const &c_tag = (*c)->tag();

			if (c_tag == tag)
				return *c;
		}

		return 0;
	}

	std::vector<std::string> Master::availableClasses()
	{
		ClassInfoList::iterator c;
		std::vector<std::string> classes;
		for(c = Instance()->class_info_.begin(); c != Instance()->class_info_.end(); ++c)
			classes.push_back((*c)->className());
		return classes;
	}

	void Master::RegisterClass(xgui::ClassInfo * ginfo)
	{
		Instance()->class_info_.insert(ginfo);
	} 

	void Master::UnregisterClass(std::string const &node) 
	{
		ClassInfoList::iterator c;
		for(c = Instance()->class_info_.begin(); c != Instance()->class_info_.end(); ++c) {
			if ((*c)->className() == node) {
				Instance()->class_info_.erase(c);
				return;
			}
		}
	}

	xgui::Object * Master::CreateObject(std::string const &class_name, xgui::Object * parent, xgui::StringMap const &properties)
	{
		xgui::ClassInfo * cinfo = Instance()->FindClass(class_name);
		if (cinfo && cinfo->isInstanceable()) {
			xgui::StringMap pmap = properties;
			xgui::Object * o = cinfo->create(parent, pmap);
			return o;
		}
	
		XGUI_ERROR("Trying to create a non instanceable object");
		return 0;
	}

	xgui::Object * Master::FinalizeObject(xgui::Object * o)
	{
		xgui::ClassInfo * cinfo = Instance()->FindClass(o->className());
		if (cinfo->mustFinalize())
			cinfo->finalize(o);
		return o;
	}

	DLLEXPORT void Master::DestroyObject(xgui::Object * object)
	{
		if (object->getClass()->isWidget()) { 
			xgui::Widget * w = dynamic_cast<xgui::Widget*>(object);
			ASSERT(w); 
			Master::DestroyWidget(w); 
		}
		else if (object->getClass()->isModel()) { 
			xgui::Model * m = dynamic_cast<xgui::Model*>(object);
			ASSERT(m); 
			Master::DestroyModel(m); 
		}
		else if (object->getClass()->isStyleManager()) { 
			xgui::StyleManager * s = dynamic_cast<xgui::StyleManager*>(object);
			ASSERT(s); 
			Master::DestroyStyleManager(s); 
		}
		else 
			delete object;
	}

	// XML support removed - use LoadJson instead
	xgui::Object * Master::ParseXmlNode(xgui::Object * parent_object, void * n)
	{
		XGUI_ERROR("XML support has been removed. Please use LoadJson() instead.");
		return 0;
	}

	DLLEXPORT xgui::Object * xgui::Master::LoadXml(std::string const &file, xgui::Object * parent)
	{
		XGUI_ERROR("XML support has been removed. Please use LoadJson() instead.");
		return 0;
	}

	xgui::Object * Master::ParseJsonValue(xgui::Object * parent_object, const nlohmann::json &j)
	{
		xgui::Object * result = 0;

		xgui::StringMap properties;
		std::vector<nlohmann::json> children;

		// Get the widget type from the "type" field
		if (!j.contains("type")) {
			XGUI_ERROR("JSON object missing 'type' field");
			return 0;
		}

		std::string node_name = j["type"];
		xgui::ClassInfo * class_builder = 0;

		class_builder = Instance()->FindClassByTag(node_name);
		if (class_builder == 0) {
			XGUI_ERROR("Unable to find a class builder for node " << node_name);
			return 0;
		}

		// Parse all properties except "type" and "children"
		for (auto& el : j.items()) {
			const std::string& key = el.key();

			if (key == "type" || key == "children")
				continue;

			if (el.value().is_string()) {
				properties[key] = el.value().get<std::string>();
			}
			else if (el.value().is_number()) {
				properties[key] = std::to_string(el.value().get<double>());
			}
			else if (el.value().is_boolean()) {
				properties[key] = el.value().get<bool>() ? "1" : "0";
			}
			else {
				// For complex types, convert to string representation
				properties[key] = el.value().dump();
			}
		}

		// Get children array if present
		if (j.contains("children") && j["children"].is_array()) {
			for (const auto& child : j["children"]) {
				children.push_back(child);
			}
		}

		if (class_builder != 0) {
			result = class_builder->create(parent_object, properties);

			if (result) {
				for (xgui::StringMap::iterator p = properties.begin(); p != properties.end(); ++p) {
					if (result->hasProperty(p->first) == xgui::PROPERTY_NOT_FOUND) {
						XGUI_ERROR("Trying to set a not available property: " << p->first);
					}
					else
						result->set(p->first, p->second);
				}
			}

			for (std::vector<nlohmann::json>::iterator i = children.begin(); i != children.end(); ++i)
				ParseJsonValue(result, *i);

			if (class_builder->mustFinalize())
				class_builder->finalize(result);
		}

		return result;
	}

	DLLEXPORT xgui::Object * xgui::Master::LoadJson(std::string const &file, xgui::Object * parent)
	{
		std::ifstream input_file(file.c_str());

		if (!input_file.is_open()) {
			XGUI_ERROR("Unable to open JSON file: " << file);
			return 0;
		}

		nlohmann::json j;
		try {
			input_file >> j;
		}
		catch (const std::exception& e) {
			XGUI_ERROR("JSON parse error: " << e.what());
			return 0;
		}

		xgui::Object * o = ParseJsonValue(parent, j);

		return o;
	}

	//------------------ TOOLBAR CLASSES INFORMATIONS -----------------------//
	struct ToolbarSeparatorClass : public xgui::ClassInfo
	{
		ToolbarSeparatorClass() : ClassInfo() {}

		virtual bool isInstanceable() { return false; }
		virtual bool mustFinalize() { return false; }
		virtual std::string const &className() { static std::string const c_name = "xml::ToolbarSeparator"; return c_name; }

		virtual bool isContainer() { return false; }
		virtual bool isWidget() { return false; }
		virtual bool isModel() { return false; }
		virtual bool isStyleManager() { return false; }

		bool canContain(xgui::ClassInfo * c) { return false; }
		bool canBeContainedIn(xgui::ClassInfo * c) 
		{
			if (dynamic_cast<xgui::ToolbarClass*>(c)) return true;
			else return false;
		}

		void finalize(xgui::Object * o) {}
		
		xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties)
		{
			xgui::Toolbar * w = dynamic_cast<xgui::Toolbar*>(parent);

			if (!parent) {
				XGUI_ERROR("Cannot create a ToolbarSeparator without parent");
				return 0;
			}

			if (!w) {
				XGUI_ERROR("Cannot create a ToolbarSeparator as a child of a " << parent->className());
				return 0;
			}

			w->addItem( "", "", 0, "", "separator" );

			return 0;
		}
	};

	struct ToolbarButtonClass : public xgui::ClassInfo
	{
		ToolbarButtonClass() : ClassInfo() 
		{
			registerInitProperty("id", false);
			registerInitProperty("text", false);
			registerInitProperty("image", false);
			registerInitProperty("tooltip", false);
		}

		virtual bool isInstanceable() { return false; }
		virtual bool mustFinalize() { return false; }
		virtual std::string const &className() { static std::string const c_name = "xml::ToolbarButton"; return c_name; }

		virtual bool isContainer() { return false; }
		virtual bool isWidget() { return false; }
		virtual bool isModel() { return false; }
		virtual bool isStyleManager() { return false; }

		bool canContain(xgui::ClassInfo * c) { return false; }
		bool canBeContainedIn(xgui::ClassInfo * c) 
		{
			if (dynamic_cast<xgui::ToolbarClass*>(c)) return true;
			else return false;
		}

		void finalize(xgui::Object * o) {}
		
		xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties)
		{
			xgui::Toolbar * w = dynamic_cast<xgui::Toolbar*>(parent);

			if (!parent) {
				XGUI_ERROR("Cannot create a ToolbarButton without parent");
				return 0;
			}

			if (!w) {
				XGUI_ERROR("Cannot create a ToolbarButton as a child of a " << parent->className());
				return 0;
			}

			xgui::StringMap::iterator prop_i;
	
			std::string id = "";
			std::string text = "";
			xgui::Image * image = 0;
			std::string tooltip = "";
	
			if ( (prop_i = properties.find("id")) != properties.end() ) {
				id = prop_i->second;
				properties.erase(prop_i);
			}
			
			if ( (prop_i = properties.find("text")) != properties.end() ) {
				text = prop_i->second;
				properties.erase(prop_i);
			}
	
			if ( (prop_i = properties.find("image")) != properties.end() ) {
				image = xgui::Master::LoadImage(prop_i->second);
				properties.erase(prop_i);
			}
	
			if ( (prop_i = properties.find("tooltip")) != properties.end() ) {
				tooltip = prop_i->second;
				properties.erase(prop_i);
			}
	
			w->addItem( id, text, image, tooltip, "button" );

			return 0;
		}
	};

	struct ToolbarRadioGroupClass : public xgui::ClassInfo
	{
		ToolbarRadioGroupClass() : ClassInfo() {}

		virtual bool isInstanceable() { return false; }
		virtual bool mustFinalize() { return true; }
		virtual std::string const &className() { static std::string const c_name = "xml::ToolbarRadioGroup"; return c_name; }

		virtual bool isContainer() { return false; }
		virtual bool isWidget() { return false; }
		virtual bool isModel() { return false; }
		virtual bool isStyleManager() { return false; }


		bool canBeContainedIn(xgui::ClassInfo * c) 
		{
			if (dynamic_cast<xgui::ToolbarClass*>(c)) return true;
			else return false;
		}
		bool canContain(xgui::ClassInfo * c);

		xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties)
		{
			xgui::Toolbar * w = dynamic_cast<xgui::Toolbar*>(parent);
	
			if (!parent) {
				XGUI_ERROR("Cannot create a ToolbarRadioGroup without parent");
				return 0;
			}
	
			if (!w) {
				XGUI_ERROR("Cannot create a ToolbarRadioGroup as a child of a " << parent->className());
				return 0;
			}
	
			w->addItem("", "", 0, "", "radio_begin");
			return w;
		}

		void finalize(xgui::Object * object)
		{
			if(!object) return;
	
			xgui::Toolbar * w = dynamic_cast<xgui::Toolbar*>(object);
			w->addItem("", "", 0, "", "radio_end");
		}
	};

	struct ToolbarCheckButtonClass : public xgui::ClassInfo
	{
		ToolbarCheckButtonClass() : ClassInfo() 
		{
			registerInitProperty("id", false);
			registerInitProperty("text", false);
			registerInitProperty("image", false);
			registerInitProperty("tooltip", false);
		}

		virtual bool isInstanceable() { return false; }
		virtual bool mustFinalize() { return false; }
		virtual std::string const &className() { static std::string const c_name = "xml::ToolbarCheckButton"; return c_name; }

		virtual bool isContainer() { return false; }
		virtual bool isWidget() { return false; }
		virtual bool isModel() { return false; }
		virtual bool isStyleManager() { return false; }

		bool canContain(xgui::ClassInfo * c) { return false; }
		bool canBeContainedIn(xgui::ClassInfo * c) 
		{
			if (dynamic_cast<xgui::ToolbarClass*>(c)) return true;
			else if (dynamic_cast<xgui::ToolbarRadioGroupClass*>(c)) return true;
			else return false;
		}

		void finalize(xgui::Object * o) {}
		
		xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties)
		{
			xgui::Toolbar * w = dynamic_cast<xgui::Toolbar*>(parent);
	
			if (!parent) {
				XGUI_ERROR("Cannot create a ToolbarCheckButton without parent");
				return 0;
			}

			if (!w) {
				XGUI_ERROR("Cannot create a ToolbarCheckButton as a child of a " << parent->className());
				return 0;
			}

			xgui::StringMap::iterator prop_i;
	
			std::string id = "";
			std::string text = "";
			xgui::Image * image = 0;
			std::string tooltip = "";
	
			if ( (prop_i = properties.find("id")) != properties.end() ) {
				id = prop_i->second;
				properties.erase(prop_i);
			}
			
			if ( (prop_i = properties.find("text")) != properties.end() ) {
				text = prop_i->second;
				properties.erase(prop_i);
			}
	
			if ( (prop_i = properties.find("image")) != properties.end() ) {
				image = xgui::Master::LoadImage(prop_i->second);
				properties.erase(prop_i);
			}
	
			if ( (prop_i = properties.find("tooltip")) != properties.end() ) {
				tooltip = prop_i->second;
				properties.erase(prop_i);
			}
	
			w->addItem( id, text, image, tooltip, "check" );

			return 0;
		}
	};

	bool ToolbarRadioGroupClass::canContain(xgui::ClassInfo * c) 
	{
		if (dynamic_cast<xgui::ToolbarCheckButtonClass*>(c)) return true;
		else return false; 
	}

	ToolbarRadioGroupClass tb_radio_group_class;
	ToolbarButtonClass tb_btn_class;
	ToolbarCheckButtonClass tb_check_btn_class;
	ToolbarSeparatorClass tb_separator_class;

	std::string const &ToolbarClass::className() { static std::string const c_name = "xgui::Toolbar"; return c_name; }

	bool ToolbarClass::isInstanceable() { return true; }

	bool ToolbarClass::canContain(xgui::ClassInfo * c) 
	{ 
		if (dynamic_cast<xgui::ToolbarButtonClass*>(c)) return true;
		else if (dynamic_cast<xgui::ToolbarSeparatorClass*>(c)) return true;
		else if (dynamic_cast<xgui::ToolbarCheckButtonClass*>(c)) return true;
		else if (dynamic_cast<xgui::ToolbarRadioGroupClass*>(c)) return true;
		else return false; 
	}
	
	xgui::Object * ToolbarClass::create(xgui::Object * parent, xgui::StringMap &properties)
	{
		xgui::StringMap::iterator prop_i;
		xgui::Container * wparent = dynamic_cast<xgui::Container*>( parent );
		xgui::Toolbar * w = 0;

		if (!parent) {
			XGUI_ERROR("Cannot create a Toolbar without parent");
			return 0;
		}

		if (!wparent) {
			XGUI_ERROR("Cannot create a Toolbar as a child of a " << parent->className());
			return 0;
		}

		w = xgui::Master::CreateToolbar(wparent);

		return w;
	}
	//-------------------END TOOLBAR CLASSES INFORMATIONS ------------------//


	//-------------------BEGIN TAB CLASSES INFORMATIONS -------------------//
	class TabPage : public xgui::Object
	{	
		public:
			std::string label_;
			std::string image_;

			TabPage(std::string const &label, std::string const &image);
			~TabPage() {}

			std::string dump() { return ""; }
	};	

	struct TabPageClass : public xgui::ContainerClass
	{
		TabPageClass() : ContainerClass() 
		{
			registerInitProperty("label", true);
			registerInitProperty("icon", false);
		}

		virtual bool isInstanceable() { return false; }
		virtual bool mustFinalize() { return true; }
		virtual std::string const &className() { static std::string const c_name = "xml::TabPage"; return c_name; }

		virtual bool isContainer() { return false; }
		virtual bool isWidget() { return false; }
		virtual bool isModel() { return false; }
		virtual bool isStyleManager() { return false; }

		bool canContain(xgui::ClassInfo * c) 
		{
			return (dynamic_cast<xgui::WidgetClass*>(c) != 0);
		}

		bool canBeContainedIn(xgui::ClassInfo * c) 
		{
			if (dynamic_cast<xgui::TabClass*>(c)) return true;
			else return false;
		}

		xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties)
		{
			xgui::Container * cont = dynamic_cast<xgui::Container*>(parent);
			xgui::StringMap::iterator prop_i;

			xgui::Tab * tb = dynamic_cast<xgui::Tab*>(cont);

			if (!parent) {
				XGUI_ERROR("Cannot create a TabPage without parent");
				return 0;
			}

			if ( (!tb) ) {
				XGUI_ERROR("Cannot create a TabPage outside of a Tab");
				return 0;
			}

			std::string label = "0";
			if ( (prop_i = properties.find("label")) != properties.end() ) {
				label = prop_i->second;
				properties.erase(prop_i);
			}

			std::string icon = "";
			if ( (prop_i = properties.find("icon")) != properties.end() ) {
				icon = prop_i->second;
				properties.erase(prop_i);
			}

			cont->store("page", new TabPage(label, icon));

			return parent;
		}
	
		void finalize(xgui::Object * o) 
		{
			xgui::Container * cont = dynamic_cast<xgui::Container*>(o);
			TabPage * page = dynamic_cast<TabPage*>(cont->retrieve("page"));
			
			int size = cont->size();
			if (size) {
				xgui::Widget * w = cont->getChild(size-1);
				w->set("tab-label", page->label_);	
				w->set("tab-icon", page->image_);
			}

			delete page;
			cont->unstore("page");
		}
	};

	TabPageClass tab_page_class;

	TabPage::TabPage(std::string const &label, std::string const &image) 
	: Object(&tab_page_class), label_(label), image_(image) {}

	std::string const &TabClass::className() { static std::string const c_name = "xgui::Tab"; return c_name; }

	bool TabClass::isInstanceable() { return true; }

	bool TabClass::canContain(xgui::ClassInfo * c) 
	{
		return (dynamic_cast<xgui::TabPageClass*>(c) != 0);
	}

	xgui::Object * TabClass::create(xgui::Object * parent, xgui::StringMap &properties)
	{
		xgui::StringMap::iterator prop_i;
		xgui::Container * wparent = dynamic_cast<xgui::Container*>( parent );
		xgui::Tab * w = 0;

		if (!parent) {
			XGUI_ERROR("Cannot create a Tab without parent");
			return 0;
		}

		if (!wparent) {
			XGUI_ERROR("Cannot create a Tab as a child of a " << parent->className());
			return 0;
		}

		std::string align = "";

		if ( (prop_i = properties.find("align")) != properties.end() ) {
			align = prop_i->second;
			properties.erase(prop_i);
		}

		w = xgui::Master::CreateTab(wparent, align);

		return w;
	}
	//------------------------ END TAB CLASSES INFORMATIONS ------------------//

	//------------------------ BEGIN MODEL CLASSES INFORMATIONS ----------------//
	struct ModelEntryClass : public xgui::ClassInfo
	{
		ModelEntryClass() : ClassInfo() { registerInitProperty("text", false); }

		virtual bool isInstanceable() { return false; }
		virtual bool mustFinalize() { return false; }
		virtual std::string const &className() { static std::string const c_name = "xml::ModelEntry"; return c_name; }

		virtual bool isContainer() { return false; }
		virtual bool isWidget() { return false; }
		virtual bool isModel() { return true; }
		virtual bool isStyleManager() { return false; }

		bool canContain(xgui::ClassInfo * c) 
		{
			return false;
		}

		bool canBeContainedIn(xgui::ClassInfo * c) 
		{
			if (dynamic_cast<xgui::ModelClass*>(c)) return true;
			else return false;
		}

		void finalize(xgui::Object * o) {}
		
		xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties)
		{
			xgui::StringMap::iterator prop_i;
			xgui::Model * mparent = dynamic_cast<xgui::Model*>( parent );

			if (!parent) {
				XGUI_ERROR("Cannot create a ModelEntry without parent");
				return 0;
			}

			if ( (!mparent) ) {
				XGUI_ERROR("Cannot create a ModelEntry as a child of a " << parent->className());
				return 0;
			}

			std::string text;	

			if ( (prop_i = properties.find("text")) != properties.end() ) {
				text = prop_i->second;
				properties.erase(prop_i);
			}

			mparent->addString(text);

			return 0;
		}
	};

	struct ListDataClass : public ModelClass 
	{ 
		ListDataClass() : ModelClass() {}
		bool isInstanceable() { return false; }
		std::string const &className() { static std::string const c_name = "xml::ListData"; return c_name; }

	};

	struct TreeDataClass : public ModelClass 
	{
		TreeDataClass() : ModelClass() {}
		bool isInstanceable() { return false; }
		std::string const &className() { static std::string const c_name = "xml::TreeData"; return c_name; }
	};

	struct ComboboxDataClass : public ModelClass
	{
		ComboboxDataClass() : ModelClass() {}
		bool isInstanceable() { return false; }
		std::string const &className() { static std::string const c_name = "xml::ComboboxData"; return c_name; }
	};

	struct lrClass : public ModelClass 
	{
		lrClass() : ModelClass() {}
		bool isInstanceable() { return false; }
		std::string const &className() { static std::string const c_name = "xml::lrow"; return c_name; }
	 };

	struct liClass : public ModelEntryClass
	{
		liClass() : ModelEntryClass() {}
		std::string const &className() { static std::string const c_name = "xml::litem"; return c_name; }
	};

	struct ModelNodeClass : public xgui::ClassInfo
	{
		ModelNodeClass() : ClassInfo() 
		{
			registerInitProperty("text", false);
		}

		virtual bool isInstanceable() { return false; }
		virtual bool mustFinalize() { return false; }
		virtual std::string const &className() { static std::string const c_name = "xml::ModelNode"; return c_name; }

		virtual bool isContainer() { return false; }
		virtual bool isWidget() { return false; }
		virtual bool isModel() { return true; }
		virtual bool isStyleManager() { return false; }

		void finalize(xgui::Object * o) {}

		bool canContain(xgui::ClassInfo * c) 
		{
			return (dynamic_cast<xgui::ModelNodeClass*>(c) != 0);
		}

		bool canBeContainedIn(xgui::ClassInfo * c) 
		{
			if (dynamic_cast<xgui::ModelNodeClass*>(c)) return true;
			else if (dynamic_cast<xgui::ModelClass*>(c)) return true;
			else return false;
		}

		xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties)
		{
			xgui::StringMap::iterator prop_i;
			xgui::Model * mparent = dynamic_cast<xgui::Model*>( parent );

			if (!parent) {
				XGUI_ERROR("Cannot create a ModelNode without parent");
				return 0;
			}

			if ( (!mparent) ) {
				XGUI_ERROR("Cannot create a ModelNode as a child of a " << parent->className());
				return 0;
			}

			std::string text;	

			if ( (prop_i = properties.find("text")) != properties.end() ) {
				text = prop_i->second;
				properties.erase(prop_i);
			}

			xgui::Model * m = xgui::Master::CreateModel();
			m->addString(text);
			mparent->appendChild(m);

			return m;
		}
	};

	struct CBoxItemClass : public ModelNodeClass
	{
		CBoxItemClass() : ModelNodeClass() {}
		std::string const &className() { static std::string const c_name = "xml::CBoxItem"; return c_name; }
	};

	struct TreeNodeClass : public ModelNodeClass
	{
		TreeNodeClass() : ModelNodeClass() { }
		std::string const &className() { static std::string const c_name = "xml::TreeNode"; return c_name; }
	};

	std::string const &ModelClass::className() { static std::string const c_name = "xgui::Model"; return c_name; }

	bool ModelClass::isInstanceable() { return true; }

	bool ModelClass::isModel() { return true; }

	bool ModelClass::mustFinalize() { return false; }

	bool ModelClass::canContain(xgui::ClassInfo * c) 
	{
		if (dynamic_cast<xgui::ModelClass*>(c)) return true;
		else if (dynamic_cast<xgui::ModelEntryClass*>(c)) return true;
		else if (dynamic_cast<xgui::ModelNodeClass*>(c)) return true;
		else return false;
	}

	bool ModelClass::canBeContainedIn(xgui::ClassInfo * c) 
	{
		if (dynamic_cast<xgui::ModelClass*>(c)) return true;
		else if (dynamic_cast<xgui::ViewClass*>(c)) return true;
		else return false;
	}

	xgui::Object * ModelClass::create(xgui::Object * parent, xgui::StringMap &properties)
	{
		xgui::StringMap::iterator prop_i;
		xgui::View * vparent = dynamic_cast<xgui::View*>( parent );
		xgui::Model * mparent = dynamic_cast<xgui::Model*>( parent );

		if ( (parent) && ((!vparent) && (!mparent)) ) {
			XGUI_ERROR("Cannot create a Model as a child of a " << parent->className());
			return 0;
		}

		std::string style;
		if ( (prop_i = properties.find("style")) != properties.end() ) {
			style = prop_i->second;
			properties.erase(prop_i);
		}

		xgui::Model * m = xgui::Master::CreateModel();
		m->setStyle(style);

		if (parent) {
			xgui::Model * mparent = dynamic_cast<xgui::Model*>( parent );
			xgui::View * vparent = dynamic_cast<xgui::View*>( parent );
	
			if (vparent)
				vparent->setModel(m);
			else if(mparent)
				mparent->appendChild(m);
		}

		return m;
	}

	void ModelClass::finalize(xgui::Object * o) { }

	xgui::ModelEntryClass mentry_class;
	xgui::ListDataClass list_data_class;
	xgui::TreeDataClass tree_data_class;
	xgui::ComboboxDataClass cbox_data_class;
	xgui::lrClass lr_class;
	xgui::liClass li_class;
	xgui::ModelNodeClass mnode_class;
	xgui::CBoxItemClass cbitem_class;
	xgui::TreeNodeClass tree_node_class;
	//--------------------------- END MODEL CLASSES INFORMATIONS -------------------//

	//--------------------------- BEGIN MENU CLASSES INFORMATIONS -------------------//
	class MenuItem : public xgui::Object
	{	
		public:
			xgui::Menu * mnu;
			std::string item_parent;

			MenuItem(xgui::Menu * m, std::string const &id);
			~MenuItem() { }

			std::string dump() { return ""; }
	};

	struct MenuItemClass : public xgui::ClassInfo
	{
		MenuItemClass() : ClassInfo() 
		{
			registerInitProperty("id", true);
			registerInitProperty("text", false);
			registerInitProperty("icon", false);
			registerInitProperty("type", false);
		}

		virtual bool isInstanceable() { return false; }
		virtual bool mustFinalize() { return true; }
		virtual std::string const &className() { static std::string const c_name = "xml::MenuItem"; return c_name; }

		virtual bool isContainer() { return false; }
		virtual bool isWidget() { return false; }
		virtual bool isModel() { return false; }
		virtual bool isStyleManager() { return false; }

		bool canContain(xgui::ClassInfo * c) 
		{
			if (dynamic_cast<xgui::MenuItemClass*>(c)) return true;
			else return false; 
		}

		bool canBeContainedIn(xgui::ClassInfo * c) 
		{
			if (dynamic_cast<xgui::MenuClass*>(c)) return true;
			else if (dynamic_cast<xgui::MenuItemClass*>(c)) return true;
			else return false;
		}

		xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties)
		{
			xgui::Menu * menu = dynamic_cast<xgui::Menu*>(parent);
			MenuItem * menu_item = dynamic_cast<MenuItem*>(parent);

			if (!parent) {
				XGUI_ERROR("Cannot create a MenuItem without parent");
				return 0;
			}

			if ( (!menu_item) && (!menu) ) {
				XGUI_ERROR("Cannot create a MenuItem as a child of a " << parent->className());
				return 0;
			}

			std::string item_parent;
			xgui::StringMap::iterator prop_i;
	
			if (menu_item) {
				item_parent = menu_item->item_parent;
				menu = menu_item->mnu;
			}

			std::string id = "";
			std::string text = "";
			xgui::Image * icon = 0;
			std::string type = "";
	
			if ( (prop_i = properties.find("id")) != properties.end() ) {
				id = prop_i->second;
				properties.erase(prop_i);
			}
			
			if ( (prop_i = properties.find("text")) != properties.end() ) {
				text = prop_i->second;
				properties.erase(prop_i);
			}
	
			if ( (prop_i = properties.find("icon")) != properties.end() ) {
				icon = xgui::Master::LoadImage(prop_i->second);
				properties.erase(prop_i);
			}
	
			if ( (prop_i = properties.find("type")) != properties.end() ) {
				type = prop_i->second;
				properties.erase(prop_i);
			}

			menu_item = new MenuItem(menu, id);	
			menu->addItem( item_parent, id, text, icon, type );
	
			return menu_item;
		}

		void finalize(xgui::Object * o) 
		{
			if(!o) return;

			MenuItem * menu_item = dynamic_cast<MenuItem*>(o);
			delete menu_item;
		}
	};
	MenuItemClass mnu_item_class;

	MenuItem::MenuItem(xgui::Menu * m, std::string const &id) 
	: Object(&mnu_item_class), mnu(m), item_parent(id) {}

	MenuClass::MenuClass() : WidgetClass()
	{
		registerInitProperty("visible", false);

		registerMethod("addItem", new DMethod5< Type<int>, xgui::Menu, Ref<Const<Type<std::string> > >,
		                                Ref<Const<Type<std::string> > >, Ref<Const<Type<std::string> > >,
		                                Ptr<Type<xgui::Image> >, Ref<Const<Type<std::string> > > >
		                                (&xgui::Menu::addItem));
		registerMethod("delItem", new DMethod1< Type<int>, xgui::Menu, Ref<Const<Type<std::string> > > >
		                              (&xgui::Menu::delItem));

		registerMethod("popupMenu", new DMethod1< Type<int>, xgui::Menu, Ref<Const<Type<std::string> > > >
		                              (&xgui::Menu::popupMenu));

		registerMethod("enableItem", new DMethod1< Type<int>, xgui::Menu, Ref<Const<Type<std::string> > > >
		                              (&xgui::Menu::enableItem));
		registerMethod("disableItem", new DMethod1< Type<int>, xgui::Menu, Ref<Const<Type<std::string> > > >
		                              (&xgui::Menu::disableItem));
		registerMethod("isItemEnabled", new DMethod1<Type<bool>,xgui::Menu,Ref<Const<Type<std::string> > > >
		                              (&xgui::Menu::isItemEnabled));

		registerMethod("checkItem", new DMethod1< Type<int>, xgui::Menu, Ref<Const<Type<std::string> > > >
		                              (&xgui::Menu::checkItem));
		registerMethod("uncheckItem", new DMethod1< Type<int>, xgui::Menu, Ref<Const<Type<std::string> > > >
 		                              (&xgui::Menu::uncheckItem));
		registerMethod("isItemChecked", new DMethod1<Type<bool>,xgui::Menu,Ref<Const<Type<std::string> > > >
		                              (&xgui::Menu::isItemChecked));

		registerMethod("setItemText", new DMethod2<Type<int>, xgui::Menu, Ref<Const<Type<std::string> > >,
		                              Ref<Const<Type<std::string> > > >(&xgui::Menu::setItemText));
		registerMethod("getItemText", new DMethod1<Type<std::string>, xgui::Menu, 
		                              Ref<Const<Type<std::string> > > >(&xgui::Menu::getItemText));
	}

	std::string const &MenuClass::className() { static std::string const c_name = "xgui::Menu"; return c_name; }

	bool MenuClass::isInstanceable() { return true; }

	bool MenuClass::canContain(xgui::ClassInfo * c) 
	{
		if (dynamic_cast<xgui::MenuItemClass*>(c)) return true;
		else return false; 
	}

	bool MenuClass::canBeContainedIn(xgui::ClassInfo * c) 
	{
		if (dynamic_cast<xgui::WindowClass*>(c)) return true;
		return false;
	}

	xgui::Object * MenuClass::create(xgui::Object * parent, xgui::StringMap &properties)
	{
		xgui::StringMap::iterator prop_i;
		xgui::Window * wparent = dynamic_cast<xgui::Window*>( parent );
		xgui::Menu * w = 0;

		if (!parent) {
			XGUI_ERROR("Cannot create a Menu without parent");
			return 0;
		}

		if (!wparent) {
			XGUI_ERROR("Cannot create a Menu as a child of a " << parent->className());
			return 0;
		}

		bool visible = true;

		if ( (prop_i = properties.find("visible")) != properties.end() ) {
			visible = xgui::semantic_cast<bool>(prop_i->second);
			properties.erase(prop_i);
		}

		w = xgui::Master::CreateMenu(wparent, visible);

		return w;
	}
	//------------------------------- END MENU CLASSES INFORMATIONS -------------------//

	//------------------------------ BEGIN LIST CLASSES INFORMATIONS -------------------//
	struct ListColumnClass : public xgui::ClassInfo
	{
		ListColumnClass() : ClassInfo() 
		{
			registerInitProperty("align", false); 
			registerInitProperty("width", false);
			registerInitProperty("position", true);
			registerInitProperty("id", true);
			registerInitProperty("name", false);
		}

		virtual bool isInstanceable() { return false; }
		virtual bool mustFinalize() { return false; }
		virtual std::string const &className() { static std::string const c_name = "xml::ListColumn"; return c_name; }

		virtual bool isContainer() { return false; }
		virtual bool isWidget() { return false; }
		virtual bool isModel() { return false; }
		virtual bool isStyleManager() { return false; }
		
		bool canContain(xgui::ClassInfo * c) { return false; }
		bool canBeContainedIn(xgui::ClassInfo * c) 
		{
			if (dynamic_cast<xgui::ListClass*>(c)) return true;
			else return false;
		}

		void finalize(xgui::Object * o) {}
		
		xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties)
		{
			xgui::List * list = dynamic_cast<xgui::List*>( parent );
			if (list == 0) {
				XGUI_ERROR("Cannot create a ListColumn for something different from a List");
				return 0;
			}
	
			std::string align = "left";
			int width = 100;
	
			if (properties.find("align") != properties.end())
				align = properties["align"];	
			if (properties.find("width") != properties.end())
				width = xgui::semantic_cast<int>(properties["width"]);
	
			list->addColumn(xgui::semantic_cast<int>(properties["position"]), properties["id"], properties["name"], width, align);

			return 0;
		}
	};

	xgui::ListColumnClass ls_column_class;

	std::string const &ListClass::className() { static std::string const c_name = "xgui::List"; return c_name; }

	bool ListClass::isInstanceable() { return true; }

	bool ListClass::canContain(xgui::ClassInfo * c) 
	{
		if (dynamic_cast<xgui::ListColumnClass*>(c)) return true;
		else if (dynamic_cast<xgui::ModelClass*>(c)) return true;
		else return false; 
	}

	xgui::Object * ListClass::create(xgui::Object * parent, xgui::StringMap &properties)
	{
		xgui::StringMap::iterator prop_i;
		xgui::Container * wparent = dynamic_cast<xgui::Container*>( parent );
		xgui::List * w = 0;

		if (!parent) {
			XGUI_ERROR("Cannot create a List without parent");
			return 0;
		}

		if (!wparent) {
			XGUI_ERROR("Cannot create a List as a child of a " << parent->className());
			return 0;
		}

		std::string model_id;	
		if ( (prop_i = properties.find("model")) != properties.end() ) {
			model_id = prop_i->second;
			properties.erase(prop_i);
		}

		std::string sm_id;	
		if ( (prop_i = properties.find("stylemanager")) != properties.end() ) {
			sm_id = prop_i->second;
			properties.erase(prop_i);
		}

		xgui::Model * model = dynamic_cast<xgui::Model*>(Master::FindInPool(model_id));
		xgui::StyleManager * sm = dynamic_cast<xgui::StyleManager*>(Master::FindInPool(sm_id));

		w = xgui::Master::CreateList(wparent, sm);
		if (model)
			w->setModel(model);

		return w;
	}
	//--------------------------------- END LIST CLASSES INFORMATIONS ------------------------//

	//--------------------------------- BEGIN STYLE CLASSES INFORMATIONS ---------------------//
		struct StyleClass : public xgui::ClassInfo
	{
		StyleClass() : ClassInfo()
		{
			registerInitProperty("id", true); 
			registerInitProperty("icon", false); 
			registerInitProperty("fgcolor", false); 
			registerInitProperty("bgcolor", false); 
			registerInitProperty("italic", false); 
			registerInitProperty("bold", false); 
			registerInitProperty("underline", false); 
			registerInitProperty("strikeout", false); 
		}

		virtual bool isInstanceable() { return false; }
		virtual bool mustFinalize() { return false; }
		virtual std::string const &className() { static std::string const c_name = "xml::Style"; return c_name; }

		virtual bool isContainer() { return false; }
		virtual bool isWidget() { return false; }
		virtual bool isModel() { return false; }
		virtual bool isStyleManager() { return false; }

		bool canContain(xgui::ClassInfo * c) { return false; }
		bool canBeContainedIn(xgui::ClassInfo * c) 
		{
			if (dynamic_cast<xgui::StyleManagerClass*>(c)) return true;
			return false;
		}

		xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties)
		{
			xgui::StyleManager * sm = dynamic_cast<xgui::StyleManager*>(parent);
			xgui::StringMap::iterator prop_i;

			if (!parent) {
				XGUI_ERROR("Cannot create a Style without a StyleManager");
				return 0;
			}

			if ( (!sm) ) {
				XGUI_ERROR("Cannot create a Style inside of a " << parent->className());
				return 0;
			}

			std::string id = "";
			if ( (prop_i = properties.find("id")) != properties.end() ) {
				id = prop_i->second;
				properties.erase(prop_i);
			}

			std::string icon = "";
			if ( (prop_i = properties.find("icon")) != properties.end() ) {
				icon = prop_i->second;
				properties.erase(prop_i);
			}

			std::string fgcolor = "";
			if ( (prop_i = properties.find("fgcolor")) != properties.end() ) {
				fgcolor = prop_i->second;
				properties.erase(prop_i);
			}

			std::string bgcolor = "";
			if ( (prop_i = properties.find("bgcolor")) != properties.end() ) {
				bgcolor = prop_i->second;
				properties.erase(prop_i);
			}

			bool italic = false;
			if ( (prop_i = properties.find("italic")) != properties.end() ) {
				italic = xgui::semantic_cast<bool>(prop_i->second);
				properties.erase(prop_i);
			}

			bool bold = false;
			if ( (prop_i = properties.find("bold")) != properties.end() ) {
				bold = xgui::semantic_cast<bool>(prop_i->second);
				properties.erase(prop_i);
			}

			bool underline = false;
			if ( (prop_i = properties.find("underline")) != properties.end() ) {
				underline = xgui::semantic_cast<bool>(prop_i->second);
				properties.erase(prop_i);
			}

			bool strikeout = false;
			if ( (prop_i = properties.find("strikeout")) != properties.end() ) {
				strikeout = xgui::semantic_cast<bool>(prop_i->second);
				properties.erase(prop_i);
			}

			int txt_style = 0;

			if (italic) txt_style |= xgui::StyleManager::TxtItalic;
			if (bold) txt_style |= xgui::StyleManager::TxtBold;
			if (underline) txt_style |= xgui::StyleManager::TxtUnderline;
			if (strikeout) txt_style |= xgui::StyleManager::TxtStrikeOut;

			sm->addStyle(id, xgui::Master::LoadImage(icon), fgcolor, bgcolor, txt_style);

			return 0;
		}

		void finalize(xgui::Object *) {}
	};
	StyleClass style_class;

	bool StyleManagerClass::canContain(xgui::ClassInfo * c) 
	{
		if (dynamic_cast<xgui::StyleClass*>(c)) return true;
		return false; 
	}

	bool StyleManagerClass::canBeContainedIn(xgui::ClassInfo * c) 
	{
		return false;
	}
	//--------------------------- END STYLE CLASSES INFORMATIONS ------------------------//

	//--------------------------- BEGIN BOXES CLASSES INFORMTIONS ----------------------//
	class BoxLayout : public xgui::Object
	{	
		public:
			std::string expand_;
			unsigned int size_;

			BoxLayout(std::string const &expand, unsigned int size);
			~BoxLayout() {}

			std::string dump() { return ""; }
	};	

	struct BoxLayoutClass : public xgui::ContainerClass
	{
		BoxLayoutClass() : ContainerClass() { registerInitProperty("expand", true); }

		virtual bool isInstanceable() { return false; }
		virtual bool mustFinalize() { return true; }
		virtual std::string const &className() { static std::string const c_name = "xml::BoxLayout"; return c_name; }

		virtual bool isContainer() { return true; }
		virtual bool isWidget() { return false; }
		virtual bool isModel() { return false; }
		virtual bool isStyleManager() { return false; }

		bool canContain(xgui::ClassInfo * c) 
		{
			return (dynamic_cast<xgui::WidgetClass*>(c) != 0);
		}

		bool canBeContainedIn(xgui::ClassInfo * c) 
		{
			if (dynamic_cast<xgui::VBoxClass*>(c)) return true;
			else if (dynamic_cast<xgui::HBoxClass*>(c)) return true;
			else return false;
		}

		xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties)
		{
			xgui::Container * cont = dynamic_cast<xgui::Container*>(parent);
			xgui::StringMap::iterator prop_i;

			xgui::VBox * vb = dynamic_cast<xgui::VBox*>(cont);
			xgui::HBox * hb = dynamic_cast<xgui::HBox*>(cont);

			if (!parent) {
				XGUI_ERROR("Cannot create a BoxLayout without parent");
				return 0;
			}

			if ( (!vb) && (!hb) ) {
				XGUI_ERROR("Cannot create a BoxLayout outside of a VBox or HBox");
				return 0;
			}

			std::string expand = "0";
			if ( (prop_i = properties.find("expand")) != properties.end() ) {
				expand = prop_i->second;
				properties.erase(prop_i);
			}
			unsigned int size = cont->size();

			cont->store("layout", new BoxLayout(expand, size));

			return parent;
		}
	
		void finalize(xgui::Object * o) 
		{
			xgui::Container * cont = dynamic_cast<xgui::Container*>(o);
			BoxLayout * layout = dynamic_cast<BoxLayout*>(cont->retrieve("layout"));
			
			int new_size = cont->size();
			for(int i= layout->size_; i < new_size; ++i) {
				xgui::Widget * w = cont->getChild(i);
				w->set("expand", layout->expand_);	
			}

			delete layout;
			cont->unstore("layout");
		}
	};
	static BoxLayoutClass box_layout_class;

	BoxLayout::BoxLayout(std::string const &expand, unsigned int size) 
	: Object(&box_layout_class), expand_(expand), size_(size) {}

	bool VBoxClass::canContain(xgui::ClassInfo * c) 
	{
		if (dynamic_cast<xgui::WidgetClass*>(c)) return true;
		else if (dynamic_cast<xgui::BoxLayoutClass*>(c)) return true;
		return false; 
	}

	bool HBoxClass::canContain(xgui::ClassInfo * c) 
	{
		if (dynamic_cast<xgui::WidgetClass*>(c)) return true;
		else if (dynamic_cast<xgui::BoxLayoutClass*>(c)) return true;
		return false; 
	}
	//---------------------------- END BOXES CLASSES INFORMATIONS --------------//

	extern xgui::TabPageClass tab_page_class;
	extern xgui::ListDataClass list_data_class;
	extern xgui::TreeDataClass tree_data_class;
	extern xgui::ComboboxDataClass cbox_data_class;
	extern xgui::lrClass lr_class;
	extern xgui::liClass li_class;
	extern xgui::ModelNodeClass mnode_class;
	extern xgui::CBoxItemClass cbitem_class;
	extern xgui::TreeNodeClass tree_node_class;
	extern xgui::MenuItemClass mnu_item_class;
	extern xgui::ToolbarRadioGroupClass tb_radio_group_class;
	extern xgui::ToolbarButtonClass tb_btn_class;
	extern xgui::ToolbarCheckButtonClass tb_check_btn_class;
	extern xgui::ToolbarSeparatorClass tb_separator_class;
	extern xgui::ListColumnClass ls_column_class;
	extern xgui::StyleClass style_class;
	extern xgui::ModelEntryClass mentry_class;
	extern xgui::ButtonClass button_class_info;
	extern xgui::CalendarClass calendar_class_info;
	extern xgui::CheckboxClass chkbox_class_info;
	extern xgui::ComboboxClass combobox_class_info;
	extern xgui::ContainerClass container_class_info;
	extern xgui::EntryClass entry_class_info;
	extern xgui::FrameClass frame_class_info;
	extern xgui::HBoxClass hbox_class_info;
	extern xgui::ImageClass img_class_info;
	extern xgui::ImageViewClass imgview_class_info;
	extern xgui::LabelClass lbl_class_info;
	extern xgui::ListClass list_class_info;
	extern xgui::MenuClass menu_class_info;
	extern xgui::ModelClass model_class_info;
	extern xgui::ObjectClass object_class_info;
	extern xgui::ProgressbarClass pbar_class_info;
	extern xgui::SliderClass slider_class_info;
	extern xgui::SpaceClass space_class_info;
	extern xgui::SpinClass spin_class_info;
	extern xgui::StyleManagerClass smanager_class_info;
	extern xgui::TabClass tab_class_info;
	extern xgui::ToolbarClass toolbar_class_info;
	extern xgui::TreeClass tree_class_info;
	extern xgui::VBoxClass vbox_class_info;
	extern xgui::ViewClass view_class_info;
	extern xgui::WidgetClass widget_class_info;
	extern xgui::WindowClass win_class_info;

	void Master::InitDynamicClasses()
	{
		RegisterClass(&button_class_info);
		RegisterClass(&calendar_class_info);
		RegisterClass(&chkbox_class_info);
		RegisterClass(&combobox_class_info);
		RegisterClass(&container_class_info);
		RegisterClass(&entry_class_info);
		RegisterClass(&frame_class_info);
		RegisterClass(&hbox_class_info);
		RegisterClass(&img_class_info);
		RegisterClass(&imgview_class_info);
		RegisterClass(&lbl_class_info);
		RegisterClass(&list_class_info);
		RegisterClass(&menu_class_info);
		RegisterClass(&model_class_info);
		RegisterClass(&object_class_info);
		RegisterClass(&pbar_class_info);
		RegisterClass(&slider_class_info);
		RegisterClass(&space_class_info);
		RegisterClass(&spin_class_info);
		RegisterClass(&smanager_class_info);
		RegisterClass(&tab_class_info);
		RegisterClass(&toolbar_class_info);
		RegisterClass(&tree_class_info);
		RegisterClass(&vbox_class_info);
		RegisterClass(&view_class_info);
		RegisterClass(&widget_class_info);
		RegisterClass(&win_class_info);
		RegisterClass(&mnu_item_class);
		RegisterClass(&ls_column_class);
		RegisterClass(&tb_radio_group_class);
		RegisterClass(&tb_btn_class);
		RegisterClass(&tb_check_btn_class);
		RegisterClass(&tb_separator_class);
		RegisterClass(&list_data_class);
		RegisterClass(&tree_data_class);
		RegisterClass(&cbox_data_class);
		RegisterClass(&mentry_class);
		RegisterClass(&lr_class);
		RegisterClass(&li_class);
		RegisterClass(&mnode_class);
		RegisterClass(&cbitem_class);
		RegisterClass(&tree_node_class);
		RegisterClass(&box_layout_class);
		RegisterClass(&tab_page_class);
		RegisterClass(&style_class);
	}

	/* DLL Windows
		HMODULE lib = LoadLibrary("pippo.dll");
		FARPROC f = GetProcAddress(lib, "xgui_module_init");
		FreeLibrary(lib);
	*/

	void Master::LoadLibrary(std::string const &filename) 
	{
		DMESSAGE("Loading Library: " << filename);
		void (*module_init)(void);

		StrVoidMap::iterator l = Instance()->modules_map_.find(filename);
		if (l != Instance()->modules_map_.end()) return;

		#ifdef WIN32
			HMODULE lib = ::LoadLibraryA(filename.c_str());
		#else
			void * lib = dlopen(filename.c_str(), RTLD_LAZY);
		#endif

		if (lib)
			Instance()->modules_map_[filename] = lib;
		else {
			std::string error = "";
			#ifndef WIN32
				error += " -> ";
				error += dlerror();
			#endif
			XGUI_ERROR("Unable to load external module " << filename << error);

			return;
		}

		#ifdef WIN32
			module_init = (void(*)(void))GetProcAddress(lib, "xgui_module_init");
		#else
			module_init = (void(*)(void))dlsym(lib, "xgui_module_init");
		#endif

		if (module_init)
			module_init();
		else
			UnloadLibrary(filename);
	}

	void Master::UnloadLibrary(std::string const &filename) 
	{
		DMESSAGE("Unloading Library: " << filename);
		void (*module_close)(void);

		StrVoidMap::iterator l = Instance()->modules_map_.find(filename);
		if (l == Instance()->modules_map_.end()) return;

		#ifdef WIN32
			module_close = (void(*)(void))GetProcAddress((HMODULE)l->second, "xgui_module_close");
		#else
			module_close = (void(*)(void))dlsym(l->second, "xgui_module_close");
		#endif

		if (module_close)
			module_close();

		#ifdef WIN32
			FreeLibrary((HMODULE)l->second);
		#else
			dlclose(l->second);
		#endif

		Instance()->modules_map_.erase(l);
	}


	std::string Master::GenerateDTD()
	{
		Master * master = Instance();

		std::string dtd = "<!--\n" \
			"ciao\n" \
			"-->\n\n";

		ClassInfoList::iterator tag_class_iter;
		for ( tag_class_iter = master->class_info_.begin();
			tag_class_iter != master->class_info_.end(); ++tag_class_iter ) {

			ClassInfo * tag_class = *tag_class_iter;

			std::string entity = tag_class->tag() + "_children";

			dtd += "<!ENTITY % xgui." + entity + " \"";

			std::string child_list = "";

			if (tag_class->hasProperty("text") == PROPERTY_READ_WRITE)
				child_list += "#PCDATA";
			else if (tag_class->initProperties().find("text") != tag_class->initProperties().end())
				child_list += "#PCDATA";
			else
				child_list += "empty";

			ClassInfoList::iterator child_class_iter;
			for ( child_class_iter = master->class_info_.begin();
				child_class_iter != master->class_info_.end(); ++child_class_iter ) {

				ClassInfo * child_class = *child_class_iter;

				if ( child_class->canBeContainedIn ( tag_class ) &&
					tag_class->canContain ( child_class ) ) {

					std::string child_tag = child_class->tag();
					child_list += "|" + child_tag;

				}

			}

			dtd += child_list;
			dtd += "\">\n";
			dtd += tag_class->dtd() + "\n";

		}

		return dtd + "\n";
	}
}
