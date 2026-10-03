// Regression test: the toolbar is a box of buttons (GTK 4 has no GtkToolbar).
#include <cstdio>
#include <string>

#include "master.h"
#include "window.h"
#include "vbox.h"
#include "toolbar.h"
#include "callback.h"
#include "gtk/toolbar_impl.h"

static std::string events_log;

/** Records onclick as "<id>=<state>;". */
static int on_click(xgui::Widget *, std::string const &id, int state)
{
	events_log += id + "=" + std::to_string(state) + ";";
	return 1;
}

/** Compares a condition and prints the step on failure; returns 1 on failure. */
static int expect(bool ok, char const * step)
{
	if (ok) return 0;
	std::fprintf(stderr, "%s failed (log '%s')\n", step, events_log.c_str());
	return 1;
}

int main()
{
	xgui::Window * win = xgui::Master::CreateWindow();
	xgui::VBox * box = xgui::Master::CreateVBox(win, 0, false);
	xgui::Toolbar * tb = xgui::Master::CreateToolbar(box);
	xguimpl::Toolbar * impl = static_cast<xguimpl::Toolbar *>(tb->getImpl());

	xgui::CppFTextStatusCallback click_cb(on_click);
	tb->linkEvent("onclick", &click_cb);

	tb->addItem("new", "New", 0, "Create");
	tb->addItem("bold", "Bold", 0, "", "check");
	tb->addItem("", "", 0, "", "radio_begin");
	tb->addItem("left", "Left", 0, "", "check");
	tb->addItem("right", "Right", 0, "", "check");
	tb->addItem("", "", 0, "", "radio_end");
	tb->addItem("", "", 0, "", "separator");

	int failures = 0;
	GtkWidget * button = impl->findButton("new");
	failures += expect(button && GTK_IS_BUTTON(button), "plain button");
	failures += expect(button && std::string(gtk_widget_get_tooltip_text(button)) == "Create", "tooltip");

	if (button) g_signal_emit_by_name(button, "clicked");
	failures += expect(events_log == "new=1;", "click");
	events_log.clear();

	tb->checkButton("bold");
	failures += expect(tb->isButtonChecked("bold") && events_log == "bold=1;", "check button");
	events_log.clear();

	failures += expect(tb->isButtonChecked("left") && !tb->isButtonChecked("right"), "first radio active");
	tb->checkButton("right");
	failures += expect(tb->isButtonChecked("right") && !tb->isButtonChecked("left"), "radio group");
	events_log.clear();

	tb->disableButton("new");
	failures += expect(button && !gtk_widget_get_sensitive(button), "disable");
	tb->enableButton("new");
	failures += expect(button && gtk_widget_get_sensitive(button), "enable");

	tb->set("style", "text");
	std::string style;
	tb->get("style", style);
	failures += expect(style == "text", "style");

	tb->delItem("new");
	failures += expect(impl->findButton("new") == NULL, "remove");
	return failures ? 1 : 0;
}
