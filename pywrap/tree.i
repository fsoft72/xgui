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
		TreeClass();
		std::string const &className();
		bool isInstanceable();
		xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);

		virtual bool canContain(xgui::ClassInfo * c);
	};

	class Tree : public xgui::View
	{
		protected:
			Tree ( xgui::Container * parent, xgui::StyleManager * sm );
			virtual ~Tree();

		public:
			virtual void sort ( xgui::Model * node, xgui::TextPairCallback * callback );
			virtual void expand ( xgui::Model * node );
			virtual void collapse ( xgui::Model * node );

			virtual bool isChecked ( xgui::Model * parent, int child_pos );
			virtual void check ( xgui::Model * parent, int child_pos, bool state );
			virtual void checkAll ( xgui::Model * parent, bool state );
	};
}

#endif
