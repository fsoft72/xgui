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

#include "precomp.h"

namespace xguimpl
{
	static HWND MkTab ( xgui::Container * parent, const std::string & alignment )
	{
		DWORD style = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | TCS_HOTTRACK | TCS_MULTILINE;
		if ( alignment == "bottom" ) style |= TCS_BOTTOM;
		else if ( alignment == "left" ) style |= TCS_VERTICAL;
		else if ( alignment == "right" ) style |= TCS_VERTICAL | TCS_RIGHT;

		return CreateWindowEx ( WS_EX_CONTROLPARENT,
			WC_TABCONTROL, "", style, CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT, (HWND)parent->getImpl()->widget, NULL, MAIN_HINSTANCE, NULL );
	}

	Tab::Tab ( HWND real_w ) :
		Widget ( real_w ), current(0), page_count(0), hImageList(0)
	{
		LONG origWndProc = GetWindowLong ( widget, GWL_WNDPROC );
		SetProp ( widget, "xgui_orig_wnd_proc", (HANDLE)origWndProc );
		SetWindowLong ( widget, GWL_WNDPROC, (LONG)Master::subProc );

		DWORD style = GetClassLong ( widget, GCL_STYLE );
		SetClassLong ( widget, GCL_STYLE, style | CS_DBLCLKS );

		h_events["onselect"] = OnSelect;
		h_events["onmove"] = OnMove;
	}

	Tab::Tab ( xgui::Container * parent, const std::string & alignment ) :
		Widget ( MkTab ( parent, alignment ) ), current(0), page_count(0), hImageList(0)
	{
		LONG origWndProc = GetWindowLong ( widget, GWL_WNDPROC );
		SetProp ( widget, "xgui_orig_wnd_proc", (HANDLE)origWndProc );
		SetWindowLong ( widget, GWL_WNDPROC, (LONG)Master::subProc );

		DWORD style = GetClassLong ( widget, GCL_STYLE );
		SetClassLong ( widget, GCL_STYLE, style | CS_DBLCLKS );

		SendMessage ( widget, WM_SETFONT, (WPARAM) GetStockObject ( DEFAULT_GUI_FONT ), MAKELPARAM ( TRUE, 0 ) );

		min_x = 0;
		min_y = 0;

		h_events["onselect"] = OnSelect;
		h_events["onmove"] = OnMove;
	}

	Tab::~Tab()
	{
		if ( hImageList )
			ImageList_Destroy ( hImageList );
	}


	bool Tab::linkEvent ( std::string const &name )
	{
		if ( name == "onselect" )
			return true;
		else if ( name == "onmove" )
			return true;
		else
			return Widget::linkEvent(name);
	}


	char const * Tab::bindWMNOTIFY ( LPNMHDR nmhdr )
	{
		switch (nmhdr->code) {
			case TCN_SELCHANGE:
				return "onselect";

		}

		return "";
	}



	bool Tab::addChild ( xgui::Widget * wid )
	{
		xguimpl::Widget * w = wid->getImpl();

		w->h_resize = true;
		w->v_resize = true;

		int count = TabCtrl_GetItemCount ( widget );

		TCITEM tc;
		memset(&tc, 0, sizeof(tc));
		tc.mask = TCIF_PARAM;
		tc.lParam = (LPARAM)w;

		TabCtrl_InsertItem ( widget, count, &tc );

		if ( current ) ShowWindow ( w->widget, SW_HIDE );
		else current = wid;

		SetParent ( w->widget, this_widget->getParent()->getImpl()->getInternalAsParent() );

		++page_count;

		this_widget->getContext()->redraw();

		OnMove ( this_tab, NULL, 0, 0, 0 );

		RECT rc;
		GetClientRect ( widget, &rc );
		if ((rc.left + rc.bottom + rc.right + rc.top) != 0)
			TabCtrl_AdjustRect ( widget, FALSE, &rc );

		int cw = rc.right - rc.left;
		int ch = rc.bottom - rc.top;

		w->giveSize ( std::make_pair(cw, ch) );

		return true;
	}


