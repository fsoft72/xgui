//
// C++ Implementation: list_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "dndlist_impl.h"
#include "list.h"
#include "callback.h"
#include "model.h"

namespace xguimpl
{
	/** Builds the payload of a drag: the first string of the selected row. */
	static GdkContentProvider * int_on_drag_prepare ( GtkDragSource * source, double x, double y, DndList * li )
	{
		std::string data;
		if ( !li->dragData(data) ) return NULL;
		return gdk_content_provider_new_typed ( G_TYPE_STRING, data.c_str() );
	}

	/** A moved row is removed from the source list once the drop succeeded. */
	static void int_on_drag_end ( GtkDragSource * source, GdkDrag * drag, gboolean delete_data, DndList * li )
	{
		if ( delete_data ) li->dragDelete();
	}

	/** A dropped string becomes a new row of the target list. */
	static gboolean int_on_drop ( GtkDropTarget * target, const GValue * value, double x, double y, DndList * li )
	{
		if ( !G_VALUE_HOLDS_STRING(value) ) return FALSE;
		const char * data = g_value_get_string ( value );
		return li->dropData ( data ? data : "" );
	}

	DndList::DndList ( xgui::Container * parent ) : List ( parent )
	{
		GtkWidget * view = dragWidget();

		GtkDragSource * source = gtk_drag_source_new();
		gtk_drag_source_set_actions ( source, (GdkDragAction)(GDK_ACTION_MOVE | GDK_ACTION_COPY) );
		g_signal_connect ( source, "prepare", G_CALLBACK ( int_on_drag_prepare ), this );
		g_signal_connect ( source, "drag-end", G_CALLBACK ( int_on_drag_end ), this );
		gtk_widget_add_controller ( view, GTK_EVENT_CONTROLLER ( source ) );

		GtkDropTarget * target = gtk_drop_target_new ( G_TYPE_STRING, (GdkDragAction)(GDK_ACTION_MOVE | GDK_ACTION_COPY) );
		g_signal_connect ( target, "drop", G_CALLBACK ( int_on_drop ), this );
		gtk_widget_add_controller ( view, GTK_EVENT_CONTROLLER ( target ) );
	}

	GtkWidget * DndList::dragWidget()
	{
		return GTK_WIDGET ( list_view );
	}

	/** Returns the position of the last selected row of the model, or -1. */
	static int int_selected_position ( xgui::DndList * l )
	{
		xgui::Model * m = l->getModel();
		if ( !m ) return -1;

		int selected = -1;
		for (int i = 0; i < (int)m->size(); ++i)
			if (l->isSelected(m, i)) selected = i;
		return selected;
	}

	bool DndList::dragData ( std::string &data )
	{
		xgui::DndList * l = dynamic_cast<xgui::DndList*>(this_list);
		int selected = l ? int_selected_position(l) : -1;
		if ( selected < 0 ) return false;

		data = l->getModel()->getChild(selected)->getString(0);
		return true;
	}

	void DndList::dragDelete ()
	{
		xgui::DndList * l = dynamic_cast<xgui::DndList*>(this_list);
		int selected = l ? int_selected_position(l) : -1;
		if ( selected >= 0 ) l->getModel()->removeChild(selected);
	}

	bool DndList::dropData ( std::string const &data )
	{
		xgui::DndList * l = dynamic_cast<xgui::DndList*>(this_list);
		xgui::Model * m = l ? l->getModel() : 0;
		if ( !m ) return false;

		m->appendChild(data);
		return true;
	}
}
