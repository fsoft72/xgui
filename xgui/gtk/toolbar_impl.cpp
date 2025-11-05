//
// C++ Implementation: toolbar_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "toolbar_impl.h"
#include "callback.h"
#include "image.h"
#include "image_impl.h"
#include "master_impl.h"

static GtkWidget * widget_find_by_name (GtkWidget *parent, std::string const &name)
{
  GtkWidget *w, *namedw = NULL;
  GList *children, *l;

  if ((name == gtk_widget_get_name(parent)))
    namedw = parent;

  else {
    if (GTK_CONTAINER(parent)) {
      children = gtk_container_children (GTK_CONTAINER(parent));
      for (l=children; l; l=l->next) {
        if (GTK_IS_WIDGET(l->data)) {
          w = GTK_WIDGET (l->data);
          if ((name == gtk_widget_get_name(w))) {
            namedw = w;
            break;
          }
          if (GTK_IS_CONTAINER (w)) {
            namedw = widget_find_by_name (w, name);
            if (namedw != NULL)
              break;
          }
        }
      }
    }
  }

  return namedw;
}

namespace xguimpl
{
	Toolbar::Toolbar ( GtkWidget * real_w ) : Widget( real_w ) {}
	Toolbar::Toolbar ( xgui::Container * parent ) : Widget ( gtk_toolbar_new() ) { }
	Toolbar::~Toolbar() { }

	bool Toolbar::linkEvent(std::string const & name) 
	{
		if ( name == "onclick" )
			return true;
		
		return Widget::linkEvent(name);
	}

	void Toolbar::setAccessor(xgui::Widget * w)
	{
		xgui::Toolbar * downcasted_widget = dynamic_cast<xgui::Toolbar*>(w);
		ASSERT(downcasted_widget);
		this_tb = downcasted_widget;

		Widget::setAccessor(w);
	}

	void Toolbar::setStyle(std::string const &vals)
	{
		GtkToolbarStyle style;
		if (vals == "text") style = GTK_TOOLBAR_TEXT;
		else if (vals == "icons") style = GTK_TOOLBAR_ICONS;
		else style = GTK_TOOLBAR_BOTH;
	
		gtk_toolbar_set_style ( GTK_TOOLBAR ( widget ), style);
	}

	std::string Toolbar::getStyle()
	{
		std::string vals;

		GtkToolbarStyle style = gtk_toolbar_get_style( GTK_TOOLBAR(widget) );
		if (style == GTK_TOOLBAR_TEXT) vals = "text";
		else if (style == GTK_TOOLBAR_ICONS) vals = "icons";
		else vals = "both";

		return vals;
	}

	void Toolbar::addSeparator() 
	{ 
		gtk_toolbar_insert ( GTK_TOOLBAR ( widget ), gtk_separator_tool_item_new(), this_tb->nelements_ ); 
	}

	void Toolbar::addButton ( const xgui::TbButtonInfo & btn )
	{
		GtkToolItem * item = gtk_tool_button_new( btn.image ? GTK_WIDGET ( btn.image->getImpl()->getImage() ) : 0, btn.text.c_str());
		gtk_widget_set_name( GTK_WIDGET(item), btn.name.c_str() );
	
		if ( !btn.tooltip.empty() )
			gtk_tool_item_set_tooltip ( item, xgui::Master::Instance()->getImpl()->app_tooltips, btn.tooltip.c_str(), btn.tooltip.c_str());
	
		gtk_toolbar_insert ( GTK_TOOLBAR( widget ), item, this_tb->nelements_ );
		gtk_signal_connect ( GTK_OBJECT ( item ), "clicked", G_CALLBACK ( OnClick ), this );
	
		if ( btn.image ) {
			btn.image->ref();
			this_tb->images_.push_back(btn.image);
		}
	}
	
	void Toolbar::addToggleButton ( const xgui::TbButtonInfo & btn )
	{
		GtkToolItem * item = gtk_toggle_tool_button_new();
		gtk_widget_set_name( GTK_WIDGET(item), btn.name.c_str() );
	
		if ( !btn.text.empty() )
			gtk_tool_button_set_label ( GTK_TOOL_BUTTON ( item ), btn.text.c_str() );
	
		if ( btn.image ) {
			gtk_tool_button_set_icon_widget ( GTK_TOOL_BUTTON ( item ), GTK_WIDGET(btn.image->getImpl()->getImage()) );
			btn.image->ref();
			this_tb->images_.push_back(btn.image);
		}
	
		if ( !btn.tooltip.empty() )
			gtk_tool_item_set_tooltip ( item, xgui::Master::Instance()->getImpl()->app_tooltips, btn.tooltip.c_str(), btn.tooltip.c_str());
	
		gtk_toolbar_insert ( GTK_TOOLBAR( widget ), item, this_tb->nelements_ );
		gtk_signal_connect ( GTK_OBJECT ( item ), "toggled", G_CALLBACK ( OnClick ), this );
	}
	
