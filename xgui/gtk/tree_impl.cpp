//
// C++ Implementation: tree_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "tree_impl.h"
#include "tree.h"
#include "model.h"
#include "image.h"
#include "image_impl.h"
#include "style.h"

namespace xguimpl
{
	Tree::Tree ( GtkWidget * real_w ) 
	: View (gtk_scrolled_window_new( NULL, NULL )), tree_view( GTK_TREE_VIEW(real_w) ), has_icons(false), has_checks(false),
	fg_column_id(0), bg_column_id(1), italic_column_id(2), 
	bold_column_id(3), underline_column_id(4), strike_column_id(5), check_column_id(6), 
	icon_column_id(7), field_column_id(8)
	{
		gtk_scrolled_window_add_with_viewport ( GTK_SCROLLED_WINDOW ( widget ), GTK_WIDGET ( tree_view ) );
		gtk_scrolled_window_set_policy ( GTK_SCROLLED_WINDOW ( widget ), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC );
	}

	Tree::Tree ( xgui::Container * parent )
	: View(gtk_scrolled_window_new( NULL, NULL )), tree_view( GTK_TREE_VIEW(gtk_tree_view_new()) ), has_icons(false), has_checks(false),
	fg_column_id(0), bg_column_id(1), italic_column_id(2), 
	bold_column_id(3), underline_column_id(4), strike_column_id(5), check_column_id(6), 
	icon_column_id(7), field_column_id(8)
	{
		gtk_tree_view_set_enable_search( GTK_TREE_VIEW(tree_view), FALSE );
		gtk_tree_view_set_headers_visible( GTK_TREE_VIEW(tree_view), FALSE );
		gtk_tree_selection_set_mode ( gtk_tree_view_get_selection ( GTK_TREE_VIEW ( tree_view ) ), GTK_SELECTION_SINGLE );
	
		left_align_text_renderer = gtk_cell_renderer_text_new ();
		g_object_set ( G_OBJECT ( left_align_text_renderer ), "xalign", 0.0f, 0 );
		center_align_text_renderer = gtk_cell_renderer_text_new ();
		g_object_set ( G_OBJECT ( center_align_text_renderer ), "xalign", 0.5f, 0 );
		right_align_text_renderer = gtk_cell_renderer_text_new ();
		g_object_set ( G_OBJECT ( right_align_text_renderer ), "xalign", 1.0f, 0 );
		
		check_renderer = gtk_cell_renderer_toggle_new();
		g_object_set ( G_OBJECT ( check_renderer ), "activatable", TRUE, 0 );
		icon_renderer = gtk_cell_renderer_pixbuf_new();
	
		gtk_scrolled_window_add_with_viewport ( GTK_SCROLLED_WINDOW ( widget ), GTK_WIDGET ( tree_view ) );
		gtk_scrolled_window_set_policy ( GTK_SCROLLED_WINDOW ( widget ), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC );

		g_signal_connect (G_OBJECT (check_renderer), "toggled", G_CALLBACK ( OnCheck ), this);

		buildGtkModel();
	}

	Tree::~Tree() 
	{ 
		gtk_widget_destroy ( GTK_WIDGET ( tree_view ) );
	}

	bool Tree::linkEvent( std::string const &name )
	{
		if ( (name == "onselect") )
			return true;
		else if ( name == "onrefresh" )
			return true;
		else if ( name == "oncheck" )
			return true;
		else if ( name == "onexpand" ) {
			g_signal_connect ( G_OBJECT(tree_view), "test-expand-row", G_CALLBACK( OnExpand ), this );
			g_signal_connect ( G_OBJECT(tree_view), "test-collapse-row", G_CALLBACK( OnCollapse ), this );
			return true;
		}
	
		return Widget::linkEvent(name);
	}

	void Tree::setAccessor(xgui::Widget * w)
	{
		xgui::Tree * downcasted_widget = dynamic_cast<xgui::Tree*>(w);
		ASSERT(downcasted_widget);
		this_tree = downcasted_widget;

		Widget::setAccessor(w);
	}

