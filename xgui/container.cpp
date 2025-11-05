//
// C++ Implementation: container
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
#	include <algorithm>
#	include "container.h"
#	include "dynamic.h"
#endif

struct CmpWidgetName : public std::binary_function<xgui::Widget *, std::string, bool> {
	bool operator()( xgui::Widget * child, std::string name) const {
		std::string id;
		child->get("id", id);
	
		if (id == name) return true;
		return false;
	}
};


namespace xgui
{
	xgui::ContainerClass container_class_info;

	ContainerClass::ContainerClass() : WidgetClass() 
	{
		registerMethod("findChild", new DMethod1< Ptr< Type<xgui::Widget> >, xgui::Container, Ref< Const< Type<std::string> > > >(&xgui::Container::findChild));
		registerMethod("findDirectChild", new DMethod1< Ptr< Type<xgui::Widget> >, xgui::Container, Ref< Const< Type<std::string> > > >(&xgui::Container::findDirectChild));
		registerMethod("getChild", new DMethod1< Ptr< Type<xgui::Widget> >, xgui::Container, Type<int> >(&xgui::Container::getChild));
		registerMethod("size", new DMethod0< Type<unsigned int>, xgui::Container >(&xgui::Container::size));
	}

	bool ContainerClass::isContainer() { return true; }
	std::string const &ContainerClass::className() { static std::string const c_name = "xgui::Container"; return c_name; }

	bool ContainerClass::canContain(xgui::ClassInfo * c) 
	{
		if (dynamic_cast<xgui::WidgetClass*>(c)) return true;
		return false; 
	}

	bool ContainerClass::canBeContainedIn(xgui::ClassInfo * c) 
	{
		if (dynamic_cast<xgui::ContainerClass*>(c)) return true;
		return false;
	}
}

namespace xgui
{
	Container::Container(xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::Widget * widget)
	: xgui::Widget(parent, (cinfo ? cinfo : &container_class_info), widget)
	{
	}
	
	Container::~Container()
	{
		for(WidgetsVector::iterator i = children_.begin(); i != children_.end(); ++i)
			(*i)->setAsFloating();
	}
	
	void Container::delChild(xgui::Widget * wid)
	{
		children_.erase(std::remove(children_.begin(), children_.end(), wid), children_.end());
		recalcLayout();
	}
	
	xgui::Widget * Container::findChild(std::string const &child_name)
	{
		if ( id_ == child_name ) {
			return static_cast<xgui::Widget*>(this);
		}
		else {
			for ( WidgetsVector::iterator i = children_.begin(); i != children_.end(); ++i ) {
				xgui::Container * subcont = dynamic_cast<xgui::Container*>(*i);
				if (subcont) {
					xgui::Widget * found_child = subcont->findChild(child_name);
					if (found_child) return found_child;
				}
				else {
					std::string child_id;
					if ((*i)->isIdentifiedAs(child_name))
						return static_cast<xgui::Widget*>(*i);
				}
			}
		}
	
		return 0;
	}

	void Container::addChild(xgui::Widget * w) { children_.push_back( w ); }

	xgui::Widget * Container::findDirectChild(std::string const &child_name)
	{
		WidgetsVector::iterator c = std::find_if(children_.begin(), children_.end(), std::bind2nd(CmpWidgetName(), child_name));
		if (c != children_.end()) return *c;
		return 0;	
	}

	xgui::Widget * Container::getChild(int pos)
	{
		if ((pos >= 0) && ((unsigned int)pos < children_.size()))
			return children_[pos];
		else
			return 0;
	}

	unsigned int Container::size() { return children_.size(); }

	std::string Container::dump()
	{
		std::string xml = "<";
		xml+=cinfo_->tag();
		xml+=dumpProperties();
		xml+=">\n";

		for(WidgetsVector::iterator c = children_.begin(); c != children_.end(); ++c)
			xml += (*c)->dump();

		xml+="</";
		xml+=cinfo_->tag();
		xml+=">\n";

		return xml;
	}
}
