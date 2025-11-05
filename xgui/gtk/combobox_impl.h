//
// C++ Interface: combobox_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

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
		GtkWidget * real_widget;
		GtkEntry  * entry;
		xgui::Combobox * this_cbox;

		//methods
		Combobox ( GtkWidget * real_w );
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

		static int OnSelect ( GtkComboBox * w, Combobox *cbox );
		static int OnSubmit ( GtkWidget * w, Combobox * e );
		static void OnTextInsert ( GtkWidget * editable, gchar * new_text, gint new_text_length, gint * position, Combobox * e );
		static void OnTextDelete ( GtkEditable *editable, gint start_pos, gint end_pos, Combobox * e );
	};
}

#endif
