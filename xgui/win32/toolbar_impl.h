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

#ifndef _XGUI_TOOLBAR_IMPL_H_
#define _XGUI_TOOLBAR_IMPL_H_

#include "widget_impl.h"

namespace xgui 
{
	class Window; class Image; 
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
		std::vector<std::string> tips;
		HIMAGELIST hImageList;
		std::string tb_style;
		int last_id;
		IdMap idmap;
		std::vector<std::string> check_groups;
		std::map<int, int> group_map;

		xgui::Toolbar * this_tbar;

		//methods
		DLLEXPORT Toolbar ( HWND real_w );
		DLLEXPORT Toolbar ( xgui::Container * parent );
		DLLEXPORT virtual ~Toolbar();

		DLLEXPORT bool linkEvent( std::string const &name );
		DLLEXPORT char const * bindWMCOMMAND ( WORD command_id );

		DLLEXPORT virtual void setAccessor ( xgui::Widget * w );

		DLLEXPORT void setStyle ( std::string const &style );
		DLLEXPORT std::string getStyle();

		DLLEXPORT int addButton ( const xgui::TbButtonInfo & btn );
		DLLEXPORT int addSeparator();
		DLLEXPORT int addToggleButton ( const xgui::TbButtonInfo & btn );
		DLLEXPORT int addRadioButtons ( std::vector<xgui::TbButtonInfo> const & checkgroup );
		DLLEXPORT bool removeButton ( const std::string & name );
		DLLEXPORT void enableButton ( const std::string & name );
		DLLEXPORT void disableButton ( const std::string & name );
		DLLEXPORT void checkButton ( const std::string & name );
		DLLEXPORT void uncheckButton ( const std::string & name );
		DLLEXPORT bool isButtonChecked ( const std::string & name );

		DLLEXPORT int _add_button ( const xgui::TbButtonInfo & btn, BYTE style );
		DLLEXPORT void updateSize();

		DLLEXPORT static LRESULT CALLBACK toolbarProc ( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		DLLEXPORT static int OnClick ( xgui::Toolbar * tb, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	};
}

#endif
