//
// C++ Implementation: tab_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "tab.h"
#include "tab_impl.h"
#include "image.h"
#include "image_impl.h"
#include "callback.h"

static void int_item_icon_destroy_notify( xgui::Image * icon )
{
	if (icon)
		icon->unref();
}

static void int_item_text_destroy_notify( std::string * txt )
{
	if (txt)
		delete txt;
}

namespace xguimpl
{

	Tab::Tab(xgui::Container * parent, const std::string & alignment) : Widget(gtk_notebook_new()), this_tab(0)
	{
		GtkPositionType pos;
		
		if (alignment == "left") pos = GTK_POS_LEFT;
		else if (alignment == "bottom") pos = GTK_POS_BOTTOM;
		else if (alignment == "right") pos = GTK_POS_RIGHT;
		else pos = GTK_POS_TOP;
		
		gtk_notebook_set_tab_pos ( GTK_NOTEBOOK(widget), pos );
	}

	Tab::Tab(GtkWidget * real_w) : Widget(real_w), this_tab(0) { }

	Tab::~Tab() { }

	void Tab::setAccessor ( xgui::Widget * w )
	{
		this_tab = dynamic_cast<xgui::Tab *>(w);
		ASSERT(this_tab);

		Widget::setAccessor ( w );
	}

	bool Tab::linkEvent( std::string const &name )
	{
		if ( name == "onchange" ) {
			g_signal_connect ( G_OBJECT(widget), "switch-page", G_CALLBACK(OnSelect), this );
			return true;
		}

		return Widget::linkEvent(name);
	}

	void Tab::OnSelect( GtkNotebook * widget, GtkWidget *page, int page_num, Tab * w )
	{
		DMESSAGE("Notebook Page Switch: " << page_num);

		xgui::Callback * cb = w->this_widget->getEvent("onchange");
		if (!cb) return;

		xgui::IntCallback * real_cb = dynamic_cast<xgui::IntCallback*>(cb);
		if (!real_cb) {
			DMESSAGE("onchange event of xgui::Tab expected an IntCallback");
			return;
		}

		real_cb->call(w->this_widget, page_num);
	}


	void Tab::delChild ( xgui::Widget * child ) { }

	bool Tab::addChild ( xgui::Widget * wid )
	{	
		gtk_notebook_append_page ( GTK_NOTEBOOK(widget), wid->getImpl()->widget, 0);
		return true;
	}

	bool Tab::set_tab_prop ( std::string const &name, std::string const &vals )
	{
		if ( name == "selected-num" ) {
			gtk_notebook_set_current_page ( GTK_NOTEBOOK(widget), xgui::semantic_cast<int>(vals) );
			return true;
		}
		else if ( name == "selected-page" ) {
			xgui::Widget * child = this_tab->findDirectChild(vals);
			if (child)
				gtk_notebook_set_current_page ( GTK_NOTEBOOK(widget), gtk_notebook_page_num (GTK_NOTEBOOK(widget), child->getImpl()->widget) );
			return true;
		}

		return false;
	}

	bool Tab::get_tab_prop ( std::string const &name, std::string &vals )
	{
		if ( name == "selected-num" ) {
			vals = xgui::semantic_cast<std::string>(gtk_notebook_get_current_page(GTK_NOTEBOOK(widget)));
			return true;
		}
		else if ( name == "selected-page" ) {
			int ppos = gtk_notebook_get_current_page(GTK_NOTEBOOK(widget));
			xgui::Widget * sel_widget = this_tab->getChild(ppos);
			if (sel_widget)
				sel_widget->get("id", vals);
			else
				vals = "";
			return true;
		}
		else if ( name == "page-count" ) {
			vals = xgui::semantic_cast<std::string>( gtk_notebook_get_n_pages (GTK_NOTEBOOK(widget)));
			return true;
		}

		return false;
	}

	void Tab::setPageIcon ( xgui::Widget * w, xgui::Image * img )
	{
		if (!img) return;

		GtkWidget * child = w->getImpl()->widget;

		std::string * old_txt = 0;

		GtkWidget * old_box = gtk_notebook_get_tab_label (GTK_NOTEBOOK(widget), GTK_WIDGET(child));
		if (old_box)
			old_txt = (std::string*)g_object_get_data(G_OBJECT(old_box), "tab-text");

		GtkBox * new_box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0));

		img->ref();
		GtkWidget * gtkimg = GTK_WIDGET(img->getImpl()->getImage());
		gtk_box_pack_start(new_box, gtkimg, 0, 0, 0);
		g_object_set_data_full(G_OBJECT(new_box), "tab-icon", (void*)img,
	                         (GDestroyNotify)int_item_icon_destroy_notify);

		if (old_txt) {
			std::string * txt = new std::string(*old_txt);
			GtkWidget * lbl = gtk_label_new(old_txt->c_str());
			gtk_box_pack_start(new_box, lbl, 1, 1, 0);
			g_object_set_data_full(G_OBJECT(new_box), "tab-text", (void*)txt,
		                         (GDestroyNotify)int_item_text_destroy_notify);
		}

		if (old_box)
			gtk_widget_destroy(old_box);

		gtk_notebook_set_tab_label(GTK_NOTEBOOK(widget), GTK_WIDGET(child), GTK_WIDGET(new_box));
		gtk_widget_show_all(GTK_WIDGET(new_box));
	}

	void Tab::setPageLabel ( xgui::Widget * w, const std::string & label)
	{
		GtkWidget * child = w->getImpl()->widget;

		xgui::Image * old_img = 0;

		GtkWidget * old_box = gtk_notebook_get_tab_label (GTK_NOTEBOOK(widget), GTK_WIDGET(child));
		if (old_box)
			old_img = (xgui::Image*)g_object_get_data(G_OBJECT(old_box), "tab-icon");

		GtkBox * new_box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0));

		if (old_img) {
			old_img->ref();
			GtkWidget * gtkimg = GTK_WIDGET(old_img->getImpl()->getImage());
			gtk_box_pack_start(new_box, gtkimg, 0, 0, 0);
			g_object_set_data_full(G_OBJECT(new_box), "tab-icon", old_img,
	                                 (GDestroyNotify)int_item_icon_destroy_notify);
		}

		std::string * text = new std::string(label);
		GtkWidget * lbl = gtk_label_new(label.c_str());
		gtk_box_pack_start(new_box, lbl, 1, 1, 0);
		g_object_set_data_full(G_OBJECT(new_box), "tab-text", text,
	                         (GDestroyNotify)int_item_text_destroy_notify);
		if (old_box)
			gtk_widget_destroy(old_box);

		gtk_notebook_set_tab_label(GTK_NOTEBOOK(widget), GTK_WIDGET(child), GTK_WIDGET(new_box));
		gtk_widget_show_all(GTK_WIDGET(new_box));
	}
}
