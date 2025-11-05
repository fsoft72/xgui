//
// C++ Implementation: model
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
#	include "master.h"
#	include "dynamic.h"
#endif

namespace xgui
{
	static void Model_set(xgui::Object * o, std::string const &name, std::string const &val)
	{
		xgui::Model * self = dynamic_cast<xgui::Model*>(o);
	
		self->setStyle(val);
	}
	
	static void Model_get(xgui::Object * o, std::string const &name, std::string &dest)
	{
		xgui::Model * self = dynamic_cast<xgui::Model*>(o);
		dest = self->getStyle();
	}

	ModelClass::ModelClass() : ObjectClass()
	{
		registerInitProperty("style", false); 

		registerProperty("style", Model_set, Model_get, true);

		registerMethod("appendChild", new DMethod1< Type<int>, Model, Ptr< Type<TreeNode> > >((int(Model::*)(TreeNode*))(&Model::appendChild)));

		registerMethod("addString", new DMethod1< Type<int>, Model, TypeOf<std::string const &>::Type >(&Model::addString));

		registerMethod("setString", new DMethod2< Type<int>, Model, TypeOf<std::string const &>::Type, Type<int> >(&Model::setString));
	}

	xgui::ModelClass model_class_info;
}

namespace xgui
{
	TreeNode::TreeNode () : Object(&model_class_info), _owncount(0), _parent(0) { }
	TreeNode::~TreeNode ()
	{
		for ( iterator iter = _children.begin(); iter != _children.end(); ++iter )
			(*iter)->unref();
	}
	
	int TreeNode::appendChild ( TreeNode * child )
	{
		_children.push_back ( child );
		child->ref();
		child->_parent = this;
		return _children.size() - 1;
	}
	
	void TreeNode::insertChild ( TreeNode * child, int pos ) 
	{ 
		_children.insert ( _children.begin() + pos, child );
		child->ref(); 
		child->_parent = this;
	}
	
	void TreeNode::removeChild ( int pos ) 
	{ 
		iterator child = _children.begin() + pos;
		(*child)->_parent = 0;
		(*child)->unref();
		_children.erase ( child );
	}
	
	TreeNode * TreeNode::getChild ( int pos )
	{
		return _children[pos];
	}
	
	std::string TreeNode::path()
	{
		std::string path = _parent ? _parent->path() + "/" : "";
		path += id_;
		return path;
	}

	xgui::TreeNode * TreeNode::findByPath(std::string const &path)
	{
		if (id_ == path) return this;

		std::string::size_type slash_pos = path.find("/");
		std::string child_id = path.substr(0, slash_pos);
		std::string sub_child_path = child_id;

		if ( (slash_pos != std::string::npos) && (slash_pos+1 < path.size()) )
			sub_child_path = path.substr(slash_pos+1, std::string::npos);

		if (id_ == child_id) return findByPath(sub_child_path);

		for ( iterator child = _children.begin(); child != _children.end(); ++child ) {
			if ((*child)->id() == child_id)
				return (*child)->findByPath(sub_child_path);
		}

		return 0;
	}

	void TreeNode::setChild ( TreeNode * new_child, int pos )
	{
		if ((pos >= 0) && (pos < (int)_children.size())) {
			TreeNode *&child = _children[pos];
			if (child)
				child->unref();
			child = new_child;
			child->ref();
			child->_parent = this;
		}
	}
	
	TreeNode::iterator TreeNode::begin () { return _children.begin(); }
	TreeNode::iterator TreeNode::end() { return _children.end(); }
	TreeNode * TreeNode::getParent() { return _parent; }
	
	void TreeNode::ref() { ++_owncount; }
	void TreeNode::unref() { if (--_owncount <= 0) { delete this; } }
	
	unsigned int TreeNode::size()
	{
		return _children.size();
	}
	
	
	void TreeNode::removeAllChildren()
	{
		while ( _children.size() )
			removeChild(0);
	}

	Model::Model() :  TreeNode(), flags(0) { }
	Model::Model ( const std::vector<std::string> & mstring_list ) : string_list(mstring_list), flags(0) { }
	Model::~Model() {}
	
	void Model::destroy()
	{
		if (view_list.empty()) delete this;
	}
	
	void Model::setFlag(unsigned long flag) { flags |= flag; }
	void Model::unsetFlag(unsigned long flag) { flags &= ~flag; }
	bool Model::getFlagStatus(unsigned long flag) { return (flag & flags) != 0; }
	
	void Model::setStyle(std::string const &stname) { style_name = stname; }
	std::string const &Model::getStyle() { return style_name; }
	
	int Model::addString(std::string const &str) { int pos = string_list.size(); string_list.push_back(str); return pos; }
	void Model::delString(int pos) { string_list.erase(string_list.begin() + pos); }
	int Model::setString(std::string const &str, int pos) { string_list.at(pos) = str; return 0; }
	const std::string & Model::getString ( int pos ) 
	{
		static std::string null_str = "";
		if ( (pos >= 0) && (pos < (int)string_list.size()) )
			return string_list.at(pos); 
		else
			return null_str;
	}
	void Model::clearStrings() { string_list.clear(); }
	unsigned int Model::numberOfStrings() { return string_list.size(); }
	
