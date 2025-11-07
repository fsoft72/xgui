//
// C++ Implementation: progressbar
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
#	include "progressbar.h"
#	ifdef OPENSTEP
#		include "openstep/progressbar_impl.h"
#	else
#		include "gtk/progressbar_impl.h"
#	endif
#	include "master.h"
#	include "container.h"
#	include "dynamic.h"
#endif

namespace xgui
{
	xgui::ProgressbarClass pbar_class_info;

	ProgressbarClass::ProgressbarClass() : WidgetClass() 
	{
		registerProperty("min", Progressbar::Progressbar_set, Progressbar::Progressbar_get, true);
		registerProperty("max", Progressbar::Progressbar_set, Progressbar::Progressbar_get, true);
		registerProperty("pos", Progressbar::Progressbar_set, Progressbar::Progressbar_get, true);

		registerInitProperty("min", false);
		registerInitProperty("max", false);

		registerMethod("getPos", new DMethod0< Type<int>, xgui::Progressbar >(&xgui::Progressbar::getPos));
		registerMethod("setPos", new DMethod1< Type<int>, xgui::Progressbar, Type<int> >(&xgui::Progressbar::setPos));
	}

	std::string const &ProgressbarClass::className() { static std::string const c_name = "xgui::Progressbar"; return c_name; }

	bool ProgressbarClass::isInstanceable() { return true; }

	xgui::Object * ProgressbarClass::create(xgui::Object * parent, xgui::StringMap &properties)
	{
		xgui::StringMap::iterator prop_i;
		xgui::Container * wparent = dynamic_cast<xgui::Container*>( parent );
		xgui::Progressbar * w = 0;

		if (!parent) {
			XGUI_ERROR("Cannot create a Progressbar without parent");
			return 0;
		}

		if (!wparent) {
			XGUI_ERROR("Cannot create a Progressbar as a child of a " << parent->className());
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


		w = xgui::Master::CreateProgressbar(wparent, min, max);

		return w;
	}
}

namespace xgui
{
	void Progressbar::init()
	{
		pbar_impl_ = dynamic_cast<xguimpl::Progressbar *>(impl_);
		ASSERT(pbar_impl_);
	}

	Progressbar::Progressbar ( xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::Progressbar * widget )
	: Widget ( parent, cinfo, widget ), min_(0), max_(0), pos_(0)
	{
		init();
	}

	Progressbar::Progressbar ( xgui::Container * parent, int min, int max )
	: Widget ( parent, &pbar_class_info, new xguimpl::Progressbar ( parent, min, max ) ), min_(min), max_(max), pos_(min)
	{
		impl_->setAccessor(this);
		post_construction();
		init();
	}

	Progressbar::~Progressbar() { }

	void Progressbar::Progressbar_set ( xgui::Object * o, std::string const &name, std::string const &val )
	{
		xgui::Progressbar * self = dynamic_cast<xgui::Progressbar*>(o);
		xguimpl::Progressbar * impl = dynamic_cast<xguimpl::Progressbar*>(self->getImpl());
	
		if ( name == "pos" ) {
			self->pos_ = xgui::semantic_cast<int>(val);
			impl->setState(self->min_, self->max_, self->pos_);
		}
		else if (name == "max") {
			self->max_ = xgui::semantic_cast<int>(val);
			impl->setState(self->min_, self->max_, self->pos_);
		}
		else if (name == "min") {
			self->min_ = xgui::semantic_cast<int>(val);
			impl->setState(self->min_, self->max_, self->pos_);
		}
	}
	
	void Progressbar::Progressbar_get ( xgui::Object * o, std::string const &name, std::string &dest )
	{
		xgui::Progressbar * self = dynamic_cast<xgui::Progressbar*>(o);
	
		if ( name == "min" ) {
			dest = xgui::semantic_cast<std::string>(self->min_);
		}
		else if ( name == "max" ) {
			dest = xgui::semantic_cast<std::string>(self->max_);
		}
		else if (name == "pos" ) {
			dest = xgui::semantic_cast<std::string>(self->pos_);
		}
	}

	int Progressbar::getPos()
	{
		return pos_;
	}

	int Progressbar::setPos ( int pos )
	{
		pos_ = pos;
		pbar_impl_->setState ( min_, max_, pos_ );
		return pos_;
	}
}
