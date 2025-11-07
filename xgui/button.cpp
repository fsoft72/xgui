//
// C++ Implementation: button
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
#	include "button.h"
#	ifdef OPENSTEP
#		include "openstep/button_impl.h"
#	else
#		include "gtk/button_impl.h"
#	endif
#	include "master.h"
#	include "image.h"
#	include "container.h"
#	include "dynamic.h"
#endif

static void btn_prop_setter(xgui::Object * o, std::string const &prop, std::string const &val)
{
	xgui::Button * b = dynamic_cast<xgui::Button*>(o);
	xguimpl::Button * impl = dynamic_cast<xguimpl::Button*>(b->getImpl());

	if ( prop == "image-path" ) {
		b->setIcon(xgui::Master::LoadImage(val));
	}
	else
		impl->set_button_prop(prop, val);
}

static void btn_prop_getter(xgui::Object * o, std::string const &prop, std::string &val)
{
	xgui::Button * b = dynamic_cast<xgui::Button*>(o);
	xguimpl::Button * impl = dynamic_cast<xguimpl::Button*>(b->getImpl());

	if ( prop == "image-path" ) {
		xgui::Image * icon = b->getIcon();
		if (icon)
			val = icon->getPath();
		else
			val = "";
	}
	else
		impl->get_button_prop(prop, val);
}

namespace xgui
{
	xgui::ButtonClass button_class_info;

	ButtonClass::ButtonClass() : WidgetClass() 
	{
		registerProperty("image-path", btn_prop_setter, btn_prop_getter, true);
		registerProperty("text", btn_prop_setter, btn_prop_getter, true);
		registerProperty("pressed", btn_prop_setter, btn_prop_getter, true);
		registerProperty("alignment", btn_prop_setter, btn_prop_getter, true);

		registerInitProperty("image-path", false);
		registerInitProperty("toggle-button", false);
		registerInitProperty("text", false);

		registerMethod("setIcon", new DMethod1< Type<int>, xgui::Button, Ptr< Type<xgui::Image> > >(&xgui::Button::setIcon));
		registerMethod("getIcon", new DMethod0< Ptr< Type<xgui::Image> >, xgui::Button>(&xgui::Button::getIcon));
	}

	std::string const &ButtonClass::className() { static std::string const c_name = "xgui::Button"; return c_name; }

	bool ButtonClass::isInstanceable() { return true; }

	xgui::Object * ButtonClass::create(xgui::Object * parent, xgui::StringMap &properties)
	{
		xgui::StringMap::iterator prop_i;
		xgui::Container * wparent = dynamic_cast<xgui::Container*>( parent );
		xgui::Button * w = 0;

		if (!parent) {
			XGUI_ERROR("Cannot create a Button without parent");
			return 0;
		}

		if (!wparent) {
			XGUI_ERROR("Cannot create a Button as a child of a " << parent->className());
			return 0;
		}

		std::string text = "";
		xgui::Image * image = 0;
		bool toggle = false;

		if ( (prop_i = properties.find("image-path")) != properties.end()) {
			image = xgui::Master::LoadImage(prop_i->second);
			properties.erase(prop_i);
		}

		if ( (prop_i = properties.find("toggle-button")) != properties.end()) {
			toggle = xgui::semantic_cast<bool>(prop_i->second);
			properties.erase(prop_i);
		}

		if ( (prop_i = properties.find("text")) != properties.end()) {
			text = prop_i->second;
			properties.erase(prop_i);
		}

		w = xgui::Master::CreateButton(wparent, text, image, toggle);

		return w;
	}
}

namespace xgui
{
	void Button::init()
	{
		button_impl_ = dynamic_cast<xguimpl::Button *>(impl_);
		icon_ = 0;
		
		ASSERT(button_impl_);
	}

	Button::Button(xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::Button * widget)
	: Widget ( parent, cinfo, widget )
	{
		init();
	}

	Button::Button(xgui::Container * parent, std::string const &text, xgui::Image * image, bool toggle_button)
	: Widget ( parent, &button_class_info, new xguimpl::Button(parent, text, image, toggle_button) )
	{
		impl_->setAccessor(this);
		post_construction();
		init();
	}

	Button::~Button() 
	{
		if (icon_) icon_->unref();
	}

	int Button::setIcon ( Image * img ) 
	{ 
		if (img)
			img->ref();

		if (icon_) {
			icon_->unref();
			icon_ = 0;
		}
	
		if (img) {
			button_impl_->setIcon(img);
			icon_ = img;
		}

		return (icon_ != 0);
	}

	xgui::Image * Button::getIcon() { return icon_; }
}
