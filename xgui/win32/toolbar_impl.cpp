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

#include "precomp.h"

namespace xguimpl
{
	static HWND MkToolbar ( xgui::Container * parent )
	{
		return CreateWindowEx ( WS_EX_NOPARENTNOTIFY, TOOLBARCLASSNAME, "",
			WS_CHILD | WS_VISIBLE | CCS_NOPARENTALIGN,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, parent->getImpl()->widget, NULL,
			MAIN_HINSTANCE, NULL );
	}


	Toolbar::Toolbar ( HWND real_w ) : Widget ( real_w ),
		hImageList(0), tb_style("both"), last_id(0)
	{
		LONG style = GetClassLong ( widget, GCL_STYLE );
		SetClassLong ( widget, GCL_STYLE, style | CS_DBLCLKS );
	}

	Toolbar::Toolbar ( xgui::Container * parent ) :
		Widget ( MkToolbar ( parent ) ),
		hImageList(0), tb_style("both"), last_id(0)
	{
		SendMessage ( widget, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0 );
		SendMessage ( widget, TB_SETTOOLTIPS, (WPARAM)MASTER_INSTANCE->tip_wnd, 0 );
		SendMessage ( widget, TB_SETEXTENDEDSTYLE, 0, (LPARAM)TBSTYLE_EX_MIXEDBUTTONS );

		SendMessage ( widget, WM_SETFONT, (WPARAM) GetStockObject ( DEFAULT_GUI_FONT ), MAKELPARAM ( TRUE, 0 ) );

		LONG style = GetClassLong ( widget, GCL_STYLE );
		SetClassLong ( widget, GCL_STYLE, style | CS_DBLCLKS );

		LONG origWndProc = GetWindowLong ( widget, GWL_WNDPROC );
		SetProp ( widget, "xgui_orig_wnd_proc", (HANDLE)origWndProc );
		SetWindowLong ( widget, GWL_WNDPROC, (LONG) toolbarProc );

		min_x = 100;
		min_y = 0;
	}


	Toolbar::~Toolbar()
	{
		if ( hImageList )
			ImageList_Destroy ( hImageList );
	}


	bool Toolbar::linkEvent( std::string const &name )
	{
		if ( name == "onclick" )
			h_events["onclick"] = OnClick;

		else
			return Widget::linkEvent(name);

		return true;
	}


	char const * Toolbar::bindWMCOMMAND ( WORD )
	{
		return "onclick";
	}


	void Toolbar::setAccessor ( xgui::Widget * w )
	{
		this_tbar = dynamic_cast<xgui::Toolbar *>(w);
		ASSERT(this_tbar);

		Widget::setAccessor ( w );
	}