	void Tree::buildGtkModel()
	{
		int num_columns = field_column_id + 1;
		GType * columns_types = new GType[num_columns];
	
		columns_types[fg_column_id] = GDK_TYPE_COLOR;
		columns_types[bg_column_id] = GDK_TYPE_COLOR;
		columns_types[italic_column_id] = PANGO_TYPE_STYLE;
		columns_types[bold_column_id] = G_TYPE_INT;
		columns_types[underline_column_id] = PANGO_TYPE_UNDERLINE;
		columns_types[strike_column_id] = G_TYPE_BOOLEAN;
		columns_types[check_column_id] = G_TYPE_BOOLEAN;
		columns_types[icon_column_id] = GDK_TYPE_PIXBUF;
		columns_types[field_column_id] = G_TYPE_STRING;
	
		GtkTreeStore *store = gtk_tree_store_newv(num_columns, columns_types);
		delete [] columns_types;
		
		gtk_tree_view_set_model ( GTK_TREE_VIEW ( tree_view ), GTK_TREE_MODEL ( store ) );
		g_object_unref ( store );

		GtkCellRenderer * renderer = left_align_text_renderer;
		/*if (coli->align == "right") renderer = right_align_text_renderer;
		else if (coli->align == "center") renderer = center_align_text_renderer;*/

		GtkTreeViewColumn *column = gtk_tree_view_column_new();
		gtk_tree_view_column_pack_start(column, renderer, TRUE);
 		gtk_tree_view_column_set_attributes(column, renderer, 
					"text", field_column_id,
					"foreground-gdk", fg_column_id,
					"background-gdk", bg_column_id,
					"style", italic_column_id,
					"weight", bold_column_id,
					"underline", underline_column_id,
					"strikethrough", strike_column_id,
					NULL);

		gtk_tree_view_column_set_clickable(column, FALSE);
		gtk_tree_view_column_set_resizable(column, TRUE);
		
		gtk_tree_view_insert_column ( GTK_TREE_VIEW( tree_view ), column, 0 );
	}

	bool Tree::set_tree_prop ( std::string const &name, std::string const &val )
	{
		if ( name == "search-enabled" ) {
			gtk_tree_view_set_enable_search( GTK_TREE_VIEW(tree_view), (val == "1" ? 1 : 0) );
		} else if ( name == "has-icons" ) {
			bool should_put_icons = ((val == "1") ? true : false);
			if ( should_put_icons == has_icons ) return true;
	
			if (should_put_icons) {
				has_icons = true;
				GtkTreeViewColumn * icon_col = gtk_tree_view_column_new();
				gtk_tree_view_column_pack_start ( icon_col, icon_renderer, FALSE);
				gtk_tree_view_column_add_attribute ( icon_col, icon_renderer, "pixbuf", icon_column_id );

				int col_pos = 0;
				if (has_checks) ++col_pos;
				gtk_tree_view_insert_column ( GTK_TREE_VIEW( tree_view ), icon_col, col_pos);
			}
			else {
				int col_pos = 0;
				if (has_checks) ++col_pos;

				has_icons = false;
				gtk_tree_view_remove_column( GTK_TREE_VIEW(tree_view), gtk_tree_view_get_column( GTK_TREE_VIEW(tree_view), col_pos ) );
			}
		} else if ( name == "has-checkboxes" ) {
			bool should_put_checks = ((val == "1") ? true : false);
			if ( should_put_checks == has_checks ) return true;
	
			if (should_put_checks) {
				has_checks = true;
				GtkTreeViewColumn * check_col = gtk_tree_view_column_new();
				gtk_tree_view_column_pack_start ( check_col, check_renderer, FALSE);
				gtk_tree_view_column_add_attribute ( check_col, check_renderer, "active", check_column_id );

 				gtk_tree_view_insert_column ( GTK_TREE_VIEW( tree_view ), check_col, 0);
			}
			else {
				has_checks = false;
				gtk_tree_view_remove_column( GTK_TREE_VIEW(tree_view), gtk_tree_view_get_column( GTK_TREE_VIEW(tree_view), 0 ) );
			}
		} else
			return false;

		return true;
	}

	bool Tree::get_tree_prop ( std::string const &name, std::string &dest )
	{
		if ( name == "search-enabled" ) {
			dest = gtk_tree_view_get_enable_search( GTK_TREE_VIEW(tree_view) ) ? "1" : "0";
		} else if ( name == "has-icons" ) {
			dest = has_icons ? "1" : "0";
		} else if ( name == "has-checkboxes" ) {
			dest = has_checks ? "1" : "0";
		} else
			return false;

		return true;
	}