	void Tab::delChild ( xgui::Widget * child )
	{
		TCITEM item;
		memset(&item, 0, sizeof(item));
		item.mask = TCIF_PARAM;

		int i;
		for ( i = 0; i < page_count; ++i ) {
			TabCtrl_GetItem ( widget, i, &item );
			if ( item.lParam == (LPARAM) child ) break;
		}
		if ( i < page_count ) {
			TabCtrl_DeleteItem ( widget, i );
			--page_count;
		}

		if ( child == current ) {
			current = NULL;
			TabCtrl_SetCurSel ( widget, 0 );
			OnSelect ( this_tab, NULL, 0, 0, 0 );
		}
	}


	void Tab::giveSize ( std::pair<int, int> size )
	{
		cur_x = size.first;
		cur_y = size.second;

		SetWindowPos ( widget, NULL, 0, 0, cur_x, cur_y, SWP_NOMOVE | SWP_NOZORDER );

		if ( current ) {

			RECT rc;
			GetClientRect ( widget, &rc );
			TabCtrl_AdjustRect ( widget, FALSE, &rc );

			int w = rc.right - rc.left;
			int h = rc.bottom - rc.top;

			current->getImpl()->giveSize ( std::make_pair(w, h) );

		}

		OnMove ( this_tab, NULL, 0, 0, 0 );
	}


	void Tab::recalcLayout()
	{
		min_x = 0;
		min_y = 0;

		for ( xgui::WidgetsVector::iterator wid_iter = this_tab->children_.begin();
			wid_iter != this_tab->children_.end(); ++wid_iter )
		{
			xgui::Widget * c = *wid_iter;
			Widget * child = c->getImpl();

			child->recalcLayout();
			
			min_x = max ( min_x, child->min_x );
			min_y = max ( min_y, child->min_y );
		}

		min_x += 32;
		min_y += 32;
	}

	
	bool Tab::set_tab_prop ( std::string const &name, std::string const &vals )
	{
		if ( name == "selected-page" ) {

			int idx = 0;

			xgui::WidgetsVector::iterator wid_iter;
			for ( wid_iter = this_tab->children_.begin();
				wid_iter != this_tab->children_.end(); ++wid_iter )
			{
				std::string id;
				(*wid_iter)->get ( "id", id );
				if ( id == vals ) break;
				++idx;
			}
			if ( wid_iter == this_tab->children_.end() ) return false;

			TabCtrl_SetCurSel ( widget, idx );

		} else if ( name == "selected-index" ) {

			int idx = xgui::semantic_cast<int>(vals);
			TabCtrl_SetCurSel ( widget, idx );
			OnSelect ( this_tab, NULL, 0, 0, 0 );

		} else
			return false;

		return true;
	}


	bool Tab::get_tab_prop ( std::string const &name, std::string &vals )
	{
		if ( name == "page-count" ) {
			
			vals = xgui::semantic_cast<std::string>(page_count);
		
		} else if ( name == "selected-page" ) {

			int idx = TabCtrl_GetCurSel ( widget );
			if (idx < 0 || idx >= page_count) return false;

			return this_tab->children_[idx]->get ( "id", vals );

		} else if ( name == "selected-index" ) {

			int idx = TabCtrl_GetCurSel ( widget );
			vals = xgui::semantic_cast<std::string>(idx);

		} else
			return false;

		return true;
	}


	HWND Tab::getInternalAsParent()
	{
		return this_widget->getParent()->getImpl()->getInternalAsParent();
	}


