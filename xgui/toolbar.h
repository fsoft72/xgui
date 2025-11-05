//
// C++ Interface: toolbar
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//


#ifndef _XGUI_TOOLBAR_H_
#define _XGUI_TOOLBAR_H_

#include "xguifwd.h"
#include "widget.h"

namespace xguimpl { struct Toolbar; }

namespace xgui
{
	class Image;
	class TbButtonInfo;

	struct ToolbarClass : public xgui::WidgetClass
	{
		DLLEXPORT ToolbarClass();
		DLLEXPORT std::string const &className();
		DLLEXPORT bool isInstanceable();
		DLLEXPORT xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);

		DLLEXPORT virtual bool canContain(xgui::ClassInfo * c);
	};

	class Toolbar : public xgui::Widget
	{
		protected:
			std::vector<Image *> images_;
			int nelements_;
			xguimpl::Toolbar * tb_impl_;
			std::vector<TbButtonInfo*> items_;

			DLLEXPORT Toolbar(xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::Toolbar * widget);
			DLLEXPORT Toolbar(xgui::Container * parent);
			DLLEXPORT virtual ~Toolbar();

		public:	
			DLLEXPORT int addItem ( std::string const &id, std::string const &text, xgui::Image * icon, std::string const &tooltip, std::string const & type = "" );
			DLLEXPORT int delItem ( const std::string & name );

			DLLEXPORT int removeButton ( const std::string & name );
			DLLEXPORT int enableButton ( const std::string & name );
			DLLEXPORT int disableButton ( const std::string & name );
			DLLEXPORT int checkButton ( const std::string & name );
			DLLEXPORT int uncheckButton ( const std::string & name );
			DLLEXPORT bool isButtonChecked ( const std::string & name );

			DLLEXPORT virtual std::string dump();
		
		friend class Master;
		friend struct xguimpl::Toolbar;
	};
}

#endif