	void Tree::insertSubTree(xgui::Model * parent, int child_pos, xgui::Model * child)
	{
		GtkTreeIter curr_ele_iter;
		/*GtkTreeSelection * sel = gtk_tree_view_get_selection ( GTK_TREE_VIEW ( tree_view ) );*/
		GtkTreeStore *store = GTK_TREE_STORE ( gtk_tree_view_get_model ( GTK_TREE_VIEW ( tree_view ) ) );
		GtkTreeIter * iter_parent = (parent != 0) ? &tree_map[parent] : 0;

		gtk_tree_store_insert( store, &curr_ele_iter, iter_parent, child_pos);
		tree_map[child] = curr_ele_iter;

		GValue str = {0, };
		g_value_init (&str, G_TYPE_STRING);
		g_value_set_static_string ( &str, child->getString(0).c_str() );
		gtk_tree_store_set_value ( store, &curr_ele_iter, field_column_id, &str );
		g_value_unset (&str);

		if (this_tree->styleman_) {
			std::string style = child->getStyle();
			xgui::StyleManager * sm = this_tree->styleman_;

			xgui::Image * icon = sm->getIcon(style);
			if (icon)
				gtk_tree_store_set(store, &curr_ele_iter, icon_column_id, icon->getImpl()->getPixbuf(), -1);
	
			std::string const &fg_color_str = sm->getFgColor(style);
			if (!fg_color_str.empty()) {
				GdkColor fg_color;
				gdk_color_parse ( fg_color_str.c_str(), &fg_color);
				gtk_tree_store_set ( store, &curr_ele_iter, fg_column_id, &fg_color, -1 );
			}
	
			std::string const &bg_color_str = sm->getBgColor(style);
			if (!bg_color_str.empty()) {
				GdkColor bg_color;
				gdk_color_parse ( bg_color_str.c_str(), &bg_color);
				gtk_tree_store_set ( store, &curr_ele_iter, bg_column_id, &bg_color, -1 );
			}
	
	
			gboolean strike = sm->isTextStriked(style);
			gint bold = (sm->isTextBold(style) ? PANGO_WEIGHT_BOLD : PANGO_WEIGHT_NORMAL);
			PangoStyle italic = (sm->isTextItalic(style) ? PANGO_STYLE_ITALIC : PANGO_STYLE_NORMAL);
			PangoUnderline underline = (sm->isTextUnderlined(style) ? PANGO_UNDERLINE_SINGLE : PANGO_UNDERLINE_NONE);
	
			gtk_tree_store_set ( store, &curr_ele_iter, 
						bold_column_id, bold, strike_column_id, strike, italic_column_id, italic,
						underline_column_id, underline, -1 );

			/*if ( this_tree->isSelected(child) ) 
				gtk_tree_selection_select_iter ( sel, &curr_ele_iter );*/
		}

		int pos = 0;
		for ( xgui::Model::iterator sub_child = child->begin(); sub_child != child->end(); ++sub_child )
			insertSubTree(child, pos++, dynamic_cast<xgui::Model*>(*sub_child) );
	}

	void Tree::removeSubTree ( xgui::Model * child )
	{
		for ( xgui::Model::iterator sub_child = child->begin(); sub_child != child->end(); ++sub_child )
			removeSubTree(dynamic_cast<xgui::Model*>(*sub_child));

		GtkTreeStore *store = GTK_TREE_STORE ( gtk_tree_view_get_model ( GTK_TREE_VIEW ( tree_view ) ) );
		GtkTreeIter to_remove = tree_map[child];
		gtk_tree_store_remove(store, &to_remove);
		tree_map.erase(child);
		delSelection(child);
	}

	void Tree::insertItem ( xgui::Model * parent, int child_pos, xgui::Model * child )
	{
		if ( (!tree_view) || (!this_tree->getModel()) ) return;

		if ( GTK_WIDGET_REALIZED ( tree_view ) )
			gdk_window_freeze_updates ( gtk_widget_get_parent_window ( GTK_WIDGET ( tree_view ) ) );

		GtkTreeSelection * sel = gtk_tree_view_get_selection ( GTK_TREE_VIEW ( tree_view ) );
		g_signal_handlers_disconnect_by_func( G_OBJECT(sel), (void*)G_CALLBACK ( OnSelect ), this);
	
		if (parent == this_tree->getModel())
			parent = 0;

		insertSubTree(parent, child_pos, child);
	
		if ( GTK_WIDGET_REALIZED ( tree_view ) )
			gdk_window_thaw_updates ( gtk_widget_get_parent_window ( GTK_WIDGET ( tree_view ) ) );

		xgui::Callback * refresh_cb = this_tree->getEvent("onrefresh");
		if (refresh_cb) refresh_cb->call( this_tree );
	
		g_signal_connect (G_OBJECT (sel), "changed", G_CALLBACK ( OnSelect ), this);
	}

