//
// C++ Implementation: vbox
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
#	include "vbox.h"
#	ifdef OPENSTEP
#		include "openstep/vbox_impl.h"
#	else
#		include "gtk/vbox_impl.h"
#	endif
#	include "master.h"
#	include "dynamic.h"
#endif

static void Expand_get(xgui::Object * o, std::string const &name, std::string &val)
{
	xgui::Widget * self = dynamic_cast<xgui::Widget*>(o);
	xgui::Container * cont = self->getParent();

	xgui::VBox * box = dynamic_cast<xgui::VBox*>(cont);
	val = xgui::semantic_cast<std::string>(box->getExpand(self));
}

static void Expand_set(xgui::Object * o, std::string const &name, std::string const &val)
{
	xgui::Widget * self = dynamic_cast<xgui::Widget*>(o);
	xgui::Container * cont = self->getParent();

	xgui::VBox * box = dynamic_cast<xgui::VBox*>(cont);
	box->setExpand(self, xgui::semantic_cast<float>(val));
}

namespace xgui
{
	xgui::VBoxClass vbox_class_info;

	VBoxClass::VBoxClass() : ContainerClass()
	{
		registerInitProperty("space", false);
		registerInitProperty("border", false);

		registerMethod("setExpand", new DMethod2< Type<int>, xgui::VBox, Ptr< Type<xgui::Widget> >, Type<float> >(&xgui::VBox::setExpand));
		registerMethod("getExpand", new DMethod1< Type<float>, xgui::VBox, Ptr< Type<xgui::Widget> > >(&xgui::VBox::getExpand));
	}

	std::string const &VBoxClass::className() { static std::string const c_name = "xgui::VBox"; return c_name; }

	bool VBoxClass::isInstanceable() { return true; }

	xgui::Object * VBoxClass::create(xgui::Object * parent, xgui::StringMap &properties)
	{
		xgui::StringMap::iterator prop_i;
		xgui::Container * wparent = dynamic_cast<xgui::Container*>( parent );
		xgui::VBox * w = 0;

		if (!parent) {
			XGUI_ERROR("Cannot create a VBox without parent");
			return 0;
		}

		if (!wparent) {
			XGUI_ERROR("Cannot create a VBox as a child of a " << parent->className());
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

		w = xgui::Master::CreateVBox(wparent, space, border);

		return w;
	}
}

namespace xgui
{
	VBox::VBox(xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::VBox * widget)
	: Container ( parent, cinfo, widget )
	{
		vbox_impl_ = dynamic_cast<xguimpl::VBox *>(impl_);
	}

	VBox::VBox(xgui::Container * parent, int space, bool border)
	: Container ( parent, &vbox_class_info, new xguimpl::VBox(parent, space, border) )
	{
		vbox_impl_ = dynamic_cast<xguimpl::VBox *>(impl_);
		impl_->setAccessor(this);
		post_construction();
	}

	VBox::~VBox() {}

	int VBox::setExpand(xgui::Widget * w, float expand) 
	{
		expansions_[w] = expand;
		vbox_impl_->setExpand(w, expand); 
		return 1;
	}

	float VBox::getExpand(xgui::Widget * w) { return expansions_[w]; }

	void VBox::addChild(xgui::Widget * w) 
	{
		w->registerProperty("expand", Expand_set, Expand_get, false);
		Container::addChild(w); 
		vbox_impl_->addChild(w);
	}

	std::string VBox::dump()
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