	void Model::linkView(View *view)
	{
		if (view_list.insert(view).second) {
			view->setModel(this);
			view->refresh();
		}
	}
	
	void Model::unlinkView(View *view) { view_list.erase(view); }
	
	void Model::appendChild ( std::vector<std::string> const & string_list, std::string const & style_name )
	{
		Model * child = new Model(string_list);
		child->setStyle(style_name);
		appendChild(child);
		DMESSAGE("Appending Child Of Size: " << child->string_list.size());
	}
	
	xgui::Model * Model::getChild( int pos )
	{
		TreeNode * child = 0;
	
		if ((pos < (int)_children.size()) && (pos >= 0))
			child = TreeNode::getChild(pos);
		return dynamic_cast<Model*>(child);
	}

	xgui::Model * Model::findByPath ( std::string const &path )
	{
		return dynamic_cast<Model*>(TreeNode::findByPath(path));
	}

	int Model::findChildPos(Model * child)
	{
		int p = 0;
		for(std::vector < TreeNode * >::iterator i = _children.begin(); i != _children.end(); ++i) {
			if (*i == static_cast<TreeNode*>(child))
				return p;
			++p;
		}

		return -1;
	}

	void Model::appendChild ( std::string const & value, std::string const & style_name )
	{
		Model * child = new Model();
		child->addString(value);
		child->setStyle(style_name);
		appendChild(child);
	}
	
	int Model::appendChild ( TreeNode * child )
	{
		int append_pos = size();
		TreeNode::appendChild(child);
		insertInViews(dynamic_cast<xgui::Model*>(this), append_pos, dynamic_cast<xgui::Model*>(child));
		return append_pos;
	}
	
	void Model::insertChild ( TreeNode * child, int pos )
	{
		TreeNode::insertChild ( child, pos );
		insertInViews(dynamic_cast<xgui::Model*>(this), pos, dynamic_cast<xgui::Model*>(child));
	}
	
	void Model::removeChild ( int pos )
	{
		xgui::Model * child = dynamic_cast<xgui::Model*>(_children[pos]);
		removeInViews(dynamic_cast<xgui::Model*>(this), pos, dynamic_cast<xgui::Model*>(child));
		TreeNode::removeChild ( pos );
	}

	void Model::removeAllChildren()
	{
		TreeNode::removeAllChildren();
		updateViews();
	}
	
	void Model::setChild ( TreeNode * new_child, int pos )
	{
		xgui::Model * old_child = dynamic_cast<xgui::Model*>(_children[pos]);
		removeInViews(dynamic_cast<xgui::Model*>(this), pos, dynamic_cast<xgui::Model*>(old_child));
	
		TreeNode::setChild (new_child, pos);
		insertInViews(dynamic_cast<xgui::Model*>(this), pos, dynamic_cast<xgui::Model*>(new_child));
	}
	
	
	void Model::updateViews()
	{
		if (_parent) dynamic_cast<Model *>(_parent)->updateViews();
	
		DMESSAGE("updateView()");
	
		std::set < View * >::iterator iter = view_list.begin();
		for (; iter != view_list.end(); ++iter )
		{
			View * v = *iter;
			v->refresh();
		}
	}
	
	void Model::insertInViews(Model * element_parent, int element_pos, Model * element)
	{
		if (_parent) dynamic_cast<Model *>(_parent)->insertInViews(element_parent, element_pos, element);
	
		DMESSAGE("insertInViews()");
	
		std::set < View * >::iterator iter = view_list.begin();
		for (; iter != view_list.end(); ++iter )
			(*iter)->insertItem(element_parent, element_pos, element);
	}
	
	void Model::removeInViews(Model * element_parent, int element_pos, Model * element)
	{
		if (_parent) dynamic_cast<Model *>(_parent)->removeInViews(element_parent, element_pos, element);
	
		DMESSAGE("removeInViews()");
	
		std::set < View * >::iterator iter = view_list.begin();
		for (; iter != view_list.end(); ++iter )
			(*iter)->removeItem(element_parent, element_pos, element);
	}

	std::string Model::dump()
	{
		std::string xml = "<";
		xml+=cinfo_->tag();

		xml+=dumpProperties();

		xml+="style=\"";
		xml+=getStyle();
		xml+="\" ";

		xml+=">\n";

		for(std::vector<std::string>::iterator s = string_list.begin(); s != string_list.end(); ++s) {
			xml += "<modelentry>";
			xml += *s;
			xml += "</modelentry>\n";
		}

		for(std::vector<TreeNode*>::iterator c = _children.begin(); c != _children.end(); ++c) {
			Model * child = dynamic_cast<Model*>(*c);
			xml += child->dump();
		}

		xml+="</";
		xml+=cinfo_->tag();
		xml+=">\n";

		return xml;
	}
}
