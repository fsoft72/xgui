//
// C++ Interface: toolbar_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

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

		Toolbar ( GtkWidget * real_w );
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

		static void OnClick (GtkToolItem *tbutton, Toolbar * xtb);
	};
}

#endif
