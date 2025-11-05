//
// C++ Interface: tree
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_TREE_H_
#define _XGUI_TREE_H_

#include "xguifwd.h"
#include "view.h"

namespace xguimpl { struct Tree; }

namespace xgui
{
	class TextPairCallback;

	struct TreeClass : public xgui::ViewClass
	{
		DLLEXPORT TreeClass();
		DLLEXPORT std::string const &className();
		DLLEXPORT bool isInstanceable();
		DLLEXPORT xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);

		DLLEXPORT virtual bool canContain(xgui::ClassInfo * c);
	};

	class Tree : public xgui::View
	{
		protected:
			xgui::StyleManager * styleman_;

			xguimpl::Tree * tree_impl_;

			DLLEXPORT Tree ( xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::Tree * widget );
			DLLEXPORT Tree ( xgui::Container * parent, xgui::StyleManager * sm );
			DLLEXPORT void init();

			DLLEXPORT virtual ~Tree();

		public:
			DLLEXPORT virtual int sort ( xgui::Model * node, xgui::TextPairCallback * callback );
			DLLEXPORT virtual int expand ( xgui::Model * node );
			DLLEXPORT virtual int collapse ( xgui::Model * node );

			DLLEXPORT virtual bool isChecked ( xgui::Model * parent, int child_pos );
			DLLEXPORT virtual int check ( xgui::Model * parent, int child_pos, bool state );
			DLLEXPORT virtual int checkAll ( xgui::Model * parent, bool state );

			DLLEXPORT virtual std::string dump();

		friend class Master;
		friend struct xguimpl::Tree;
	};
}

#endif
