//
// C++ Implementation: toolbar
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
#	include "toolbar.h"
#	include "gtk/toolbar_impl.h"
#	include "master.h"
#	include "image.h"
#	include "container.h"
#	include "dynamic.h"
#endif

static void Toolbar_set(xgui::Object * o, std::string const &name, std::string const &val)
{
	xgui::Toolbar * self = dynamic_cast<xgui::Toolbar*>(o);
	xguimpl::Toolbar * impl = dynamic_cast<xguimpl::Toolbar*>(self->getImpl());

	if ( name == "style" )
		impl->setStyle(val);
}

static void Toolbar_get(xgui::Object * o, std::string const &name, std::string &dest)
{
	xgui::Toolbar * self = dynamic_cast<xgui::Toolbar*>(o);
	xguimpl::Toolbar * impl = dynamic_cast<xguimpl::Toolbar*>(self->getImpl());

	if ( name == "style" )
		dest = impl->getStyle();
}

namespace xgui
{
	ToolbarClass::ToolbarClass() : WidgetClass() 
	{
		registerProperty("style", Toolbar_set, Toolbar_get, true);

		registerMethod("addItem", new DMethod5< Type<int>, xgui::Toolbar, Ref<Const<Type<std::string> > >,
		                                Ref<Const<Type<std::string> > >, Ptr<Type<xgui::Image> >,
		                                Ref<Const<Type<std::string> > >, Ref<Const<Type<std::string> > > >
		                                (&xgui::Toolbar::addItem));
		registerMethod("delItem", new DMethod1< Type<int>, xgui::Toolbar, Ref<Const<Type<std::string> > > >
		                                (&xgui::Toolbar::delItem));

		registerMethod("enableButton", new DMethod1< Type<int>, Toolbar, Ref<Const<Type<std::string> > > >
		                                (&xgui::Toolbar::enableButton));
		registerMethod("disableButton", new DMethod1< Type<int>, Toolbar, Ref<Const<Type<std::string> > > >
		                                (&xgui::Toolbar::disableButton));
		registerMethod("checkButton", new DMethod1< Type<int>, Toolbar, Ref<Const<Type<std::string> > > >
		                                (&xgui::Toolbar::checkButton));
		registerMethod("uncheckButton", new DMethod1< Type<int>, Toolbar, Ref<Const<Type<std::string> > > >
		                                (&xgui::Toolbar::uncheckButton));
		registerMethod("isButtonChecked", new DMethod1<Type<bool>, Toolbar,Ref<Const<Type<std::string> > > >
		                                (&xgui::Toolbar::isButtonChecked));		
	}

	ToolbarClass toolbar_class_info;
}

namespace xgui
{	
	Toolbar::Toolbar(xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::Toolbar * widget)
	: Widget ( parent, cinfo, widget ), nelements_(0), tb_impl_(0)
	{
		tb_impl_ = dynamic_cast<xguimpl::Toolbar *>(impl_);
		ASSERT(tb_impl_);

	}

	Toolbar::Toolbar(xgui::Container * parent)
	: Widget ( parent, &toolbar_class_info, new xguimpl::Toolbar(parent) ), nelements_(0), tb_impl_(0)
	{
		tb_impl_ = dynamic_cast<xguimpl::Toolbar *>(impl_);
		ASSERT(tb_impl_);

		impl_->setAccessor(this);
		post_construction();
	}

	Toolbar::~Toolbar() 
	{
		for (std::vector<Image *>::iterator iter = images_.begin(); iter != images_.end(); ++iter )
			(*iter)->unref();

		for(std::vector<TbButtonInfo*>::iterator i = items_.begin(); i != items_.end(); ++i) {
			delete *i;
		}
	}

