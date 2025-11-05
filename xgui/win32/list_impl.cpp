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

#include "precomp.h"

namespace xguimpl
{
	static HWND MkList ( xgui::Container * parent )
	{
		return CreateWindowEx(WS_EX_CLIENTEDGE | WS_EX_NOPARENTNOTIFY, "sysListView32", "",
			WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS |
			LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SHAREIMAGELISTS,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, (HWND)parent->getImpl()->widget,
			NULL, MAIN_HINSTANCE, NULL);
	}


	List::List ( HWND real_w ) : View ( real_w ),
		sort_column(0), sort_callback(0), last_sel_status(0), last_sel_model(0)
	{
		LONG origWndProc = GetWindowLong ( widget, GWL_WNDPROC );
		SetProp ( widget, "xgui_orig_wnd_proc", (HANDLE)origWndProc );
		SetWindowLong ( widget, GWL_WNDPROC, (LONG)Master::subProc );

		LONG style = GetClassLong ( widget, GCL_STYLE );
		SetClassLong ( widget, GCL_STYLE, style | CS_DBLCLKS );

		h_events["oncustomdraw"] = OnCustomDraw;
	}

	List::List ( xgui::Container * parent ) :
		View ( MkList ( parent ) ), sort_column(0), sort_callback(0), last_sel_status(0), last_sel_model(0)
	{
		LONG origWndProc = GetWindowLong ( widget, GWL_WNDPROC );
		SetProp ( widget, "xgui_orig_wnd_proc", (HANDLE)origWndProc );
		SetWindowLong ( widget, GWL_WNDPROC, (LONG)Master::subProc );

		ListView_SetExtendedListViewStyle ( widget, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
		ListView_SetHoverTime ( widget, 300000 );

		SendMessage ( widget, WM_SETFONT, (WPARAM) GetStockObject ( DEFAULT_GUI_FONT ), MAKELPARAM ( TRUE, 0 ) );

		LONG style = GetClassLong ( widget, GCL_STYLE );
		SetClassLong ( widget, GCL_STYLE, style | CS_DBLCLKS );

		RECT rc;
		HDC hdc = GetDC ( widget );
		SIZE sz;
		GetTextExtentPoint32 ( hdc, "x", 1, &sz );
		ReleaseDC ( widget, hdc );

		GetClientRect ( widget, &rc );

		min_x = 16;
		min_y = sz.cy * 4 + 8;

		h_events["oncustomdraw"] = OnCustomDraw;
	}


	List::~List()
	{
	}


	bool List::linkEvent ( std::string const &name )
	{
		if ( name == "onselect" ) return true;
		else if ( name == "ondeselect" ) return true;
		else if ( name == "onrefresh" ) return true;
		else if ( name == "onheaderclick" ) {
			h_events["onheaderclick"] = OnHeaderClick;
		} else
			return Widget::linkEvent ( name) ;

		return true;
	}


	char const * List::bindWMNOTIFY ( LPNMHDR nmhdr )
	{
		switch (nmhdr->code) {
			case LVN_ITEMCHANGING:
			{
				LPNMLISTVIEW lpnmlv = (LPNMLISTVIEW)nmhdr;
				if (lpnmlv->uChanged & LVIF_STATE) {
					BOOL oldSelected = lpnmlv->uOldState & LVIS_SELECTED;
					BOOL newSelected = lpnmlv->uNewState & LVIS_SELECTED;
					if (!oldSelected && newSelected)
						return "onselect";
					else if (oldSelected && !newSelected)
						return "ondeselect";
				}
			}
			break;

			case LVN_COLUMNCLICK:
				return "onheaderclick";

			case LVN_KEYDOWN:
			{
				LPNMLVKEYDOWN lvk = (LPNMLVKEYDOWN)nmhdr;
				if ( lvk->wVKey == VK_SPACE ) {
					int pos = ListView_GetNextItem ( widget, -1, LVNI_FOCUSED );
					if ( pos >= 0 ) {
						int old_state = ListView_GetCheckState ( widget, pos );
						LVITEM lvi;
						lvi.mask = LVIF_PARAM;
						lvi.iItem = pos;
						lvi.iSubItem = 0;
						ListView_GetItem ( widget, &lvi );
						OnCheck ( this_list, (xgui::Model *)lvi.lParam, old_state ? true : false);
					}
				}
			}
			break;

			case NM_CUSTOMDRAW:
				return "oncustomdraw";

			case NM_CLICK:
			{
				LVHITTESTINFO ht;
				GetCursorPos ( &ht.pt );
				RECT rc;
				GetWindowRect ( widget, &rc );
				ht.pt.x -= rc.left;
				ht.pt.y -= rc.top;
				int pos = ListView_HitTest ( widget, &ht );

				if ( (pos >= 0) && ((ht.flags & LVHT_ONITEM) == LVHT_ONITEMSTATEICON) ) {
					int old_state = ListView_GetCheckState ( widget, pos );
					LVITEM lvi;
					lvi.mask = LVIF_PARAM;
					lvi.iItem = pos;
					lvi.iSubItem = 0;
					ListView_GetItem ( widget, &lvi );
					OnCheck ( this_list, (xgui::Model *)lvi.lParam, old_state ? true : false);
				}
			}
			break;
		}

		return "";
	}


	bool List::set_list_prop ( std::string const &name, std::string const &vals )
	{
		xgui::List * list = dynamic_cast<xgui::List *>(this_widget);

		if ( name == "visible-headers" ) {

			DWORD style = GetWindowLong ( widget, GWL_STYLE );
			if ( vals == "0" ) style |= LVS_NOCOLUMNHEADER;
			else style &= ~LVS_NOCOLUMNHEADER;
			SetWindowLong ( widget, GWL_STYLE, style );

		} else if ( name == "has-icons" ) {

			if ( vals == "1" ) {
				ListView_SetImageList ( widget, list->styleman_->getImpl()->getImageListBig(), LVSIL_NORMAL );
				ListView_SetImageList ( widget, list->styleman_->getImpl()->getImageListSmall(), LVSIL_SMALL );
			} else {
				ListView_SetImageList ( widget, NULL, LVSIL_NORMAL );
				ListView_SetImageList ( widget, NULL, LVSIL_SMALL );
			}

		} else if ( name == "has-checkboxes" ) {

			DWORD style = ListView_GetExtendedListViewStyle ( widget );
			ListView_SetExtendedListViewStyle ( widget, style | LVS_EX_CHECKBOXES );

		} else
			return false;

		return true;
	}


	bool List::get_list_prop ( std::string const &name, std::string &vals )
	{
		if ( name == "search-enabled" ) {

			vals = "1";

		} else if ( name == "visible-headers" ) {

			DWORD style = GetWindowLong ( widget, GWL_STYLE );
			vals = (style & LVS_NOCOLUMNHEADER) ? "1" : "0";

		} else if ( name == "has-icons" ) {

			HIMAGELIST hImgList = ListView_GetImageList ( widget, LVSIL_NORMAL );
			vals == (hImgList ? "1" : "0");

		} else if ( name == "has-checkboxes" ) {

			DWORD style = ListView_GetExtendedListViewStyle ( widget );
			vals = (style & LVS_EX_CHECKBOXES) ? "1" : "0";

		} else
			return false;

		return true;
	}


	void List::refresh()
	{
		xgui::List * list = dynamic_cast<xgui::List *>(this_widget);

		xgui::View * v = dynamic_cast<xgui::View *>(this_widget);
		xgui::Model * model = v->getModel();
		if (!model) return;

		h_events.erase("onselect");
		h_events.erase("ondeselect");

		ListView_DeleteAllItems ( widget );

		int idx = 0;

		xgui::Model::iterator iter = model->begin();
		for ( ; iter != model->end(); ++iter )
		{
			xgui::Model * child = dynamic_cast<xgui::Model *>(*iter);
			insertItem ( NULL, idx++, child );
		}

		OnRefresh ( list );

		h_events["onselect"] = OnSelect;
		h_events["ondeselect"] = OnDeselect;
	}


	void List::insertItem ( xgui::Model * parent, int child_pos, xgui::Model * child )
	{
		parent;

		xgui::View * view = dynamic_cast<xgui::View *>(this_widget);

		xgui::List * list = dynamic_cast<xgui::List *>(this_widget);
		xgui::StyleManager * styleman = list->styleman_;

		h_events.erase("onselect");
		h_events.erase("ondeselect");

		const std::string & style = child->getStyle();

		LVITEM lvi;
		memset ( &lvi, 0, sizeof(lvi) );
		lvi.mask = LVIF_TEXT | LVIF_PARAM;
		if ( styleman && styleman->getIcon(style) ) {
			lvi.mask |= LVIF_IMAGE;
			lvi.iImage = styleman->getImpl()->getIconIndex ( style );
		}
		lvi.pszText = (char *)child->getString(0).c_str();
		lvi.lParam = (LPARAM)child;
		lvi.iItem = child_pos;
		lvi.iSubItem = 0;
		int pos = ListView_InsertItem ( widget, &lvi );

		for ( int col = 1; col < list->num_columns_; ++col ) {
			ListView_SetItemText ( widget, pos, col, (char *)child->getString(col).c_str() );
		}

		OnRefresh ( list );

		h_events["onselect"] = OnSelect;
		h_events["ondeselect"] = OnDeselect;
	}


	void List::removeItem ( xgui::Model * parent, int child_pos, xgui::Model * child )
	{
		parent; child_pos;

		xgui::List * list = dynamic_cast<xgui::List *>(this_widget);

		h_events.erase("onselect");
		h_events.erase("ondeselect");

		LVFINDINFO lvf;
		lvf.flags = LVFI_PARAM;
		lvf.lParam = (LPARAM)child;
		lvf.vkDirection = VK_DOWN;
		int idx = ListView_FindItem ( widget, -1, &lvf );
		if ( idx == -1 ) return;

		ListView_DeleteItem ( widget, idx );

		OnRefresh ( list );

		h_events["onselect"] = OnSelect;
		h_events["ondeselect"] = OnDeselect;
	}


	bool List::isSelected ( xgui::Model * parent, int child_pos )
	{
		return ListView_GetItemState ( widget, child_pos, LVIS_SELECTED ) == LVIS_SELECTED;
	}


	void List::select ( xgui::Model * parent, int child_pos, bool state )
	{
		ListView_SetItemState ( widget, child_pos, state ? LVIS_SELECTED : 0, LVIS_SELECTED );
	}


	void List::selectAll ( xgui::Model * parent, bool state )
	{
		int count = ListView_GetItemCount ( widget );
		for ( int i = 0; i < count; ++i )
			select ( parent, i, state );
	}


	void List::addColumn ( int cpos, const std::string & cid, const std::string & cname,
        int cwidth, const std::string & calign )
	{
		LVCOLUMN lvc;
		memset ( &lvc, 0, sizeof(lvc) );
		lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
		lvc.pszText = (LPSTR)cname.c_str();
		lvc.cx = cwidth;
		if (calign == "right") lvc.fmt = LVCFMT_RIGHT;
		else if (calign == "center") lvc.fmt = LVCFMT_CENTER;
		else lvc.fmt = LVCFMT_LEFT;

		int idx = ListView_InsertColumn ( widget, cpos, &lvc );
		col_map.set ( idx, cid );

		refresh();
	}


	bool List::delColumn ( const std::string & id )
	{
		int icol = col_map.get_id ( id );
		if ( icol == -1 ) return false;

		ListView_DeleteColumn ( widget, icol );
		return true;
	}


	void List::sort ( const std::string & column_id, xgui::TextPairCallback * callback )
	{
		sort_column = col_map.get_id ( column_id );
		if ( sort_column == -1 ) return;

		sort_callback = callback;

		SendMessage ( widget, LVM_SORTITEMS, (WPARAM)this, (LPARAM)CompareFunc );
	}


	bool List::isChecked ( xgui::Model * parent, int child_pos )
	{
		int count = ListView_GetItemCount ( widget );
		if ( child_pos >= count ) return false;

		return ListView_GetCheckState ( widget, child_pos ) != 0;
	}


	void List::check ( xgui::Model * parent, int child_pos, bool state )
	{
		ListView_SetCheckState ( widget, child_pos, state );
	}


	void List::checkAll ( xgui::Model * parent, bool state )
	{
		int count = ListView_GetItemCount ( widget );
		for ( int i = 0; i < count; ++i )
			check ( parent, i, state );
	}


	int CALLBACK List::CompareFunc ( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort )
	{
		List * ls = (List *)lParamSort;

		xgui::Model * item1 = (xgui::Model *)lParam1;
		xgui::Model * item2 = (xgui::Model *)lParam2;

		return ls->sort_callback->call ( dynamic_cast<xgui::Widget *>(ls),
			item1->getString(ls->sort_column),
			item2->getString(ls->sort_column) );
	}


	int List::OnRefresh ( xgui::List * l )
	{
		xgui::Callback  * cb =  (xgui::Callback *)l->getEvent("onrefresh");
		if ( cb ) cb->call ( dynamic_cast<xgui::Widget *>(l) );
		return 0;
	}


	int List::OnSelect ( xgui::List * l, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		xgui::Model * selection = (xgui::Model *)((LPNMLISTVIEW)lParam)->lParam;
		int selindex = ((LPNMLISTVIEW)lParam)->iItem;
		xgui::TextStatusCallback * cb = dynamic_cast<xgui::TextStatusCallback *>(l->getEvent("onselect"));

		List * list = dynamic_cast<List *>(l->getImpl());

		if ( selection == list->last_sel_model && list->last_sel_status == 1 ) {
			SetWindowLong ( hWnd, DWL_MSGRESULT, TRUE );
			return TRUE;
		}
		list->last_sel_model = selection;
		list->last_sel_status = 1;

		DMESSAGE ( "List: OnSelect()" );

		bool ret = xgui::EVT_PROPAGATE;

		if ( cb ) ret = cb->call ( dynamic_cast<xgui::Widget *>(l),
			selection->path(), 1 ) == 0 ? false : true;

		if ( ret == xgui::EVT_BLOCK )
			SetWindowLong ( hWnd, DWL_MSGRESULT, TRUE );

		return (ret == xgui::EVT_BLOCK) ? TRUE : FALSE;
	}


	int List::OnDeselect ( xgui::List * l, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		xgui::Model * selection = (xgui::Model *)((LPNMLISTVIEW)lParam)->lParam;
		int selindex = ((LPNMLISTVIEW)lParam)->iItem;
		xgui::TextStatusCallback * cb = dynamic_cast<xgui::TextStatusCallback *>(l->getEvent("onselect"));

		List * list = dynamic_cast<List *>(l->getImpl());

		if ( selection == list->last_sel_model && list->last_sel_status == 0 ) {
			SetWindowLong ( hWnd, DWL_MSGRESULT, TRUE );
			return TRUE;
		}

		list->last_sel_model = selection;
		list->last_sel_status = 0;

		DMESSAGE ( "List: OnDeselect()" );

		bool ret = xgui::EVT_PROPAGATE;

		if ( cb ) ret = cb->call ( dynamic_cast<xgui::Widget *>(l),
			selection->path(), 0 ) == 0 ? false : true;

		if ( ret == xgui::EVT_BLOCK )
			SetWindowLong ( hWnd, DWL_MSGRESULT, TRUE );

		return (ret == xgui::EVT_BLOCK) ? TRUE : FALSE;
	}


	int List::OnCheck ( xgui::List * l, xgui::Model * model, bool uncheck )
	{
		xgui::TextStatusCallback * cb = dynamic_cast<xgui::TextStatusCallback *>(l->getEvent("oncheck"));

		bool ret = xgui::EVT_PROPAGATE;

		if ( cb ) ret = cb->call ( dynamic_cast<xgui::Widget *>(l),
			model->path(), uncheck ? 0 : 1 ) == 0 ? false : true;

		DMESSAGE ( (uncheck ? "list: uncheck" : "list: check") );

		return 0;
	}


	int List::OnHeaderClick ( xgui::List * l, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		hWnd; msg; wParam;

		List * list = dynamic_cast<List *>(l->getImpl());

		LPNMLISTVIEW lpnmlv = (LPNMLISTVIEW)lParam;
		int icol = lpnmlv->iSubItem;

		std::string id = list->col_map.get_string ( icol );

		xgui::TextCallback  * cb =  dynamic_cast<xgui::TextCallback *>(l->getEvent("onheaderclick"));

		if ( cb ) cb->call ( dynamic_cast<xgui::Widget *>(l), id );

		return 0;
	}


	int List::OnCustomDraw ( xgui::List * l, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		msg; wParam;

		List * list = dynamic_cast<List *>(l->getImpl());

		LPNMLVCUSTOMDRAW cd = (LPNMLVCUSTOMDRAW)lParam;

		switch ( cd->nmcd.dwDrawStage ) {
			case CDDS_PREPAINT:
			{
				SetWindowLong ( hWnd, DWL_MSGRESULT, CDRF_NOTIFYITEMDRAW );
				return TRUE;
			}
			break;

			case CDDS_ITEMPREPAINT:
			{
				DWORD item = cd->nmcd.dwItemSpec;
				if ( item >= (DWORD)ListView_GetItemCount ( list->widget ) ) {
					SetWindowLong ( hWnd, DWL_MSGRESULT, CDRF_DODEFAULT );
					return TRUE;
				}

				xgui::Model * mitem = (xgui::Model *)cd->nmcd.lItemlParam;
				if ( l->styleman_ && mitem ) {

					const std::string & style = mitem->getStyle();
					
					if ( !l->styleman_->getFgColor(style).empty() )
						cd->clrText = l->styleman_->parseColorString ( l->styleman_->getFgColor(style) );

					if ( !l->styleman_->getBgColor(style).empty() )
						cd->clrTextBk = l->styleman_->parseColorString ( l->styleman_->getBgColor(style) );

					bool bold = l->styleman_->isTextBold(style);
					bool italic = l->styleman_->isTextItalic(style);
					bool underline = l->styleman_->isTextUnderlined(style);
					bool strikeout = l->styleman_->isTextStriked(style);

					if ( bold || italic || underline || strikeout ) {

						HFONT hFont = (HFONT) GetCurrentObject ( cd->nmcd.hdc, OBJ_FONT );
						LOGFONT lf;
						GetObject ( hFont, sizeof ( lf ), &lf );
						if ( italic ) lf.lfItalic = TRUE;
						if ( bold ) lf.lfWeight = FW_BOLD;
						if ( underline ) lf.lfUnderline = TRUE;
						if ( strikeout) lf.lfStrikeOut = TRUE;
						hFont = CreateFontIndirect ( &lf );
						DeleteObject ( SelectObject ( cd->nmcd.hdc, hFont ) );

						SetWindowLong ( hWnd, DWL_MSGRESULT, CDRF_DODEFAULT );
						return TRUE;

					}

				}
			}
			break;

		}

		SetWindowLong ( hWnd, DWL_MSGRESULT, CDRF_DODEFAULT );
		return 0;
	}


	void List::setAccessor ( xgui::Widget * w )
	{
		this_list = dynamic_cast<xgui::List *>(w);
		ASSERT(this_list);

		Widget::setAccessor ( w );
	}
}
