//
// C++ Implementation: master_impl
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

static ULONG_PTR InitGDIPlus ()
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	return gdiplusToken;
}

static void QuitGDIPlus( ULONG_PTR token )
{
	Gdiplus::GdiplusShutdown ( token );
}


namespace xguimpl
{
	typedef int ( * XGUIEventWin32CB  ) ( void * d, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );


	Master::Master ( xgui::Master * master ) :
		master_(master), gui_thread_id(0), mouse_down_widget(0), gdiplusToken(InitGDIPlus())
	{
		INITCOMMONCONTROLSEX ex;
		WNDCLASS wc;

		hInstance = GetModuleHandle(NULL);

		// Enable DPI awareness for modern high-DPI displays (Windows 7+)
		// This ensures proper scaling on high-DPI monitors
		typedef BOOL (WINAPI *SetProcessDPIAwareFunc)(void);
		HMODULE hUser32 = GetModuleHandleW(L"user32.dll");
		if (hUser32) {
			SetProcessDPIAwareFunc pSetProcessDPIAware =
				reinterpret_cast<SetProcessDPIAwareFunc>(GetProcAddress(hUser32, "SetProcessDPIAware"));
			if (pSetProcessDPIAware) {
				pSetProcessDPIAware();
			}
		}

		ex.dwSize = sizeof(ex);
		ex.dwICC = ICC_BAR_CLASSES | ICC_COOL_CLASSES | ICC_DATE_CLASSES | ICC_LISTVIEW_CLASSES |
			ICC_PROGRESS_CLASS | ICC_TAB_CLASSES | ICC_TREEVIEW_CLASSES | ICC_UPDOWN_CLASS;
		InitCommonControlsEx(&ex);

		// Register window classes using Unicode API for modern Windows
		memset(&wc, 0, sizeof(wc));
		wc.style = CS_DBLCLKS;
		wc.hInstance = hInstance;
		wc.lpfnWndProc = DefDlgProc;
		wc.cbWndExtra = DLGWINDOWEXTRA;
		wc.lpszClassName = L"XGUIWindowClass";
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = reinterpret_cast<HBRUSH>(static_cast<INT_PTR>(COLOR_BTNFACE + 1));
		RegisterClassW(&wc);

		memset(&wc, 0, sizeof(wc));
		wc.style = CS_DBLCLKS;
		wc.hInstance = hInstance;
		wc.lpfnWndProc = DefDlgProc;
		wc.cbWndExtra = DLGWINDOWEXTRA;
		wc.lpszClassName = L"XGUIBox";
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = reinterpret_cast<HBRUSH>(static_cast<INT_PTR>(COLOR_BTNFACE + 1));
		RegisterClassW(&wc);

		memset(&wc, 0, sizeof(wc));
		wc.style = CS_DBLCLKS;
		wc.hInstance = hInstance;
		wc.lpfnWndProc = xguimpl::ImageView::wndProc;
		wc.cbWndExtra = 0;
		wc.lpszClassName = L"XGUIImageView";
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
		RegisterClassW(&wc);

		tip_wnd = CreateWindowEx ( WS_EX_TOPMOST, TOOLTIPS_CLASS, "", WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
			CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT,
			NULL, NULL, hInstance, NULL );

		SetWindowPos(tip_wnd,
			HWND_TOPMOST,
			0,
			0,
			0,
			0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	}


	Master::~Master()
	{
		PostQuitMessage(0);
		QuitGDIPlus( gdiplusToken );
	}


	void Master::initThreads()
	{
	}

	void Master::quitThreads()
	{
	}

	void Master::guiLock()
	{
		for ( xgui::WidgetsSet::iterator i = master_->contexts_.begin(); i != master_->contexts_.end(); ++i )
			SendMessage ( (*i)->getImpl()->widget, WM_NULL, 0, 0 );
	}

	void Master::guiUnlock()
	{
	}

	void Master::redrawTopLevels()
	{
		for ( xgui::WidgetsSet::iterator i = master_->contexts_.begin(); i != master_->contexts_.end(); ++i) {
			xgui::Window * w = dynamic_cast<xgui::Window *>(*i);
			xguimpl::Window * wi = dynamic_cast<xguimpl::Window *>(w->getImpl());

			RECT rc;
			HWND hDlg = wi->widget;
			
			w->recalcLayout();

			GetClientRect ( hDlg, &rc );
			LockWindowUpdate ( hDlg );
			wi->in_redraw = true;
			dlgProc ( hDlg, WM_SIZE, 0, MAKELPARAM ( rc.right + 1, rc.bottom + 1) );
			dlgProc ( hDlg, WM_SIZE, 0, MAKELPARAM ( rc.right, rc.bottom ) );
			wi->in_redraw = false;
			LockWindowUpdate ( NULL );
		}
	}

	int Master::run()
	{
		if ( !gui_thread_id )
			gui_thread_id = GetCurrentThreadId();
		else if ( gui_thread_id != GetCurrentThreadId() ) {
			DMESSAGE ( "Master::Run() called outside GUI thread" );
			return 0;
		}

		++master_->loop_level_;
		MSG msg;

		while ( GetMessage ( &msg, NULL, 0, 0 ) ) {
			xgui::WidgetsSet::iterator w;

			if ( msg.message == WM_KEYDOWN && msg.wParam == VK_RETURN ) {
				if ( dlgProc ( msg.hwnd, WM_SUBMIT, 0, 0 ) )
					continue;
			}

			bool is_dialog = false;
			for ( w = master_->contexts_.begin(); w != master_->contexts_.end(); ++w ) {
				if ( IsDialogMessage ( (*w)->getImpl()->widget, &msg ) ) {
					is_dialog = true;
					break;
				}
			}
			
			if ( !is_dialog ) {
				TranslateMessage ( &msg );
				DispatchMessage ( &msg );
			}

		}

		return msg.wParam;
	}

	bool Master::quit()
	{
		if (--master_->loop_level_ <= 0) {
			return true;
		}
		else {
			PostQuitMessage(0);
			return false;
		}
	}


	void Master::registerIdleCallback ( xgui::Callback * cbk, xgui::Widget * wid )
	{
		PostMessage ( wid->getImpl()->widget, WM_IDLE_CALLBACK, (WPARAM)cbk, (LPARAM)wid );
	}


	BOOL CALLBACK Master::dlgProc ( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		xgui::Widget * xbwc;
		xguimpl::Widget * xbw;
		HWND event_source;
		const char * event_name = NULL;
		XGUIEventWin32CB cb;
		WidgetsHash & widgets = (xgui::Master::Instance())->master_impl_->widgets;

		if ( msg == WM_COMMAND && lParam ) event_source = (HWND)lParam;
		else if (msg == WM_HSCROLL || msg == WM_VSCROLL) event_source = (HWND)lParam;
		else if ( msg == WM_NOTIFY ) event_source = ((LPNMHDR)lParam)->hwndFrom;
		else event_source = hWnd;

		WidgetsHash::iterator i = widgets.find ( event_source );
		if ( i == widgets.end() ) return FALSE;
		xbw = i->second;
		xbwc = xbw->getAccessor();

		if ( msg != WM_REDRAW ) xbw->redrawing = false;

		switch (msg) {
		case WM_INITDIALOG:
			return TRUE;

		case WM_REDRAW:
			{
				if ( xbw->redrawing ) return TRUE;

				RECT rc;
				GetClientRect ( hWnd, &rc );
				xbw->giveSize(std::make_pair(rc.right, rc.bottom));
				
				LockWindowUpdate ( hWnd );
				xbw->in_redraw = true;
				Master::dlgProc ( hWnd, WM_SIZE, 0, MAKELPARAM ( rc.right + 1, rc.bottom + 1) );
				Master::dlgProc ( hWnd, WM_SIZE, 0, MAKELPARAM ( rc.right, rc.bottom ) );
				xbw->in_redraw = false;
				LockWindowUpdate ( NULL );

				xbw->redrawing = true;
			}
			return TRUE;

		case WM_MOUSELEAVE:
			if (xbw->mouse_in) {
				xbw->mouse_in = false;
				event_name = "onmouseout";
			}
			break;

		case WM_NCHITTEST:
			if ( xbw->isMouseInWindow() )
				event_name = "onmousemove";
			break;

		case WM_SHOWWINDOW:
			event_name = wParam ? "onshow" : "onhide";
			break;

		case WM_ENABLE:
			event_name = wParam ? "onenable" : "ondisable";
			break;

		case WM_COMMAND:
			if ( lParam ) event_name = xbw->bindWMCOMMAND( HIWORD ( wParam ) );
			else {
				//menu command
				xgui::Window * w = dynamic_cast<xgui::Window *>(xbwc);
				if ( w && w->getMenu() ) {
					xbwc = w->getMenu();
					xbw = xbwc->getImpl();
					event_name = "onselect";
				}
			}
			break;

		case WM_NOTIFY:
			event_name = xbw->bindWMNOTIFY ( (LPNMHDR)lParam );
			break;

		case WM_SUBMIT:
			event_name = "onsubmit";
			break;

		case WM_CLOSE:
			event_name = "onclose";
			break;

		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
			event_name = "onmousedown";
			lParam = msg;
			break;

		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
			event_name = "onmouseup";
			lParam = msg;
			break;

		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
			event_name = "onmousedblclick";
			lParam = msg;
			break;

		case WM_SIZE:
			event_name = "onresize";
			break;

		case WM_GETMINMAXINFO:
			event_name = "ongetminmaxinfo";
			break;

		case WM_ACTIVATE:
			if (LOWORD(wParam) == WA_INACTIVE)
				event_name = "onblur";
			else
				event_name = "onfocus";
			break;

		case WM_DESTROY:
			event_name = "ondestroy";
			break;

		case WM_MOVE:
			event_name = "onmove";
			break;

		case WM_KEYDOWN:
			event_name = "onkeydown";
			break;

		case WM_KEYUP:
			event_name = "onkeyup";
			break;

		case WM_CHAR:
		case WM_DEADCHAR:
			event_name = "onkeypress";
			break;

		case WM_VSCROLL:
		case WM_HSCROLL:
			{
				xguimpl::Slider * sld = dynamic_cast<xguimpl::Slider *>(xbw);
				if (sld) event_name = "onchange";
			}
			break;

		case WM_PAINT:
			event_name = "onpaint";
			break;

		case WM_IDLE_CALLBACK:
			{
				xgui::Callback * cbk = (xgui::Callback *)wParam;
				xgui::Widget * wid = (xgui::Widget *)lParam;

				cbk->call ( wid );
			}
			break;

		}

		if ( !event_name ) return FALSE;

		xgui::StrVoidMap::iterator iter = xbw->h_events.find ( event_name );
		if ( iter == xbw->h_events.end() ) return FALSE;
		cb = ( XGUIEventWin32CB ) iter->second;
		if ( !cb ) return FALSE;
		
		return cb ( xbwc, hWnd, msg, wParam, lParam );
	}


	LRESULT CALLBACK Master::subProc ( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		HANDLE origWndProc = GetProp ( hWnd, "xgui_orig_wnd_proc" );
		if (!origWndProc) return 0;

		switch ( msg ) {
		case WM_NCHITTEST:
			{
				LRESULT ret = CallWindowProc ( (WNDPROC)origWndProc, hWnd, msg, wParam, lParam );
				if ( ret == HTTRANSPARENT ) ret = HTCLIENT;
				return ret;
			}
			break;

		case WM_DESTROY:
			RemoveProp ( hWnd, "xgui_orig_wnd_proc" );
			//FALLTHROUGH

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
		case WM_SIZE:
		case WM_MOVE:
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_CHAR:
		case WM_DEADCHAR:
		case WM_PAINT:
		case WM_COMMAND:
		case WM_NOTIFY:
		case WM_IDLE_CALLBACK:
			dlgProc ( hWnd, msg, wParam, lParam );
			break;
		}

		return CallWindowProc ( (WNDPROC)origWndProc, hWnd, msg, wParam, lParam );
	}


	void Master::registerWidget ( Widget * w )
	{
		widgets[w->widget] = w;
	}


	void Master::unregisterWidget ( Widget * w )
	{
		widgets.erase(w->widget);
	}

	void Master::destroyWidget ( xgui::Widget * w )
	{
		DestroyWindow ( w->getImpl()->widget );
	}


	xgui::Widget * Master::findWidget ( HWND hWnd )
	{
		WidgetsHash::iterator i = widgets.find ( hWnd );
		if ( i == widgets.end() ) return NULL;
		return i->second->getAccessor();
	}


	xgui::StrVector Master::openFileDialog ( xgui::Window * parent, xgui::FileExtensionsVector const &filter, std::string const &title, std::string const &path, bool multiselection )
	{
		std::string flt;

		xgui::FileExtensionsVector::const_iterator iter = filter.begin();
		for (; iter != filter.end(); ++iter) {
			flt.append ( iter->name );
			flt.append ( "\0", 1 );
			flt.append ( iter->extension );
			flt.append ( "\0", 1 );
		}
		flt.append ( "\0", 1 );

		OPENFILENAME ofn;
		memset(&ofn, 0, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = parent->getImpl()->widget;
		ofn.hInstance = hInstance;
		ofn.lpstrFilter = flt.c_str();
		ofn.lpstrInitialDir = path.c_str();
		ofn.lpstrTitle = title.c_str();
		ofn.Flags = (multiselection ? OFN_ALLOWMULTISELECT : 0) | OFN_FILEMUSTEXIST |
			OFN_HIDEREADONLY | OFN_EXPLORER;

		char buffer[10240] = {0};
		ofn.lpstrFile = buffer;
		ofn.nMaxFile = sizeof(buffer);

		xgui::StrVector retval;

		BOOL ret = GetOpenFileName ( &ofn );
		if (ret) {
			std::string path = buffer;
			char * pc = buffer + strlen(buffer) + 1;
			if (*pc) {
				for (; *pc; pc += strlen(pc) + 1)
					retval.push_back(path + "\\" + pc);
			} else {
				retval.push_back(path);
			}
		}

		return retval;
	}


	std::string Master::saveFileDialog ( xgui::Window * parent, xgui::FileExtensionsVector const &filter, std::string const &title, std::string const &path, std::string const &file_name )
	{
		std::string flt;

		xgui::FileExtensionsVector::const_iterator iter = filter.begin();
		for (; iter != filter.end(); ++iter) {
			flt.append ( iter->name );
			flt.append ( "\0", 1 );
			flt.append ( iter->extension );
			flt.append ( "\0", 1 );
		}
		flt.append ( "\0", 1 );

		OPENFILENAME ofn;
		memset(&ofn, 0, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = parent->getImpl()->widget;
		ofn.hInstance = hInstance;
		ofn.lpstrFilter = flt.c_str();
		ofn.lpstrInitialDir = path.c_str();
		ofn.lpstrTitle = title.c_str();
		ofn.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_EXPLORER;

		char buffer[1024] = {0};
		file_name.copy(buffer, sizeof(buffer) - 1);
		ofn.lpstrFile = buffer;
		ofn.nMaxFile = sizeof(buffer);

		BOOL ret = GetSaveFileName ( &ofn );
		if ( ret ) return ofn.lpstrFile;
		else return "";
	}


	int Master::messageDialog ( xgui::Window * parent, const std::string & title, const std::string & text, const std::string & d_type, const std::string & d_buttons )
	{
		// Use proper locale-aware conversion instead of deprecated tolower
		std::string type = d_type;
		std::transform ( type.begin(), type.end(), type.begin(),
			[](unsigned char c){ return static_cast<char>(std::tolower(c)); } );

		std::string buttons = d_buttons;
		std::transform ( buttons.begin(), buttons.end(), buttons.begin(),
			[](unsigned char c){ return static_cast<char>(std::tolower(c)); } );

		UINT t = 0;
		if ( type.find ( "warn" ) == 0 ) t |= MB_ICONWARNING;
		else if ( type.find ( "question" ) == 0 ) t |= MB_ICONQUESTION;
		else if ( type.find ( "err" ) == 0 ) t |= MB_ICONERROR;
		else if ( type.find ( "info" ) == 0 ) t |= MB_ICONINFORMATION;

		if ( buttons == "ok-cancel" ) t |= MB_OKCANCEL;
		else if ( buttons == "yes-no" ) t |= MB_YESNO;
		else if ( buttons == "yes-no-cancel" ) t |= MB_YESNOCANCEL;
		else t |= MB_OK;

		int retval = MessageBox ( parent->getImpl()->widget, text.c_str(), title.c_str(), t );

		if ( (retval == IDABORT) || (retval == IDCANCEL) || (retval == IDIGNORE) ) return 0;
		else if ( (retval == IDOK) || (retval == IDYES) || (retval == IDCONTINUE) ) return 1;
		else if ( (retval == IDNO) ) return 2;
		else return -1;
	}
}

#ifdef _USRDLL
BOOL WINAPI ::DllMain ( HINSTANCE hInstance, DWORD dwReason, LPVOID )
{
	if ( dwReason == DLL_PROCESS_ATTACH ) {
		DisableThreadLibraryCalls ( (HMODULE)hInstance );
	}

	return TRUE;
}
#endif

