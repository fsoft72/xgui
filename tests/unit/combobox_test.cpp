// Regression test: Combobox on GtkDropDown (not editable) and on an entry with a list popover (editable).
#include <cstdio>
#include <string>

#include "master.h"
#include "window.h"
#include "vbox.h"
#include "combobox.h"
#include "model.h"
#include "callback.h"
#include "gtk/combobox_impl.h"

static std::string events_log;
static bool g_reject = false;

/** Records onchange as "chg:<text>;" and rejects texts when asked to. */
static int on_change(xgui::Widget *, std::string const &text) { events_log += "chg:" + text + ";"; return g_reject ? 0 : 1; }

/** Compares a condition and prints the step on failure; returns 1 on failure. */
static int expect(bool ok, char const * step)
{
	if (ok) return 0;
	std::fprintf(stderr, "%s failed (log '%s')\n", step, events_log.c_str());
	return 1;
}

/** Returns a property of an object. */
static std::string prop(xgui::Object * o, char const * name)
{
	std::string value;
	o->get(name, value);
	return value;
}

int main()
{
	xgui::Window * win = xgui::Master::CreateWindow();
	xgui::VBox * box = xgui::Master::CreateVBox(win, 0, false);
	xgui::CppFTextCallback change_cb(on_change);
	int failures = 0;

	xgui::Model * model = xgui::Master::CreateModel();
	model->appendChild("red");
	model->appendChild("green");

	// Not editable: a drop down over the model strings; GtkDropDown always selects the first item, without onchange
	xgui::Combobox * combo = xgui::Master::CreateCombobox(box, false);
	xguimpl::Combobox * impl = static_cast<xguimpl::Combobox *>(combo->getImpl());
	combo->setModel(model);
	failures += expect(combo->linkEvent("onchange", &change_cb), "link onchange");
	failures += expect(GTK_IS_DROP_DOWN(impl->dropdown) && g_list_model_get_n_items(G_LIST_MODEL(impl->items)) == 2, "drop down items");
	failures += expect(prop(combo, "selected-item") == "0" && events_log.empty(), "first item selected");

	model->appendChild("blue");
	failures += expect(impl->itemText(2) == "blue" && events_log.empty(), "insert item");

	combo->set("selected-item", "1");
	failures += expect(prop(combo, "text") == "green" && events_log == "chg:green;", "select by code");
	events_log.clear();

	// Inserting before the selected item keeps the selection on it
	xgui::Model * first = xgui::Master::CreateModel();
	model->insertChild(first, 0);
	failures += expect(prop(combo, "text") == "green" && events_log.empty(), "selection follows insertions");
	model->removeChild(0);
	failures += expect(prop(combo, "selected-item") == "1" && prop(combo, "text") == "green", "selection follows removals");
	combo->popup();
	combo->popdown();

	// Editable: an entry, the list of items in a popover
	xgui::Combobox * edit = xgui::Master::CreateCombobox(box, true);
	xguimpl::Combobox * eimpl = static_cast<xguimpl::Combobox *>(edit->getImpl());
	edit->setModel(model);
	edit->linkEvent("onchange", &change_cb);
	failures += expect(GTK_IS_ENTRY(eimpl->entry) && GTK_IS_MENU_BUTTON(eimpl->menu_button) && !eimpl->dropdown, "entry and button");

	events_log.clear();
	edit->set("text", "cyan");
	failures += expect(prop(edit, "text") == "cyan" && events_log == "chg:;chg:cyan;", "typed text asks onchange");
	events_log.clear();

	g_reject = true;
	int position = 0;
	gtk_editable_insert_text(GTK_EDITABLE(eimpl->entry), "X", 1, &position);
	g_reject = false;
	failures += expect(prop(edit, "text") == "cyan", "rejected text is not inserted");
	events_log.clear();

	// Choosing an item from the list writes its text
	xguimpl::Combobox::OnListActivate(NULL, 2, eimpl);
	failures += expect(prop(edit, "text") == "blue" && prop(edit, "selected-item") == "2", "item chosen from the list");
	edit->set("maxlength", "10");
	failures += expect(prop(edit, "maxlength") == "10", "maxlength");
	edit->popup();
	edit->popdown();

	// Switching to not editable keeps the items and the event handlers
	edit->set("editable", "0");
	failures += expect(GTK_IS_DROP_DOWN(eimpl->dropdown) && !eimpl->entry && g_list_model_get_n_items(G_LIST_MODEL(eimpl->items)) == 3, "switch to not editable");
	events_log.clear();
	edit->set("selected-item", "2");
	failures += expect(events_log == "chg:blue;", "onchange after the switch");
	return failures ? 1 : 0;
}