	void Tree::removeItem ( xgui::Model * parent, int child_pos, xgui::Model * child ) 
	{
		if ( (!tree_view) || (!this_tree->getModel()) ) return;
	
		if ( GTK_WIDGET_REALIZED ( tree_view ) )
			gdk_window_freeze_updates ( gtk_widget_get_parent_window ( GTK_WIDGET ( tree_view ) ) );

		GtkTreeSelection * sel = gtk_tree_view_get_selection ( GTK_TREE_VIEW ( tree_view ) );
		g_signal_handlers_disconnect_by_func( G_OBJECT(sel), (void*)G_CALLBACK ( OnSelect ), this);
	
		removeSubTree(child);
	
		if ( GTK_WIDGET_REALIZED ( tree_view ) )
			gdk_window_thaw_updates ( gtk_widget_get_parent_window ( GTK_WIDGET ( tree_view ) ) );

		xgui::Callback * refresh_cb = this_tree->getEvent("onrefresh");
		if (refresh_cb) refresh_cb->call( this_tree );
	
		g_signal_connect (G_OBJECT (sel), "changed", G_CALLBACK ( OnSelect ), this);
	}

	void Tree::refresh() 
	{
		xgui::Model * model = this_tree->getModel();
		if ( (!tree_view) || (!model) ) return;
	
		GtkTreeSelection * sel = gtk_tree_view_get_selection ( GTK_TREE_VIEW ( tree_view ) );
		g_signal_handlers_disconnect_by_func( G_OBJECT(sel), (void*)G_CALLBACK ( OnSelect ), this);
		
		if ( GTK_WIDGET_REALIZED ( tree_view ) )
			gdk_window_freeze_updates ( gtk_widget_get_parent_window ( GTK_WIDGET ( tree_view ) ) );
		
		GtkTreeStore *store = GTK_TREE_STORE ( gtk_tree_view_get_model ( GTK_TREE_VIEW ( tree_view ) ) );
		gtk_tree_store_clear ( store );
		tree_map.clear();
	
		int curr_row_num = 0;
		for ( xgui::Model::iterator iter = model->begin(); iter != model->end(); ++iter )
		{
			xgui::Model * child = dynamic_cast<xgui::Model *>(*iter);
			insertSubTree(0, curr_row_num++, child);
		}
	
		if ( GTK_WIDGET_REALIZED ( tree_view ) )
			gdk_window_thaw_updates ( gtk_widget_get_parent_window ( GTK_WIDGET ( tree_view ) ) );
	
		xgui::Callback * refresh_cb = this_tree->getEvent("onrefresh");
		if (refresh_cb) refresh_cb->call( this_tree );
	
		g_signal_connect (G_OBJECT (sel), "changed", G_CALLBACK ( OnSelect ), this);
	}


	void Tree::selectSubTree(GtkTreeSelection * sel, xgui::Model * model, int child_pos, xgui::TextStatusCallback * sel_cb )
	{
		GtkTreeIter curr_iter = tree_map[model];
		bool selection_selected = gtk_tree_selection_iter_is_selected ( sel, &curr_iter );

		if ( !selection_selected && isSelectionCached(model) ) {
			if ( !sel_cb || sel_cb->call( this_tree, model->path(), 0 ) )
				delSelection(model);
		}
		else if ( selection_selected && !isSelectionCached(model)) {
			if ( !sel_cb || sel_cb->call( this_tree, model->path(), 1 ) )
				addSelection(model);
		}

		for ( unsigned int row_num = 0; row_num < model->size(); ++row_num ) {
			xgui::Model * curr_data = dynamic_cast<xgui::Model*>(model->getChild(row_num));
			selectSubTree(sel, curr_data, row_num, sel_cb);
		}
	}

