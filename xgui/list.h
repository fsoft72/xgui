//
// C++ Interface: list
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_LIST_H_
#define _XGUI_LIST_H_

#include "xguifwd.h"
#include "view.h"

namespace xguimpl { struct List; struct ListColumnInfo; }

namespace xgui
{
	class TextPairCallback;

	struct ListClass : public xgui::ViewClass
	{
		DLLEXPORT ListClass();
		DLLEXPORT bool isInstanceable();
		DLLEXPORT std::string const &className();
		DLLEXPORT xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);

		DLLEXPORT virtual bool canContain(xgui::ClassInfo * c);
	};

	class List : public xgui::View
	{
		protected:
			int num_columns_;
			xgui::StyleManager * styleman_;
			std::map<std::string, xguimpl::ListColumnInfo*> columns_;

			xguimpl::List * list_impl_;

			DLLEXPORT List(xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::List * widget);
			DLLEXPORT List(xgui::Container * parent, xgui::StyleManager * sm);
			DLLEXPORT void init();

			DLLEXPORT virtual ~List();

		public:
			DLLEXPORT int addColumn ( int cpos, const std::string & cid, const std::string & cname, int cwidth = 100, const std::string & calign = "left" );
			DLLEXPORT int delColumn ( const std::string & id );
			DLLEXPORT int sort ( const std::string & column_id, xgui::TextPairCallback * callback );

			DLLEXPORT bool isChecked ( xgui::Model * parent, int child_pos );
			DLLEXPORT int check ( xgui::Model * parent, int child_pos, bool state );
			DLLEXPORT int checkAll ( xgui::Model * parent, bool state );

			DLLEXPORT std::string dump();

		friend class Master;
		friend struct xguimpl::List;
	};
}

#endif
