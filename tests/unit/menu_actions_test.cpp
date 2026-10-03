// Regression test: menus are GMenu models driven by actions (GTK 4 has no GtkMenu).
#include <cstdio>
#include <string>

#include "master.h"
#include "window.h"
#include "menu.h"
#include "callback.h"
#include "gtk/menu_impl.h"

static std::string events_log;

/** Records onclick as "<id>=<state>;". */
static int on_select(xgui::Widget *, std::string const &id, int state)
{
	events_log += id + "=" + std::to_string(state) + ";";
	return 1;
}

/** Activates the action of an item through the action group, as a click on the item would. */
static void activate(xguimpl::Menu * impl, std::string const &id, bool radio)
{
	GVariant * target = radio ? g_variant_new_string(id.c_str()) : NULL;
	g_action_group_activate_action(G_ACTION_GROUP(impl->actions), impl->actionName(id).c_str(), target);
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
	xgui::Menu * menu = xgui::Master::CreateMenu(win);
	xguimpl::Menu * impl = static_cast<xguimpl::Menu *>(menu->getImpl());

	xgui::CppFTextStatusCallback select_cb(on_select);
	menu->linkEvent("onclick", &select_cb);

	menu->addItem("", "file", "&File");
	menu->addItem("file", "open", "&Open");
	menu->addItem("file", "sep", "", 0, "separator");
	menu->addItem("file", "wrap", "&Wrap", 0, "check");
	menu->addItem("file", "", "", 0, "radio_begin");
	menu->addItem("file", "small", "Small");
	menu->addItem("file", "large", "Large");
	menu->addItem("file", "", "", 0, "radio_end");

	int failures = 0;
	failures += expect(GTK_IS_POPOVER_MENU_BAR(impl->widget), "menu bar widget");
	failures += expect(gtk_widget_get_ancestor(impl->widget, GTK_TYPE_WINDOW) == win->getImpl()->widget, "menu bar inside the window");

	// "file" got a submenu with two sections (the separator starts the second one)
	GMenuModel * root = G_MENU_MODEL(impl->root);
	GMenuModel * submenu = g_menu_model_get_item_link(root, 0, G_MENU_LINK_SUBMENU);
	failures += expect(submenu && g_menu_model_get_n_items(submenu) == 2, "submenu sections");
	if (submenu) g_object_unref(submenu);

	activate(impl, "open", false);
	failures += expect(events_log == "open=1;", "plain item");
	events_log.clear();

	activate(impl, "wrap", false);
	failures += expect(events_log == "wrap=1;" && menu->isItemChecked("wrap"), "check item on");
	events_log.clear();
	menu->uncheckItem("wrap");
	failures += expect(!menu->isItemChecked("wrap"), "uncheck item");

	failures += expect(menu->isItemChecked("small") && !menu->isItemChecked("large"), "first radio item selected");
	activate(impl, "large", true);
	failures += expect(events_log == "small=0;large=1;" && menu->isItemChecked("large"), "radio item");
	events_log.clear();

	menu->disableItem("open");
	failures += expect(!menu->isItemEnabled("open") && !g_action_group_get_action_enabled(G_ACTION_GROUP(impl->actions), impl->actionName("open").c_str()), "disable item");
	menu->enableItem("open");
	failures += expect(menu->isItemEnabled("open"), "enable item");

	failures += expect(menu->getItemText("open") == "Open", "mnemonic stripped from text");
	menu->setItemText("open", "Open_file");
	failures += expect(menu->getItemText("open") == "Open_file", "set item text");

	menu->delItem("open");
	failures += expect(menu->getItemText("open").empty() && impl->actionName("open").empty(), "delete item");
	return failures ? 1 : 0;
}
