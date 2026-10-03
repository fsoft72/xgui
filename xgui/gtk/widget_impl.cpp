//
// C++ Implementation: widget_impl
//
// Description:
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "widget_impl.h"
#include "callback.h"

/** The widget that got the last button press, used to detect clicks (press and release on the same widget). */
static xguimpl::Widget * last_pressed_widget = 0;

/** The key held down, used to tell a first key press from an autorepeat. */
static guint last_key_pressed = 0;

/**
 * Returns false when the event being handled by the controller was already handled by another widget.
 * GTK delivers an event to the controllers of the target widget and of its ancestors (bubble phase):
 * click, double click and key events are reported only for the innermost widget, as with GTK 3.
 */
static bool int_first_handler ( GtkEventController * ctrl )
{
	static GdkEvent * last_event = NULL;
	static guint32 last_time = 0;
	static int last_type = -1;

	GdkEvent * event = gtk_event_controller_get_current_event(ctrl);
	if ( !event ) return true;

	guint32 time = gdk_event_get_time(event);
	int type = gdk_event_get_event_type(event);
	if ( (event == last_event) && (time == last_time) && (type == last_type) ) return false;

	last_event = event;
	last_time = time;
	last_type = type;
	return true;
}

namespace xguimpl
{
	void detachWidget ( GtkWidget * w )
	{
		GtkWidget * parent = gtk_widget_get_parent(w);
		if ( !parent ) return;

		if ( GTK_IS_BOX(parent) ) gtk_box_remove(GTK_BOX(parent), w);
		else if ( GTK_IS_FRAME(parent) ) gtk_frame_set_child(GTK_FRAME(parent), NULL);
		else if ( GTK_IS_WINDOW(parent) ) gtk_window_set_child(GTK_WINDOW(parent), NULL);
		else if ( GTK_IS_SCROLLED_WINDOW(parent) ) gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(parent), NULL);
		else if ( GTK_IS_VIEWPORT(parent) ) gtk_viewport_set_child(GTK_VIEWPORT(parent), NULL);
		else {
			// Notebook pages live in an internal stack of the notebook
			GtkWidget * notebook = gtk_widget_get_ancestor(parent, GTK_TYPE_NOTEBOOK);
			int page = notebook ? gtk_notebook_page_num(GTK_NOTEBOOK(notebook), w) : -1;
			if ( page >= 0 )
				gtk_notebook_remove_page(GTK_NOTEBOOK(notebook), page);
			else
				gtk_widget_unparent(w);
		}
	}

	GtkAlign toGtkAlign ( float align )
	{
		if ( align == 0.0f ) return GTK_ALIGN_START;
		if ( align == 1.0f ) return GTK_ALIGN_END;
		return GTK_ALIGN_CENTER;
	}

	Widget::Widget(GtkWidget * real_w)
	: this_widget(0), widget(real_w), last_w(0), last_h(0),
	  click_gesture(0), motion_controller(0), key_controller(0), focus_controller(0), resize_tick_id(0)
	{
		g_signal_connect ( G_OBJECT ( widget ), "destroy", G_CALLBACK ( OnDestroy ), this );
	}

	Widget::~Widget()
	{
		if ( last_pressed_widget == this ) last_pressed_widget = 0;
		if ( !GTK_IS_WIDGET(widget) ) return;

		// The GTK widget may outlive this object while it is disposed: no handler may reach it any more
		if ( resize_tick_id ) gtk_widget_remove_tick_callback(widget, resize_tick_id);
		if ( click_gesture ) g_signal_handlers_disconnect_by_data(click_gesture, this);
		if ( motion_controller ) g_signal_handlers_disconnect_by_data(motion_controller, this);
		if ( key_controller ) g_signal_handlers_disconnect_by_data(key_controller, this);
		if ( focus_controller ) g_signal_handlers_disconnect_by_data(focus_controller, this);
		g_signal_handlers_disconnect_by_data(widget, this);
	}

	void Widget::setAccessor(xgui::Widget * w)
	{
 		this_widget = w;
	}

	GtkWidget * Widget::eventWidget() { return widget; }

	bool Widget::isResizing(int w, int h)
	{
		if ((w != last_w) || (h != last_h)) {
			last_w = w;
			last_h = h;
			return true;
		}
		return false;
	}

	void Widget::redraw()
	{
		gtk_widget_queue_draw(widget);
	}

	std::pair<int, int> Widget::getMousePos()
	{
		std::pair<int, int> pos(0, 0);

		GtkNative * native = gtk_widget_get_native(widget);
		if ( !native ) return pos;
		GdkSurface * surface = gtk_native_get_surface(native);
		if ( !surface ) return pos;

		GdkDevice * device = gdk_seat_get_pointer(gdk_display_get_default_seat(gtk_widget_get_display(widget)));
		double sx = 0, sy = 0;
		if ( !device || !gdk_surface_get_device_position(surface, device, &sx, &sy, NULL) ) return pos;

		// Surface coordinates -> native widget coordinates -> this widget coordinates
		double tx = 0, ty = 0;
		gtk_native_get_surface_transform(native, &tx, &ty);
		graphene_point_t in = GRAPHENE_POINT_INIT((float)(sx - tx), (float)(sy - ty));
		graphene_point_t out;
		if ( !gtk_widget_compute_point(GTK_WIDGET(native), widget, &in, &out) ) return pos;

		pos.first = (int)out.x;
		pos.second = (int)out.y;
		return pos;
	}

	std::pair<float, float> Widget::splitAlignment(std::string const &align)
	{
		float valign_v = 0.5f, halign_v = 0.5f;
		std::string::size_type p = align.find('-');
		if (p != std::string::npos) {
			std::string valign = align.substr(0, p);
			std::string halign = align.substr(++p, std::string::npos);

			if (valign == "bottom") valign_v = 1.0f;
			else if (valign == "top") valign_v = 0.0f;

			if (halign == "right") halign_v = 1.0f;
			else if (halign == "left") halign_v = 0.0f;

		}
		else {
			if (align == "top") valign_v = 0.0f;
			else if (align == "left") halign_v = 0.0f;
			else if (align == "right") halign_v = 1.0f;
			else if (align == "bottom") valign_v = 1.0f;
		}

		return std::make_pair(halign_v, valign_v);
	}

	std::string Widget::joinAlignment(float x, float y)
	{
		std::string alignment = "center";

		if ( (x == 0.5f) || (y == 0.5f) ) {
			if (x == 0.0f) alignment = "left";
			else if (x == 1.0f) alignment = "right";
			else if (y == 0.0f) alignment = "top";
			else if (y == 1.0f) alignment = "bottom";
		}
		else {
			std::string valign, halign;

			if (y == 1.0f) valign = "bottom";
			else if (y == 0.0f) valign = "top";

			if (x == 1.0f) halign = "right";
			else if (x == 0.0f) halign = "left";

			alignment = valign + "-" + halign;
		}

		return alignment;
	}

	bool Widget::get_widget_prop ( std::string const &name, std::string &vals )
	{
		if ( name == "tooltip-text" ) {
			vals = tip_text;
			return true;
		}
		else if ( name == "visible" ) {
			vals = gtk_widget_get_visible(widget) ? "1" : "0";
			return true;
		}
		else if ( name == "width" ) {
			vals =  xgui::semantic_cast<std::string>(gtk_widget_get_width(widget));
			return true;
		}
		else if ( name == "height" ) {
			vals =  xgui::semantic_cast<std::string>(gtk_widget_get_height(widget));
			return true;
		}
		else if ( name == "enabled" ) {
			vals = xgui::semantic_cast<std::string>(gtk_widget_get_sensitive(widget));
			return true;
		}
		else if ( name == "focused" ) {
			// Composite widgets (e.g. GtkEntry) give the focus to an inner child
			GtkRoot * root = gtk_widget_get_root(eventWidget());
			GtkWidget * focus = root ? gtk_root_get_focus(root) : NULL;
			bool focused = focus && ((focus == eventWidget()) || gtk_widget_is_ancestor(focus, eventWidget()));
			vals = xgui::semantic_cast<std::string>(focused ? 1 : 0);
			return true;
		}

		return false;
	}

	bool Widget::set_widget_prop ( std::string const &name, std::string const &vals )
	{
		if ( name == "tooltip-text" ) {
			tip_text = vals;
			gtk_widget_set_tooltip_text(widget, vals.c_str());
			return true;
		}
		else if ( name == "visible" ) {
			if ( vals == "1" )
				gtk_widget_set_visible(widget, TRUE);
			else if ( vals == "0" )
				gtk_widget_set_visible(widget, FALSE);
			return true;
		}
		else if ( name == "enabled" ) {
			int val = xgui::semantic_cast<int>(vals);
			gtk_widget_set_sensitive ( widget, val );
			return true;
		}
		else if ( name == "focused" ) {
			if ( xgui::semantic_cast<int>(vals) ) gtk_widget_grab_focus(eventWidget());
			return true;
		}

		return false;
	}

	void Widget::ensureClickGesture()
	{
		if ( click_gesture ) return;
		click_gesture = gtk_gesture_click_new();
		// Button 0: report every mouse button, not only the primary one
		gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(click_gesture), 0);
		g_signal_connect ( click_gesture, "pressed", G_CALLBACK ( OnClickPressed ), this );
		g_signal_connect ( click_gesture, "released", G_CALLBACK ( OnClickReleased ), this );
		gtk_widget_add_controller(eventWidget(), GTK_EVENT_CONTROLLER(click_gesture));
	}

	void Widget::ensureMotionController()
	{
		if ( motion_controller ) return;
		motion_controller = gtk_event_controller_motion_new();
		g_signal_connect ( motion_controller, "motion", G_CALLBACK ( OnMotion ), this );
		g_signal_connect ( motion_controller, "enter", G_CALLBACK ( OnEnter ), this );
		g_signal_connect ( motion_controller, "leave", G_CALLBACK ( OnLeave ), this );
		gtk_widget_add_controller(eventWidget(), motion_controller);
	}

	void Widget::ensureKeyController()
	{
		if ( key_controller ) return;
		key_controller = gtk_event_controller_key_new();
		g_signal_connect ( key_controller, "key-pressed", G_CALLBACK ( OnKeyPressed ), this );
		g_signal_connect ( key_controller, "key-released", G_CALLBACK ( OnKeyReleased ), this );
		gtk_widget_add_controller(eventWidget(), key_controller);
	}

	void Widget::ensureFocusController()
	{
		if ( focus_controller ) return;
		focus_controller = gtk_event_controller_focus_new();
		g_signal_connect ( focus_controller, "enter", G_CALLBACK ( OnFocusEnter ), this );
		g_signal_connect ( focus_controller, "leave", G_CALLBACK ( OnFocusLeave ), this );
		gtk_widget_add_controller(eventWidget(), focus_controller);
	}

	void Widget::ensureResizeTick()
	{
		if ( resize_tick_id ) return;
		// GTK 4 has no size-allocate/configure signal: compare the size on every frame instead
		resize_tick_id = gtk_widget_add_tick_callback(widget, OnResizeTick, this, NULL);
	}

	bool Widget::linkEvent(std::string const &name)
	{
		if ( (name == "onfocus") || (name == "onblur") ) {
			ensureFocusController();
			return true;
		}
		else if ( (name == "onmouseover") || (name == "onmouseout") || (name == "onmousemove") ) {
			ensureMotionController();
			return true;
		}
		else if ( (name == "onmousedown") || (name == "onmouseup") || (name == "onmouseclick") || (name == "onmousedblclick") ) {
			ensureClickGesture();
			return true;
		}
		else if ( (name == "onkeydown") || (name == "onkeyup") || (name == "onkeypress") ) {
			ensureKeyController();
			return true;
		}
		else if ( name == "onshow" ) {
			g_signal_connect ( G_OBJECT ( widget ), "show", G_CALLBACK ( OnShow ), this );
			return true;
		}
		else if ( name == "onhide" ) {
			g_signal_connect ( G_OBJECT ( widget ), "hide", G_CALLBACK ( OnHide ), this );
			return true;
		}
		else if ( (name == "onenable") || (name == "ondisable") ) {
			// onenable and ondisable share one handler: connect it only once
			if ( !g_signal_handler_find ( widget, (GSignalMatchType)(G_SIGNAL_MATCH_FUNC | G_SIGNAL_MATCH_DATA), 0, 0, NULL, (gpointer)OnEnableDisable, this ) )
				g_signal_connect ( G_OBJECT ( widget ), "notify::sensitive", G_CALLBACK ( OnEnableDisable ), this );
			return true;
		}
		else if ( name == "onresize" ) {
			ensureResizeTick();
			return true;
		}
		else if ( name == "onmove" ) {
			// GTK 4 cannot report the position of a window
			return false;
		}
		else if ( name == "ondestroy" )
			return true;

		return false;
	}

	void Widget::recalcLayout() {}

	void Widget::decodeButtonEvent ( guint button, GdkModifierType state, std::string & button_name, int & status )
	{
		button_name.clear();
		status = 0;

		switch (button)
		{
			case 1: button_name = "left"; break;
			case 2: button_name = "middle"; break;
			case 3: button_name = "right"; break;
		}

		if (state & GDK_BUTTON1_MASK) status |= xgui::mouseLeftMask;
		if (state & GDK_BUTTON2_MASK) status |= xgui::mouseMiddleMask;
		if (state & GDK_BUTTON3_MASK) status |= xgui::mouseRightMask;

		if (state & GDK_CONTROL_MASK) status |= xgui::keyControlMask;
		if (state & GDK_SHIFT_MASK) status |= xgui::keyShiftMask;
		if (state & GDK_ALT_MASK) status |= xgui::keyAltMask;
	}

	/** Calls a TextStatusCallback event; returns true when the callback consumed the event. */
	static bool int_call_text_status ( xgui::Widget * w, char const * event, std::string const &text, int status )
	{
		xgui::TextStatusCallback * cb = dynamic_cast<xgui::TextStatusCallback *>(w->getEvent(event));
		return cb && !cb->call(w, text, status);
	}

	bool Widget::dispatchPress ( guint button, GdkModifierType state, int n_press, bool first_handler )
	{
		std::string button_name;
		int status = 0;
		decodeButtonEvent ( button, state, button_name, status );

		bool consumed = int_call_text_status(this_widget, "onmousedown", button_name, status);
		if ( !first_handler ) return consumed;

		if ( n_press == 2 ) {
			last_pressed_widget = 0;
			consumed = int_call_text_status(this_widget, "onmousedblclick", button_name, status) || consumed;
		}
		else
			last_pressed_widget = this;

		return consumed;
	}

	bool Widget::dispatchRelease ( guint button, GdkModifierType state, bool first_handler )
	{
		std::string button_name;
		int status = 0;
		decodeButtonEvent ( button, state, button_name, status );

		bool consumed = int_call_text_status(this_widget, "onmouseup", button_name, status);
		if ( !first_handler ) return consumed;

		bool clicked = (last_pressed_widget == this);
		last_pressed_widget = 0;
		if ( clicked )
			consumed = int_call_text_status(this_widget, "onmouseclick", button_name, status) || consumed;

		return consumed;
	}

	bool Widget::dispatchKey ( guint keyval, bool pressed, bool first_handler )
	{
		if ( !first_handler ) return false;

		char const * name = gdk_keyval_name(keyval);
		std::string key_name = name ? name : "";
		xgui::TextCallback * cb = 0;

		if ( pressed ) {
			if ( last_key_pressed == 0 ) {
				last_key_pressed = keyval;
				if ((cb = dynamic_cast<xgui::TextCallback *>(this_widget->getEvent("onkeydown"))))
					cb->call(this_widget, key_name);
			}
			cb = dynamic_cast<xgui::TextCallback *>(this_widget->getEvent("onkeypress"));
		}
		else {
			last_key_pressed = 0;
			cb = dynamic_cast<xgui::TextCallback *>(this_widget->getEvent("onkeyup"));
		}

		return cb && !cb->call(this_widget, key_name);
	}

	void Widget::OnClickPressed ( GtkGestureClick * gesture, int n_press, double x, double y, Widget * xw )
	{
		GtkEventController * ctrl = GTK_EVENT_CONTROLLER(gesture);
		guint button = gtk_gesture_single_get_current_button(GTK_GESTURE_SINGLE(gesture));
		GdkModifierType state = gtk_event_controller_get_current_event_state(ctrl);

		if ( xw->dispatchPress(button, state, n_press, int_first_handler(ctrl)) )
			gtk_gesture_set_state(GTK_GESTURE(gesture), GTK_EVENT_SEQUENCE_CLAIMED);
	}

	void Widget::OnClickReleased ( GtkGestureClick * gesture, int n_press, double x, double y, Widget * xw )
	{
		GtkEventController * ctrl = GTK_EVENT_CONTROLLER(gesture);
		guint button = gtk_gesture_single_get_current_button(GTK_GESTURE_SINGLE(gesture));
		GdkModifierType state = gtk_event_controller_get_current_event_state(ctrl);

		xw->dispatchRelease(button, state, int_first_handler(ctrl));
	}

	void Widget::OnMotion ( GtkEventControllerMotion * ctrl, double x, double y, Widget * xw )
	{
		xgui::TextCallback * cb = dynamic_cast<xgui::TextCallback*>(xw->this_widget->getEvent ( "onmousemove" ));
		if ( cb ) cb->call( xw->this_widget, xgui::semantic_cast<std::string>(std::make_pair(x, y)));
	}

	void Widget::OnEnter ( GtkEventControllerMotion * ctrl, double x, double y, Widget * xw )
	{
		xgui::Callback * cb = xw->this_widget->getEvent("onmouseover");
		if ( cb ) cb->call( xw->this_widget );
	}

	void Widget::OnLeave ( GtkEventControllerMotion * ctrl, Widget * xw )
	{
		xgui::Callback * cb = xw->this_widget->getEvent("onmouseout");
		if ( cb ) cb->call( xw->this_widget );
	}

	gboolean Widget::OnKeyPressed ( GtkEventControllerKey * ctrl, guint keyval, guint keycode, GdkModifierType state, Widget * xw )
	{
		return xw->dispatchKey(keyval, true, int_first_handler(GTK_EVENT_CONTROLLER(ctrl)));
	}

	void Widget::OnKeyReleased ( GtkEventControllerKey * ctrl, guint keyval, guint keycode, GdkModifierType state, Widget * xw )
	{
		xw->dispatchKey(keyval, false, int_first_handler(GTK_EVENT_CONTROLLER(ctrl)));
	}

	void Widget::OnFocusEnter ( GtkEventControllerFocus * ctrl, Widget * xw )
	{
		xgui::Callback * cb = xw->this_widget->getEvent("onfocus");
		if ( cb ) cb->call( xw->this_widget );
	}

	void Widget::OnFocusLeave ( GtkEventControllerFocus * ctrl, Widget * xw )
	{
		xgui::Callback * cb = xw->this_widget->getEvent("onblur");
		if ( cb ) cb->call( xw->this_widget );
	}

	gboolean Widget::OnResizeTick ( GtkWidget * w, GdkFrameClock * clock, gpointer data )
	{
		Widget * xw = (Widget*)data;
		int width = gtk_widget_get_width(w);
		int height = gtk_widget_get_height(w);

		if ( xw->isResizing(width, height) ) {
			xgui::TextCallback * cb = dynamic_cast<xgui::TextCallback *>(xw->this_widget->getEvent("onresize"));
			if (cb) cb->call( xw->this_widget, xgui::semantic_cast<std::string>( std::make_pair( width, height ) ) );
		}
		return G_SOURCE_CONTINUE;
	}

	int Widget::OnShow ( GtkWidget * w, Widget * xw )
	{
		xgui::Callback * cb = dynamic_cast<xgui::Callback*>(xw->this_widget->getEvent ( "onshow" ));
		if ( cb ) return !cb->call( xw->this_widget );
		return 0;
	}

	int Widget::OnHide ( GtkWidget * w, Widget * xw )
	{
		xgui::Callback * cb = dynamic_cast<xgui::Callback*>(xw->this_widget->getEvent ( "onhide" ));
		if ( cb ) return !cb->call( xw->this_widget );
		return 0;
	}

	int Widget::OnDestroy ( GtkWidget * w, Widget * xw )
	{
		int rv = 0;

		xgui::Callback * cb = xw->this_widget->getEvent("ondestroy");
		if ( cb ) rv = !cb->call( xw->this_widget );

		delete xw->this_widget;

		return rv;
	}

	int Widget::OnEnableDisable ( GtkWidget *w, GParamSpec * pspec, Widget * xw )
	{
		xgui::Callback * cb = xw->this_widget->getEvent( gtk_widget_get_sensitive(w) ? "onenable" : "ondisable" );

		if (cb) return !cb->call(xw->this_widget);
		return 0;
	}

}
