//
// C++ Implementation: list_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "list_impl.h"
#include "list.h"
#include "callback.h"
#include "model.h"
#include "image.h"
#include "image_impl.h"

static std::pair<GtkTreeViewColumn *, int> int_find_column ( GtkTreeView * list_view, std::string const & name )
{
	GList * columns = gtk_tree_view_get_columns( GTK_TREE_VIEW(list_view) );
	GList * cur_column = columns;
	GtkTreeViewColumn * col_data = 0;
	unsigned int column_pos = 0;
	unsigned int curr_col_n = 0;
	while(cur_column != NULL) {
		col_data = (GtkTreeViewColumn *)cur_column->data;
		std::string title = gtk_tree_view_column_get_title(col_data);
		if (name == title) {
			column_pos = curr_col_n;
			break;
		}
		cur_column = cur_column->next;
		++curr_col_n;
	}
	g_list_free(columns);

	return std::make_pair(col_data, column_pos);
}

static float int_map_align ( std::string const & alignment )
{
	if (alignment == "right") return 1.0f;
	else if (alignment == "center") return 0.5f;
	else return 0.0f;
}

static void int_col_id_destroy_notify( std::string * str )
{
	delete str;
}

struct SortInformations {
	xguimpl::List * list_ptr;
	xgui::TextPairCallback * compare_cb;
	unsigned int sort_column;
};

static int int_sort_func ( GtkTreeModel *model, GtkTreeIter *a, GtkTreeIter *b, SortInformations * si )
{
	std::string str1, str2;
	GValue val = {0, };

	gtk_tree_model_get_value ( model, a, si->sort_column, &val );
	str1 = g_value_get_string(&val);
	g_value_unset(&val);

	gtk_tree_model_get_value ( model, b, si->sort_column, &val );
	str2 = g_value_get_string(&val);
	g_value_unset(&val);

	if (si->compare_cb)
		return si->compare_cb->call( dynamic_cast<xgui::Widget*>(si->list_ptr), str1, str2 );
	else
		return 0;
}


namespace xguimpl
{
	List::List ( GtkWidget * real_w ) 
	: View ( real_w ), list_view(0), fg_column_id(0), bg_column_id(1), italic_column_id(2), 
	bold_column_id(3), underline_column_id(4), strike_column_id(5), check_column_id(6), 
	icon_column_id(7), first_fields_column(8), has_icons(false), has_checks(false) { }

	List::List ( xgui::Container * parent ) : View ( gtk_scrolled_window_new( NULL, NULL ) ), list_view(0), fg_column_id(0), 
	bg_column_id(1), italic_column_id(2), bold_column_id(3), underline_column_id(4), strike_column_id(5),check_column_id(6), 
	icon_column_id(7), first_fields_column(8), has_icons(false), has_checks(false)
	{
		list_view = (GtkTreeView*)gtk_tree_view_new();
		gtk_tree_view_set_enable_search( GTK_TREE_VIEW(list_view), FALSE );
		gtk_tree_selection_set_mode ( gtk_tree_view_get_selection ( GTK_TREE_VIEW ( list_view ) ), GTK_SELECTION_MULTIPLE );
	
		left_align_text_renderer = gtk_cell_renderer_text_new ();
		g_object_set ( G_OBJECT ( left_align_text_renderer ), "xalign", 0.0f, 0 );
		center_align_text_renderer = gtk_cell_renderer_text_new ();
		g_object_set ( G_OBJECT ( center_align_text_renderer ), "xalign", 0.5f, 0 );
		right_align_text_renderer = gtk_cell_renderer_text_new ();
		g_object_set ( G_OBJECT ( right_align_text_renderer ), "xalign", 1.0f, 0 );
	
		check_renderer = gtk_cell_renderer_toggle_new();
		g_object_set ( G_OBJECT ( check_renderer ), "activatable", TRUE, 0 );
		icon_renderer = gtk_cell_renderer_pixbuf_new();
	
		g_signal_connect (G_OBJECT (check_renderer), "toggled", G_CALLBACK ( OnCheck ), this);

		gtk_scrolled_window_add_with_viewport ( GTK_SCROLLED_WINDOW ( widget ), GTK_WIDGET ( list_view ) );
		gtk_scrolled_window_set_policy ( GTK_SCROLLED_WINDOW ( widget ), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC );
	}

