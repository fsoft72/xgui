//
// C++ Implementation: image_impl
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

static Gdiplus::Bitmap * LoadBmp ( std::string const &filename )
{
	int fname_len = (int)filename.size();
	WCHAR * path = new WCHAR[fname_len+1];

	MultiByteToWideChar(CP_ACP, 0, filename.c_str(), fname_len + 1, path,
		fname_len * sizeof(WCHAR) + sizeof(WCHAR) );

	Gdiplus::Bitmap * bmp = new Gdiplus::Bitmap(path);

	delete [] path;

	Gdiplus::Status ret = bmp->GetLastStatus();
	if ( ret != Gdiplus::Ok ) {
		delete bmp;
		bmp = 0;
	}

	return bmp;
}


namespace xguimpl
{
	Image::Image(std::string const &path) :
		this_image(0), image(0)
	{
		image = LoadBmp ( path );
	}

	Image::~Image() {
		if (image)
			delete image;
	}

	void Image::setAccessor(xgui::Image * accessor) 
	{ 
		this_image = accessor; 

		if (image) {
			this_image->width_ = (int)image->GetWidth();
			this_image->height_ = (int)image->GetHeight();
		}
	}

	bool Image::isValid() { return image != NULL; }

	Gdiplus::Bitmap * Image::getImage()
	{
		return image;
	}


	HBITMAP Image::getBitmap()
	{
		if (!image) return NULL;
		HBITMAP rv;
		image->GetHBITMAP(Gdiplus::Color(), &rv);
		return rv;
	}

	HBITMAP Image::getButtonBitmap()
	{
		if (!image) return NULL;

		int width = (int)image->GetWidth();
		int height = (int)image->GetHeight();

		Gdiplus::Bitmap * bmp = image->Clone(0, 0, width, height, PixelFormatDontCare);

		Gdiplus::Color color;
		Gdiplus::Color bgcol;
		bgcol.SetFromCOLORREF(GetSysColor(COLOR_BTNFACE));

		for (int x = 0; x < width; ++x) {
			for (int y = 0; y < height; ++y) {
				bmp->GetPixel(x, y, &color);
				if (color.GetAlpha() != 0xFF) {
					BYTE alpha = color.GetAlpha();
					BYTE inverse_alpha = 0xFF - alpha;

					BYTE red_result = ((color.GetRed() * alpha) + (bgcol.GetRed() * inverse_alpha)) >> 8;
					BYTE green_result = ((color.GetGreen() * alpha) + (bgcol.GetGreen() * inverse_alpha)) >> 8;
					BYTE blue_result = ((color.GetBlue() * alpha) + (bgcol.GetBlue() * inverse_alpha)) >> 8;

					bmp->SetPixel(x, y, Gdiplus::Color(0xFF, red_result, green_result, blue_result));
				}
			}
		}

		HBITMAP rv;
		bmp->GetHBITMAP ( Gdiplus::Color(), &rv );
		delete bmp;

		return rv;
	}

	HICON Image::getIcon()
	{
		if (!image) return NULL;
		HICON rv;
		image->GetHICON(&rv);
		return rv;
	}
}
