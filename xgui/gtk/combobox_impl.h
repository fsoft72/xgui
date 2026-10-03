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
	/**
	 * A combobox: a GtkDropDown when not editable, otherwise an entry with a button
	 * opening the list of items (GTK 4 has no editable drop down).
	 */
	struct Combobox : public View
	{
		/** The items of the model (first string of every child). */
		GtkStringList * items;

		/** Not editable: the drop down. */
		GtkWidget * dropdown;
		/** Editable: the entry, the button opening the items and the selection of the list inside its popover. */
		GtkWidget * entry;
		GtkWidget * menu_button;
		GtkSingleSelection * list_selection;
		/** Editable: index of the item last chosen from the list, -1 when the text was typed. */
		int editable_selected;

		/** True while the combobox changes itself: selection and text changes are not reported. */
		bool updating;

		xgui::Combobox * this_cbox;

		//methods
		Combobox ( GtkWidget * real_w );
		Combobox ( xgui::Container * parent, bool editable );
		virtual ~Combobox();

		virtual void setAccessor(xgui::Widget * w);
		virtual GtkWidget * eventWidget();
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

		/** Creates the drop down or the entry with its list, inside the container box. */
		void build(bool editable);
		/** Removes the widgets created by build(). */
		void unbuild();
		/** Returns the text of an item, or "" when the index is out of range. */
		std::string itemText(int index);
		/** Calls the onrefresh event. */
		void notifyRefresh();
		/** Calls onchange with the given text; returns false if the callback rejects it. */
		bool notifyChange(std::string const &text);
		/** Calls onsubmit with the current text. */
		int notifySubmit();

		static void OnSelect ( GObject * dropdown, GParamSpec * pspec, Combobox * cbox );
		static void OnSubmit ( GtkWidget * w, Combobox * e );
		static void OnListActivate ( GtkListView * list, guint position, Combobox * cbox );
		static void OnTextInsert ( GtkEditable * editable, gchar * new_text, gint new_text_length, gint * position, Combobox * e );
		static void OnTextDelete ( GtkEditable *editable, gint start_pos, gint end_pos, Combobox * e );
	};
}

#endif