	List::~List() 
	{ 
		gtk_widget_destroy ( GTK_WIDGET ( list_view ) );
	}

	bool List::linkEvent( std::string const &name )
	{
		if ( (name == "onselect") )
			return true;
		else if ( name == "onheaderclick" )
			return true;
		else if ( name == "onrefresh" )
			return true;
		else if ( name == "oncheck" )
			return true;
	
		return Widget::linkEvent(name);
	}

	void List::setAccessor(xgui::Widget * w)
	{
		xgui::List * downcasted_widget = dynamic_cast<xgui::List*>(w);
		ASSERT(downcasted_widget);
		this_list = downcasted_widget;

		Widget::setAccessor(w);
	}
	
	void List::rebuildGtkModel()
	{
		int &num_columns = this_list->num_columns_;

		GType * columns_types = new GType[first_fields_column + num_columns];
	
		columns_types[fg_column_id] = GDK_TYPE_COLOR;
		columns_types[bg_column_id] = GDK_TYPE_COLOR;
		columns_types[italic_column_id] = PANGO_TYPE_STYLE;
		columns_types[bold_column_id] = G_TYPE_INT;
		columns_types[underline_column_id] = PANGO_TYPE_UNDERLINE;
		columns_types[strike_column_id] = G_TYPE_BOOLEAN;
		columns_types[check_column_id] = G_TYPE_BOOLEAN;
		columns_types[icon_column_id] = GDK_TYPE_PIXBUF;
	
		for (int curr_col_num = first_fields_column; curr_col_num < (first_fields_column + num_columns); ++curr_col_num )
			columns_types[curr_col_num] = G_TYPE_STRING;
	
		GtkListStore *store = gtk_list_store_newv(first_fields_column + num_columns, columns_types);
		delete [] columns_types;
		
		gtk_tree_view_set_model ( GTK_TREE_VIEW ( list_view ), GTK_TREE_MODEL ( store ) );
		g_object_unref ( store );
		refresh();
	}

	bool List::get_list_prop ( std::string const &name, std::string &vals )
	{
		if ( name == "search-enabled" ) {
			vals = gtk_tree_view_get_enable_search( GTK_TREE_VIEW(list_view) ) ? "1" : "0";
		} else if ( name == "visible-headers" ) {
			vals = gtk_tree_view_get_headers_visible(GTK_TREE_VIEW ( list_view )) ? "1" : "0";
		} else if ( name == "has-icons" ) {
			vals = has_icons ? "1" : "0";
		} else if ( name == "has-checkboxes" ) {
			vals = has_checks ? "1" : "0";
		} else
			return false;
	
		return true;
	}