	void Toolbar::setStyle ( std::string const &style )
	{
		tb_style = style;

		int i;
		int cx, cy;

		if ( tb_style == "icons" )
		{
			DWORD s = SendMessage ( widget, TB_GETSTYLE, 0, 0 );
            SendMessage ( widget, TB_SETSTYLE, 0, s | TBSTYLE_LIST );
			for (i = 0; i < this_tbar->nelements_; ++i) {
				TBBUTTONINFO bi;
				bi.cbSize = sizeof ( bi );
				bi.dwMask = TBIF_STYLE | TBIF_IMAGE | TBIF_LPARAM;
				SendMessage ( widget, TB_GETBUTTONINFO, i, (LPARAM)&bi );
				if (bi.fsStyle & BTNS_SEP) continue;
				bi.fsStyle &= ~(BTNS_SHOWTEXT | BTNS_AUTOSIZE);
				bi.iImage = bi.lParam;
				SendMessage ( widget, TB_SETBUTTONINFO, i, (LPARAM)&bi );
			}
			ImageList_GetIconSize ( hImageList, &cx, &cy );
			SendMessage ( widget, TB_SETBUTTONSIZE, 0, MAKELPARAM(cx, cy) );	
		} else if ( tb_style == "text" ) {
		
			DWORD s = SendMessage ( widget, TB_GETSTYLE, 0, 0 );
			SendMessage ( widget, TB_SETSTYLE, 0, s | TBSTYLE_LIST );
			for (i = 0; i < this_tbar->nelements_; ++i) {
				TBBUTTONINFO bi;
				bi.cbSize = sizeof ( bi );
				bi.dwMask = TBIF_STYLE | TBIF_IMAGE;
				SendMessage ( widget, TB_GETBUTTONINFO, i, (LPARAM)&bi );
				if (bi.fsStyle & BTNS_SEP) continue;
				bi.fsStyle |= BTNS_SHOWTEXT | BTNS_AUTOSIZE;
				bi.iImage = I_IMAGENONE;
				SendMessage ( widget, TB_SETBUTTONINFO, i, (LPARAM)&bi );
			}
			ImageList_GetIconSize ( hImageList, &cx, &cy );
			SendMessage ( widget, TB_SETBUTTONSIZE, 0, MAKELPARAM(cx, cy) );

		} else {
			tb_style = "both";
			DWORD s = SendMessage ( widget, TB_GETSTYLE, 0, 0 );
            SendMessage ( widget, TB_SETSTYLE, 0, s & ~TBSTYLE_LIST );
			for (i = 0; i < this_tbar->nelements_; ++i) {
				TBBUTTONINFO bi;
				bi.cbSize = sizeof ( bi );
				bi.dwMask = TBIF_STYLE | TBIF_IMAGE | TBIF_LPARAM;
				SendMessage ( widget, TB_GETBUTTONINFO, i, (LPARAM)&bi );
				if (bi.fsStyle & BTNS_SEP) continue;
				bi.fsStyle |= BTNS_SHOWTEXT | BTNS_AUTOSIZE;
				bi.iImage = bi.lParam;
				SendMessage ( widget, TB_SETBUTTONINFO, i, (LPARAM)&bi );
			}
			ImageList_GetIconSize ( hImageList, &cx, &cy );
			{
				SIZE sz;
				HDC hdc = GetDC ( widget );
				GetTextExtentPoint32 ( hdc, "M", 1, &sz );
				ReleaseDC ( widget, hdc );
				cy += sz.cy;
			}
			SendMessage ( widget, TB_SETBUTTONSIZE, 0, MAKELPARAM(cx, cy + 4) );
		}

		InvalidateRect ( widget, NULL, TRUE );
		UpdateWindow ( widget );

		updateSize();
	}


	std::string Toolbar::getStyle()
	{
		return tb_style;
	}


	int Toolbar::_add_button ( const xgui::TbButtonInfo & btn, BYTE style )
	{
		TBBUTTON tb;

		if (btn.image) {
			xgui::Image * img = btn.image;
			TBADDBITMAP ab;
			if (!hImageList) {
				hImageList = ImageList_Create ( img->getWidth(), img->getHeight(), ILC_COLOR32, 0, 1 );
				ImageList_SetBkColor ( hImageList, GetSysColor(COLOR_BTNFACE) );
				SendMessage ( widget, TB_SETIMAGELIST, 0, (LPARAM)hImageList );
			}
			ab.hInst = NULL;
			ab.nID = (UINT_PTR)img->getImpl()->getImage();
			HBITMAP hBmp = img->getImpl()->getButtonBitmap();
			tb.iBitmap = ImageList_Add ( hImageList, hBmp, NULL );
			DeleteObject ( hBmp );
		} else
			tb.iBitmap = I_IMAGENONE;

		tb.idCommand = last_id++;
		tb.fsState = TBSTATE_ENABLED;
		tb.fsStyle = style | ((tb_style != "icons") ? BTNS_SHOWTEXT : 0);
		tb.dwData = tb.iBitmap;
		if ( tb_style == "text" ) tb.iBitmap = I_IMAGENONE;
		tb.iString = (INT_PTR)btn.text.c_str();
		SendMessage ( widget, TB_ADDBUTTONS, 1, (LPARAM)&tb );

		if ( tb_style == "icons" ) {
			int cx = 32, cy = 32;
			if (hImageList) ImageList_GetIconSize ( hImageList, &cx, &cy );
			SendMessage ( widget, TB_SETBUTTONSIZE, 0, MAKELPARAM(cx, cy) );
		}

		tips.push_back(btn.tooltip);

		idmap.set(tb.idCommand, btn.name);

		if (this_tbar->nelements_ == 0) {
			updateSize();
			SetFocus(NULL);
		}

		return tb.idCommand;
	}