	void Tab::setPageIcon ( xgui::Widget * w, xgui::Image * img )
	{
		if ( !hImageList ) {
			hImageList = ImageList_Create ( img->getWidth(), img->getHeight(), ILC_COLOR32, 0, 1 );
			ImageList_SetBkColor ( hImageList, GetSysColor(COLOR_BTNFACE) );
			TabCtrl_SetImageList ( widget, hImageList );
		}

		int index = 0;
		xgui::WidgetsVector::iterator wid_iter;
		for ( wid_iter = this_tab->children_.begin();
			wid_iter != this_tab->children_.end(); ++wid_iter )
		{
			if ( (*wid_iter) == w ) break;
			++index;
		}
		if ( wid_iter == this_tab->children_.end() ) return;

		TCITEM tc;
		memset(&tc, 0, sizeof(tc));
		tc.mask |= TCIF_IMAGE;

		HBITMAP hBmp = img->getImpl()->getButtonBitmap();
		tc.iImage = ImageList_Add ( hImageList, hBmp, NULL );
		DeleteObject ( hBmp );

		TabCtrl_SetItem ( widget, index, &tc );
	}


	void Tab::setPageLabel ( xgui::Widget * w, const std::string & label )
	{
		int index = 0;
		xgui::WidgetsVector::iterator wid_iter;
		for ( wid_iter = this_tab->children_.begin();
			wid_iter != this_tab->children_.end(); ++wid_iter )
		{
			if ( (*wid_iter) == w ) break;
			++index;
		}
		if ( wid_iter == this_tab->children_.end() ) return;

		TCITEM tc;
		memset(&tc, 0, sizeof(tc));
		tc.mask = TCIF_TEXT;
		tc.pszText = (LPSTR)label.c_str();

		TabCtrl_SetItem ( widget, index, &tc );
	}


	int Tab::OnMove ( xgui::Tab * tab, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		hWnd; msg; wParam; lParam;

		Tab * t = dynamic_cast<Tab *>(tab->getImpl());

		if ( !t->current ) return FALSE;

		HWND hTab = t->widget;
		xgui::Widget * w = t->current;
		HWND hw = w->getImpl()->widget;

		RECT rc;
		GetClientRect ( hTab, &rc );
		if ((rc.bottom + rc.left + rc.right + rc.top) != 0)
			TabCtrl_AdjustRect ( hTab, FALSE, &rc );

		w->getImpl()->setPos ( std::make_pair ( rc.left + t->pos_x, rc.top + t->pos_y ) );

		InvalidateRect ( hw, NULL, TRUE );
		UpdateWindow ( hw );

		return FALSE;
	}


	int Tab::OnSelect ( xgui::Tab * tab, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		hWnd; msg; wParam; lParam;

		Tab * t = dynamic_cast<Tab *>(tab->getImpl());

		int idx = TabCtrl_GetCurSel ( t->widget );
		if ( idx < 0 || idx >= t->page_count ) return FALSE;

		if ( t->current) ShowWindow ( t->current->getImpl()->widget, SW_HIDE );
		t->current = t->this_tab->children_[idx];
		ShowWindow ( t->current->getImpl()->widget, SW_SHOW );

		OnMove ( tab, NULL, 0, 0, 0 );

		RECT rc;
		GetClientRect ( t->widget, &rc );
		TabCtrl_AdjustRect ( t->widget, FALSE, &rc );

		int w = rc.right - rc.left;
		int h = rc.bottom - rc.top;

		t->current->getImpl()->giveSize ( std::make_pair(w, h) );

		xgui::TextStatusCallback * cb =  dynamic_cast<xgui::TextStatusCallback *>(t->this_widget->getEvent("onselect"));

		std::string page_id;
		t->current->get ( "id", page_id );

		if ( cb ) cb->call ( dynamic_cast<xgui::Widget *>(t), page_id, idx );

		return FALSE;
	}


	void Tab::setAccessor ( xgui::Widget * w )
	{
		this_tab = dynamic_cast<xgui::Tab *>(w);
		ASSERT(this_tab);

		Widget::setAccessor ( w );
	}
}