	void Toolbar::addRadioButtons ( std::vector<xgui::TbButtonInfo> const & checkgroup )
	{
		GSList *group = 0;
		std::vector<xgui::TbButtonInfo>::const_iterator end = checkgroup.end();
	
		int button_pos = this_tb->nelements_;
		for ( std::vector<xgui::TbButtonInfo>::const_iterator btn = checkgroup.begin(); btn != end; ++btn ) {
			GtkToolItem * item =  gtk_radio_tool_button_new(group);
			gtk_widget_set_name( GTK_WIDGET(item), btn->name.c_str() );		
	
			if ( !btn->text.empty() )
				gtk_tool_button_set_label ( GTK_TOOL_BUTTON ( item ), btn->text.c_str() );
			if ( btn->image ) {
				gtk_tool_button_set_icon_widget ( GTK_TOOL_BUTTON ( item ), GTK_WIDGET( btn->image->getImpl()->getImage() ) );
				btn->image->ref();
				this_tb->images_.push_back(btn->image);
			}
			if ( !btn->tooltip.empty() )
				gtk_tool_item_set_tooltip ( item,
				                            xgui::Master::Instance()->getImpl()->app_tooltips, 
			                                    btn->tooltip.c_str(), btn->tooltip.c_str() );
	
			group = gtk_radio_tool_button_get_group ( GTK_RADIO_TOOL_BUTTON ( item ) );
			gtk_toolbar_insert ( GTK_TOOLBAR( widget ), item, button_pos++ );
			gtk_signal_connect ( GTK_OBJECT ( item ), "toggled", G_CALLBACK ( OnClick ), this );
		}
	}

	bool Toolbar::removeButton ( const std::string & name )
	{
		GtkWidget * child = widget_find_by_name(widget, name);
		if (!child) return false;
		gtk_container_remove(GTK_CONTAINER(widget), child);
		return true;
	}
	
	void Toolbar::enableButton ( const std::string & name )
	{
		GtkWidget * child = widget_find_by_name(widget, name);
		gtk_widget_set_sensitive(child, TRUE);
	}
	
	void Toolbar::disableButton ( const std::string & name )
	{
		GtkWidget * child = widget_find_by_name(widget, name);
		gtk_widget_set_sensitive(child, FALSE);
	}
	
	void Toolbar::checkButton ( const std::string & name )
	{
		GtkWidget * child = widget_find_by_name(widget, name);
		gtk_toggle_tool_button_set_active (GTK_TOGGLE_TOOL_BUTTON(child), TRUE);
	}
	
	void Toolbar::uncheckButton ( const std::string & name )
	{
		GtkWidget * child = widget_find_by_name(widget,name);
		gtk_toggle_tool_button_set_active (GTK_TOGGLE_TOOL_BUTTON(child), FALSE);
	}
	
	bool Toolbar::isButtonChecked ( const std::string & name )
	{
		GtkWidget * child = widget_find_by_name(widget, name);
		return gtk_toggle_tool_button_get_active (GTK_TOGGLE_TOOL_BUTTON(child));
	}

	void Toolbar::OnClick (GtkToolItem *tbutton, Toolbar * xtb)
	{
		xgui::Callback * base_cb = xtb->this_tb->getEvent("onclick");
		if (!base_cb) return;
		
		xgui::TextStatusCallback * cb = dynamic_cast<xgui::TextStatusCallback *>(base_cb);
		if (!cb) {
			DMESSAGE("onclick event of xgui::Toolbar expected a TextStatusCallback");
			return;
		}
	
		std::string item_name = gtk_widget_get_name ( GTK_WIDGET(tbutton) );
		int item_state = 1;
	
		if ( (gtk_toggle_tool_button_get_type() == G_OBJECT_TYPE(tbutton)) || (gtk_radio_tool_button_get_type() == G_OBJECT_TYPE(tbutton)) )
			item_state = gtk_toggle_tool_button_get_active ( GTK_TOGGLE_TOOL_BUTTON(tbutton) );
	
		cb->call ( xtb->this_tb, item_name, item_state );
	}
}
