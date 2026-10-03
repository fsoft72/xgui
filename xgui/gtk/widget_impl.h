//
// C++ Interface: widget_impl
//
// Description:
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_WIDGET_IMPL_H_
#define _XGUI_WIDGET_IMPL_H_

#include <gtk/gtk.h>
#include "widget.h"

namespace xguimpl
{
	/** Removes a widget from its parent, using the parent specific API when there is one. */
	void detachWidget ( GtkWidget * w );

	/** Converts a float alignment (0, 0.5, 1) to a GtkAlign. */
	GtkAlign toGtkAlign ( float align );

	struct Widget
	{
		xgui::Widget * this_widget;
		GtkWidget * widget;

		std::string tip_text;
		int last_w, last_h;

		/** Event controllers, created on demand by linkEvent(). */
		GtkGesture * click_gesture;
		GtkEventController * motion_controller;
		GtkEventController * key_controller;
		GtkEventController * focus_controller;
		guint resize_tick_id;

		Widget(GtkWidget * real_w);
		virtual ~Widget();

		bool isResizing(int w, int h);
		std::pair<float, float> splitAlignment(std::string const &align);
		std::string joinAlignment(float x, float y);
		void redraw();
		std::pair<int, int> getMousePos();

		bool get_widget_prop ( std::string const &name, std::string &vals );
		bool set_widget_prop ( std::string const &name, std::string const &vals );

		virtual bool linkEvent(std::string const &name);
		virtual void recalcLayout();
		virtual void setAccessor(xgui::Widget * w);

		/** The widget receiving the input controllers (a wrapped widget may differ from the container one). */
		virtual GtkWidget * eventWidget();

		void ensureClickGesture();
		void ensureMotionController();
		void ensureKeyController();
		void ensureFocusController();
		void ensureResizeTick();

		/** Dispatches a button press: onmousedown, and onmousedblclick for the second press. Returns true if consumed. */
		bool dispatchPress ( guint button, GdkModifierType state, int n_press, bool first_handler );
		/** Dispatches a button release: onmouseup, and onmouseclick if the press happened on this widget. Returns true if consumed. */
		bool dispatchRelease ( guint button, GdkModifierType state, bool first_handler );
		/** Dispatches a key press or release (onkeydown, onkeypress, onkeyup). Returns true if consumed. */
		bool dispatchKey ( guint keyval, bool pressed, bool first_handler );

		/** Converts a button number and modifier state into the xgui button name ("left", "middle", "right") and modifier mask. */
		static void decodeButtonEvent ( guint button, GdkModifierType state, std::string & button_name, int & status );

		static void OnClickPressed ( GtkGestureClick * gesture, int n_press, double x, double y, Widget * xw );
		static void OnClickReleased ( GtkGestureClick * gesture, int n_press, double x, double y, Widget * xw );
		static void OnMotion ( GtkEventControllerMotion * ctrl, double x, double y, Widget * xw );
		static void OnEnter ( GtkEventControllerMotion * ctrl, double x, double y, Widget * xw );
		static void OnLeave ( GtkEventControllerMotion * ctrl, Widget * xw );
		static gboolean OnKeyPressed ( GtkEventControllerKey * ctrl, guint keyval, guint keycode, GdkModifierType state, Widget * xw );
		static void OnKeyReleased ( GtkEventControllerKey * ctrl, guint keyval, guint keycode, GdkModifierType state, Widget * xw );
		static void OnFocusEnter ( GtkEventControllerFocus * ctrl, Widget * xw );
		static void OnFocusLeave ( GtkEventControllerFocus * ctrl, Widget * xw );
		static gboolean OnResizeTick ( GtkWidget * w, GdkFrameClock * clock, gpointer xw );
		static int OnShow ( GtkWidget * w, Widget * xw );
		static int OnHide ( GtkWidget * w, Widget * xw );
		static int OnDestroy ( GtkWidget * w, Widget * xw );
		static int OnEnableDisable ( GtkWidget *w, GParamSpec * pspec, Widget * xw );
	};
}

#endif
