//
// C++ Implementation: menu
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
#	include "menu.h"
#	ifdef OPENSTEP
#		include "openstep/menu_impl.h"
#	else
#		include "gtk/menu_impl.h"
#	endif
#	include "master.h"
#	include "image.h"
#endif

namespace xguimpl
{
	struct MenuItem
	{
		std::string id;
		std::string text;
		xgui::Image * icon;
		std::string type;
		std::vector<MenuItem*> children;
		MenuItem * parent;

		MenuItem(std::string const &id, std::string const &text, xgui::Image * icon, std::string const & type);
		~MenuItem();

		MenuItem * findChild(std::string const &id);
		void appendChild(MenuItem * i);
		MenuItem * getChild(int pos);
		int size();

		std::string dump();
	};

	MenuItem::MenuItem(std::string const &idc, std::string const &textc, xgui::Image * iconc, std::string const & typec)
	: id(idc), text(textc), icon(iconc), type(typec), parent(0)
	{
		if (icon)
			icon->ref();
	}

	MenuItem::~MenuItem()
	{
		if(parent) {
			std::vector<MenuItem*>::iterator this_i = std::find(parent->children.begin(), parent->children.end(), this);
			parent->children.erase(this_i);
		}

		if (icon)
			icon->unref();

		while(!children.empty()) 
			delete (*children.begin());
	}

	MenuItem * MenuItem::getChild(int pos) { return children[pos]; }
	MenuItem * MenuItem::findChild(std::string const &idc)
	{
		if (id == idc)
			return this;

		for(std::vector<MenuItem*>::iterator i = children.begin(); i != children.end(); ++i) {
			MenuItem * item = (*i)->findChild(idc);
			if(item)
				return item;
		}

		return 0;
	}

	void MenuItem::appendChild(MenuItem * i)
	{
		children.push_back(i);
		i->parent = this;
	}

	int MenuItem::size() { return children.size(); }

	std::string MenuItem::dump()
	{
		std::string xml;

		if (type != "root") {
			xml+="<menuitem ";

			xml+="id=\"";
			xml+=id;
			xml+="\" ";

			xml+="text=\"";
			xml+=text;
			xml+="\" ";

			if(icon) {
				xml+="icon=\"";
				xml+=icon->getPath();
				xml+="\" ";
			}

			xml+="type=\"";
			xml+=type;
			xml+="\" ";

			xml+=">\n";
		}

		for(std::vector<MenuItem*>::iterator i = children.begin(); i != children.end(); ++i)
			xml+=(*i)->dump();

		if(type != "root")
			xml+="</menuitem>\n";

		return xml;
	}
}

namespace xgui
{
	xgui::MenuClass menu_class_info;
}

namespace xgui
{
	Menu::Menu(xgui::Window * parent, xgui::ClassInfo * cinfo, xguimpl::Menu * widget)
	: Widget ( static_cast<xgui::Container*>(parent), cinfo, widget )
	{
		root_ = new xguimpl::MenuItem("", "", 0, "root");
		menu_impl_ = dynamic_cast<xguimpl::Menu *>(impl_);
		ASSERT(menu_impl_);
	}

	Menu::Menu(xgui::Window * parent, bool visible)
	: Widget ( static_cast<xgui::Container*>(parent), &menu_class_info, new xguimpl::Menu(parent, visible) )
	{
		root_ = new xguimpl::MenuItem("", "", 0, "root");
		menu_impl_ = dynamic_cast<xguimpl::Menu *>(impl_);
		ASSERT(menu_impl_);

		impl_->setAccessor(this);
		post_construction();
	}

	Menu::~Menu() 
	{
		delete root_;
	}
	
	int Menu::addItem(std::string const &parent_id, std::string const &id, std::string const &text, xgui::Image * icon, std::string const & type) 
	{
		if (parent_id.empty())
			root_->appendChild(new xguimpl::MenuItem(id, text, icon, type));
		else {
			xguimpl::MenuItem * parent = root_->findChild(parent_id);
			parent->appendChild(new xguimpl::MenuItem(id, text, icon, type));
		}

		menu_impl_->addItem(parent_id, id, text, icon, type); 
		return 1; 
	}

	int Menu::delItem(std::string const &id)
	{
		xguimpl::MenuItem * item = root_->findChild(id);
		if (!item) return 0;

		delete item;
		menu_impl_->delItem(id);
		return 1;
	}
	
	int Menu::popupMenu(std::string const &id) { menu_impl_->popupMenu(id); return 1;}
	
	int Menu::enableItem(std::string const &id) { menu_impl_->enableItem(id); return 1;}
	int Menu::disableItem(std::string const &id) { menu_impl_->disableItem(id); return 1;}
	bool Menu::isItemEnabled(std::string const &id) { return menu_impl_->isItemEnabled(id); }
	
	int Menu::checkItem(std::string const &id) { menu_impl_->checkItem(id); return 1;}
	int Menu::uncheckItem(std::string const &id) { menu_impl_->uncheckItem(id); return 1;}
	bool Menu::isItemChecked(std::string const &id) { return menu_impl_->isItemChecked(id); }
	
	int Menu::setItemText(std::string const &id, std::string const &text) { menu_impl_->setItemText(id, text); return 1;}
	std::string Menu::getItemText(std::string const &id) { return menu_impl_->getItemText(id); }

	std::string Menu::dump()
	{
		std::string xml = "<";
		xml+=cinfo_->tag();
		xml+=dumpProperties();
		xml+=">\n";

		if(root_)
			xml+=root_->dump();

		xml+="</";
		xml+=cinfo_->tag();
		xml+=">\n";

		return xml;
	}
}