	bool Tree::isSelected(xgui::Model * model, int pos) 
	{
		GtkTreeSelection * sel = gtk_tree_view_get_selection ( GTK_TREE_VIEW ( tree_view ) );
		xgui::Model * child = model->getChild(pos);

		GtkTreeIter iter = tree_map[child];
		return gtk_tree_selection_iter_is_selected(sel, &iter);
	}

	void Tree::select(xgui::Model * parent, int pos, bool state) 
	{
		GtkTreeSelection * sel = gtk_tree_view_get_selection ( GTK_TREE_VIEW ( tree_view ) );
		xgui::Model * child = parent->getChild(pos);

		GtkTreeIter iter = tree_map[child];
		if (state)
			gtk_tree_selection_select_iter(sel, &iter);
		else
			gtk_tree_selection_unselect_iter(sel, &iter);
	}

	void Tree::selectAll(xgui::Model * model, bool state) 
	{
		GtkTreeSelection * sel = gtk_tree_view_get_selection ( GTK_TREE_VIEW ( tree_view ) );
		/*GtkTreeIter begin_iter, end_iter;

		begin_iter = tree_map[model->getChild(0)];
		end_iter = tree_map[model->getChild(model->size()-1)];
		
		GtkTreeStore *store = GTK_TREE_STORE ( gtk_tree_view_get_model ( GTK_TREE_VIEW ( tree_view ) ) );
		GtkTreePath * begin_path = gtk_tree_model_get_path ( GTK_TREE_MODEL(store), &begin_iter );
		GtkTreePath * end_path = gtk_tree_model_get_path ( GTK_TREE_MODEL(store), &end_iter );

		if (state)
			gtk_tree_selection_select_range(sel, begin_path, end_path);
		else
			gtk_tree_selection_unselect_range(sel, begin_path, end_path);

		gtk_tree_path_free(begin_path);
		gtk_tree_path_free(end_path);*/

		if (!state)
			gtk_tree_selection_unselect_all (sel);
	}

	bool Tree::isChecked(xgui::Model * parent, int pos)
	{
		GtkTreeStore *store = GTK_TREE_STORE ( gtk_tree_view_get_model ( GTK_TREE_VIEW ( tree_view ) ) );
		GtkTreeIter * iter = &tree_map[parent->getChild(pos)];
 		gboolean state;

		gtk_tree_model_get ( GTK_TREE_MODEL(store), iter, check_column_id, &state, -1);

		return state;
	}

	void Tree::check(xgui::Model * parent, int pos, bool state)
	{
		GtkTreeStore *store = GTK_TREE_STORE ( gtk_tree_view_get_model ( GTK_TREE_VIEW ( tree_view ) ) );
		GtkTreeIter * iter = &tree_map[parent->getChild(pos)];
		gtk_tree_store_set ( store, iter, check_column_id, state, -1);
	}

	void Tree::checkAll(xgui::Model * parent, bool state)
	{
		GtkTreeStore *store = GTK_TREE_STORE ( gtk_tree_view_get_model ( GTK_TREE_VIEW ( tree_view ) ) );
		for (xgui::Model::iterator i = parent->begin(); i != parent->end(); ++i) {
			GtkTreeIter * iter = &tree_map[dynamic_cast<xgui::Model*>(*i)];
			gtk_tree_store_set ( store, iter, check_column_id, state, -1);
		}
	}

	void Tree::OnSelect ( GtkTreeSelection * sel, Tree *xl )
	{
		xgui::TextStatusCallback * sel_cb = 0;
	
		xgui::Callback * cb = xl->this_tree->getEvent("onselect");
		if (cb) {
			sel_cb = dynamic_cast<xgui::TextStatusCallback*>(cb);
			if (!sel_cb)
				DMESSAGE("onselect event of xgui::Tree expected a TextStatusCallback");
		}
	
		xgui::Model * model = xl->this_tree->getModel();

		int pos = 0;
		for ( xgui::Model::iterator child = model->begin(); child != model->end(); ++child )
			xl->selectSubTree(sel, dynamic_cast<xgui::Model*>(*child), pos++, sel_cb);
	}

	void Tree::expand ( xgui::Model * node )
	{
		if (!node) return;

		GtkTreeStore *store = GTK_TREE_STORE ( gtk_tree_view_get_model ( GTK_TREE_VIEW ( tree_view ) ) );
		GtkTreeIter iter = tree_map[node];
		GtkTreePath * path = gtk_tree_model_get_path(GTK_TREE_MODEL(store), &iter);

		gtk_tree_view_expand_row ( tree_view, path, false);

		gtk_tree_path_free(path);
	}