	bool List::set_list_prop ( std::string const &name, const std::string &vals ) 
	{
		if ( name == "search-enabled" ) {
			gtk_tree_view_set_enable_search( GTK_TREE_VIEW(list_view), (vals == "1" ? 1 : 0) );
		} else if ( name == "visible-headers" ) {
			gtk_tree_view_set_headers_visible( GTK_TREE_VIEW ( list_view ), (vals == "0" ? 0 : 1) );
		} else if ( name == "has-icons" ) {
			bool should_put_icons = ((vals == "1") ? true : false);
			if ( should_put_icons == has_icons ) return true;
	
			if (should_put_icons) {
				has_icons = true;
				GtkTreeViewColumn * icon_col = gtk_tree_view_column_new();
				gtk_tree_view_column_pack_start ( icon_col, icon_renderer, FALSE);
				gtk_tree_view_column_add_attribute ( icon_col, icon_renderer, "pixbuf", icon_column_id );

				int col_pos = 0;
				if (has_checks) ++col_pos;
				gtk_tree_view_insert_column ( GTK_TREE_VIEW( list_view ), icon_col, col_pos);
			}
			else {
				int col_pos = 0;
				if (has_checks) ++col_pos;

				has_icons = false;
				gtk_tree_view_remove_column( GTK_TREE_VIEW(list_view), gtk_tree_view_get_column( GTK_TREE_VIEW(list_view), col_pos ) );
			}
		} else if ( name == "has-checkboxes" ) {
			bool should_put_checks = ((vals == "1") ? true : false);
			if ( should_put_checks == has_checks ) return true;
	
			if (should_put_checks) {
				has_checks = true;
				GtkTreeViewColumn * check_col = gtk_tree_view_column_new();
				gtk_tree_view_column_pack_start ( check_col, check_renderer, FALSE);
				gtk_tree_view_column_add_attribute ( check_col, check_renderer, "active", check_column_id );

 				gtk_tree_view_insert_column ( GTK_TREE_VIEW( list_view ), check_col, 0);
			}
			else {
				has_checks = false;
				gtk_tree_view_remove_column( GTK_TREE_VIEW(list_view), gtk_tree_view_get_column( GTK_TREE_VIEW(list_view), 0 ) );
			}
		} else
			return false;
	
		return true;
	}

	void List::insertItem ( xgui::Model * parent, int child_pos, xgui::Model * child )
	{
		if ( (!list_view) || (!this_list->getModel()) || (!this_list->num_columns_) ) return;
		int &num_columns = this_list->num_columns_;

		GtkTreeSelection * sel = gtk_tree_view_get_selection ( GTK_TREE_VIEW ( list_view ) );
		g_signal_handlers_disconnect_by_func( G_OBJECT(sel), (void*)G_CALLBACK ( OnSelect ), this);
	
		GtkTreeIter curr_ele_iter;	
		GtkListStore *store = GTK_LIST_STORE ( gtk_tree_view_get_model ( GTK_TREE_VIEW ( list_view ) ) );
		gtk_list_store_insert ( store, &curr_ele_iter, child_pos );
		iter_map[child] = curr_ele_iter;
	
		unsigned int curr_col_num = 0;
		for ( ; curr_col_num <  (unsigned int)num_columns; ++curr_col_num ) {
			GValue str = {0, };

			if (curr_col_num < child->numberOfStrings()) {
				g_value_init (&str, G_TYPE_STRING);
				g_value_set_static_string ( &str, child->getString(curr_col_num).c_str() );
				gtk_list_store_set_value ( store, &curr_ele_iter, first_fields_column + curr_col_num, &str );
				g_value_unset (&str);
			}
		}
	
		if (this_list->styleman_) {
			std::string style = child->getStyle();
			xgui::StyleManager * sm = this_list->styleman_;

			xgui::Image * icon = sm->getIcon(style);
			if (icon)
				gtk_list_store_set(store, &curr_ele_iter, icon_column_id, icon->getImpl()->getPixbuf(), -1);
	
			std::string const &fg_color_str = sm->getFgColor(style);
			if (!fg_color_str.empty()) {
				GdkColor fg_color;
				gdk_color_parse ( fg_color_str.c_str(), &fg_color);
				gtk_list_store_set ( store, &curr_ele_iter, fg_column_id, &fg_color, -1 );
			}
	
			std::string const &bg_color_str = sm->getBgColor(style);
			if (!bg_color_str.empty()) {
				GdkColor bg_color;
				gdk_color_parse ( bg_color_str.c_str(), &bg_color);
				gtk_list_store_set ( store, &curr_ele_iter, bg_column_id, &bg_color, -1 );
			}
	
	
			gboolean strike = sm->isTextStriked(style);
			gint bold = (sm->isTextBold(style) ? PANGO_WEIGHT_BOLD : PANGO_WEIGHT_NORMAL);
			PangoStyle italic = (sm->isTextItalic(style) ? PANGO_STYLE_ITALIC : PANGO_STYLE_NORMAL);
			PangoUnderline underline = (sm->isTextUnderlined(style) ? PANGO_UNDERLINE_SINGLE : PANGO_UNDERLINE_NONE);
	
			gtk_list_store_set ( store, &curr_ele_iter, 
						bold_column_id, bold, strike_column_id, strike, italic_column_id, italic,
						underline_column_id, underline, -1 );
		}
	
	
		xgui::Callback * refresh_cb = this_list->getEvent("onrefresh");
		if (refresh_cb) refresh_cb->call( this_list );
	
		g_signal_connect (G_OBJECT (sel), "changed", G_CALLBACK ( OnSelect ), this);
	}
	
