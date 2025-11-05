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

#ifndef _XGUI_BUTTON_IMPL_H_
#define _XGUI_BUTTON_IMPL_H_

#include "widget_impl.h"
#include "toolbar.h"

namespace xgui 
{
	class Image; 
	class TbButtonInfo
	{
		public:
			TbButtonInfo() : image(0) {}
			TbButtonInfo ( const std::string & cname, const std::string & ctext,
				xgui::Image * cimage, const std::string & ctooltip, const std::string & t ) : 
			name(cname), text(ctext), image(cimage), tooltip(ctooltip), type(t) {}
	
			~TbButtonInfo() {}
	
			xgui::TbButtonInfo & operator = (const xgui::TbButtonInfo & btn)
			{
				name = btn.name;
				text = btn.text;
				image = btn.image;
				tooltip = btn.tooltip;
				type = btn.type;
				return *this;
			}
	
			std::string name;
			std::string text;
			xgui::Image * image;
			std::string tooltip;
			std::string type;
	};

	typedef std::vector<xgui::TbButtonInfo> TbButtonInfoVector;
}

namespace xguimpl
{
	struct Toolbar : public Widget
	{
		xgui::Toolbar * this_tb;

		Toolbar ( OpenStepWidget * real_w );
		Toolbar ( xgui::Container * parent );
		virtual ~Toolbar();

		virtual void setAccessor(xgui::Widget * w);
		bool linkEvent( std::string const &name );

		void setStyle(std::string const &style);
		std::string getStyle();

		void addButton ( const xgui::TbButtonInfo & btn );
		void addSeparator();
		void addToggleButton ( const xgui::TbButtonInfo & btn );
		void addRadioButtons ( std::vector<xgui::TbButtonInfo> const & checkgroup );

		bool removeButton ( const std::string & name );
		void enableButton ( const std::string & name );
		void disableButton ( const std::string & name );
		void checkButton ( const std::string & name );
		void uncheckButton ( const std::string & name );
		bool isButtonChecked ( const std::string & name );
	};
}

#endif
