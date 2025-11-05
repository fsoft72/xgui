//
// C++ Interface: master_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef _XGUI_MASTER_IMPL_H_
#define _XGUI_MASTER_IMPL_H_

#include <windows.h>
#include <gdiplus.h>

#define WM_SUBMIT                               WM_APP + 100
#define WM_REDRAW                               WM_APP + 101
#define WM_IDLE_CALLBACK                        WM_APP + 102

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved);

namespace xguimpl
{
	struct Widget;

	typedef std::map<HWND, xguimpl::Widget *> WidgetsHash;

	struct Master
	{
		xgui::Master * master_;

		HINSTANCE hInstance;
		HWND	  tip_wnd;

		DWORD gui_thread_id;
		WidgetsHash widgets;
		xgui::Widget * mouse_down_widget;

		ULONG_PTR gdiplusToken;

		DLLEXPORT Master ( xgui::Master * master );
		DLLEXPORT ~Master();

		DLLEXPORT int run();
		DLLEXPORT bool quit();
		DLLEXPORT void registerIdleCallback ( xgui::Callback * cbk, xgui::Widget * wid );
		
		DLLEXPORT void registerWidget ( xguimpl::Widget * w );
		DLLEXPORT void unregisterWidget ( xguimpl:: Widget * w );
		DLLEXPORT xgui::Widget * findWidget ( HWND hWnd );

		DLLEXPORT void destroyWidget ( xgui::Widget * w );

		DLLEXPORT static BOOL CALLBACK dlgProc ( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		DLLEXPORT static LRESULT CALLBACK subProc ( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		DLLEXPORT static LRESULT CALLBACK toolbarProc ( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

		DLLEXPORT xgui::StrVector openFileDialog(xgui::Window * parent, xgui::FileExtensionsVector const &filter, std::string const &title, std::string const &path, bool multiselection);
		DLLEXPORT std::string saveFileDialog(xgui::Window * parent, xgui::FileExtensionsVector const &filter, std::string const &title, std::string const &path, std::string const &file_name);
		DLLEXPORT int messageDialog ( xgui::Window * parent, const std::string & title, const std::string & text, const std::string & d_type, const std::string & d_buttons );

		DLLEXPORT void redrawTopLevels();

		DLLEXPORT void initThreads();
		DLLEXPORT void quitThreads();

		DLLEXPORT void guiLock();
		DLLEXPORT void guiUnlock();

	};

}

#endif
