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

#include "precomp.h"

namespace xguimpl
{
	static HWND MkTree ( xgui::Container * parent )
	{
		return CreateWindowEx ( WS_EX_CLIENTEDGE | WS_EX_NOPARENTNOTIFY, "sysTreeView32", "",
			WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS |
			TVS_LINESATROOT | TVS_HASBUTTONS | TVS_HASLINES | TVS_SHOWSELALWAYS,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, (HWND)parent->getImpl()->widget,
			NULL, MAIN_HINSTANCE, NULL );
	}


	Tree::Tree ( HWND real_w ) : View ( real_w ),
		sort_callback(0)
	{
		LONG origWndProc = GetWindowLong ( widget, GWL_WNDPROC );
		SetProp ( widget, "xgui_orig_wnd_proc", (HANDLE)origWndProc );
		SetWindowLong ( widget, GWL_WNDPROC, (LONG)Master::subProc );

		LONG style = GetClassLong ( widget, GCL_STYLE );
		SetClassLong ( widget, GCL_STYLE, style | CS_DBLCLKS );

		h_events["oncustomdraw"] = OnCustomDraw;
	}

	Tree::Tree ( xgui::Container * parent ) :
		View ( MkTree ( parent ) ), sort_callback(0)
	{
		LONG origWndProc = GetWindowLong ( widget, GWL_WNDPROC );
		SetProp ( widget, "xgui_orig_wnd_proc", (HANDLE)origWndProc );
		SetWindowLong ( widget, GWL_WNDPROC, (LONG)Master::subProc );

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


	Tree::~Tree()
	{
	}


	bool Tree::linkEvent ( std::string const &name )
	{
		if ( name == "onselect" ) return true;
		else if ( name == "onexpand" ) return true;
		else if ( name == "onrefresh" ) return true;
		else
			return Widget::linkEvent ( name) ;

		return true;
	}


	char const * Tree::bindWMNOTIFY ( LPNMHDR nmhdr )
	{
		switch (nmhdr->code) {
			case TVN_SELCHANGING:
			{
				LPNMTREEVIEW lpnmtv = (LPNMTREEVIEW)nmhdr;
				BOOL oldSelected = lpnmtv->itemOld.state & TVIS_SELECTED;
				BOOL newSelected = lpnmtv->itemNew.state & TVIS_SELECTED;
				if (oldSelected)
					OnSelect ( this_tree, NULL, WM_NOTIFY, 0, (LPARAM)nmhdr, true );

				OnSelect ( this_tree, NULL, WM_NOTIFY, 0, (LPARAM)nmhdr, false );

				return "";
			}
			break;

			case TVN_ITEMEXPANDING:
			{
				OnExpand ( this_tree, NULL, WM_NOTIFY, 0, (LPARAM)nmhdr );
				return "";
			}
			break;

			case TVN_KEYDOWN:
			{
				LPNMTVKEYDOWN tvk = (LPNMTVKEYDOWN)nmhdr;
				if ( tvk->wVKey == VK_SPACE ) {
					HTREEITEM hItem = TreeView_GetSelection ( widget );
					if ( hItem ) {
						int old_state = TreeView_GetCheckState ( widget, hItem );
						TVITEM tvi;
						tvi.mask = TVIF_PARAM;
						tvi.hItem = hItem;
						TreeView_GetItem ( widget, &tvi );
						OnCheck ( this_tree, (xgui::Model *)tvi.lParam, old_state ? true : false);
					}
				}
			}
			break;

			case NM_CUSTOMDRAW:
				return "oncustomdraw";

			case NM_CLICK:
			{
				TVHITTESTINFO ht;
				GetCursorPos ( &ht.pt );
				RECT rc;
				GetWindowRect ( widget, &rc );
				ht.pt.x -= rc.left;
				ht.pt.y -= rc.top;
				TreeView_HitTest ( widget, &ht );

				if ( ht.hItem && (ht.flags & TVHT_ONITEMSTATEICON) ) {
					int old_state = TreeView_GetCheckState ( widget, ht.hItem );
					TVITEM tvi;
					tvi.mask = TVIF_PARAM;
					tvi.hItem = ht.hItem;
					TreeView_GetItem ( widget, &tvi );
					OnCheck ( this_tree, (xgui::Model *)tvi.lParam, old_state ? true : false);
				}
			}
			break;

		}

		return "";
	}


	bool Tree::set_tree_prop ( std::string const &name, std::string const &vals )
	{
		xgui::Tree * tree = dynamic_cast<xgui::Tree *>(this_widget);

		if ( name == "has-icons" ) {

			if ( vals == "1" && tree->styleman_ ) {
				TreeView_SetImageList ( widget, tree->styleman_->getImpl()->getImageListSmall(), LVSIL_NORMAL );
			} else {
				TreeView_SetImageList ( widget, NULL, LVSIL_NORMAL );
			}

		} else if ( name == "has-checkboxes" ) {

			DWORD style = GetWindowLong ( widget, GWL_STYLE );
			SetWindowLong ( widget, GWL_STYLE, style | TVS_CHECKBOXES );

		} else
			return false;

		return true;
	}


	bool Tree::get_tree_prop ( std::string const &name, std::string &vals )
	{
		if ( name == "search-enabled" ) {

			vals = "1";

		} else if ( name == "has-icons" ) {

			HIMAGELIST hImgList = TreeView_GetImageList ( widget, LVSIL_NORMAL );
			vals == (hImgList ? "1" : "0");

		} else if ( name == "has-checkboxes" ) {

			DWORD style = GetWindowLong ( widget, GWL_STYLE );
			vals = ( style & TVS_CHECKBOXES ) ? "1" : "0";

		} else
			return false;

		return true;
	}


	void Tree::_insertSubTree ( xgui::Model * parent, xgui::Model * model )
	{
		if ( parent )
			insertItem ( parent, -1, model );

		xgui::Model::iterator iter = model->begin();
		for ( ; iter != model->end(); ++iter )
		{
			xgui::Model * child = dynamic_cast<xgui::Model *>(*iter);
			_insertSubTree ( model, child );
		}
	}


	void Tree::refresh()
	{
		xgui::Tree * tree = dynamic_cast<xgui::Tree *>(this_widget);

		xgui::View * v = dynamic_cast<xgui::View *>(this_widget);
		xgui::Model * model = v->getModel();
		if (!model) return;

		TreeView_DeleteAllItems ( widget );
		tree_map.clear();

		_insertSubTree ( NULL, model );

		OnRefresh ( tree );
	}


	void Tree::insertItem ( xgui::Model * parent, int child_pos, xgui::Model * child )
	{
		HTREEITEM hParent = NULL;
		HTREEITEM hInsertAfter = TVI_LAST;

		if ( parent ) {
			hParent = tree_map[parent];
			if ( child_pos == -1 ) hInsertAfter = TVI_LAST;
			else if ( child_pos <= 0 ) hInsertAfter = TVI_FIRST;
			else {
				xgui::Model * prev_child = parent->getChild ( child_pos );
				hInsertAfter = (prev_child ? tree_map[prev_child] : TVI_LAST);
			}
		}

		xgui::View * view = dynamic_cast<xgui::View *>(this_widget);

		xgui::Tree * tree = dynamic_cast<xgui::Tree *>(this_widget);
		xgui::StyleManager * styleman = tree->styleman_;

		const std::string & style = child->getStyle();

		TVITEM tvi;
		memset ( &tvi, 0, sizeof(tvi) );
		tvi.mask = TVIF_TEXT | TVIF_PARAM | TVIF_CHILDREN;
		if ( styleman && styleman->getIcon(style) ) {
			tvi.mask |= TVIF_IMAGE | TVIF_SELECTEDIMAGE;
			tvi.iImage = styleman->getImpl()->getIconIndex ( style );
			tvi.iSelectedImage = styleman->getImpl()->getIconIndex ( style + "-selected" );
		}
		tvi.pszText = (char *)child->getString(0).c_str();
		tvi.lParam = (LPARAM)child;
		tvi.cChildren = child->size() > 0 ? 1 : 0;

		TVINSERTSTRUCT tvs;
		memset ( &tvs, 0, sizeof(tvs) );
		tvs.hParent = hParent;
		tvs.hInsertAfter = hInsertAfter;
		tvs.item = tvi;
		HTREEITEM hItem = TreeView_InsertItem ( widget, &tvs );

		if ( hItem ) tree_map[child] = hItem;

		if ( hParent ) {
			tvi.hItem = hParent;
			tvi.mask = TVIF_CHILDREN;
			tvi.cChildren = 1;
			TreeView_SetItem ( widget, &tvi );
		}

		OnRefresh ( tree );
	}


	void Tree::removeItem ( xgui::Model * parent, int child_pos, xgui::Model * child )
	{
		parent; child_pos;

		xgui::Tree * tree = dynamic_cast<xgui::Tree *>(this_widget);

		TreeView_DeleteItem ( widget, tree_map[child] );
		tree_map.erase ( child );

		OnRefresh ( tree );
	}


	bool Tree::isSelected ( xgui::Model * parent, int child_pos )
	{
		xgui::Model * child = parent->getChild ( child_pos );
		if ( !child ) return false;

		HTREEITEM hItem = tree_map[child];

		return TreeView_GetSelection ( widget ) == hItem;
	}


	void Tree::select ( xgui::Model * parent, int child_pos, bool state )
	{
		xgui::Model * child = parent->getChild ( child_pos );
		HTREEITEM hItem = tree_map[child];

		TreeView_SelectItem ( widget, state ? hItem : NULL );
	}


	void Tree::selectAll ( xgui::Model * parent, bool state )
	{
		if ( !state ) TreeView_SelectItem ( widget, NULL );
	}


	void Tree::sort ( xgui::Model * node, xgui::TextPairCallback * callback )
	{
		sort_callback = callback;
		TVSORTCB tvs;
		tvs.hParent = tree_map[node];
		tvs.lpfnCompare = CompareFunc;
		tvs.lParam = (LPARAM)this;
		SendMessage ( widget, TVM_SORTCHILDRENCB, 0, (LPARAM)&tvs );
	}


	void Tree::expand ( xgui::Model * node )
	{
		TreeView_Expand ( widget, tree_map[node], TVE_EXPAND );
	}


	void Tree::collapse ( xgui::Model * node )
	{
		TreeView_Expand ( widget, tree_map[node], TVE_COLLAPSE );
	}


	bool Tree::isChecked ( xgui::Model * parent, int child_pos )
	{
		xgui::Model * child = parent->getChild ( child_pos );
		if ( !child ) return false;

		HTREEITEM hItem = tree_map[child];

		return TreeView_GetCheckState ( widget, hItem ) == 1;
	}


	void Tree::check ( xgui::Model * parent, int child_pos, bool state )
	{
		xgui::Model * child = parent->getChild ( child_pos );
		HTREEITEM hItem = tree_map[child];

		TreeView_SetCheckState ( widget, hItem, state );
	}


	void Tree::checkAll ( xgui::Model * parent, bool state )
	{
		UINT count = parent->size();
		for ( UINT i = 0; i < count; ++i )
			check ( parent, i, state );
	}


	int CALLBACK Tree::CompareFunc ( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort )
	{
		Tree * ts = (Tree *)lParamSort;

		xgui::Model * item1 = (xgui::Model *)lParam1;
		xgui::Model * item2 = (xgui::Model *)lParam2;

		return ts->sort_callback->call ( dynamic_cast<xgui::Widget *>(ts),
			item1->getString(0),
			item2->getString(0) );
	}


	int Tree::OnRefresh ( xgui::Tree * t )
	{
		xgui::Callback  * cb =  (xgui::Callback *)t->getEvent("onrefresh");
		if ( cb ) cb->call ( dynamic_cast<xgui::Widget *>(t) );
		return 0;
	}


	int Tree::OnSelect ( xgui::Tree * t, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, bool deselect )
	{
		xgui::Model * selection = (xgui::Model *)((LPNMTREEVIEW)lParam)->itemNew.lParam;
		xgui::TextStatusCallback * cb = dynamic_cast<xgui::TextStatusCallback *>(t->getEvent("onselect"));

		if (!selection) return FALSE;

		bool ret = xgui::EVT_PROPAGATE;

		if ( cb ) ret = cb->call ( dynamic_cast<xgui::Widget *>(t),
			selection->path(), deselect ? 0 : 1 ) == 0 ? false : true;

		if ( ret == xgui::EVT_BLOCK )
			SetWindowLong ( hWnd, DWL_MSGRESULT, TRUE );

		DMESSAGE ( (deselect ? "tree: deselect" : "tree: select") );

		return (ret == xgui::EVT_BLOCK) ? TRUE : FALSE;
	}


	int Tree::OnCheck ( xgui::Tree * t, xgui::Model * model, bool uncheck )
	{
		xgui::TextStatusCallback * cb = dynamic_cast<xgui::TextStatusCallback *>(t->getEvent("oncheck"));

		bool ret = xgui::EVT_PROPAGATE;

		if ( cb ) ret = cb->call ( dynamic_cast<xgui::Widget *>(t),
			model->path(), uncheck ? 0 : 1 ) == 0 ? false : true;

		DMESSAGE ( (uncheck ? "tree: uncheck" : "tree: check") );

		return 0;
	}


	int Tree::OnExpand ( xgui::Tree * t, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		xgui::Model * selection = (xgui::Model *)((LPNMTREEVIEW)lParam)->itemNew.lParam;
		bool expand = (((LPNMTREEVIEW)lParam)->action & TVE_EXPAND) ? true : false;
		xgui::TextStatusCallback * cb = dynamic_cast<xgui::TextStatusCallback *>(t->getEvent("onexpand"));

		bool ret = xgui::EVT_PROPAGATE;

		if ( cb ) ret = cb->call ( dynamic_cast<xgui::Widget *>(t),
			selection->path(), expand ? 1 : 0 ) == 0 ? false : true;

		if ( ret == xgui::EVT_BLOCK )
			SetWindowLong ( hWnd, DWL_MSGRESULT, TRUE );
		
		DMESSAGE ( ( expand ? "tree: expand" : "tree: collapse" ) );

		return 0;
	}


	int Tree::OnCustomDraw ( xgui::Tree * t, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		Tree * tree = dynamic_cast<Tree *>(t->getImpl());

		LPNMTVCUSTOMDRAW cd = (LPNMTVCUSTOMDRAW)lParam;

		switch ( cd->nmcd.dwDrawStage ) {
			case CDDS_PREPAINT:
			{
				SetWindowLong ( hWnd, DWL_MSGRESULT, CDRF_NOTIFYITEMDRAW );
				return TRUE;
			}
			break;

			case CDDS_ITEMPREPAINT:
			{
				HTREEITEM item = (HTREEITEM)cd->nmcd.dwItemSpec;
				if ( !item ) {
					SetWindowLong ( hWnd, DWL_MSGRESULT, CDRF_DODEFAULT );
					return TRUE;
				}

				xgui::Model * mitem = (xgui::Model *)cd->nmcd.lItemlParam;
				if ( t->styleman_ && mitem ) {

					const std::string & style = mitem->getStyle();

					if ( !t->styleman_->getFgColor(style).empty() )
						cd->clrText = t->styleman_->parseColorString ( t->styleman_->getFgColor(style) );

					if ( !t->styleman_->getBgColor(style).empty() )
						cd->clrTextBk = t->styleman_->parseColorString ( t->styleman_->getFgColor(style) );

					bool bold = t->styleman_->isTextBold(style);
					bool italic = t->styleman_->isTextItalic(style);
					bool underline = t->styleman_->isTextUnderlined(style);
					bool strikeout = t->styleman_->isTextStriked(style);

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


	void Tree::setAccessor ( xgui::Widget * w )
	{
		this_tree = dynamic_cast<xgui::Tree *>(w);
		ASSERT(this_tree);

		Widget::setAccessor ( w );
	}
}
