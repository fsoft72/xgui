// Regression test: List rows on GtkColumnView (checkboxes, removal of selected rows, header clicks, onselect).
#include <cstdio>
#include <string>
#include <vector>

#include "master.h"
#include "window.h"
#include "list.h"
#include "model.h"
#include "callback.h"
#include "gtk/list_impl.h"

static std::string events_log;

/** Records onselect / oncheck as "<event>:<path>=<state>;". */
static int on_select(xgui::Widget *, std::string const &path, int state) { events_log += "sel:" + path + "=" + std::to_string(state) + ";"; return 1; }
static int on_check(xgui::Widget *, std::string const &path, int state) { events_log += "chk:" + path + "=" + std::to_string(state) + ";"; return 1; }
static int on_header(xgui::Widget *, std::string const &id) { events_log += "hdr:" + id + ";"; return 1; }

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
	xgui::List * list = xgui::Master::CreateList(win, 0);
	list->addColumn(0, "name", "Name", 100);
	list->set("has-checkboxes", "1");
	xguimpl::List * impl = static_cast<xguimpl::List *>(list->getImpl());

	xgui::Model * model = xgui::Master::CreateModel();
	model->appendChild("a");
	model->appendChild("b");
	model->appendChild("c");
	list->setModel(model);

	xgui::CppFTextStatusCallback select_cb(on_select), check_cb(on_check);
	xgui::CppFTextCallback header_cb(on_header);
	int failures = 0;
	failures += expect(list->linkEvent("onselect", &select_cb), "link onselect");
	failures += expect(list->linkEvent("oncheck", &check_cb), "link oncheck");
	failures += expect(list->linkEvent("onheaderclick", &header_cb), "link onheaderclick");

	std::string path_b = model->getChild(1)->path();
	list->select(model, 1, true);
	failures += expect(events_log == "sel:" + path_b + "=1;" && list->isSelected(model, 1), "select reports onselect");
	events_log.clear();

	// Removing the selected row forgets it: later selection changes only report the remaining rows
	xgui::Model * removed = model->getChild(1);
	model->removeChild(1);
	failures += expect(g_list_model_get_n_items(G_LIST_MODEL(impl->store)) == 2 && impl->rowOf(removed) == NULL, "row removed");
	failures += expect(!impl->isSelectionCached(removed), "removed row left the selection cache");
	list->selectAll(model, true);
	list->selectAll(model, false);
	std::string expected_log;
	for (int state = 1; state >= 0; --state)
		for (int i = 0; i < 2; ++i)
			expected_log += "sel:" + model->getChild(i)->path() + "=" + std::to_string(state) + ";";
	failures += expect(events_log == expected_log, "select all reports only the remaining rows");
	events_log.clear();

	// Programmatic checks do not report oncheck, user toggles do
	list->check(model, 0, true);
	failures += expect(list->isChecked(model, 0) && !list->isChecked(model, 1) && events_log.empty(), "check");
	XguiRow * row = impl->rowOf(model->getChild(1));
	GtkWidget * button = gtk_check_button_new();
	g_object_set_data(G_OBJECT(button), "xgui-row", row);
	g_signal_connect(button, "toggled", G_CALLBACK(+[](GtkCheckButton * b, xguimpl::List * l) {
		XguiRow * r = (XguiRow *)g_object_get_data(G_OBJECT(b), "xgui-row");
		xgui_row_set_checked(r, gtk_check_button_get_active(b));
		l->notifyCheck(r);
	}), impl);
	gtk_check_button_set_active(GTK_CHECK_BUTTON(button), TRUE);
	failures += expect(events_log == "chk:" + model->getChild(1)->path() + "=1;" && list->isChecked(model, 1), "user toggle");
	g_object_ref_sink(button);
	g_object_unref(button);
	events_log.clear();
	list->checkAll(model, false);
	failures += expect(!list->isChecked(model, 0) && !list->isChecked(model, 1), "uncheck all");

	list->set("visible-headers", "0");
	std::string headers;
	list->get("visible-headers", headers);
	failures += expect(headers == "0", "hide headers");
	list->set("visible-headers", "1");
	list->get("visible-headers", headers);
	failures += expect(headers == "1", "show headers");

	// A click on a column title selects it as sort column of the view
	gtk_column_view_sort_by_column(impl->column_view, impl->findColumn("name")->column, GTK_SORT_ASCENDING);
	failures += expect(events_log == "hdr:name;", "header click");
	return failures ? 1 : 0;
}
