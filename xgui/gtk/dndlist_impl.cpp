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
	static void DNDDataRequestCB(GtkWidget *widget, GdkDragContext *dc, GtkSelectionData *selection_data, guint info, guint t, DndList * li)
	{
		xgui::DndList * l = dynamic_cast<xgui::DndList*>(li->this_list);

		xgui::Model * m = l->getModel();
		xgui::Model * selected = 0;

		for(int i = 0; i < (int)m->size(); ++i) {
			xgui::Model * c = m->getChild(i);
			if (l->isSelected(m, i))
				selected = c;
		}

		std::string data = "ERROR";
			
		if (selected)
			data = selected->getString(0);	

		gtk_selection_data_set(selection_data, GDK_SELECTION_TYPE_STRING, 8, (const guchar*)data.c_str(), data.size() );
	}

	static void DNDDataRecievedCB(GtkWidget *widget, GdkDragContext *dc, gint x, gint y, GtkSelectionData *selection_data, guint info, guint t, DndList * li)
	{
		xgui::DndList * l = dynamic_cast<xgui::DndList*>(li->this_list);

		xgui::Model * m = l->getModel();

		const guchar* raw_data = gtk_selection_data_get_data(selection_data);
		std::string data = raw_data ? (const char*)raw_data : "";

		if(data != "ERROR")
			m->appendChild(data);
	}

	static void DNDDataDeleteCB(GtkWidget *widget, GdkDragContext *dc, DndList * li)
	{
		xgui::DndList * l = dynamic_cast<xgui::DndList*>(li->this_list);

		xgui::Model * m = l->getModel();
		int selected = -1;

		for(int i = 0; i < (int)m->size(); ++i) {
			if (l->isSelected(m, i))
				selected = i;
		}

		if(selected != -1)
			m->removeChild(selected);
	}

	DndList::DndList ( xgui::Container * parent ) : List ( parent )
	{
 		GtkTargetEntry target_entry[1];
                target_entry[0].target = "text/plain";
                target_entry[0].flags = 0;
                target_entry[0].info =  0;

                gtk_drag_dest_set(GTK_WIDGET(list_view), GTK_DEST_DEFAULT_ALL, target_entry, sizeof(target_entry) / sizeof(GtkTargetEntry), (GdkDragAction)(GDK_ACTION_MOVE | GDK_ACTION_COPY) );

		gtk_drag_source_set(GTK_WIDGET(list_view), (GdkModifierType)(GDK_BUTTON1_MASK | GDK_BUTTON2_MASK), target_entry,        sizeof(target_entry) / sizeof(GtkTargetEntry), (GdkDragAction)(GDK_ACTION_MOVE | GDK_ACTION_COPY) );

                g_signal_connect(
                        G_OBJECT(list_view), "drag_data_get",
                        G_CALLBACK(DNDDataRequestCB), this
                );
                g_signal_connect(
                        G_OBJECT(list_view), "drag_data_received",
                        G_CALLBACK(DNDDataRecievedCB), this
                );
                g_signal_connect(
                        G_OBJECT(list_view), "drag_data_delete",
                        G_CALLBACK(DNDDataDeleteCB), this
                );
	}
}

