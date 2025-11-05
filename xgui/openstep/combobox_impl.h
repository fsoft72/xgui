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

#ifndef _XGUI_COMBOBOX_IMPL_H_
#define _XGUI_COMBOBOX_IMPL_H_

#include "style.h"
#include "combobox.h"
#include "view_impl.h"

namespace xgui { class Combobox; }

namespace xguimpl
{
	struct Combobox : public View
	{
		OpenStepWidget * real_widget;
		xgui::Combobox * this_cbox;

		//methods
		Combobox ( OpenStepWidget * real_w );
		Combobox ( xgui::Container * parent, bool editable );
		virtual ~Combobox();

		virtual void setAccessor(xgui::Widget * w);
		bool linkEvent( std::string const &name );

		virtual void insertItem ( xgui::Model * parent, int child_pos, xgui::Model * child );
		virtual void removeItem ( xgui::Model * parent, int child_pos, xgui::Model * child );
		virtual void refresh();

		void insertText(std::string const &text, unsigned int pos);
		void appendText(std::string const &text);
		void prependText(std::string const &text);

		void setEntryText(std::string const &text);
		std::string getEntryText();

		void setSelected(int num);
		int getSelected();
		std::string getSelectedItemText();

		void setMaxLength(int len);
		int getMaxLength();

		void setAsEditable(bool iseditable);

		void popup();
		void popdown();

		bool isSelected(xgui::Model * parent, int pos);
		void select(xgui::Model * parent, int pos, bool state);
		void selectAll(xgui::Model * parent, bool state);
	};
}

#endif
