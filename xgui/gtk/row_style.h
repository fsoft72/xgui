//
// C++ Interface: row_style
//
// Description: Applies a xgui style (icon, colors, text attributes) to the cells of List and Tree rows
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
	/** Returns the icon of the style named `style`, or NULL when it has none. */
	static inline GdkTexture * rowIcon(xgui::StyleManager * sm, std::string const & style)
	{
		if (!sm) return NULL;
		xgui::Image * icon = sm->getIcon(style);
		return icon ? icon->getImpl()->getTexture() : NULL;
	}

	/** Parses a style color into a Pango color attribute; invalid colors are reported and give NULL. */
	static inline PangoAttribute * colorAttribute(std::string const & color_str, bool background)
	{
		GdkRGBA color;
		if (!gdk_rgba_parse(&color, color_str.c_str())) {
			XGUI_ERROR("Invalid color in style: " << color_str);
			return NULL;
		}
		guint16 r = (guint16)(color.red * 65535), g = (guint16)(color.green * 65535), b = (guint16)(color.blue * 65535);
		return background ? pango_attr_background_new(r, g, b) : pango_attr_foreground_new(r, g, b);
	}

	/**
	 * Applies the text attributes and colors of the style named `style` to the label of a cell.
	 * Shared by List and Tree so that both render styles identically. Without a style manager the label is plain.
	 */
	static inline void applyLabelStyle(GtkLabel * label, xgui::StyleManager * sm, std::string const & style)
	{
		if (!sm) {
			gtk_label_set_attributes(label, NULL);
			return;
		}

		PangoAttrList * attrs = pango_attr_list_new();

		std::string const & fg_color = sm->getFgColor(style);
		PangoAttribute * fg = fg_color.empty() ? NULL : colorAttribute(fg_color, false);
		if (fg) pango_attr_list_insert(attrs, fg);

		std::string const & bg_color = sm->getBgColor(style);
		PangoAttribute * bg = bg_color.empty() ? NULL : colorAttribute(bg_color, true);
		if (bg) pango_attr_list_insert(attrs, bg);

		if (sm->isTextBold(style)) pango_attr_list_insert(attrs, pango_attr_weight_new(PANGO_WEIGHT_BOLD));
		if (sm->isTextItalic(style)) pango_attr_list_insert(attrs, pango_attr_style_new(PANGO_STYLE_ITALIC));
		if (sm->isTextUnderlined(style)) pango_attr_list_insert(attrs, pango_attr_underline_new(PANGO_UNDERLINE_SINGLE));
		if (sm->isTextStriked(style)) pango_attr_list_insert(attrs, pango_attr_strikethrough_new(TRUE));

		gtk_label_set_attributes(label, attrs);
		pango_attr_list_unref(attrs);
	}
}

#endif
