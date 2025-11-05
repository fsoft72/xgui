//
// C++ Implementation: combobox
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
#	include "combobox.h"
#	include "gtk/combobox_impl.h"
#	include "master.h"
#	include "container.h"
#	include "model.h"
#	include "dynamic.h"
#endif

static void Combobox_set(xgui::Object * o, std::string const &name, std::string const &vals)
{
	xgui::Combobox * self = dynamic_cast<xgui::Combobox*>(o);
	xguimpl::Combobox * impl = dynamic_cast<xguimpl::Combobox*>(self->getImpl());

	bool editable_ = self->isEditable();

	if ( name == "text" ) {
		if (editable_)
			impl->setEntryText(vals);
	}
	else if ( name == "selected-item" ) {
		impl->setSelected(xgui::semantic_cast<int>(vals));
	}
	else if ( name == "maxlength" ) {
		if (editable_)
			impl->setMaxLength(xgui::semantic_cast<int>(vals));
	}
	else if ( name == "editable" ) {
		unsigned int editable = xgui::semantic_cast<unsigned int>(vals);	
		self->setAsEditable(editable ? true : false);
	}
}

static void Combobox_get(xgui::Object * o, std::string const &name, std::string &dest)
{
	xgui::Combobox * self = dynamic_cast<xgui::Combobox*>(o);
	xguimpl::Combobox * impl = dynamic_cast<xguimpl::Combobox*>(self->getImpl());

	bool editable_ = self->isEditable();

	if ( name == "text" ) {
		if (editable_)
			dest = impl->getEntryText();
		else
			dest = impl->getSelectedItemText();
	}
	else if ( name == "selected-item" ) {
		dest = xgui::semantic_cast<std::string>(impl->getSelected());
	}
	else if ( name == "editable" ) {
		dest = editable_ ? "1" : "0";
	}
	else if ( name == "maxlength" ) {
		if (editable_)
			dest = xgui::semantic_cast<std::string>(impl->getMaxLength());
		else
			dest = "0";
	}
}

namespace xgui
{
	xgui::ComboboxClass combobox_class_info;

	ComboboxClass::ComboboxClass() : ViewClass() 
	{
		registerProperty("text", Combobox_set, Combobox_get, true);
		registerProperty("selected-item", Combobox_set, Combobox_get, false);
		registerProperty("maxlength", Combobox_set, Combobox_get, true);
		registerProperty("editable", Combobox_set, Combobox_get, true);

		registerInitProperty("editable", false);
		registerInitProperty("model", false);

		registerMethod("insertText", new DMethod2< Type<int>, xgui::Combobox, Ref< Const< Type<std::string> > >, Type<unsigned int> >(&xgui::Combobox::insertText));
		registerMethod("appendText", new DMethod1< Type<int>, xgui::Combobox, Ref< Const< Type<std::string> > > >(&xgui::Combobox::appendText));
		registerMethod("prependText", new DMethod1< Type<int>, xgui::Combobox, Ref< Const< Type<std::string> > > >(&xgui::Combobox::prependText));

		registerMethod("popup", new DMethod0< Type<int>, xgui::Combobox >(&xgui::Combobox::popup));
		registerMethod("popdown", new DMethod0< Type<int>, xgui::Combobox >(&xgui::Combobox::popdown));

		registerMethod("isEditable", new DMethod0< Type<bool>, xgui::Combobox >(&xgui::Combobox::isEditable));
		registerMethod("setAsEditable", new DMethod1< Type<int>, xgui::Combobox, Type<bool> >(&xgui::Combobox::setAsEditable));
	}

	std::string const &ComboboxClass::className() { static std::string const c_name = "xgui::Combobox"; return c_name; }

	bool ComboboxClass::isInstanceable() { return true; }

	bool ComboboxClass::canContain(xgui::ClassInfo * c) 
	{
		if (dynamic_cast<xgui::ModelClass*>(c)) return true;
		else return false; 
	}

	xgui::Object * ComboboxClass::create(xgui::Object * parent, xgui::StringMap &properties)
	{
		xgui::StringMap::iterator prop_i;
		xgui::Container * wparent = dynamic_cast<xgui::Container*>( parent );
		xgui::Combobox * w = 0;

		if (!parent) {
			XGUI_ERROR("Cannot create a Combobox without parent");
			return 0;
		}

		if (!wparent) {
			XGUI_ERROR("Cannot create a Combobox as a child of a " << parent->className());
			return 0;
		}

		bool editable = false;

		if ( (prop_i = properties.find("editable")) != properties.end()) {
			editable = xgui::semantic_cast<bool>(prop_i->second);
			properties.erase(prop_i);
		}

		std::string model_id;	
		if ( (prop_i = properties.find("model")) != properties.end() ) {
			model_id = prop_i->second;
			properties.erase(prop_i);
		}

		xgui::Model * model = dynamic_cast<xgui::Model*>(Master::FindInPool(model_id));

		w = xgui::Master::CreateCombobox(wparent, editable);
		if (model)
			w->setModel(model);


		return w;
	}
}

namespace xgui
{
	void Combobox::init()
	{
		cbox_impl_ = dynamic_cast<xguimpl::Combobox *>(impl_);
		ASSERT(cbox_impl_);
	}

	Combobox::Combobox(xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::Combobox * widget)
	: View ( parent, cinfo, widget ), editable_(false)
	{
		init();
	}

	Combobox::Combobox(xgui::Container * parent, bool editable)
	: View ( parent, &combobox_class_info, new xguimpl::Combobox(parent, editable) ), editable_(editable)
	{
		impl_->setAccessor(this);
		post_construction();
		init();
	}

	Combobox::~Combobox() { }

	bool Combobox::isEditable() { return editable_; }
	int Combobox::setAsEditable(bool editable)
	{
		if (editable == editable_) return 0;
		
		editable_ = editable;
		cbox_impl_->setAsEditable(editable_);
		return 1;
	}
	
	int Combobox::insertText(std::string const &text, unsigned int pos)
	{
		if (editable_) {
			cbox_impl_->insertText(text, pos);
			return 1;
		}
		return 0;
	}
	
	int Combobox::appendText(std::string const &text)
	{
		if (editable_) {
			cbox_impl_->appendText(text);
			return 1;
		}
		return 0;
	}
	
	int Combobox::prependText(std::string const &text)
	{
		if (editable_) {
			cbox_impl_->prependText(text);
			return 1;
		}
		return 0;
	}
	
	int Combobox::popup()
	{
		cbox_impl_->popup();
		return 1;
	}
	
	int Combobox::popdown()
	{
		cbox_impl_->popdown();
		return 1;
	}
}
