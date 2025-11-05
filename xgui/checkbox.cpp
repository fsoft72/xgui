//
// C++ Implementation: checkbox
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
#	include "checkbox.h"
#	include "gtk/checkbox_impl.h"
#	include "master.h"
#	include "image.h"
#	include "container.h"
#	include "dynamic.h"
#endif

static void Checkbox_set(xgui::Object * o, std::string const &name, std::string const &val)
{
	xgui::Checkbox * self = dynamic_cast<xgui::Checkbox*>(o);
	xguimpl::Checkbox * impl = dynamic_cast<xguimpl::Checkbox*>(self->getImpl());

	if (name == "value") 
		self->setValue(val);
	else
		impl->set_checkbox_prop(name, val);
}

static void Checkbox_get(xgui::Object * o, std::string const &name, std::string &dest)
{
	xgui::Checkbox * self = dynamic_cast<xgui::Checkbox*>(o);
	xguimpl::Checkbox * impl = dynamic_cast<xguimpl::Checkbox*>(self->getImpl());

	if (name == "value")
		dest = self->getValue();
	else	
		impl->get_checkbox_prop(name, dest);
}

namespace xgui
{
	xgui::CheckboxClass chkbox_class_info;

	CheckboxClass::CheckboxClass() : WidgetClass() 
	{
		registerProperty("value", Checkbox_set, Checkbox_get, true);
		registerProperty("text", Checkbox_set, Checkbox_get, true);
		registerProperty("checked", Checkbox_set, Checkbox_get, true);
		registerProperty("alignment", Checkbox_set, Checkbox_get, true);

		registerInitProperty("text", false);

		registerMethod("getValue", new DMethod0< Ref< Const< Type<std::string> > >, xgui::Checkbox >(&xgui::Checkbox::getValue));
		registerMethod("setValue", new DMethod1< Type<int>, xgui::Checkbox, Ref< Const< Type<std::string> > > >(&xgui::Checkbox::setValue));
	}

	std::string const &CheckboxClass::className() { static std::string const c_name = "xgui::Checkbox"; return c_name; }

	bool CheckboxClass::isInstanceable() { return true; }

	xgui::Object * CheckboxClass::create(xgui::Object * parent, xgui::StringMap &properties)
	{
		xgui::StringMap::iterator prop_i;
		xgui::Container * wparent = dynamic_cast<xgui::Container*>( parent );
		xgui::Checkbox * w = 0;

		if (!parent) {
			XGUI_ERROR("Cannot create a Checkbox without parent");
			return 0;
		}

		if (!wparent) {
			XGUI_ERROR("Cannot create a Checkbox as a child of a " << parent->className());
			return 0;
		}

		std::string text = "";

		if ( (prop_i = properties.find("text")) != properties.end() ) {
			text = prop_i->second;
			properties.erase(prop_i);
		}

		w = xgui::Master::CreateCheckbox(wparent, text);

		return w;
	}
}

namespace xgui
{
	void Checkbox::init()
	{
		checkbox_impl_ = dynamic_cast<xguimpl::Checkbox *>(impl_);
		ASSERT(checkbox_impl_);
	}

	Checkbox::Checkbox(xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::Checkbox * widget)
	: Widget ( parent, cinfo, widget ), value_("on")
	{
		init();
	}

	Checkbox::Checkbox(xgui::Container * parent, std::string const &text)
	: Widget ( parent, &chkbox_class_info, new xguimpl::Checkbox(parent, text) ), value_("on")
	{
		impl_->setAccessor(this);
		post_construction();
		init();
	}

	Checkbox::~Checkbox() 
	{
	}

	std::string const &Checkbox::getValue()
	{
		return value_;
	}

	int Checkbox::setValue(std::string const &val)
	{
		value_ = val;
		if (value_.empty()) { value_ = "on"; return 0; }
		return 1;
	}
}
