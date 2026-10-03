// Regression test: mouse and key events are delivered through GTK 4 event controllers.
#include <cstdio>
#include <string>

#include "master.h"
#include "window.h"
#include "vbox.h"
#include "button.h"
#include "callback.h"
#include "gtk/widget_impl.h"

static std::string events_log;

/** Records a mouse event as "<event>:<button>:<mask>;". */
static int record(char const * event, std::string const &button, int mask)
{
	events_log += std::string(event) + ":" + button + ":" + std::to_string(mask) + ";";
	return 1;
}

static int on_down(xgui::Widget *, std::string const &b, int m) { return record("down", b, m); }
static int on_up(xgui::Widget *, std::string const &b, int m) { return record("up", b, m); }
static int on_click(xgui::Widget *, std::string const &b, int m) { return record("click", b, m); }
static int on_dblclick(xgui::Widget *, std::string const &b, int m) { return record("dbl", b, m); }
static int on_key(xgui::Widget *, std::string const &key) { events_log += "key:" + key + ";"; return 1; }

/** Returns the number of event controllers attached to a GTK widget. */
static guint count_controllers(GtkWidget * w)
{
	GListModel * controllers = gtk_widget_observe_controllers(w);
	guint n = g_list_model_get_n_items(controllers);
	g_object_unref(controllers);
	return n;
}

/** Compares the event log with the expected one; returns 1 on mismatch. */
static int expect_log(char const * step, std::string const &expected)
{
	if (events_log == expected) { events_log.clear(); return 0; }
	std::fprintf(stderr, "%s: got '%s', expected '%s'\n", step, events_log.c_str(), expected.c_str());
	events_log.clear();
	return 1;
}

int main()
{
	xgui::Window * win = xgui::Master::CreateWindow();
	xgui::VBox * box = xgui::Master::CreateVBox(win, 0, false);
	xgui::Button * button = xgui::Master::CreateButton(box, "ok");
	xguimpl::Widget * impl = static_cast<xguimpl::Widget *>(button->getImpl());

	guint before = count_controllers(impl->widget);
	xgui::CppFTextStatusCallback down_cb(on_down), up_cb(on_up), click_cb(on_click), dbl_cb(on_dblclick);
	xgui::CppFTextCallback key_cb(on_key);
	button->linkEvent("onmousedown", &down_cb);
	button->linkEvent("onmouseup", &up_cb);
	button->linkEvent("onmouseclick", &click_cb);
	button->linkEvent("onmousedblclick", &dbl_cb);
	button->linkEvent("onkeydown", &key_cb);

	int failures = 0;
	// One click gesture for the four mouse events, one key controller
	if (count_controllers(impl->widget) != before + 2) {
		std::fprintf(stderr, "expected 2 new controllers, got %u\n", count_controllers(impl->widget) - before);
		++failures;
	}

	impl->dispatchPress(1, GDK_CONTROL_MASK, 1, true);
	impl->dispatchRelease(1, GDK_CONTROL_MASK, true);
	failures += expect_log("single click", "down:left:" + std::to_string(xgui::keyControlMask) + ";up:left:"
	                       + std::to_string(xgui::keyControlMask) + ";click:left:" + std::to_string(xgui::keyControlMask) + ";");

	impl->dispatchPress(3, (GdkModifierType)0, 2, true);
	impl->dispatchRelease(3, (GdkModifierType)0, true);
	failures += expect_log("double click", "down:right:0;dbl:right:0;up:right:0;");

	// An event already handled by an inner widget only reports press and release
	impl->dispatchPress(1, (GdkModifierType)0, 1, false);
	impl->dispatchRelease(1, (GdkModifierType)0, false);
	failures += expect_log("propagated click", "down:left:0;up:left:0;");

	impl->dispatchKey(GDK_KEY_a, true, true);
	impl->dispatchKey(GDK_KEY_a, false, true);
	failures += expect_log("key", "key:a;");

	// GTK 4 cannot report window positions
	if (win->linkEvent("onmove", &key_cb)) {
		std::fprintf(stderr, "onmove must not be supported\n");
		++failures;
	}
	return failures ? 1 : 0;
}