	void Tree::collapse ( xgui::Model * node )
	{
		if (!node) return;

		GtkTreeStore *store = GTK_TREE_STORE ( gtk_tree_view_get_model ( GTK_TREE_VIEW ( tree_view ) ) );
		GtkTreeIter iter = tree_map[node];
		GtkTreePath * path = gtk_tree_model_get_path(GTK_TREE_MODEL(store), &iter);

		gtk_tree_view_collapse_row ( tree_view, path);

		gtk_tree_path_free(path);
	}

	void Tree::sort ( xgui::Model * node, xgui::TextPairCallback * callback ) {}

	void Tree::swapCheck ( GtkTreePath * path )
	{
		GtkTreeStore *store = GTK_TREE_STORE ( gtk_tree_view_get_model ( GTK_TREE_VIEW ( tree_view ) ) );
  		GtkTreeIter iter;
		gtk_tree_model_get_iter ( GTK_TREE_MODEL(store), &iter, path);

		GValue val = {0, };
		gtk_tree_model_get_value ( GTK_TREE_MODEL(store), &iter, check_column_id, &val);
		bool active = g_value_get_boolean(&val);
		g_value_unset ( &val );

		active = (active ? 0 : 1);

		g_value_init (&val, G_TYPE_BOOLEAN);
		g_value_set_boolean ( &val, active );
		gtk_tree_store_set_value ( store, &iter, check_column_id, &val );
		g_value_unset (&val);

		xgui::Callback * cb = this_tree->getEvent("oncheck");
		if (!cb) return;

		xgui::TextStatusCallback * chk_cb = dynamic_cast<xgui::TextStatusCallback *>(cb);
		if (!chk_cb) {
			DMESSAGE("oncheck event of xgui::Tree expected a TextStatusCallback");
			return;
		}

		gint * indices = gtk_tree_path_get_indices (path);
		int depth = gtk_tree_path_get_depth (path);
		xgui::Model * node = this_tree->getModel();

		for ( int i = 0; i < depth; ++i )
			node = node->getChild(indices[i]);

		chk_cb->call(this_tree, node->path(), active);
	}

	void Tree::OnCheck ( GtkCellRendererToggle *cell, char const * const path_string,  Tree * xl)
	{
		GtkTreePath * path = gtk_tree_path_new_from_string (path_string);
		xl->swapCheck(path);
		gtk_tree_path_free(path);
	}

	void Tree::OnExpand ( GtkTreeView *treeview, GtkTreeIter *arg1, GtkTreePath *arg2, Tree * xl)
	{
		xgui::TextStatusCallback * sel_cb = 0;
	
		xgui::Callback * cb = xl->this_tree->getEvent("onexpand");
		if (cb) {
			sel_cb = dynamic_cast<xgui::TextStatusCallback*>(cb);
			if (!sel_cb)
				DMESSAGE("onexpand event of xgui::Tree expected a TextStatusCallback");
		}

		gint * indices = gtk_tree_path_get_indices (arg2);
		int depth = gtk_tree_path_get_depth (arg2);
		xgui::Model * node = xl->this_tree->getModel();

		for ( int i = 0; i < depth; ++i )
			node = node->getChild(indices[i]);

		sel_cb->call(xl->this_tree, node->path(), 1);
	}

	void Tree::OnCollapse ( GtkTreeView *treeview, GtkTreeIter *arg1, GtkTreePath *arg2, Tree * xl)
	{
		xgui::TextStatusCallback * sel_cb = 0;
	
		xgui::Callback * cb = xl->this_tree->getEvent("onexpand");
		if (cb) {
			sel_cb = dynamic_cast<xgui::TextStatusCallback*>(cb);
			if (!sel_cb)
				DMESSAGE("onexpand event of xgui::Tree expected a TextStatusCallback");
		}

		gint * indices = gtk_tree_path_get_indices (arg2);
		int depth = gtk_tree_path_get_depth (arg2);
		xgui::Model * node = xl->this_tree->getModel();

		for ( int i = 0; i < depth; ++i )
			node = node->getChild(indices[i]);

		sel_cb->call(xl->this_tree, node->path(), 0);
	}
}