	void List::removeItem ( xgui::Model * parent, int child_pos, xgui::Model * child ) 
	{
		if ( (!list_view) || (!this_list->getModel()) || (!this_list->num_columns_) ) return;
	
		GtkTreeSelection * sel = gtk_tree_view_get_selection ( GTK_TREE_VIEW ( list_view ) );
		g_signal_handlers_disconnect_by_func( G_OBJECT(sel), (void*)G_CALLBACK ( OnSelect ), this);
	
		GtkListStore *store = GTK_LIST_STORE ( gtk_tree_view_get_model ( GTK_TREE_VIEW ( list_view ) ) );
		GtkTreeIter to_remove = iter_map[child];
		gtk_list_store_remove (store, &to_remove);
		iter_map.erase(child);
		delSelection(child);
	
		xgui::Callback * refresh_cb = this_list->getEvent("onrefresh");
		if (refresh_cb) refresh_cb->call( dynamic_cast<xgui::Widget*>(this_list) );
	
		g_signal_connect (G_OBJECT (sel), "changed", G_CALLBACK ( OnSelect ), this);
	}

	void List::refresh() 
	{
		int &num_columns = this_list->num_columns_;
		xgui::Model * model = this_list->getModel();
		xgui::StyleManager * stylem = this_list->styleman_;

		if ( (!list_view) || (!model) || (!num_columns) ) return;
	
		GtkTreeSelection * sel = gtk_tree_view_get_selection ( GTK_TREE_VIEW ( list_view ) );
		g_signal_handlers_disconnect_by_func( G_OBJECT(sel), (void*)G_CALLBACK ( OnSelect ), this);
		
		if ( gtk_widget_get_realized ( GTK_WIDGET(list_view) ) )
			gdk_window_freeze_updates ( gtk_widget_get_parent_window ( GTK_WIDGET ( list_view ) ) );
		
		GtkListStore *store = GTK_LIST_STORE ( gtk_tree_view_get_model ( GTK_TREE_VIEW ( list_view ) ) );
		gtk_list_store_clear ( store );
		iter_map.clear();
	
		int curr_row_num = 0;
		for ( xgui::Model::iterator iter = model->begin(); iter != model->end(); ++iter )
		{
			GtkTreeIter curr_ele_iter;
			xgui::Model * child = dynamic_cast<xgui::Model *>(*iter);
			
			gtk_list_store_insert ( store, &curr_ele_iter, curr_row_num++);
			iter_map[child] = curr_ele_iter;
	
			int curr_col_num = 0;
			for ( ; curr_col_num <  num_columns; ++curr_col_num ) {
				GValue str = {0, };
				g_value_init (&str, G_TYPE_STRING);
				g_value_set_static_string ( &str, child->getString(curr_col_num).c_str() );
				gtk_list_store_set_value ( store, &curr_ele_iter, first_fields_column + curr_col_num, &str );
				g_value_unset (&str);
			}
	
			if (stylem) {
				std::string style = child->getStyle();
				xgui::StyleManager * sm = this_list->styleman_;
	
				xgui::Image * icon = sm->getIcon(style);
				if (icon)
					gtk_list_store_set(store, &curr_ele_iter, icon_column_id, icon->getImpl()->getPixbuf(), -1);
		
				std::string const &fg_color_str = sm->getFgColor(style);
				if (!fg_color_str.empty()) {
					GdkColor fg_color;
					gdk_color_parse ( fg_color_str.c_str(), &fg_color);
					gtk_list_store_set ( store, &curr_ele_iter, fg_column_id, &fg_color, -1 );
				}
		
				std::string const &bg_color_str = sm->getBgColor(style);
				if (!bg_color_str.empty()) {
					GdkColor bg_color;
					gdk_color_parse ( bg_color_str.c_str(), &bg_color);
					gtk_list_store_set ( store, &curr_ele_iter, bg_column_id, &bg_color, -1 );
				}
		
		
				gboolean strike = sm->isTextStriked(style);
				gint bold = (sm->isTextBold(style) ? PANGO_WEIGHT_BOLD : PANGO_WEIGHT_NORMAL);
				PangoStyle italic = (sm->isTextItalic(style) ? PANGO_STYLE_ITALIC : PANGO_STYLE_NORMAL);
				PangoUnderline underline = (sm->isTextUnderlined(style) ? PANGO_UNDERLINE_SINGLE : PANGO_UNDERLINE_NONE);
		
				gtk_list_store_set ( store, &curr_ele_iter, 
							bold_column_id, bold, strike_column_id, strike, italic_column_id, italic,
							underline_column_id, underline, -1 );
			}
	
			//if ( this_list->isSelected(child) ) gtk_tree_selection_select_iter ( sel, &curr_ele_iter );
		}
	
		if ( gtk_widget_get_realized ( GTK_WIDGET(list_view) ) )
			gdk_window_thaw_updates ( gtk_widget_get_parent_window ( GTK_WIDGET ( list_view ) ) );
	
		xgui::Callback * refresh_cb = this_list->getEvent("onrefresh");
		if (refresh_cb) refresh_cb->call( this_list );
	
		g_signal_connect (G_OBJECT (sel), "changed", G_CALLBACK ( OnSelect ), this);
	}

