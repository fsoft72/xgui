//
// C++ Implementation: master
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
#	include "master.h"
#	include "xgui.h"
#	include "gtk/master_impl.h"
#else
#	ifdef CreateWindow
#		undef CreateWindow
#	endif
#	ifdef LoadImage
#		undef LoadImage
#	endif
#endif

namespace xgui {

	Master * Master::instance_ = 0;

	Master::Master()
	: master_impl_(0), loop_level_(0)
	{
		master_impl_ = new xguimpl::Master(this);
	}

	Master::~Master()
	{
		while (!contexts_.empty())
			DestroyWidget ( *contexts_.begin() );

		while ( !images_cache_.empty() ) {
			delete images_cache_.begin()->second;
		}

		delete master_impl_;
	}

	DLLEXPORT xgui::Image * Master::NullImage() { return 0; }

	DLLEXPORT xgui::Image * Master::GetImage(std::string const &path)
	{
		xgui::ImagesMap::iterator image = Instance()->images_cache_.find(path);
		if (image != Instance()->images_cache_.end())
			return image->second;
	
		return 0;
	}

	DLLEXPORT xgui::Image * Master::LoadImage(std::string const &path)
	{
		xgui::Image * img = GetImage(path);

		if (img == 0) {
			img = new xgui::Image(path);
			if (img->isValid())
				Instance()->images_cache_[path] = img;
			else {
				delete img;
				img = 0;
			}
		}

		return img;
	}

	DLLEXPORT void Master::InitThreads() { Master::Instance()->master_impl_->initThreads(); }
	DLLEXPORT void Master::QuitThreads() { Master::Instance()->master_impl_->quitThreads(); }
	DLLEXPORT void Master::GuiLock() { Master::Instance()->master_impl_->guiLock(); }
	DLLEXPORT void Master::GuiUnlock() { Master::Instance()->master_impl_->guiUnlock(); }

	DLLEXPORT xgui::Master * Master::Instance()
	{
		if (Master::instance_ == 0)
			Master::instance_ = new Master();
		return Master::instance_;
	}

	DLLEXPORT void Master::Quit()
	{
		if (Master::Instance()->master_impl_->quit())
			delete Master::Instance();
	}

	DLLEXPORT xgui::Container * Master::FindContext(std::string const &cname)
	{
		WidgetsSet &contexts = Master::Instance()->contexts_;

		for ( WidgetsSet::iterator c = contexts.begin(); c != contexts.end(); ++c )
			if ((*c)->isIdentifiedAs(cname)) return dynamic_cast<xgui::Container*>(*c);

		return 0;
	}

	DLLEXPORT void Master::DestroyWidget(xgui::Widget * w)
	{
		Master::Instance()->master_impl_->destroyWidget(w);
	}

	DLLEXPORT int Master::Run()
	{
		return Master::Instance()->master_impl_->run();
	}

	DLLEXPORT void Master::RegisterIdleCallback ( xgui::Callback * cbk, xgui::Widget * wid )
	{
		Master::Instance()->master_impl_->registerIdleCallback ( cbk, wid );
	}

	DLLEXPORT void Master::RegisterContext(xgui::Container * w)
	{	
		Master::Instance()->contexts_.insert(w);
	}

	DLLEXPORT void Master::UnregisterContext(xgui::Container * w)
	{
		Master::Instance()->contexts_.erase(w);
	}

	DLLEXPORT void Master::AddToPool(xgui::Object * o)
	{
		Master::Instance()->objects_pool_.insert(o);
	}

	DLLEXPORT void Master::RemoveFromPool(xgui::Object * o)
	{
		Master::Instance()->objects_pool_.erase(o);
	}

	DLLEXPORT xgui::Object * Master::FindInPool(std::string const &id)
	{
		ObjectsPool::iterator pool_begin, pool_end;

		pool_begin = Master::Instance()->objects_pool_.begin();
		pool_end = Master::Instance()->objects_pool_.end();

		for(ObjectsPool::iterator i = pool_begin; i != pool_end; ++i) {
			if ((*i)->isIdentifiedAs(id))
				return *i;
		}

		return 0;
	}

	DLLEXPORT xgui::Window * Master::CreateWindow ( xgui::Window * parent )
	{
		Master::Instance();
		return new xgui::Window(parent);
	}

	DLLEXPORT xgui::Button * Master::CreateButton(xgui::Container * parent, std::string const &text, xgui::Image * image, bool toggle_button)
	{
		Master::Instance();
		return new xgui::Button(parent, text, image, toggle_button);
	}

	DLLEXPORT xgui::Menu * Master::CreateMenu(xgui::Window * parent, bool visible)
	{
		Master::Instance();
		return new xgui::Menu(parent, visible);
	}

	DLLEXPORT xgui::VBox * Master::CreateVBox(xgui::Container * parent, int space, bool border)
	{
		Master::Instance();
		return new xgui::VBox(parent, space, border);
	}

