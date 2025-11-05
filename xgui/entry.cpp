//
// C++ Implementation: entry
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
#	include "entry.h"
#	include "gtk/entry_impl.h"
#	include "master.h"
#	include "container.h"
#	include "dynamic.h"
#endif

static std::string const entry_class_name = "xgui::Entry";

static void Entry_set(xgui::Object * o, std::string const &name, std::string const &val)
{
	xgui::Entry * self = dynamic_cast<xgui::Entry*>(o);
	xguimpl::Entry * impl = dynamic_cast<xguimpl::Entry*>(self->getImpl());

	impl->set_entry_prop(name, val);
}

static void Entry_get(xgui::Object * o, std::string const &name, std::string &dest)
{
	xgui::Entry * self = dynamic_cast<xgui::Entry*>(o);
	xguimpl::Entry * impl = dynamic_cast<xguimpl::Entry*>(self->getImpl());

	impl->get_entry_prop(name, dest);
}

namespace xgui
{
	xgui::EntryClass entry_class_info;

	EntryClass::EntryClass() : WidgetClass() 
	{
		registerProperty("text", Entry_set, Entry_get, true);
		registerProperty("editable", Entry_set, Entry_get, true);
		registerProperty("maxlength", Entry_set, Entry_get, true);
		registerProperty("password-mode", Entry_set, Entry_get, true);
		registerProperty("alignment", Entry_set, Entry_get, true);

		registerInitProperty("text", false);
		registerInitProperty("editable", false);
		registerInitProperty("maxlength", false);
		registerInitProperty("password-mode", false);

		registerMethod("insertText", new DMethod2< Type<int>, xgui::Entry, Ref< Const< Type<std::string> > >, Type<int> >(&xgui::Entry::insertText));
		registerMethod("appendText", new DMethod1< Type<int>, xgui::Entry, Ref< Const< Type<std::string> > > >(&xgui::Entry::appendText));
		registerMethod("prependText", new DMethod1< Type<int>, xgui::Entry, Ref< Const< Type<std::string> > > >(&xgui::Entry::prependText));
	}

	std::string const &EntryClass::className() { static std::string const c_name = "xgui::Entry"; return c_name; }

	bool EntryClass::isInstanceable() { return true; }

	xgui::Object * EntryClass::create(xgui::Object * parent, xgui::StringMap &properties)
	{
		xgui::StringMap::iterator prop_i;
		xgui::Container * wparent = dynamic_cast<xgui::Container*>( parent );
		xgui::Entry * w = 0;

		if (!parent) {
			XGUI_ERROR("Cannot create a Entry without parent");
			return 0;
		}

		if (!wparent) {
			XGUI_ERROR("Cannot create a Entry as a child of a " << parent->className());
			return 0;
		}

		std::string text = "";
		int maxlen = 0;
		bool editable = true;
		bool password_mode = false;

		if ( (prop_i = properties.find("maxlength")) != properties.end()) {
			maxlen = xgui::semantic_cast<int>(prop_i->second);
			properties.erase(prop_i);
		}

		if ( (prop_i = properties.find("editable")) != properties.end()) {
			editable = xgui::semantic_cast<bool>(prop_i->second);
			properties.erase(prop_i);
		}

		if ( (prop_i = properties.find("password-mode")) != properties.end()) {
			password_mode = xgui::semantic_cast<bool>(prop_i->second);
			properties.erase(prop_i);
		}

		if ( (prop_i = properties.find("text")) != properties.end()) {
			text = prop_i->second;
			properties.erase(prop_i);
		}

		w = xgui::Master::CreateEntry(wparent, text, maxlen, editable, password_mode);

		return w;
	}
}

namespace xgui
{
	void Entry::init()
	{
		entry_impl_ = dynamic_cast<xguimpl::Entry *>(impl_);
		ASSERT(entry_impl_);
	}

	Entry::Entry ( xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::Entry * widget )
	: Widget ( parent, cinfo, widget )
	{
		init();
	}

	Entry::Entry ( xgui::Container * parent, std::string const &text, int maxlen, bool editable, bool password_mode )
	: Widget ( parent, &entry_class_info, new xguimpl::Entry ( parent, text, maxlen, editable, password_mode ) )
	{
		impl_->setAccessor(this);
		post_construction();
		init();
	}

	Entry::~Entry() 
	{
	}

	int Entry::appendText ( const std::string & text )
	{
		entry_impl_->appendText ( text );
		return 1;
	}

	int Entry::prependText ( const std::string & text )
	{
		entry_impl_->prependText ( text );
		return 1;
	}

	int Entry::insertText ( const std::string & text, int pos )
	{
		entry_impl_->insertText ( text, pos );
		return 1;
	}
}
