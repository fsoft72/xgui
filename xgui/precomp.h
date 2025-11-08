#ifndef _XGUI_PRECOMP_H_
#define _XGUI_PRECOMP_H_

#ifdef WIN32

#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <stack>
#include <vector>
#include <memory>
#include <locale>

#include "xgui.h"

// Target Windows 7 and later (0x0601) for modern API support
#define _WIN32_WINNT	0x0601
#define NTDDI_VERSION	0x06010000
#include <windows.h>
#include <commctrl.h>
#include <gdiplus.h>

#include "win32/idmap.h"

#include "win32/master_impl.h"
#include "win32/widget_impl.h"
#include "win32/window_impl.h"
#include "win32/image_impl.h"
#include "win32/button_impl.h"
#include "win32/menu_impl.h"
#include "win32/vbox_impl.h"
#include "win32/hbox_impl.h"
#include "win32/entry_impl.h"
#include "win32/label_impl.h"
#include "win32/view_impl.h"
#include "win32/style_impl.h"
#include "win32/list_impl.h"
#include "win32/slider_impl.h"
#include "win32/calendar_impl.h"
#include "win32/imageview_impl.h"
#include "win32/tab_impl.h"
#include "win32/progressbar_impl.h"
#include "win32/checkbox_impl.h"
#include "win32/spin_impl.h"
#include "win32/frame_impl.h"
#include "win32/combobox_impl.h"
#include "win32/space_impl.h"
#include "win32/toolbar_impl.h"
#include "win32/tree_impl.h"

#define MASTER_INSTANCE		(xgui::Master::Instance()->getImpl())
#define MAIN_HINSTANCE		(MASTER_INSTANCE->hInstance)

#undef LoadImage
#undef CreateWindow
#undef RegisterClass
#undef UnregisterClass
#undef LoadLibrary

#endif

#endif
