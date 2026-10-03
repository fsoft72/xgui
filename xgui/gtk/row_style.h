//
// C++ Interface: row_style
//
// Description: Applies a xgui style (icon, colors, text attributes) to a row of a GTK list or tree store
//
//

#ifndef _XGUI_ROW_STYLE_H_
#define _XGUI_ROW_STYLE_H_

#include <string>
#include <gtk/gtk.h>

#include "base.h"
#include "image.h"
#include "image_impl.h"
#include "style.h"

namespace xguimpl
{
	/** Store column ids holding the style attributes of a row. */
	struct RowStyleColumns
	{
		int fg;
		int bg;
		int italic;
		int bold;
		int underline;
		int strike;
		int icon;
	};

	/** Sets a single cell of a GtkListStore or GtkTreeStore row. */
	static inline void setRowValue(GtkTreeModel * store, GtkTreeIter * iter, int column, GValue * val)
	{
		if (GTK_IS_LIST_STORE(store))
			gtk_list_store_set_value(GTK_LIST_STORE(store), iter, column, val);
		else
			gtk_tree_store_set_value(GTK_TREE_STORE(store), iter, column, val);
	}

	/** Sets an integer-like (int, enum or boolean) cell. */
	static inline void setRowInt(GtkTreeModel * store, GtkTreeIter * iter, int column, GType type, int value)
	{
		GValue val = G_VALUE_INIT;
		g_value_init(&val, type);
		if (type == G_TYPE_BOOLEAN) g_value_set_boolean(&val, value);
		else if (type == G_TYPE_INT) g_value_set_int(&val, value);
		else g_value_set_enum(&val, value);
		setRowValue(store, iter, column, &val);
		g_value_unset(&val);
	}

	/** Sets a color cell from a color string; invalid colors are reported and leave the cell empty. */
	static inline void setRowColor(GtkTreeModel * store, GtkTreeIter * iter, int column, std::string const & color_str)
	{
		GdkRGBA color;
		if (!gdk_rgba_parse(&color, color_str.c_str())) {
			XGUI_ERROR("Invalid color in style: " << color_str);
			return;
		}

		GValue val = G_VALUE_INIT;
		g_value_init(&val, GDK_TYPE_RGBA);
		g_value_set_boxed(&val, &color);
		setRowValue(store, iter, column, &val);
		g_value_unset(&val);
	}

	/**
	 * Applies the style named `style` of the style manager `sm` to a row.
	 * Shared by List and Tree so that both render styles identically.
	 */
	static inline void applyRowStyle(GtkTreeModel * store, GtkTreeIter * iter, RowStyleColumns const & cols,
	                                 xgui::StyleManager * sm, std::string const & style)
	{
		xgui::Image * icon = sm->getIcon(style);
		if (icon) {
			GValue val = G_VALUE_INIT;
			g_value_init(&val, GDK_TYPE_TEXTURE);
			g_value_set_object(&val, icon->getImpl()->getTexture());
			setRowValue(store, iter, cols.icon, &val);
			g_value_unset(&val);
		}

		std::string const & fg_color = sm->getFgColor(style);
		if (!fg_color.empty()) setRowColor(store, iter, cols.fg, fg_color);

		std::string const & bg_color = sm->getBgColor(style);
		if (!bg_color.empty()) setRowColor(store, iter, cols.bg, bg_color);

		setRowInt(store, iter, cols.bold, G_TYPE_INT, sm->isTextBold(style) ? PANGO_WEIGHT_BOLD : PANGO_WEIGHT_NORMAL);
		setRowInt(store, iter, cols.strike, G_TYPE_BOOLEAN, sm->isTextStriked(style));
		setRowInt(store, iter, cols.italic, PANGO_TYPE_STYLE, sm->isTextItalic(style) ? PANGO_STYLE_ITALIC : PANGO_STYLE_NORMAL);
		setRowInt(store, iter, cols.underline, PANGO_TYPE_UNDERLINE, sm->isTextUnderlined(style) ? PANGO_UNDERLINE_SINGLE : PANGO_UNDERLINE_NONE);
	}
}

#endif
