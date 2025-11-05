//
// C++ Implementation: slider
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
#	include "slider.h"
#	include "gtk/slider_impl.h"
#	include "master.h"
#	include "container.h"
#	include "dynamic.h"
#endif

static void Slider_set(xgui::Object * o, std::string const &name, std::string const &val)
{
	xgui::Slider * self = dynamic_cast<xgui::Slider*>(o);
	xguimpl::Slider * impl = dynamic_cast<xguimpl::Slider*>(self->getImpl());

	impl->set_slider_prop(name, val);
}

static void Slider_get(xgui::Object * o, std::string const &name, std::string &dest)
{
	xgui::Slider * self = dynamic_cast<xgui::Slider*>(o);
	xguimpl::Slider * impl = dynamic_cast<xguimpl::Slider*>(self->getImpl());

	impl->get_slider_prop(name, dest);
}

namespace xgui
{
	xgui::SliderClass slider_class_info;

	SliderClass::SliderClass() : WidgetClass() 
	{
		registerProperty("min", Slider_set, Slider_get, true);
		registerProperty("max", Slider_set, Slider_get, true);
		registerProperty("pos", Slider_set, Slider_get, true);

		registerInitProperty("min", false);
		registerInitProperty("max", false);
		registerInitProperty("vertical", false);

		registerMethod("getPos", new DMethod0< Type<int>, xgui::Slider >(&xgui::Slider::getPos));
		registerMethod("setPos", new DMethod1< Type<int>, xgui::Slider, Type<int> >(&xgui::Slider::setPos));
	}

	std::string const &SliderClass::className() { static std::string const c_name = "xgui::Slider"; return c_name; }

	bool SliderClass::isInstanceable() { return true; }

	xgui::Object * SliderClass::create(xgui::Object * parent, xgui::StringMap &properties)
	{
		xgui::StringMap::iterator prop_i;
		xgui::Container * wparent = dynamic_cast<xgui::Container*>( parent );
		xgui::Slider * w = 0;

		if (!parent) {
			XGUI_ERROR("Cannot create a Slider without parent");
			return 0;
		}

		if (!wparent) {
			XGUI_ERROR("Cannot create a Slider as a child of a " << parent->className());
			return 0;
		}

		int min = 0;
		int max = 100;
		bool vertical = false;

		if ( (prop_i = properties.find("min")) != properties.end() ) {
			min = xgui::semantic_cast<int>(prop_i->second);
			properties.erase(prop_i);
		}

		if ( (prop_i = properties.find("max")) != properties.end() ) {
			max = xgui::semantic_cast<int>(prop_i->second);
			properties.erase(prop_i);
		}

		if ( (prop_i = properties.find("vertical")) != properties.end() ) {
			vertical = xgui::semantic_cast<bool>(prop_i->second);
			properties.erase(prop_i);
		}

		w = xgui::Master::CreateSlider(wparent, min, max, vertical);

		return w;
	}
}

namespace xgui
{
	void Slider::init()
	{
		slider_impl_ = dynamic_cast<xguimpl::Slider *>(impl_);
		ASSERT(slider_impl_);
	}

	Slider::Slider ( xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::Slider * widget )
	: Widget ( parent, cinfo, widget )
	{
		init();
	}

	Slider::Slider ( xgui::Container * parent, int min, int max, bool vertical )
	: Widget ( parent, &slider_class_info, new xguimpl::Slider ( parent, min, max, vertical ) )
	{
		impl_->setAccessor(this);
		post_construction();
		init();
	}

	Slider::~Slider() 
	{
	}

	int Slider::getPos()
	{
		return slider_impl_->getPos();
	}

	int Slider::setPos ( int pos )
	{
		slider_impl_->setPos ( pos );
		return pos;
	}
}
