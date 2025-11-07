//
// C++ Implementation: window
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
#	include "window.h"
#	ifdef OPENSTEP
#		include "openstep/window_impl.h"
#	else
#		include "gtk/window_impl.h"
#	endif
#	include "master.h"
#	include "image.h"
#	include "dynamic.h"
#endif

static void Window_set(xgui::Object * o, std::string const &name, std::string const &val)
{
	xgui::Window * self = dynamic_cast<xgui::Window*>(o);
	xguimpl::Window * impl = dynamic_cast<xguimpl::Window*>(self->getImpl());

	if ( name == "icon" )
		self->setIcon(xgui::Master::LoadImage(val));
	else
		impl->set_window_prop(name, val);
}

static void Window_get(xgui::Object * o, std::string const &name, std::string &dest)
{
	xgui::Window * self = dynamic_cast<xgui::Window*>(o);
	xguimpl::Window * impl = dynamic_cast<xguimpl::Window*>(self->getImpl());

	if ( name == "icon" ) {
		xgui::Image * icon = self->getIcon();
		if (icon)
			dest = icon->getPath();
		else
			dest = "";
	}
	else
		impl->get_window_prop(name, dest);
}

namespace xgui
{
	xgui::WindowClass win_class_info;

	WindowClass::WindowClass() : ContainerClass() 
	{
		registerProperty("size", Window_set, Window_get, true);
		registerProperty("text", Window_set, Window_get, true);
		registerProperty("position", Window_set, Window_get, true);
		registerProperty("resizable", Window_set, Window_get, true);
		registerProperty("border", Window_set, Window_get, true);
		registerProperty("status-text", Window_set, Window_get, true);
		registerProperty("icon", Window_set, Window_get, true);

		registerMethod("show", new DMethod0< Type<int>, Window >(&Window::show));
		registerMethod("modalRun", new DMethod0< Type<int>, Window >(&Window::modalRun));
		registerMethod("isModal", new DMethod0< Type<bool>, Window >(&Window::isModal));
		registerMethod("modalReturn", new DMethod1< Type<int>, Window, Type<int> >(&Window::modalReturn));

		registerMethod("getRootChild", new DMethod0< Ptr<Type<Widget> >, Window >(&Window::getRootChild));
		registerMethod("getMenu", new DMethod0< Ptr<Type<Menu> >, Window >(&Window::getMenu));

		registerMethod("maximize", new DMethod0< Type<int>, Window >(&Window::maximize));
		registerMethod("minimize", new DMethod0< Type<int>, Window >(&Window::minimize));
		registerMethod("restore", new DMethod0< Type<int>, Window >(&Window::restore));

		registerMethod("setIcon", new DMethod1< Type<int>, xgui::Window, Ptr< Type<xgui::Image> > >(&xgui::Window::setIcon));
		registerMethod("getIcon", new DMethod0< Ptr< Type<xgui::Image> >, xgui::Window>(&xgui::Window::getIcon));
	}

	std::string const &WindowClass::className() { static std::string const c_name = "xgui::Window"; return c_name; }

	bool WindowClass::isInstanceable() { return true; }

	bool WindowClass::canBeContainedIn(xgui::ClassInfo * c) 
	{
		if (dynamic_cast<xgui::WindowClass*>(c)) return true;
		return false;
	}

	xgui::Object * WindowClass::create(xgui::Object * parent, xgui::StringMap &properties)
	{
		xgui::StringMap::iterator prop_i;
		xgui::Window * wparent = dynamic_cast<xgui::Window*>( parent );

		if (parent && !wparent) {
			XGUI_ERROR("Cannot create a Window as a child of a " << parent->className());
			return 0;
		}

		xgui::Window * w = xgui::Master::CreateWindow(wparent);

		return w;
	}
}

namespace xgui
{
	void Window::init()
	{
		window_impl_ = dynamic_cast<xguimpl::Window*>(impl_);
		icon_ = 0;

		ASSERT(window_impl_);

		Master::RegisterContext(this); 
	}

	Window::Window(xgui::Window * parent, xgui::ClassInfo * cinfo, xguimpl::Window * widget) 
	: Container ( 0, cinfo, widget ), modal_(parent ? true : false), menu_(0), child_(0)
	{
		init();
	}

	Window::Window(xgui::Window * parent)
	: Container ( 0, &win_class_info, new xguimpl::Window(parent) ), modal_(parent ? true : false), menu_(0), child_(0)
	{
		impl_->setAccessor(this);
		post_construction();
		init();
	}

	Window::~Window() 
	{
		Master::UnregisterContext(this);

		if (icon_) icon_->unref();
	}

	void Window::addChild(xgui::Widget * w) 
	{ 
		if (window_impl_->addChild(w))
			Container::addChild(w);
	}

	void Window::delChild(xgui::Widget * w) 
	{ 
		window_impl_->delChild(w); 
		Container::delChild(w); 
	}

	int Window::show() { window_impl_->show(); return 1; }

	int Window::modalRun() { return window_impl_->modalRun(); }
	bool Window::isModal() { return modal_; }
	int Window::modalReturn(int response) { window_impl_->modalReturn(response); return response; }

	xgui::Widget * Window::getRootChild() { return child_; }
	xgui::Menu * Window::getMenu() { return menu_; }

	int Window::maximize() { window_impl_->maximize(); return 1; }
	int Window::minimize() { window_impl_->minimize(); return 1; }
	int Window::restore() { window_impl_->restore(); return 1; }

	int Window::setIcon ( Image * img ) 
	{ 
		if (icon_) {
			icon_->unref();
			icon_ = 0;
		}
	
		if (img) {
			img->ref();
			window_impl_->setIcon(img);
			icon_ = img;
		}

		return (img != 0);
	}

	xgui::Image * Window::getIcon() { return icon_; }
}