	DLLEXPORT xgui::HBox * Master::CreateHBox(xgui::Container * parent, int space, bool border)
	{
		Master::Instance();
		return new xgui::HBox(parent, space, border);
	}

	DLLEXPORT xgui::Entry * Master::CreateEntry(xgui::Container * parent, std::string const &text, int maxlen, bool editable, bool password_mode)
	{
		Master::Instance();
		return new xgui::Entry(parent, text, maxlen, editable, password_mode);
	}

	
	DLLEXPORT xgui::Label * Master::CreateLabel(xgui::Container * parent, std::string const &text)
	{
		Master::Instance();
		return new xgui::Label(parent, text);
	}

	DLLEXPORT xgui::List * Master::CreateList(xgui::Container * parent, xgui::StyleManager * sm)
	{
		Master::Instance();
		return new xgui::List(parent, sm);
	}

	DLLEXPORT xgui::Tree * Master::CreateTree(xgui::Container * parent, xgui::StyleManager * sm)
	{
		Master::Instance();
		return new xgui::Tree(parent, sm);
	}

	DLLEXPORT xgui::Slider * Master::CreateSlider(xgui::Container * parent, int min, int max, bool vertical)
	{
		Master::Instance();
		return new xgui::Slider(parent, min, max, vertical);
	}

	DLLEXPORT xgui::Calendar * Master::CreateCalendar(xgui::Container * parent)
	{
		Master::Instance();
		return new xgui::Calendar(parent);
	}

	DLLEXPORT xgui::ImageView * Master::CreateImageView(xgui::Container * parent, xgui::Image * img)
	{
		Master::Instance();
		return new xgui::ImageView(parent, img);
	}

	DLLEXPORT xgui::Progressbar * Master::CreateProgressbar(xgui::Container * parent, int min, int max)
	{
		Master::Instance();
		return new xgui::Progressbar(parent, min, max);
	}

	DLLEXPORT xgui::Tab * Master::CreateTab(xgui::Container * parent, const std::string & alignment)
	{
		Master::Instance();
		return new xgui::Tab(parent, alignment);
	}

	DLLEXPORT xgui::Checkbox * Master::CreateCheckbox(xgui::Container * parent, std::string const &text)
	{
		Master::Instance();
		return new xgui::Checkbox(parent, text);
	}

	DLLEXPORT xgui::Spin * Master::CreateSpin(xgui::Container * parent, int min, int max)
	{
		Master::Instance();
		return new xgui::Spin(parent, min, max);
	}

	DLLEXPORT xgui::Frame * Master::CreateFrame(xgui::Container * parent, const std::string & label)
	{
		Master::Instance();
		return new xgui::Frame(parent, label);
	}

	DLLEXPORT xgui::Combobox * Master::CreateCombobox(xgui::Container * parent, bool editable)
	{
		Master::Instance();
		return new xgui::Combobox(parent, editable);
	}

	DLLEXPORT xgui::Space * Master::CreateSpace(xgui::Container * parent, int min_width, int min_height)
	{
		Master::Instance();
		return new xgui::Space(parent, min_width, min_height);
	}

	DLLEXPORT xgui::Toolbar * Master::CreateToolbar(xgui::Container * parent)
	{
		Master::Instance();
		return new xgui::Toolbar(parent);
	}

	DLLEXPORT xgui::Model * Master::CreateModel()
	{
		Master::Instance();
		return new xgui::Model();
	}

	DLLEXPORT void Master::DestroyModel(xgui::Model * model)
	{
		Master::Instance();
		model->destroy();
	}

	DLLEXPORT xgui::StyleManager * Master::CreateStyleManager()
	{
		Master::Instance();
		return new xgui::StyleManager();
	}

	DLLEXPORT void Master::DestroyStyleManager(xgui::StyleManager * sm)
	{
		Master::Instance();
		sm->destroy();
	}

	DLLEXPORT xgui::StrVector Master::OpenFileDialog(xgui::Window * parent, xgui::FileExtensionsVector const &filter, std::string const &title, std::string const &path, bool multiselection)
	{
		return Master::Instance()->master_impl_->openFileDialog(parent, filter, title, path, multiselection);
	}

	DLLEXPORT std::string Master::SaveFileDialog(xgui::Window * parent, xgui::FileExtensionsVector const &filter, std::string const &title, std::string const &path, std::string const &file_name)
	{
		return Master::Instance()->master_impl_->saveFileDialog(parent, filter, title, path, file_name);
	}

	DLLEXPORT int Master::MessageDialog(xgui::Window * parent, const std::string & title, const std::string & text, const std::string & d_type, const std::string & d_buttons )
	{
		return Master::Instance()->master_impl_->messageDialog(parent, title, text, d_type, d_buttons);
	}
}
