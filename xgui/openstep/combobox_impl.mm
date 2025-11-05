/* * 
 * Copyright (C) 2007,2008 by 
 * 	Alessandro Molina <alessandro.molina@gmail.com>
 *  
 * This file is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 * 
 * This file is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 * General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License
 * along with this file; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *  
 * */

#include "combobox_impl.h"
#include "combobox.h"
#include "callback.h"
#include "model.h"


namespace xguimpl
{
	Combobox::Combobox ( OpenStepWidget * real_w ) 
	: View ( real_w ), real_widget(real_w), this_cbox(0) { }

	Combobox::Combobox ( xgui::Container * parent, bool editable ) 
	: View ( 0 ), real_widget(0), this_cbox(0)
	{

	}

	Combobox::~Combobox() { }

	void Combobox::setAccessor(xgui::Widget * w)
	{
		xgui::Combobox * downcasted_widget = dynamic_cast<xgui::Combobox*>(w);
		ASSERT(downcasted_widget);
		this_cbox = downcasted_widget;

		Widget::setAccessor(w);
	}
	

	bool Combobox::linkEvent( std::string const &name )
	{	
		if ( name == "onselect" )
			return true;
		else if ( name == "onrefresh" )
			return true;
		else if ( name == "onsubmit" ) {
			return true;
		}
		else if ( (name == "onchange") ) {
			return true;
		}
		
		return Widget::linkEvent(name);
	}

	void Combobox::refresh()
	{
		xgui::Model * model = this_cbox->getModel();
		if (!model) return;
	
		xgui::Callback * refresh_cb = this_cbox->getEvent("onrefresh");
		if (refresh_cb) refresh_cb->call( this_cbox );
	}
	
	void Combobox::insertItem ( xgui::Model * parent, int child_pos, xgui::Model * child )
	{
		xgui::Model * model = this_cbox->getModel();
		if(!model) return;
	
		xgui::Callback * refresh_cb = this_cbox->getEvent("onrefresh");
		if (refresh_cb) refresh_cb->call( this_cbox );
	}
	
	void Combobox::removeItem ( xgui::Model * parent, int child_pos, xgui::Model * child )
	{
		xgui::Model * model = this_cbox->getModel();
		if(!model) return;
	
		xgui::Callback * refresh_cb = this_cbox->getEvent("onrefresh");
		if (refresh_cb) refresh_cb->call( this_cbox );
	}

	void Combobox::appendText(std::string const &text) { }
	void Combobox::prependText(std::string const &text) { }
	void Combobox::insertText(std::string const &text, unsigned int pos)
	{
	}

	void Combobox::setEntryText(std::string const &text) { }
	std::string Combobox::getEntryText() { return ""; }

	void Combobox::setSelected(int num) { }
	int Combobox::getSelected() { return 0; }
	std::string Combobox::getSelectedItemText() 
	{
		return "";
	}

	void Combobox::setMaxLength(int len) { }
	int Combobox::getMaxLength() { return 0; }

	void Combobox::setAsEditable(bool iseditable)
	{
	}

	void Combobox::popup() {  }
	void Combobox::popdown() { }

	bool Combobox::isSelected(xgui::Model * model, int pos) 
	{
		return false;
	}

	void Combobox::select(xgui::Model * parent, int pos, bool state)
	{ 
		if (state) {}
		else
			if (isSelected(parent, pos)) {}
	}

	void Combobox::selectAll(xgui::Model * model, bool state) 
	{
		if (!state) {}
	}
}