	void List::addColumn ( int cpos, const std::string & cid, const std::string & cname, int cwidth, const std::string & calign ) 
	{	
		GtkCellRenderer * renderer = left_align_text_renderer;
		if (calign == "right") renderer = right_align_text_renderer;
		else if (calign == "center") renderer = center_align_text_renderer;

		GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes ( cname.c_str(), renderer, "text", 
					(cpos + first_fields_column),
					"foreground-gdk", fg_column_id,
					"background-gdk", bg_column_id,
					"style", italic_column_id,
					"weight", bold_column_id,
					"underline", underline_column_id,
					"strikethrough", strike_column_id,
					NULL);

		g_object_set_data_full( G_OBJECT(column), "xgui-column-id", (void*)new std::string(cid), (GDestroyNotify)int_col_id_destroy_notify );
		gtk_tree_view_column_set_clickable(column, TRUE);
		gtk_tree_view_column_set_min_width(column, cwidth);
		gtk_tree_view_column_set_resizable(column, TRUE);
		gtk_tree_view_column_set_alignment(column, int_map_align(calign));
		
		g_signal_connect ( G_OBJECT(column), "clicked", G_CALLBACK(OnHeaderClick), this );
		gtk_tree_view_insert_column ( GTK_TREE_VIEW( list_view ), column, (has_icons ? cpos + 1 : cpos) );
	
		rebuildGtkModel();
	}

	bool List::delColumn ( const std::string & column_name )
	{
		std::pair<GtkTreeViewColumn *, int> col_data = int_find_column( GTK_TREE_VIEW(list_view), column_name );
		if (col_data.first) {
			gtk_tree_view_remove_column(GTK_TREE_VIEW(list_view), col_data.first);
			return true;
		}
		return false;
	}