	int Toolbar::addItem( std::string const &id, std::string const &text, xgui::Image * icon, std::string const &tooltip, std::string const & type )
	{
		static std::vector<xgui::TbButtonInfo> cur_radio_group;
		static bool is_in_group = false;
		TbButtonInfo * btn = 0;

		if ( (type != "radio_begin") && (type != "radio_end") )
			btn = new TbButtonInfo(id, text, icon, tooltip, type);	
	
		items_.push_back(btn);

		if (type == "check") {
			if (!is_in_group) {
				tb_impl_->addToggleButton( *btn );
				++nelements_;
			}
			else
				cur_radio_group.push_back( *btn );
		}
		else if (type == "separator") {
			tb_impl_->addSeparator();
			++nelements_;
		}
		else if (type == "radio_begin") {
			cur_radio_group.clear();
			is_in_group = true;
		}
		else if (type == "radio_end") {
			if (is_in_group) {
				tb_impl_->addRadioButtons(cur_radio_group);
				is_in_group = false;
				nelements_+=cur_radio_group.size();
				cur_radio_group.clear();
				return nelements_;
			}
		}
		else {
			tb_impl_->addButton( *btn );
			++nelements_;
		}

		return nelements_;
	}

	int Toolbar::delItem ( const std::string & name )
	{
		if (tb_impl_->removeButton(name))
			--nelements_;

		for(std::vector<TbButtonInfo*>::iterator i = items_.begin(); i != items_.end(); ++i) {
			if ((*i)->name == name) {
				items_.erase(i);
				break;
			}
		}

		return nelements_; 
	}

	int Toolbar::enableButton ( const std::string & name ) { tb_impl_->enableButton(name); return 1; }
	int Toolbar::disableButton ( const std::string & name ) { tb_impl_->disableButton(name); return 1; }
	int Toolbar::checkButton ( const std::string & name ) { tb_impl_->checkButton(name); return 1; }
	int Toolbar::uncheckButton ( const std::string & name ) { tb_impl_->uncheckButton(name); return 1; }
	bool Toolbar::isButtonChecked ( const std::string & name ) { return tb_impl_->isButtonChecked(name); }

	std::string Toolbar::dump()
	{
		std::string xml = "<";
		xml+=cinfo_->tag();
		xml+=dumpProperties();
		xml+=">\n";

		std::vector<TbButtonInfo*>::iterator item;
		bool is_group = false;

		for(item = items_.begin(); item != items_.end(); ++item) {
			TbButtonInfo * btn = *item;
		
			if( btn == 0) {
				if (is_group == false) {
					xml += "<toolbarradiogroup>\n";	
					is_group = true;
				}
				else {
					xml += "</toolbarradiogroup>\n";
					is_group = false;
				}
			}
			else if( btn->type == "separator")
				xml += "<toolbarseparator/>\n";
			else if( btn->type == "check" ) {
				xml += "<toolbarcheckbutton ";

				xml += "id=\"";
				xml += btn->name;
				xml += "\" ";

				xml += "text=\"";
				xml += btn->text;
				xml += "\" ";

				xgui::Image * icon = btn->image;
				if (icon) {
					xml += "image=\"";
					xml += icon->getPath();
					xml += "\" ";
				}

				std::string tooltip = btn->tooltip;
				if (!tooltip.empty()) {
					xml += "tooltip=\"";
					xml += tooltip;
					xml += "\" ";
				}

				xml += "/>\n";
			}
			else {
				xml += "<toolbarbutton ";

				xml += "id=\"";
				xml += btn->name;
				xml += "\" ";

				xml += "text=\"";
				xml += btn->text;
				xml += "\" ";

				xgui::Image * icon = btn->image;
				if (icon) {
					xml += "image=\"";
					xml += icon->getPath();
					xml += "\" ";
				}

				std::string tooltip = btn->tooltip;
				if (!tooltip.empty()) {
					xml += "tooltip=\"";
					xml += tooltip;
					xml += "\" ";
				}

				xml += "/>\n";
			}
		}
		

		xml+="</";
		xml+=cinfo_->tag();
		xml+=">\n";

		return xml;
	}
}
