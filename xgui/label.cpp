//
// C++ Implementation: label
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
#	include "label.h"
#	include "gtk/label_impl.h"
#	include "master.h"
#	include "container.h"
#endif

static void Label_set(xgui::Object * o, std::string const &name, std::string const &val)
{
	xgui::Label * self = dynamic_cast<xgui::Label*>(o);
	xguimpl::Label * impl = dynamic_cast<xguimpl::Label*>(self->getImpl());

	impl->set_label_prop(name, val);
}

static void Label_get(xgui::Object * o, std::string const &name, std::string &dest)
{
	xgui::Label * self = dynamic_cast<xgui::Label*>(o);
	xguimpl::Label * impl = dynamic_cast<xguimpl::Label*>(self->getImpl());

	impl->get_label_prop(name, dest);
}

namespace xgui
{
	xgui::LabelClass lbl_class_info;

	LabelClass::LabelClass() : WidgetClass() 
	{
		registerProperty("text", Label_set, Label_get, true);
		registerProperty("alignment", Label_set, Label_get, true);

		registerInitProperty("text", false);
	}

	std::string const &LabelClass::className() { static std::string const c_name = "xgui::Label"; return c_name; }

	bool LabelClass::isInstanceable() { return true; }

	xgui::Object * LabelClass::create(xgui::Object * parent, xgui::StringMap &properties)
	{
		xgui::StringMap::iterator prop_i;
		xgui::Container * wparent = dynamic_cast<xgui::Container*>( parent );
		xgui::Label * w = 0;

		if (!parent) {
			XGUI_ERROR("Cannot create a Label without parent");
			return 0;
		}

		if (!wparent) {
			XGUI_ERROR("Cannot create a Label as a child of a " << parent->className());
			return 0;
		}

		std::string text = "";

		if ( (prop_i = properties.find("text")) != properties.end() ) {
			text = prop_i->second;
			properties.erase(prop_i);
		}

		w = xgui::Master::CreateLabel(wparent, text);

		return w;
	}
}


namespace xgui
{
	void Label::init()
	{
		label_impl_ = dynamic_cast<xguimpl::Label *>(impl_);
		ASSERT(label_impl_);
	}

	Label::Label ( xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::Label * widget )
	: Widget ( parent, cinfo, widget )
	{
		init();
	}

	Label::Label ( xgui::Container * parent, xgui::ClassInfo * cinfo, std::string const &text )
	: Widget ( parent, cinfo, new xguimpl::Label(parent, text) )
	{
		impl_->setAccessor(this);
		init();
	}

	Label::Label ( xgui::Container * parent, std::string const &text )
	: Widget ( parent, &lbl_class_info, new xguimpl::Label ( parent, text ) )
	{
		impl_->setAccessor(this);
		init();
		post_construction();
	}

	Label::~Label() 
	{
	}
}