	void List::sort ( std::string const & column_name, xgui::TextPairCallback * compare_cb )
	{
		std::pair<GtkTreeViewColumn *, int> col_data = int_find_column( GTK_TREE_VIEW(list_view), column_name );
		SortInformations si = { this, compare_cb, has_icons ? col_data.second + first_fields_column - 1 : col_data.second + first_fields_column };
	
		GtkListStore *store = GTK_LIST_STORE ( gtk_tree_view_get_model ( GTK_TREE_VIEW ( list_view ) ) );
		gtk_tree_sortable_set_default_sort_func ( GTK_TREE_SORTABLE( store ), (GtkTreeIterCompareFunc)int_sort_func, &si, 0 );
		gtk_tree_sortable_set_sort_column_id ( GTK_TREE_SORTABLE( store ), GTK_TREE_SORTABLE_DEFAULT_SORT_COLUMN_ID , GTK_SORT_ASCENDING);
		gtk_tree_sortable_set_sort_column_id ( GTK_TREE_SORTABLE( store ), GTK_TREE_SORTABLE_UNSORTED_SORT_COLUMN_ID, GTK_SORT_ASCENDING );
		gtk_tree_sortable_set_default_sort_func ( GTK_TREE_SORTABLE( store ), 0, 0, 0 );
	}

	bool List::isSelected(xgui::Model * model, int pos) 
	{
		GtkTreeSelection * sel = gtk_tree_view_get_selection ( GTK_TREE_VIEW ( list_view ) );
		xgui::Model * child = model->getChild(pos);

		GtkTreeIter iter = iter_map[child];
		return gtk_tree_selection_iter_is_selected(sel, &iter);
	}

	void List::select(xgui::Model * parent, int pos, bool state) 
	{
		GtkTreeSelection * sel = gtk_tree_view_get_selection ( GTK_TREE_VIEW ( list_view ) );
		xgui::Model * child = parent->getChild(pos);

		GtkTreeIter iter = iter_map[child];
		if (state)
			gtk_tree_selection_select_iter(sel, &iter);
		else
			gtk_tree_selection_unselect_iter(sel, &iter);
	}

	void List::selectAll(xgui::Model * model, bool state) 
	{
		GtkTreeSelection * sel = gtk_tree_view_get_selection ( GTK_TREE_VIEW ( list_view ) );
		if (state)
			gtk_tree_selection_select_all(sel);
		else
			gtk_tree_selection_unselect_all(sel);
	}

	bool List::isChecked(xgui::Model * parent, int pos)
	{
		GtkListStore *store = GTK_LIST_STORE ( gtk_tree_view_get_model ( GTK_TREE_VIEW ( list_view ) ) );
		xgui::Model * child = parent->getChild(pos);
		GtkTreeIter * iter = &iter_map[child];
 		gboolean state;

		gtk_tree_model_get ( GTK_TREE_MODEL(store), iter, check_column_id, &state, -1);

		return state;
	}

	void List::check(xgui::Model * parent, int pos, bool state)
	{
		GtkListStore *store = GTK_LIST_STORE ( gtk_tree_view_get_model ( GTK_TREE_VIEW ( list_view ) ) );
		GtkTreeIter * iter = &iter_map[parent->getChild(pos)];
		gtk_list_store_set ( store, iter, check_column_id, state, -1);
	}

	void List::checkAll(xgui::Model * parent, bool state)
	{
		GtkListStore *store = GTK_LIST_STORE ( gtk_tree_view_get_model ( GTK_TREE_VIEW ( list_view ) ) );
		for (xgui::Model::iterator i = parent->begin(); i != parent->end(); ++i) {
			GtkTreeIter * iter = &iter_map[dynamic_cast<xgui::Model*>(*i)];
			gtk_list_store_set ( store, iter, check_column_id, state, -1);
		}
	}

