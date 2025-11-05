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

#ifndef _XGUI_ENTRY_IMPL_H_
#define _XGUI_ENTRY_IMPL_H_

#include "widget_impl.h"

namespace xgui { class Entry; }

namespace xguimpl
{
	struct Entry : public Widget
	{
		xgui::Entry * this_entry;

		Entry ( OpenStepWidget * real_w );
		Entry ( xgui::Container * parent, std::string const &text, int maxlen, bool editable, bool password_mode );
		virtual ~Entry();

		virtual void setAccessor(xgui::Widget * w);
		bool linkEvent( std::string const &name );

		bool get_entry_prop ( std::string const &name, std::string &vals );
		bool set_entry_prop ( std::string const &name, const std::string &vals );

		void appendText ( const std::string & text );
		void prependText ( const std::string & text );
		void insertText ( const std::string & text, int pos );
	};
}

#endif
