//
// C++ Interface: master
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_MASTER_H_
#define _XGUI_MASTER_H_

#include "object.h"
#include "callback.h"

#ifndef WIN32
#include "json.hpp"
#endif

/*
#ifdef CreateWindow
#undef CreateWindow
#endif

#ifdef LoadImage
#undef LoadImage
#endif

#ifdef RegisterClass
#undef RegisterClass
#endif

#ifdef UnregisterClass
#undef UnregisterClass
#endif
*/

namespace xguimpl { struct Master; }

namespace xgui
{
	class Master
	{
		private:
			typedef std::set<xgui::ClassInfo *> ClassInfoList;
			typedef std::set<xgui::Object *> ObjectsPool;

			static Master * instance_;
			xguimpl::Master * master_impl_;
			int loop_level_;

			xgui::WidgetsSet contexts_;
			ObjectsPool objects_pool_;
			xgui::ImagesMap images_cache_;
			
			ClassInfoList class_info_;
			StrVoidMap modules_map_;

			DLLEXPORT Master();
			DLLEXPORT ~Master();

			DLLEXPORT static xgui::Object * ParseXmlNode(xgui::Object * parent_object, void * node);
		DLLEXPORT static xgui::Object * ParseJsonValue(xgui::Object * parent_object, const nlohmann::json &j);

		public:
			DLLEXPORT xguimpl::Master * getImpl() { return master_impl_; }

			DLLEXPORT static Master * Instance();

			DLLEXPORT static void Quit();
			DLLEXPORT static int Run();
			DLLEXPORT static void RegisterIdleCallback ( xgui::Callback * cbk, xgui::Widget * wid );

			DLLEXPORT static xgui::Container * FindContext(std::string const &cname);
			DLLEXPORT static void RegisterContext(xgui::Container * w);
			DLLEXPORT static void UnregisterContext(xgui::Container * w);

			DLLEXPORT static xgui::Image * LoadImage(std::string const &path);
			DLLEXPORT static xgui::Image * GetImage(std::string const &path);
			DLLEXPORT static xgui::Image * NullImage();

			DLLEXPORT static void InitThreads();
			DLLEXPORT static void QuitThreads();
			DLLEXPORT static void GuiLock();
			DLLEXPORT static void GuiUnlock();

			DLLEXPORT static xgui::Window * CreateWindow(xgui::Window * parent = 0);
			DLLEXPORT static xgui::Button * CreateButton(xgui::Container * parent, std::string const &text, xgui::Image * image = 0, bool toggle_button = false);
			DLLEXPORT static xgui::Menu * CreateMenu(xgui::Window * parent, bool visible = true);
			DLLEXPORT static xgui::VBox * CreateVBox(xgui::Container * parent, int space = 0, bool border = false);
			DLLEXPORT static xgui::HBox * CreateHBox(xgui::Container * parent, int space = 0, bool border = false);
			DLLEXPORT static xgui::Entry * CreateEntry(xgui::Container * parent, std::string const &text, int maxlen=0, bool editable=true, bool password_mode=false);
			DLLEXPORT static xgui::Label * CreateLabel(xgui::Container * parent, std::string const &text);
 			DLLEXPORT static xgui::List * CreateList(xgui::Container * parent, xgui::StyleManager * sm = 0);
			DLLEXPORT static xgui::Tree * CreateTree(xgui::Container * parent, xgui::StyleManager * sm = 0);
			DLLEXPORT static xgui::Slider * CreateSlider(xgui::Container * parent, int min = 0, int max = 100, bool vertical = false);
			DLLEXPORT static xgui::Calendar * CreateCalendar(xgui::Container * parent);
			DLLEXPORT static xgui::ImageView * CreateImageView(xgui::Container * parent, xgui::Image * img = 0);
			DLLEXPORT static xgui::Progressbar * CreateProgressbar(xgui::Container * parent, int min = 0, int max = 100);
			DLLEXPORT static xgui::Tab * CreateTab(xgui::Container * parent, const std::string & alignment = "top");
			DLLEXPORT static xgui::Checkbox * CreateCheckbox(xgui::Container * parent, std::string const &text);
			DLLEXPORT static xgui::Spin * CreateSpin(xgui::Container * parent, int min = 0, int max = 100);
			DLLEXPORT static xgui::Frame * CreateFrame(xgui::Container * parent, const std::string & label);
			DLLEXPORT static xgui::Combobox * CreateCombobox(xgui::Container * parent, bool editable = 0);
			DLLEXPORT static xgui::Space * CreateSpace(xgui::Container * parent, int min_width = 1, int min_height = 1);
			DLLEXPORT static xgui::Toolbar * CreateToolbar(xgui::Container * parent);
			DLLEXPORT static void DestroyWidget(xgui::Widget * w);

			DLLEXPORT static xgui::Model * CreateModel();
			DLLEXPORT static void DestroyModel(xgui::Model * model);

			DLLEXPORT static xgui::StyleManager * CreateStyleManager();
			DLLEXPORT static void DestroyStyleManager(xgui::StyleManager * sm);

			DLLEXPORT static xgui::StrVector OpenFileDialog(xgui::Window * parent, const xgui::FileExtensionsVector &filter, std::string const &title, std::string const &path = "", bool multiselection = false);
			DLLEXPORT static std::string SaveFileDialog(xgui::Window * parent, const xgui::FileExtensionsVector &filter, std::string const &title, std::string const &path = "", std::string const &file_name = "");
			DLLEXPORT static int MessageDialog(xgui::Window * parent, const std::string & title, const std::string & text, const std::string & type = "info", const std::string & buttons = "ok" );

			DLLEXPORT static void AddToPool(xgui::Object * o);
			DLLEXPORT static void RemoveFromPool(xgui::Object * o);
			DLLEXPORT static xgui::Object * FindInPool(std::string const &id);

			DLLEXPORT static void InitDynamicClasses();
			DLLEXPORT static xgui::ClassInfo * FindClassByTag(std::string const &tag);
			DLLEXPORT static xgui::ClassInfo * FindClass(std::string const &cname);
			DLLEXPORT static std::vector<std::string> availableClasses();
			DLLEXPORT static xgui::Object * LoadXml(std::string const &filename, xgui::Object * parent = 0);
			DLLEXPORT static xgui::Object * LoadJson(std::string const &filename, xgui::Object * parent = 0);
			DLLEXPORT static void SaveXml(xgui::Object * root, std::string const &filename);
			DLLEXPORT static std::string GenerateDTD();

			DLLEXPORT static void LoadLibrary(std::string const &filename);
			DLLEXPORT static void UnloadLibrary(std::string const &filename);

			DLLEXPORT static void RegisterClass(xgui::ClassInfo * ginfo);
			DLLEXPORT static void UnregisterClass(std::string const &cname);

			DLLEXPORT static xgui::Object * CreateObject(std::string const &class_name, xgui::Object * parent, xgui::StringMap const &properties);
			DLLEXPORT static xgui::Object * FinalizeObject(xgui::Object * o);
			DLLEXPORT static void DestroyObject(xgui::Object * o);

		friend struct xguimpl::Master;
		friend class Image;
	};
}

#endif