	void List::OnSelect ( GtkTreeSelection * sel, List *xl )
	{
		xgui::TextStatusCallback * sel_cb = 0;
	
		xgui::Callback * cb = xl->this_list->getEvent("onselect");
		if (cb) {
			sel_cb = dynamic_cast<xgui::TextStatusCallback*>(cb);
			if (!sel_cb)
				DMESSAGE("onselect event of xgui::List expected a TextStatusCallback");
		}
	
		std::vector<xgui::Model*> deselections;
		std::vector<xgui::Model*> selections;

		xgui::Model * model = xl->this_list->getModel();
		for ( unsigned int row_num = 0; row_num < model->size(); ++row_num ) {
			xgui::Model * curr_data = dynamic_cast<xgui::Model*>(model->getChild(row_num));
			GtkTreePath * cpath = gtk_tree_path_new_from_indices ( row_num, -1 );
			bool selection_selected = gtk_tree_selection_path_is_selected ( sel, cpath );

			if ( !selection_selected && xl->isSelectionCached(curr_data) )
				deselections.push_back(curr_data);
			else if ( selection_selected && !xl->isSelectionCached(curr_data))
				selections.push_back(curr_data);
	
			gtk_tree_path_free ( cpath );
		}

		for (std::vector<xgui::Model*>::iterator sel = deselections.begin(); sel != deselections.end(); ++sel)
			if ( !sel_cb || sel_cb->call( xl->this_list, (*sel)->path(), 0 ) )
				xl->delSelection(*sel);
			
		for (std::vector<xgui::Model*>::iterator sel = selections.begin(); sel != selections.end(); ++sel)
			if ( !sel_cb || sel_cb->call( xl->this_list, (*sel)->path(), 1 ) )
				xl->addSelection(*sel);
	}

	void List::OnHeaderClick ( GtkTreeViewColumn *tvc, List * l )
	{
		xgui::Callback * base_cb = l->this_list->getEvent("onheaderclick");
		if (!base_cb) return;
		
		xgui::TextCallback * cb = dynamic_cast<xgui::TextCallback *>(base_cb);
		if (!cb) {
			DMESSAGE("onheaderclick event of xgui::List expected a TextCallback");
			return;
		}
	
		std::string * id = (std::string*)g_object_get_data( G_OBJECT( tvc ), "xgui-column-id" );
		cb->call ( l->this_list, ( id ? *id : "" ) );
	}

	void List::swapCheck ( GtkTreePath * path )
	{
		GtkListStore *store = GTK_LIST_STORE ( gtk_tree_view_get_model ( GTK_TREE_VIEW ( list_view ) ) );
  		GtkTreeIter iter;
		gtk_tree_model_get_iter ( GTK_TREE_MODEL(store), &iter, path);

		GValue val = {0, };
		gtk_tree_model_get_value ( GTK_TREE_MODEL(store), &iter, check_column_id, &val);
		bool active = g_value_get_boolean(&val);
		g_value_unset ( &val );

		active = (active ? 0 : 1);

		g_value_init (&val, G_TYPE_BOOLEAN);
		g_value_set_boolean ( &val, active );
		gtk_list_store_set_value ( store, &iter, check_column_id, &val );
		g_value_unset (&val);

		xgui::Callback * cb = this_list->getEvent("oncheck");
		if (!cb) return;

		xgui::TextStatusCallback * chk_cb = dynamic_cast<xgui::TextStatusCallback *>(cb);
		if (!chk_cb) {
			DMESSAGE("oncheck event of xgui::List expected a TextStatusCallback");
			return;
		}

		gint * indices = gtk_tree_path_get_indices (path);
		xgui::Model * node = this_list->getModel()->getChild(indices[0]);

		chk_cb->call(this_list, node->path(), active);
	}

	void List::OnCheck ( GtkCellRendererToggle *cell, char const * const path_string,  List * xl)
	{
		GtkTreePath * path = gtk_tree_path_new_from_string (path_string);
		xl->swapCheck(path);
		gtk_tree_path_free(path);
	}
}

