//
// C++ Implementation: list
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
#	include "list.h"
#	include "gtk/list_impl.h"
#	include "master.h"
#	include "container.h"
#	include "style.h"
#	include "model.h"
#	include "dynamic.h"
#endif

static void List_set(xgui::Object * o, std::string const &name, std::string const &val)
{
	xgui::List * self = dynamic_cast<xgui::List*>(o);
	xguimpl::List * impl = dynamic_cast<xguimpl::List*>(self->getImpl());

	impl->set_list_prop(name, val);
}

static void List_get(xgui::Object * o, std::string const &name, std::string &dest)
{
	xgui::List * self = dynamic_cast<xgui::List*>(o);
	xguimpl::List * impl = dynamic_cast<xguimpl::List*>(self->getImpl());

	impl->get_list_prop(name, dest);
}

namespace xguimpl
{
	struct ListColumnInfo
	{
		int pos;
		std::string name;
		int width;
		std::string align;

		ListColumnInfo(int cpos, const std::string & cname, int cwidth, const std::string & calign)
		: pos(cpos), name(cname), width(cwidth), align(calign) {}
	};
}

namespace xgui
{
	xgui::ListClass list_class_info;

	ListClass::ListClass() : ViewClass() 
	{
		registerProperty("visible-headers", List_set, List_get, true);
		registerProperty("has-icons", List_set, List_get, true);
		registerProperty("has-checkboxes", List_set, List_get, true);

		registerInitProperty("model", false);
		registerInitProperty("stylemanager", false);

		registerMethod("addColumn", new DMethod5< Type<int>, xgui::List, Type<int>,
		                                Ref<Const<Type<std::string> > >, Ref<Const<Type<std::string> > >,
		                                Type<int>, Ref<Const<Type<std::string> > > >
		                                (&xgui::List::addColumn));
		registerMethod("delColumn", new DMethod1< Type<int>, xgui::List, TypeOf<std::string const &>::Type >
		                                (&xgui::List::delColumn));
		registerMethod("sort", new DMethod2< Type<int>, xgui::List, Ref<Const<Type<std::string> > >, 
		                       Ptr< Type<TextPairCallback> > >(&List::sort));

		registerMethod("isChecked", new DMethod2< Type<bool>, xgui::List, Ptr< Type<Model> >, Type<int> >
		                            (&List::isChecked));
		registerMethod("check", new DMethod3< Type<int>, xgui::List, Ptr< Type<Model> >, Type<int>,
		                        Type<bool> >(&List::check));
		registerMethod("checkAll", new DMethod2< Type<int>, xgui::List, Ptr< Type<Model> >,
		                           Type<bool> >(&List::checkAll));
		
	}
}

namespace xgui
{
	void List::init()
	{
		list_impl_ = dynamic_cast<xguimpl::List *>(impl_);
		ASSERT(list_impl_);
	}

	List::List(xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::List * widget)
	: View ( parent, cinfo, widget ), num_columns_(0), styleman_(0)
	{
		init();
	}

	List::List(xgui::Container * parent, xgui::StyleManager * sm)
	: View ( parent, &list_class_info, new xguimpl::List(parent) ), num_columns_(0), styleman_(sm)
	{
		impl_->setAccessor(this);
		post_construction();
		init();

		if (sm) sm->ref();
	}

	List::~List() 
	{
		std::map<std::string, xguimpl::ListColumnInfo*>::iterator col;
		for(col = columns_.begin(); col != columns_.end(); ++col)
			delete col->second;

		if (styleman_) styleman_->unref();
	}

	int List::addColumn ( int cpos, const std::string & cid, const std::string & cname, int cwidth, const std::string & calign )
	{
		if (columns_.find(cid) != columns_.end()) 
			return num_columns_;

		num_columns_ += 1;
		columns_[cid] = new xguimpl::ListColumnInfo(cpos, cname, cwidth, calign);
		list_impl_->addColumn ( cpos, cid, cname, cwidth, calign );

		return num_columns_;
	}

	int List::delColumn ( const std::string & id )
	{
		std::map<std::string, xguimpl::ListColumnInfo*>::iterator col = columns_.find(id);
		if (col == columns_.end())
			return num_columns_;
		else {
			delete col->second;
			columns_.erase(id);
		}

		if (list_impl_->delColumn ( id ))
			--num_columns_;
		return num_columns_;
	}

	int List::sort ( const std::string & column_id, xgui::TextPairCallback * callback )
	{
		list_impl_->sort ( column_id, callback );
		return 1;
	}

	bool List::isChecked ( xgui::Model * parent, int child_pos )
	{
		return list_impl_->isChecked ( parent, child_pos );
	}

	int List::check ( xgui::Model * parent, int child_pos, bool state )
	{
		list_impl_->check ( parent, child_pos, state );
		return 1;
	}

	int List::checkAll ( xgui::Model * parent, bool state )
	{
		list_impl_->checkAll ( parent, state );
		return 1;	
	}

	std::string List::dump()
	{
		std::string xml = "<";
		xml+=cinfo_->tag();
		xml+=dumpProperties();

		if (styleman_) {
			xml+="stylemanager=\"";
			xml+=styleman_->id();
			xml+="\" ";
		}

		xml+=">\n";

		std::map<std::string, xguimpl::ListColumnInfo*>::iterator col;
		for(col = columns_.begin(); col != columns_.end(); ++col) {
			xml += "<listcolumn ";
			xml += "id=\"";
			xml += col->first;
			xml += "\" ";

			xml += "position=\"";
			xml += xgui::semantic_cast<std::string>(col->second->pos);
			xml += "\" ";

			xml += "name=\"";
			xml += col->second->name;
			xml += "\" ";

			xml += "width=\"";
			xml += xgui::semantic_cast<std::string>(col->second->width);
			xml += "\" ";

			xml += "align=\"";
			xml += col->second->align;
			xml += "\" ";

			xml += "/>\n";
		}
		

		xml+="</";
		xml+=cinfo_->tag();
		xml+=">\n";

		return xml;
	}
}
