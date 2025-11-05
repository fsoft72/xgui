//
// C++ Implementation: spin
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
#	include "spin.h"
#	include "gtk/spin_impl.h"
#	include "master.h"
#	include "container.h"
#endif

namespace xgui
{
	xgui::SpinClass spin_class_info;

	SpinClass::SpinClass() : WidgetClass() 
	{
		registerProperty("min", Spin::Spin_set, Spin::Spin_get, true);
		registerProperty("max", Spin::Spin_set, Spin::Spin_get, true);
		registerProperty("value", Spin::Spin_set, Spin::Spin_get, true);
		registerProperty("alignment", Spin::Spin_set, Spin::Spin_get, true);

		registerInitProperty("min", false);
		registerInitProperty("max", false);
	}

	std::string const &SpinClass::className() { static std::string const c_name = "xgui::Spin"; return c_name; }

	bool SpinClass::isInstanceable() { return true; }

	xgui::Object * SpinClass::create(xgui::Object * parent, xgui::StringMap &properties)
	{
		xgui::StringMap::iterator prop_i;
		xgui::Container * wparent = dynamic_cast<xgui::Container*>( parent );
		xgui::Spin * w = 0;

		if (!parent) {
			XGUI_ERROR("Cannot create a Spin without parent");
			return 0;
		}

		if (!wparent) {
			XGUI_ERROR("Cannot create a Spin as a child of a " << parent->className());
			return 0;
		}

		int min = 0;
		int max = 100;

		if ( (prop_i = properties.find("min")) != properties.end() ) {
			min = xgui::semantic_cast<int>(prop_i->second);
			properties.erase(prop_i);
		}

		if ( (prop_i = properties.find("max")) != properties.end() ) {
			max = xgui::semantic_cast<int>(prop_i->second);
			properties.erase(prop_i);
		}


		w = xgui::Master::CreateSpin(wparent, min, max);

		return w;
	}
}

namespace xgui
{
	void Spin::init()
	{
		spin_impl_ = dynamic_cast<xguimpl::Spin *>(impl_);
		ASSERT(spin_impl_);
	}

	Spin::Spin ( xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::Spin * widget )
	: Widget ( parent, cinfo, widget ), min_(0), max_(0)
	{
		init();
	}

	Spin::Spin ( xgui::Container * parent, int min, int max )
	: Widget ( parent, &spin_class_info, new xguimpl::Spin ( parent, min, max ) ), min_(min), max_(max)
	{
		impl_->setAccessor(this);
		post_construction();
		init();
	}

	Spin::~Spin() { }

	void Spin::Spin_set(xgui::Object * o, std::string const &name, std::string const &val)
	{
		xgui::Spin * self = dynamic_cast<xgui::Spin*>(o);
		xguimpl::Spin * impl = dynamic_cast<xguimpl::Spin*>(self->getImpl());
	
		if ( name == "value" ) {
			int pos = xgui::semantic_cast<int>(val);
			impl->setValue(pos);
		}
		else if (name == "max") {
			self->max_ = xgui::semantic_cast<int>(val);
			impl->setRange(self->min_, self->max_);
		}
		else if (name == "min") {
			self->min_ = xgui::semantic_cast<int>(val);
			impl->setRange(self->min_, self->max_);
		}
		else if (name == "alignment") {
			impl->setAlignment(val);
		}
	}
	
	void Spin::Spin_get(xgui::Object * o, std::string const &name, std::string &dest)
	{
		xgui::Spin * self = dynamic_cast<xgui::Spin*>(o);
		xguimpl::Spin * impl = dynamic_cast<xguimpl::Spin*>(self->getImpl());
	
		if ( name == "min" ) {
			dest = xgui::semantic_cast<std::string>(self->min_);
		}
		else if ( name == "max" ) {
			dest = xgui::semantic_cast<std::string>(self->max_);
		}
		else if (name == "value" ) {
			dest = xgui::semantic_cast<std::string>(impl->getValue());
		}
		else if (name == "alignment") {
			dest = impl->getAlignment();
		}
	}
}
