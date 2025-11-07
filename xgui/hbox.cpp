//
// C++ Implementation: hbox
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
#	include "hbox.h"
#	ifdef OPENSTEP
#		include "openstep/hbox_impl.h"
#	else
#		include "gtk/hbox_impl.h"
#	endif
#	include "master.h"
#	include "dynamic.h"
#endif

static void Expand_get(xgui::Object * o, std::string const &name, std::string &val)
{
	xgui::Widget * self = dynamic_cast<xgui::Widget*>(o);
	xgui::Container * cont = self->getParent();

	xgui::HBox * box = dynamic_cast<xgui::HBox*>(cont);
	val = xgui::semantic_cast<std::string>(box->getExpand(self));
}

static void Expand_set(xgui::Object * o, std::string const &name, std::string const &val)
{
	xgui::Widget * self = dynamic_cast<xgui::Widget*>(o);
	xgui::Container * cont = self->getParent();

	xgui::HBox * box = dynamic_cast<xgui::HBox*>(cont);
	box->setExpand(self, xgui::semantic_cast<float>(val));
}

namespace xgui
{
	xgui::HBoxClass hbox_class_info;

	HBoxClass::HBoxClass() : ContainerClass()
	{
		registerInitProperty("space", false);
		registerInitProperty("border", false);

		registerMethod("setExpand", new DMethod2< Type<int>, xgui::HBox, Ptr< Type<xgui::Widget> >, Type<float> >(&xgui::HBox::setExpand));
		registerMethod("getExpand", new DMethod1< Type<float>, xgui::HBox, Ptr< Type<xgui::Widget> > >(&xgui::HBox::getExpand));
	}

	std::string const &HBoxClass::className() { static std::string const c_name = "xgui::HBox"; return c_name; }

	bool HBoxClass::isInstanceable() { return true; }

	xgui::Object * HBoxClass::create(xgui::Object * parent, xgui::StringMap &properties)
	{
		xgui::StringMap::iterator prop_i;
		xgui::Container * wparent = dynamic_cast<xgui::Container*>( parent );
		xgui::HBox * w = 0;

		if (!parent) {
			XGUI_ERROR("Cannot create a HBox without parent");
			return 0;
		}

		if (!wparent) {
			XGUI_ERROR("Cannot create a HBox as a child of a " << parent->className());
			return 0;
		}

		int space = 0;
		bool border = false;

		if ( (prop_i = properties.find("space")) != properties.end() ) {
			space = xgui::semantic_cast<int>(prop_i->second);
			properties.erase(prop_i);
		}

		if ( (prop_i = properties.find("border")) != properties.end() ) {
			border = xgui::semantic_cast<bool>(prop_i->second);
			properties.erase(prop_i);
		}

		w = xgui::Master::CreateHBox(wparent, space, border);

		return w;
	}
}


namespace xgui
{
	HBox::HBox(xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::HBox * widget)
	: Container ( parent, cinfo, widget )
	{
		hbox_impl_ = dynamic_cast<xguimpl::HBox *>(impl_);
	}

	HBox::HBox(xgui::Container * parent, int space, bool border)
	: Container ( parent, &hbox_class_info, new xguimpl::HBox(parent, space, border) )
	{
		hbox_impl_ = dynamic_cast<xguimpl::HBox *>(impl_);
		impl_->setAccessor(this);
		post_construction();
	}

	HBox::~HBox() {}

	int HBox::setExpand(xgui::Widget * w, float expand) 
	{
		expansions_[w] = expand;
		hbox_impl_->setExpand(w, expand);
		return 1;
	}

	float HBox::getExpand(xgui::Widget * w) { return expansions_[w]; }

	void HBox::addChild(xgui::Widget * w) 
	{
		w->registerProperty("expand", Expand_set, Expand_get, false);
		Container::addChild(w); 
		hbox_impl_->addChild(w); 
	}

	std::string HBox::dump()
	{
		std::string xml = "<";
		xml+=cinfo_->tag();
		xml+=dumpProperties();
		xml+=">\n";

		for(WidgetsVector::iterator c = children_.begin(); c != children_.end(); ++c) {
			std::string expand;
			(*c)->get("expand", expand);
			if(expand == "1")
				xml += "<boxlayout expand=\"1\">\n";
			else
				xml += "<boxlayout expand=\"0\">\n";

			xml += (*c)->dump();

			xml += "</boxlayout>\n";
		}

		xml+="</";
		xml+=cinfo_->tag();
		xml+=">\n";

		return xml;
	}
}
