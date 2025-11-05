//
// C++ Interface: image_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_IMAGE_IMPL_H_
#define _XGUI_IMAGE_IMPL_H_

#include <windows.h>
#include <gdiplus.h>

namespace xgui { class Image; }

namespace xguimpl
{
	struct Image
	{
		xgui::Image * this_image;
		Gdiplus::Bitmap * image;

		DLLEXPORT Image(std::string const &path);
		DLLEXPORT virtual ~Image();

		DLLEXPORT Gdiplus::Bitmap		* getImage();
		DLLEXPORT HBITMAP				getBitmap();
		DLLEXPORT HICON				getIcon();
		DLLEXPORT HBITMAP             getButtonBitmap();

		DLLEXPORT void setAccessor(xgui::Image * accessor);
		DLLEXPORT bool isValid();

	};
}

#endif
