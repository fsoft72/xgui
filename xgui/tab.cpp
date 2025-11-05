//
// C++ Implementation: tab
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
#	include "tab.h"
#	include "gtk/tab_impl.h"
#	include "master.h"
#	include "image.h"
#	include "dynamic.h"
#endif

static void Tab_set(xgui::Object * o, std::string const &name, std::string const &val)
{
	xgui::Tab * self = dynamic_cast<xgui::Tab*>(o);
	xguimpl::Tab * impl = dynamic_cast<xguimpl::Tab*>(self->getImpl());

	impl->set_tab_prop(name, val);
}

static void Tab_get(xgui::Object * o, std::string const &name, std::string &dest)
{
	xgui::Tab * self = dynamic_cast<xgui::Tab*>(o);
	xguimpl::Tab * impl = dynamic_cast<xguimpl::Tab*>(self->getImpl());

	impl->get_tab_prop(name, dest);
}

static void Tab_child_set(xgui::Object * o, std::string const &name, std::string const &val)
{
	xgui::Widget * self = dynamic_cast<xgui::Widget*>(o);
	xgui::Tab * tab = dynamic_cast<xgui::Tab*>(self->getParent());

	if (name == "tab-icon")
		tab->setPageIcon(self, xgui::Master::LoadImage(val));
	else if (name == "tab-label")
		tab->setPageLabel(self, val);
}

static void Tab_child_get(xgui::Object * o, std::string const &name, std::string &dest)
{
	xgui::Widget * self = dynamic_cast<xgui::Widget*>(o);
	xgui::Tab * tab = dynamic_cast<xgui::Tab*>(self->getParent());

	if (name == "tab-icon") {
		xgui::Image * icon = tab->getPageIcon(self);
		if (icon) 
			dest = icon->getPath();
		else
			dest = "";
	}
	else if (name == "tab-label")
		dest = tab->getPageLabel(self);
}

namespace xgui
{
	TabClass tab_class_info;

	TabClass::TabClass() : ContainerClass() 
	{
		registerProperty("selected-num", Tab_set, Tab_get, false);
		registerProperty("selected-page", Tab_set, Tab_get, false); 

		registerInitProperty("align", false);

		registerMethod("setPageIcon", new DMethod2< Type<int>, xgui::Tab, Ptr<Type<Widget> >, Ptr<Type<Image> > >(&xgui::Tab::setPageIcon));
		registerMethod("getPageIcon", new DMethod1< Ptr<Type<Image> >, xgui::Tab, Ptr<Type<Widget> > >(&xgui::Tab::getPageIcon));

		registerMethod("setPageLabel", new DMethod2< Type<int>, xgui::Tab, Ptr<Type<Widget> >, Ref<Const<Type<std::string> > > >(&xgui::Tab::setPageLabel));
		registerMethod("getPageLabel", new DMethod1< Ref<Const<Type<std::string> > >, xgui::Tab, Ptr<Type<Widget> > >(&xgui::Tab::getPageLabel));
	}
}

namespace xgui
{
	Tab::Tab(xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::Tab * widget)
	: Container ( parent, cinfo, widget )
	{
		tab_impl_ = dynamic_cast<xguimpl::Tab *>(impl_);
	}

	Tab::Tab(xgui::Container * parent, const std::string & alignment)
	: Container ( parent, &tab_class_info, new xguimpl::Tab(parent, alignment) )
	{
		tab_impl_ = dynamic_cast<xguimpl::Tab *>(impl_);
		impl_->setAccessor(this);
		post_construction();
	}

	Tab::~Tab() {}

	void Tab::addChild(xgui::Widget * w)
	{
		w->registerProperty("tab-icon", Tab_child_set, Tab_child_get, false);
		w->registerProperty("tab-label", Tab_child_set, Tab_child_get, false);

		tab_impl_->addChild ( w );
		Container::addChild ( w );
	}

	void Tab::delChild(xgui::Widget * w)
	{
		xgui::Image * img = child_props_[w].first;
		if (img) img->unref();

		tab_impl_->delChild ( w );
		Container::delChild ( w );
	}

	int Tab::setPageIcon(xgui::Widget *w, xgui::Image * icon)
	{
		if (icon) {
			icon->ref();
			child_props_[w].first = icon;
			tab_impl_->setPageIcon(w, icon);
			return 1;
		}
		return 0;
	}

	int Tab::setPageLabel(xgui::Widget *w, std::string const &str)
	{
		child_props_[w].second = str;
		tab_impl_->setPageLabel(w, str);
		return 1;
	}

	xgui::Image * Tab::getPageIcon(xgui::Widget * w) { return child_props_[w].first; }
	std::string const &Tab::getPageLabel(xgui::Widget * w) { return child_props_[w].second; }

	std::string Tab::dump()
	{
		std::string xml = "<";
		xml+=cinfo_->tag();
		xml+=dumpProperties();
		xml+=">\n";

		for(WidgetsVector::iterator c = children_.begin(); c != children_.end(); ++c) {
			std::string label;
			std::string icon;
			(*c)->get("tab-label", label);
			(*c)->get("tab-icon", icon);

			xml += "<tabpage";
			if (!label.empty())
				xml += " label=\""+label+"\" ";
			if (!icon.empty())
				xml += " icon=\""+icon+"\" ";
			xml+=">\n";

			xml += (*c)->dump();

			xml += "</tabpage>\n";
		}

		xml+="</";
		xml+=cinfo_->tag();
		xml+=">\n";

		return xml;
	}
}
