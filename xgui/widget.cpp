//
// C++ Implementation: widget
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
#	include "widget.h"
#	include "container.h"
#	include "window.h"
#	include "gtk/widget_impl.h"
#	include "image.h"
#	include "gtk/image_impl.h"
#	include "master.h"
#	include "dynamic.h"
#endif

static void Widget_set(xgui::Object * o, std::string const &name, std::string const &val)
{
	xgui::Widget * self = dynamic_cast<xgui::Widget*>(o);
	xguimpl::Widget * impl = dynamic_cast<xguimpl::Widget*>(self->getImpl());

	impl->set_widget_prop(name, val);
}

static void Widget_get(xgui::Object * o, std::string const &name, std::string &dest)
{
	xgui::Widget * self = dynamic_cast<xgui::Widget*>(o);
	xguimpl::Widget * impl = dynamic_cast<xguimpl::Widget*>(self->getImpl());

	if ( name == "path" )
		dest = self->getPath();
	else
		impl->get_widget_prop(name, dest);
}

namespace xgui
{
	xgui::WidgetClass widget_class_info;

	WidgetClass::WidgetClass() : ObjectClass() 
	{
		registerProperty("enabled", Widget_set, Widget_get, true);
		registerProperty("tooltip-text", Widget_set, Widget_get, true);
		registerProperty("visible", Widget_set, Widget_get, false);
		registerProperty("focused", Widget_set, Widget_get, false);
		registerProperty("path", 0, Widget_get, false);
		registerProperty("width", 0, Widget_get, false);
		registerProperty("height", 0, Widget_get, false);

		registerMethod("setAsFloating", new DMethod0< Type<int>, Widget >(&Widget::setAsFloating));
		registerMethod("getParent", new DMethod0< Ptr<Type<Container> >, Widget >(&Widget::getParent));
		registerMethod("getContext", new DMethod0< Ptr<Type<Window> >, Widget >(&Widget::getContext));
		registerMethod("getPath", new DMethod0< Type<std::string>, Widget >(&Widget::getPath));

		registerMethod("recalcLayout", new DMethod0< Type<int>, Widget >(&Widget::recalcLayout));
		registerMethod("redraw", new DMethod0< Type<int>, Widget >(&Widget::redraw));

		registerMethod("getMousePos", new DMethod0< Type< std::pair<int, int> >, Widget >
		                                (&Widget::getMousePos));

		registerMethod("linkEvent", new DMethod2< Type<bool>, Widget, Ref<Const<Type<std::string> > >, 
		                       Ptr< Type<Callback> > >(&Widget::linkEvent));
		registerMethod("getEvent", new DMethod1< Ptr<Type<Callback> >, xgui::Widget,
		                       Ref<Const<Type<std::string> > > >(&xgui::Widget::getEvent));
		registerMethod("hasEvent", new DMethod1< Type<bool>, xgui::Widget, Ref<Const<Type<std::string> > > >
		                                (&xgui::Widget::hasEvent));
	}

	bool WidgetClass::isWidget() { return true; }
	std::string const &WidgetClass::className() { static std::string const c_name = "xgui::Widget"; return c_name; }

	bool WidgetClass::canContain(xgui::ClassInfo * c) { return false; }
	bool WidgetClass::canBeContainedIn(xgui::ClassInfo * c) 
	{
		if (dynamic_cast<xgui::ContainerClass*>(c)) return true;
		return false;
	}
}

namespace xgui
{
	Widget::Widget(xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::Widget * i)
	: xgui::Object((cinfo ? cinfo : &widget_class_info)), impl_(i), parent_(parent)
	{

	}
	
	Widget::~Widget()
	{
		if ( parent_ )
			parent_->delChild(this);

		delete impl_;
	}

	int Widget::setAsFloating() { parent_ = 0; return 1; }

	xgui::Container * Widget::getParent() { return parent_; }

	xgui::Window * Widget::getContext()
	{
		xgui::Widget * context = this;
	
		while ( context->getParent() != 0 )
			context = (xgui::Widget*)context->getParent();
	
		return dynamic_cast<xgui::Window*>(context);
	}

	std::string Widget::getPath()
	{
		std::string path;

		if (parent_)
			path += parent_->getPath();

		path += "/";
		path += id_;

		return path;
	}

	bool Widget::linkEvent( std::string const &name, xgui::Callback * cb ) 
	{ 
		if (events_.find(name) != events_.end()) {
			events_[name] = cb;
			return true;
		}
	
		if (impl_->linkEvent(name)) {
			events_[name] = cb;
			return true;
		}


		return false;
	}

	xgui::Callback * Widget::getEvent( std::string const &name )
	{
		xgui::CallbackMap::iterator e = events_.find(name);
		if (e == events_.end())
			return 0;
		else
			return e->second;
	}
	
	bool Widget::hasEvent( std::string const & name )
	{
		return getEvent(name) ? true : false;
	}

	int Widget::recalcLayout() 
	{ 
		impl_->recalcLayout();	
		return 1;
	}

	int Widget::redraw()
	{
		impl_->redraw();
		return 1;
	}

	std::pair<int, int> Widget::getMousePos() { return impl_->getMousePos(); }

	void Widget::post_construction()
	{
		if ( parent_ ) parent_->addChild ( this );
	}
}
