//
// C++ Implementation: tree
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
#	include "tree.h"
#	ifdef OPENSTEP
#		include "openstep/tree_impl.h"
#	else
#		include "gtk/tree_impl.h"
#	endif
#	include "master.h"
#	include "style.h"
#	include "container.h"
#	include "model.h"
#	include "dynamic.h"
#endif

static void Tree_set(xgui::Object * o, std::string const &name, std::string const &val)
{
	xgui::Tree * self = dynamic_cast<xgui::Tree*>(o);
	xguimpl::Tree * impl = dynamic_cast<xguimpl::Tree*>(self->getImpl());

	impl->set_tree_prop(name, val);
}

static void Tree_get(xgui::Object * o, std::string const &name, std::string &dest)
{
	xgui::Tree * self = dynamic_cast<xgui::Tree*>(o);
	xguimpl::Tree * impl = dynamic_cast<xguimpl::Tree*>(self->getImpl());

	impl->get_tree_prop(name, dest);
}

namespace xgui
{
	xgui::TreeClass tree_class_info;

	TreeClass::TreeClass() : ViewClass() 
	{
		registerProperty("has-icons", Tree_set, Tree_get, true);
		registerProperty("has-checkboxes", Tree_set, Tree_get, true);

		registerInitProperty("model", false);
		registerInitProperty("stylemanager", false);

		registerMethod("expand", new DMethod1< Type<int>, xgui::Tree, Ptr<Type<Model> > >
		                                (&xgui::Tree::expand));
		registerMethod("collapse", new DMethod1< Type<int>, xgui::Tree, Ptr<Type<Model> > >
		                                (&xgui::Tree::collapse));

		registerMethod("sort", new DMethod2< Type<int>, xgui::Tree, Ptr<Type<Model> >, 
		                       Ptr< Type<TextPairCallback> > >(&Tree::sort));

		registerMethod("isChecked", new DMethod2< Type<bool>, xgui::Tree, Ptr< Type<Model> >, Type<int> >
		                            (&Tree::isChecked));
		registerMethod("check", new DMethod3< Type<int>, xgui::Tree, Ptr< Type<Model> >, Type<int>,
		                        Type<bool> >(&Tree::check));
		registerMethod("checkAll", new DMethod2< Type<int>, xgui::Tree, Ptr< Type<Model> >,
		                           Type<bool> >(&Tree::checkAll));
	}

	std::string const &TreeClass::className() { static std::string const c_name = "xgui::Tree"; return c_name; }

	bool TreeClass::isInstanceable() { return true; }

	bool TreeClass::canContain(xgui::ClassInfo * c) 
	{
		if (dynamic_cast<xgui::ModelClass*>(c)) return true;
		else return false; 
	}

	xgui::Object * TreeClass::create(xgui::Object * parent, xgui::StringMap &properties)
	{
		xgui::StringMap::iterator prop_i;
		xgui::Container * wparent = dynamic_cast<xgui::Container*>( parent );
		xgui::Tree * w = 0;

		if (!parent) {
			XGUI_ERROR("Cannot create a Tree without parent");
			return 0;
		}

		if (!wparent) {
			XGUI_ERROR("Cannot create a Tree as a child of a " << parent->className());
			return 0;
		}

		std::string model_id;	
		if ( (prop_i = properties.find("model")) != properties.end() ) {
			model_id = prop_i->second;
			properties.erase(prop_i);
		}

		std::string sm_id;	
		if ( (prop_i = properties.find("stylemanager")) != properties.end() ) {
			sm_id = prop_i->second;
			properties.erase(prop_i);
		}

		xgui::Model * model = dynamic_cast<xgui::Model*>(Master::FindInPool(model_id));
		xgui::StyleManager * sm = dynamic_cast<xgui::StyleManager*>(Master::FindInPool(sm_id));

		w = xgui::Master::CreateTree(wparent, sm);
		if (model)
			w->setModel(model);

		return w;
	}
}


namespace xgui
{
	void Tree::init()
	{
		tree_impl_ = dynamic_cast<xguimpl::Tree *>(impl_);
		ASSERT(tree_impl_);
	}

	Tree::Tree ( xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::Tree * widget )
	: View ( parent, cinfo, widget ), styleman_(0)
	{
		init();
	}

	Tree::Tree ( xgui::Container * parent, xgui::StyleManager * sm )
	: View ( parent, &tree_class_info, new xguimpl::Tree(parent) ), styleman_(sm)
	{
		impl_->setAccessor(this);
		post_construction();
		init();

		if (sm) sm->ref();
	}

	Tree::~Tree() 
	{
		if (styleman_) styleman_->unref();
	}

	int Tree::sort ( xgui::Model * node, xgui::TextPairCallback * callback )
	{
		tree_impl_->sort ( node, callback );
		return 1;
	}

	int Tree::expand ( xgui::Model * node )
	{
		tree_impl_->expand ( node );
		return 1;
	}

	int Tree::collapse ( xgui::Model * node )
	{
		tree_impl_->collapse ( node );
		return 1;
	}

	bool Tree::isChecked ( xgui::Model * parent, int child_pos )
	{
		return tree_impl_->isChecked ( parent, child_pos );
	}

	int Tree::check ( xgui::Model * parent, int child_pos, bool state )
	{
		tree_impl_->check ( parent, child_pos, state );
		return 1;
	}

	int Tree::checkAll ( xgui::Model * parent, bool state )
	{
		tree_impl_->checkAll ( parent, state );
		return 1;
	}

	std::string Tree::dump()
	{
		std::string xml = "<";
		xml+=cinfo_->tag();
		xml+=dumpProperties();

		if (styleman_) {
			xml+="stylemanager=\"";
			xml+=styleman_->id();
			xml+="\" ";
		}

		xml+="/>\n";

		return xml;
	}
}
