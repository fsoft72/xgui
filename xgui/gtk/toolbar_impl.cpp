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

/** Data key of the item name stored on every toolbar button. */
static char const * const ITEM_NAME_KEY = "xgui-tb-item";

namespace xguimpl
{
	Toolbar::Toolbar ( GtkWidget * real_w ) : Widget( real_w ), this_tb(0), style("both") {}
	Toolbar::Toolbar ( xgui::Container * parent ) : Widget ( gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ), this_tb(0), style("both")
	{
		// GtkToolbar is gone in GTK 4: a box with the "toolbar" style class looks the same
		gtk_widget_add_css_class ( widget, "toolbar" );
	}
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
		style = ((vals == "text") || (vals == "icons")) ? vals : "both";
		for (ItemsMap::iterator i = items.begin(); i != items.end(); ++i)
			setButtonContent ( i->second.first, i->second.second );
	}

	std::string Toolbar::getStyle()
	{
		return style;
	}

	void Toolbar::setButtonContent ( GtkWidget * button, const xgui::TbButtonInfo & btn )
	{
		bool show_icon = btn.image && (style != "text");
		// Without an icon the text is always shown, otherwise the button would be empty
		bool show_text = !btn.text.empty() && ((style != "icons") || !show_icon);

		GtkWidget * content = gtk_box_new ( GTK_ORIENTATION_VERTICAL, 2 );
		if (show_icon) gtk_box_append ( GTK_BOX(content), btn.image->getImpl()->getImage() );
		if (show_text) gtk_box_append ( GTK_BOX(content), gtk_label_new ( btn.text.c_str() ) );
		gtk_button_set_child ( GTK_BUTTON(button), content );
	}

	void Toolbar::appendButton ( GtkWidget * button, const xgui::TbButtonInfo & btn, char const * signal )
	{
		gtk_widget_set_name ( button, btn.name.c_str() );
		gtk_button_set_has_frame ( GTK_BUTTON(button), FALSE );
		g_object_set_data_full ( G_OBJECT(button), ITEM_NAME_KEY, g_strdup ( btn.name.c_str() ), g_free );
		if ( !btn.tooltip.empty() )
			gtk_widget_set_tooltip_text ( button, btn.tooltip.c_str() );

		setButtonContent ( button, btn );
		g_signal_connect ( G_OBJECT ( button ), signal, G_CALLBACK ( OnClick ), this );
		gtk_box_append ( GTK_BOX(widget), button );
		items[btn.name] = std::make_pair ( button, btn );

		if ( btn.image ) {
			btn.image->ref();
			this_tb->images_.push_back(btn.image);
		}
	}

	void Toolbar::addSeparator() 
	{ 
		gtk_box_append ( GTK_BOX(widget), gtk_separator_new ( GTK_ORIENTATION_VERTICAL ) );
	}

	void Toolbar::addButton ( const xgui::TbButtonInfo & btn )
	{
		appendButton ( gtk_button_new(), btn, "clicked" );
	}
	
	void Toolbar::addToggleButton ( const xgui::TbButtonInfo & btn )
	{
		appendButton ( gtk_toggle_button_new(), btn, "toggled" );
	}
	
	void Toolbar::addRadioButtons ( std::vector<xgui::TbButtonInfo> const & checkgroup )
	{
		GtkToggleButton * group = 0;
		for ( std::vector<xgui::TbButtonInfo>::const_iterator btn = checkgroup.begin(); btn != checkgroup.end(); ++btn ) {
			GtkWidget * button = gtk_toggle_button_new();
			// As with GTK 3 radio tool buttons, the first button of a group starts active
			if (group) gtk_toggle_button_set_group ( GTK_TOGGLE_BUTTON(button), group );
			else gtk_toggle_button_set_active ( GTK_TOGGLE_BUTTON(button), TRUE );
			appendButton ( button, *btn, "toggled" );
			if (!group) group = GTK_TOGGLE_BUTTON(button);
		}
	}

	GtkWidget * Toolbar::findButton ( const std::string & name )
	{
		ItemsMap::iterator i = items.find(name);
		return (i == items.end()) ? NULL : i->second.first;
	}

	bool Toolbar::removeButton ( const std::string & name )
	{
		GtkWidget * child = findButton(name);
		if (!child) return false;
		items.erase(name);
		gtk_box_remove ( GTK_BOX(widget), child );
		return true;
	}
	
	void Toolbar::enableButton ( const std::string & name )
	{
		GtkWidget * child = findButton(name);
		if (child) gtk_widget_set_sensitive(child, TRUE);
	}
	
	void Toolbar::disableButton ( const std::string & name )
	{
		GtkWidget * child = findButton(name);
		if (child) gtk_widget_set_sensitive(child, FALSE);
	}
	
	void Toolbar::checkButton ( const std::string & name )
	{
		GtkWidget * child = findButton(name);
		if (child && GTK_IS_TOGGLE_BUTTON(child)) gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(child), TRUE);
	}
	
	void Toolbar::uncheckButton ( const std::string & name )
	{
		GtkWidget * child = findButton(name);
		if (child && GTK_IS_TOGGLE_BUTTON(child)) gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(child), FALSE);
	}
	
	bool Toolbar::isButtonChecked ( const std::string & name )
	{
		GtkWidget * child = findButton(name);
		return child && GTK_IS_TOGGLE_BUTTON(child) && gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(child));
	}

	void Toolbar::OnClick (GtkButton *tbutton, Toolbar * xtb)
	{
		xgui::Callback * base_cb = xtb->this_tb->getEvent("onclick");
		if (!base_cb) return;
		
		xgui::TextStatusCallback * cb = dynamic_cast<xgui::TextStatusCallback *>(base_cb);
		if (!cb) {
			DMESSAGE("onclick event of xgui::Toolbar expected a TextStatusCallback");
			return;
		}
	
		char const * name = (char const *)g_object_get_data ( G_OBJECT(tbutton), ITEM_NAME_KEY );
		std::string item_name = name ? name : "";
		int item_state = 1;
		if ( GTK_IS_TOGGLE_BUTTON(tbutton) )
			item_state = gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(tbutton) );
	
		cb->call ( xtb->this_tb, item_name, item_state );
	}
}
