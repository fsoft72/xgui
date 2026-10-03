//
// C++ Interface: xgui_row
//
// Description: GObject wrapping a xgui::Model node, the item type of the GTK 4 list models of List and Tree
//
//

#ifndef _XGUI_XGUI_ROW_H_
#define _XGUI_XGUI_ROW_H_

#include <gtk/gtk.h>

namespace xgui { class Model; }

#define XGUI_TYPE_ROW (xgui_row_get_type())
G_DECLARE_FINAL_TYPE(XguiRow, xgui_row, XGUI, ROW, GObject)

/** Creates a row showing a model node. The node is not owned: the view removes the row when the node goes away. */
XguiRow * xgui_row_new ( xgui::Model * node );

/** Returns the model node shown by the row. */
xgui::Model * xgui_row_get_node ( XguiRow * row );

/** Returns the state of the checkbox of the row. */
gboolean xgui_row_get_checked ( XguiRow * row );

/** Sets the state of the checkbox of the row (notifies the "checked" property). */
void xgui_row_set_checked ( XguiRow * row, gboolean checked );

namespace xguimpl
{
	/** Shows or hides the column titles of a column view (GTK 4 has no property for it). */
	void setColumnViewHeadersVisible ( GtkColumnView * view, bool visible );

	/** Returns true when the column titles of a column view are shown. */
	bool columnViewHeadersVisible ( GtkColumnView * view );
}

#endif
