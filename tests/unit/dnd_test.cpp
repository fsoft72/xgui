// Regression test: DndList drag and drop on GTK 4 drag sources and drop targets.
#include <cstdio>
#include <string>

#include "master.h"
#include "window.h"
#include "vbox.h"
#include "model.h"
#include "dnd_list.h"
#include "gtk/dndlist_impl.h"

/** Returns the number of event controllers of a type attached to a GTK widget. */
static int count_controllers(GtkWidget * w, GType type)
{
	GListModel * controllers = gtk_widget_observe_controllers(w);
	int n = 0;
	for (guint i = 0; i < g_list_model_get_n_items(controllers); ++i) {
		GObject * c = G_OBJECT(g_list_model_get_item(controllers, i));
		if (G_TYPE_CHECK_INSTANCE_TYPE(c, type)) ++n;
		g_object_unref(c);
	}
	g_object_unref(controllers);
	return n;
}

int main()
{
	xgui::Window * win = xgui::Master::CreateWindow();
	xgui::VBox * box = xgui::Master::CreateVBox(win, 0, false);
	xgui::DndList * source = new xgui::DndList(box);
	xgui::DndList * target = new xgui::DndList(box);
	source->addColumn(0, "name", "Name", 100);
	target->addColumn(0, "name", "Name", 100);

	xgui::Model * from = xgui::Master::CreateModel();
	from->appendChild("alpha");
	from->appendChild("beta");
	source->setModel(from);
	xgui::Model * to = xgui::Master::CreateModel();
	target->setModel(to);

	xguimpl::DndList * src_impl = static_cast<xguimpl::DndList *>(source->getImpl());
	xguimpl::DndList * dst_impl = static_cast<xguimpl::DndList *>(target->getImpl());

	int failures = 0;
	GtkWidget * view = src_impl->dragWidget();
	if (count_controllers(view, GTK_TYPE_DRAG_SOURCE) != 1 || count_controllers(view, GTK_TYPE_DROP_TARGET) != 1) {
		std::fprintf(stderr, "drag source / drop target not attached\n");
		++failures;
	}

	std::string data;
	if (src_impl->dragData(data)) {
		std::fprintf(stderr, "nothing selected, but drag data '%s'\n", data.c_str());
		++failures;
	}

	source->select(from, 1, true);
	if (!src_impl->dragData(data) || data != "beta") {
		std::fprintf(stderr, "drag data is '%s', expected 'beta'\n", data.c_str());
		++failures;
	}

	// A move: the target appends the row, the source removes it
	dst_impl->dropData(data);
	src_impl->dragDelete();
	if (to->size() != 1 || to->getChild(0)->getString(0) != "beta" || from->size() != 1 || from->getChild(0)->getString(0) != "alpha") {
		std::fprintf(stderr, "after the move: source has %u rows, target %u rows\n", from->size(), to->size());
		++failures;
	}
	return failures ? 1 : 0;
}