	int Toolbar::addButton ( const xgui::TbButtonInfo & btn )
	{
		BYTE style = BTNS_BUTTON | (tb_style != "icons" ? BTNS_AUTOSIZE : 0);
		_add_button ( btn, style );

		if ( btn.image ) {
			btn.image->ref();
			this_tbar->images_.push_back(btn.image);
		}

		return this_tbar->nelements_;
	}


	int Toolbar::addSeparator()
	{
		_add_button ( xgui::TbButtonInfo(), BTNS_SEP );
		return this_tbar->nelements_;
	}


	int Toolbar::addToggleButton ( const xgui::TbButtonInfo & btn )
	{
		BYTE style = BTNS_CHECK | (tb_style != "icons" ? BTNS_AUTOSIZE : 0);
		_add_button ( btn, style );

		if ( btn.image ) {
			btn.image->ref();
			this_tbar->images_.push_back(btn.image);
		}

		return this_tbar->nelements_;
	}


	int Toolbar::addRadioButtons ( std::vector<xgui::TbButtonInfo> const & checkgroup )
	{
		int group_id = check_groups.size();
		check_groups.push_back ( "" );

		std::vector<xgui::TbButtonInfo>::const_iterator iter = checkgroup.begin();

		int first_id = -1;
		for (; iter != checkgroup.end(); ++iter) {

			if ( iter->image ) {
				iter->image->ref();
				this_tbar->images_.push_back ( iter->image );
			}

			BYTE style = BTNS_CHECKGROUP | (tb_style != "icons" ? BTNS_AUTOSIZE : 0);
			int id = _add_button ( *iter, style );

			if ( first_id == -1 ) first_id = id;

			group_map[id] = group_id;

		}

		SendMessage ( widget, TB_CHECKBUTTON, first_id, MAKELONG ( TRUE, 0 ) );

		return this_tbar->nelements_;
	}


	bool Toolbar::removeButton ( const std::string & name )
	{
		int id = idmap.get_id ( name );
		int idx = SendMessage ( widget, TB_COMMANDTOINDEX, id, 0 );

		return SendMessage ( widget, TB_DELETEBUTTON, idx, 0 ) != FALSE;
	}


	void Toolbar::enableButton ( const std::string & name )
	{
		int id = idmap.get_id ( name );
		int idx = SendMessage ( widget, TB_COMMANDTOINDEX, id, 0 );

		SendMessage ( widget, TB_ENABLEBUTTON, idx, MAKELONG ( TRUE, 0 ) );
	}


	void Toolbar::disableButton ( const std::string & name )
	{
		int id = idmap.get_id ( name );
		int idx = SendMessage ( widget, TB_COMMANDTOINDEX, id, 0 );

		SendMessage ( widget, TB_ENABLEBUTTON, idx, MAKELONG ( FALSE, 0 ) );
	}


	void Toolbar::checkButton ( const std::string & name )
	{
		int id = idmap.get_id ( name );
		int idx = SendMessage ( widget, TB_COMMANDTOINDEX, id, 0 );

		SendMessage ( widget, TB_CHECKBUTTON, idx, MAKELONG ( TRUE, 0 ) );
	}


