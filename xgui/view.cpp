//
// C++ Implementation: view
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
#	include "model.h"
#	include "view.h"
#	ifdef OPENSTEP
#		include "openstep/view_impl.h"
#	else
#		include "gtk/view_impl.h"
#	endif
#	include "dynamic.h"
#	include "master.h"
#endif

static void View_set(xgui::Object * o, std::string const &name, std::string const &vals)
{
	xgui::View * self = dynamic_cast<xgui::View*>(o);

	if (name == "model") {
		xgui::Model * model = dynamic_cast<xgui::Model*>(xgui::Master::FindInPool(vals));
		if(model)
			self->setModel(model);
		else
			self->setModel(0);
	}
}

static void View_get(xgui::Object * o, std::string const &name, std::string &dest)
{
	xgui::View * self = dynamic_cast<xgui::View*>(o);

	if (name == "model") {
		xgui::Model * model = self->getModel();
		if(model)
			dest = model->id();
		else
			dest = "";
	}
}

namespace xgui
{
	xgui::ViewClass view_class_info;

	ViewClass::ViewClass() : WidgetClass() 
	{
		registerProperty("model", View_set, View_get, true);

		registerMethod("refresh", new DMethod0< Type<int>, xgui::View >(&xgui::View::refresh));

		registerMethod("getModel", new DMethod0< Ptr< Type<Model> >, xgui::View >(&xgui::View::getModel));
		registerMethod("setModel", new DMethod1< Type<int>, xgui::View, Ptr< Type<Model> > >
		                             (&xgui::View::setModel));

		registerMethod("isSelected", new DMethod2< Type<bool>, xgui::View, Ptr< Type<Model> >, Type<int> >
		                            (&View::isSelected));
		registerMethod("select", new DMethod3< Type<int>, xgui::View, Ptr< Type<Model> >, Type<int>,
		                        Type<bool> >(&View::select));
		registerMethod("selectAll", new DMethod2< Type<int>, xgui::View, Ptr< Type<Model> >,
		                           Type<bool> >(&View::selectAll));
	}
	std::string const &ViewClass::className() { static std::string const c_name = "xgui::View"; return c_name; }
}

namespace xgui
{
	View::View(xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::View * widget)
	: Widget ( parent, cinfo, widget ), model_(0)
	{
		view_impl_ = dynamic_cast<xguimpl::View *>(impl_);
		ASSERT(view_impl_);
	}

	View::~View() 
	{
		setModel(0);
	}

	int View::setModel ( xgui::Model * vmodel )
	{
		if (vmodel == model_) return 0;
		if (model_) { model_->unlinkView(this); model_->unref(); }
		model_ = vmodel;
		if (model_) {
			model_->ref();
			model_->linkView ( this );
		}
		return 1;
	}

	xgui::Model * View::getModel()
	{
		return model_;
	}


	void View::insertItem ( xgui::Model * parent, int child_pos, xgui::Model * child ) { view_impl_->insertItem(parent, child_pos, child); }
	void View::removeItem ( xgui::Model * parent, int child_pos, xgui::Model * child ) { view_impl_->removeItem(parent, child_pos, child); }
	int View::refresh() { view_impl_->refresh(); return 1; }

	bool View::isSelected(xgui::Model * parent, int child_pos) { return view_impl_->isSelected(parent, child_pos); }
	int View::select(xgui::Model * parent, int child_pos, bool state) { view_impl_->select(parent, child_pos, state); return 1; }
	int View::selectAll(xgui::Model * parent, bool state) { view_impl_->selectAll(parent, state); return 1; }
}