	void Toolbar::uncheckButton ( const std::string & name )
	{
		int id = idmap.get_id ( name );
		int idx = SendMessage ( widget, TB_COMMANDTOINDEX, id, 0 );

		SendMessage ( widget, TB_CHECKBUTTON, idx, MAKELONG ( FALSE, 0 ) );
	}


	bool Toolbar::isButtonChecked ( const std::string & name )
	{
		int id = idmap.get_id ( name );
		int idx = SendMessage ( widget, TB_COMMANDTOINDEX, id, 0 );

		return SendMessage ( widget, TB_ISBUTTONCHECKED, idx, 0 ) != 0;
	}


	void Toolbar::updateSize()
	{
		SendMessage ( widget, TB_AUTOSIZE, 0, 0 );

		RECT rc;
		GetWindowRect(widget, &rc);
		min_y = rc.bottom - rc.top;

		this_widget->getContext()->redraw();
	}


	int Toolbar::OnClick ( xgui::Toolbar * tbar, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		hWnd; msg; lParam;

		Toolbar * tb = dynamic_cast<Toolbar *>(tbar->getImpl());

		xgui::TextStatusCallback * cb = dynamic_cast<xgui::TextStatusCallback *>(tbar->getEvent("onclick"));
		
		int id = LOWORD(wParam);
		std::string name = tb->idmap.get_string ( id );

		TBBUTTONINFO tinfo;
		tinfo.cbSize = sizeof(tinfo);
		tinfo.dwMask = TBIF_STATE | TBIF_STYLE;
		SendMessage ( tb->widget, TB_GETBUTTONINFO, id, (LPARAM)&tinfo );

		int state = 1;
		if (tinfo.fsStyle & TBSTYLE_GROUP) {

			int group_id = tb->group_map[id];
			std::string & sel_name = tb->check_groups[group_id];

			if ( sel_name != "" && cb )
					cb->call ( dynamic_cast<xgui::Widget *>(tb), sel_name, 0 );

			sel_name = name;

		}
		
		if (tinfo.fsStyle & TBSTYLE_CHECK)
			state = tinfo.fsState & TBSTATE_CHECKED ? 1 : 0;

		if ( cb ) cb->call ( dynamic_cast<xgui::Widget *>(tb), name, state );
		
		return 0;
	}


	LRESULT CALLBACK Toolbar::toolbarProc ( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		HANDLE origWndProc = GetProp ( hWnd, "xgui_orig_wnd_proc" );
		if (!origWndProc) return 0;

		switch ( msg ) {
		case WM_DESTROY:
			RemoveProp ( hWnd, "xgui_orig_wnd_proc" );
			Master::dlgProc ( hWnd, msg, wParam, lParam );
			break;

		case WM_NOTIFY:
			if (((LPNMHDR)lParam)->code == TTN_GETDISPINFO) {
				LPTOOLTIPTEXT ptt = (LPTOOLTIPTEXT)lParam;
				xgui::Toolbar * tbar = (xgui::Toolbar *)MASTER_INSTANCE->findWidget(hWnd);
				Toolbar * tb = dynamic_cast<Toolbar *>(tbar->getImpl());
				ptt->lpszText = (LPSTR)(tb->tips[ptt->hdr.idFrom].c_str());
			}
			break;

		case WM_NCHITTEST:
			{
				LRESULT ret = CallWindowProc ( (WNDPROC)origWndProc, hWnd, msg, wParam, lParam );
				if ( ret == HTTRANSPARENT ) ret = HTCLIENT;
				return ret;
			}
			break;

		case WM_MOUSELEAVE:
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
		case WM_SHOWWINDOW:
		case WM_ENABLE:
		case WM_MOVE:
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_CHAR:
		case WM_DEADCHAR:
		case WM_SIZE:
		case WM_IDLE_CALLBACK:
			Master::dlgProc ( hWnd, msg, wParam, lParam );
			break;

		}

		return CallWindowProc ( (WNDPROC)origWndProc, hWnd, msg, wParam, lParam );
	}
}
