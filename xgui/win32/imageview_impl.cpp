//
// C++ Implementation: imageview_impl
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
	ImageView::ImageView ( HWND real_w ) : Widget ( real_w ),
		h_align('c'), v_align('c'), start_x(0), start_y(0)
	{

	}

	ImageView::ImageView ( xgui::Container * parent ) :
		Widget ( MkImageView ( parent ) ),
		h_align('c'), v_align('c'), start_x(0), start_y(0)
	{
		ShowScrollBar ( widget, SB_BOTH, FALSE );

		min_x = 0;
		min_y = 0;
	}


	ImageView::~ImageView()
	{
	}


	HWND ImageView::MkImageView ( xgui::Container * parent )
	{
		return CreateWindowEx( WS_EX_NOPARENTNOTIFY, "XGUIImageView", "",
			WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | WS_VSCROLL | WS_HSCROLL,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, parent->getImpl()->widget,
			NULL, MAIN_HINSTANCE, NULL );
	}


	bool ImageView::set_imageview_prop ( std::string const &name, std::string const &vals )
	{
		if ( name == "view-size" ) {

			std::pair<int, int> size = xgui::semantic_cast< std::pair<int, int> >(vals);
			min_x = this_imageview->view_width_ = size.first;
			min_y = this_imageview->view_height_ = size.second;
			this_imageview->getContext()->redraw();
		
		} else if ( name == "alignment" ) {

			if ( vals == "top" ) {
				v_align = 't';
				h_align = 'c';
			} else if ( vals == "top-left" ) {
				v_align = 't';
				h_align = 'l';
			} else if ( vals == "top-right" ) {
				v_align = 't';
				h_align = 'r';
			} else if ( vals == "left" ) {
				v_align = 'c';
				h_align = 'l';
			} else if ( vals == "right" ) {
				v_align = 'c';
				h_align = 'r';
			} else if ( vals == "bottom" ) {
				v_align = 'b';
				h_align = 'c';
			} else if ( vals == "bottom-left" ) {
				v_align = 'b';
				h_align = 'l';
			} else if ( vals == "bottom-right" ) {
				v_align = 'b';
				h_align = 'r';
			} else {
				v_align = 'c';
				h_align = 'c';
			}

			InvalidateRect ( widget, NULL, TRUE );
			UpdateWindow ( widget );

		} else if ( name == "scrolled" ) {

			this_imageview->scrolled_ = (vals == "1");
			if ( this_imageview->scrolled_ && this_imageview->image_ ) {
				SCROLLINFO info;
				info.cbSize = sizeof(info);
				info.fMask = SIF_RANGE | SIF_POS | SIF_PAGE;
				info.nPos = 0;
				info.nMin = 0;
				info.nMax = this_imageview->image_->getWidth();
				info.nPage = cur_x;
				SetScrollInfo ( widget, SB_HORZ, &info, FALSE );
				info.nMax = this_imageview->image_->getHeight();
				info.nPage = cur_y;
				SetScrollInfo ( widget, SB_VERT, &info, FALSE );

				ShowScrollBar ( widget, SB_BOTH, TRUE );

				start_x = start_y = 0;
			}

		} else
			return false;

		return true;
	}


	bool ImageView::get_imageview_prop ( std::string const &name, std::string &vals )
	{
		if ( name == "alignment" ) {

			vals = "";
			if ( v_align == 't' ) vals = "top";
			else if ( v_align == 'b' ) vals = "bottom";

			if ( h_align == 'l' ) vals += "-left";
			else if ( h_align == 'r' ) vals += "-right";
			
			if ( vals[0] == '-' ) vals.erase ( 0, 1 );
			else if ( vals.empty() ) vals = "center";

		} else
			return false;
		
		return true;
	}


	void ImageView::setImage ( xgui::Image * img )
	{
		if (img) {

			HBITMAP hBmp = img->getImpl()->getBitmap();
			SendMessage ( widget, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBmp );
			min_x = img->getWidth();
			min_y = img->getHeight();

			if ( this_imageview->scrolled_ ) {
				SCROLLINFO info;
				info.cbSize = sizeof(info);
				info.fMask = SIF_RANGE | SIF_POS;
				info.nPos = 0;
				info.nMin = 0;
				info.nMax = min_x;
				SetScrollInfo ( widget, SB_HORZ, &info, FALSE );
				info.nMax = min_y;
				SetScrollInfo ( widget, SB_VERT, &info, FALSE );

				start_x = start_y = 0;
			}

			this_imageview->getContext()->redraw();

		} else {
			
			SendMessage ( widget, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)NULL );
			InvalidateRect ( widget, NULL, TRUE );
			UpdateWindow ( widget );

		}
	}


	LRESULT CALLBACK ImageView::wndProc ( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		xgui::Widget * w = MASTER_INSTANCE->findWidget ( hWnd );
		if ( !w ) return DefWindowProc ( hWnd, msg, wParam, lParam );

		xgui::ImageView * imvc = dynamic_cast<xgui::ImageView *>(w);
		if ( !imvc ) return DefWindowProc ( hWnd, msg, wParam, lParam );

		ImageView * imv = dynamic_cast<ImageView *>(imvc->getImpl());

		switch ( msg ) {
		case WM_PAINT:
			imv->OnPaint ( wParam, lParam );
			break;

		case WM_SIZE:
			imv->OnResize ( wParam, lParam );
			break;

		case WM_HSCROLL:
			imv->OnHScroll ( wParam, lParam );
			break;

		case WM_VSCROLL:
			imv->OnVScroll ( wParam, lParam );
			break;

		case WM_MOUSEWHEEL:
			{
				int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
				int updown = (zDelta > 0) ? SB_LINEUP : SB_LINEDOWN;
				int max = abs(zDelta) / WHEEL_DELTA;
				for ( int i = 0; i < max; ++i )
					imv->OnVScroll ( MAKEWPARAM(updown, 0), 0 );
			}
			break;
		}

		return DefWindowProc ( hWnd, msg, wParam, lParam );
	}


	void ImageView::OnPaint ( WPARAM wParam, LPARAM lParam )
	{
		wParam; lParam;

		PAINTSTRUCT pt;
		HDC hPaintDC = BeginPaint ( widget, &pt );

		if ( this_imageview->image_ ) {

			int x = 0, y = 0;

			int width = this_imageview->image_->getWidth();
			int height = this_imageview->image_->getHeight();
			int vw = this_imageview->view_width_;
			int vh = this_imageview->view_height_;

			if ( this_imageview->scrolled_ ) {
				vw = width;
				vh = height;
			}

			switch ( v_align ) {
				case 't': y = 0; break;
				case 'c': y = (cur_y - vh) / 2; break;
				case 'b': y = cur_y - vh; break;
			}

			switch ( h_align ) {
				case 'l': x = 0; break;
				case 'c': x = (cur_x - vw) / 2; break;
				case 'r': x = cur_x - vw; break;
			}

			HDC hdc = CreateCompatibleDC ( hPaintDC );
			HBITMAP hBmp = this_imageview->image_->getImpl()->getBitmap();
			DeleteObject ( SelectObject ( hdc, hBmp ) );

			HDC hMemDC = CreateCompatibleDC ( hPaintDC );
			HBITMAP hMemBmp = CreateCompatibleBitmap ( hPaintDC, cur_x, cur_y );
			DeleteObject ( SelectObject ( hMemDC, hMemBmp ) );

			FillRect ( hMemDC, &pt.rcPaint, (HBRUSH) (COLOR_BTNFACE + 1) );

			if ( this_imageview->scrolled_ ) {
				if ( cur_x < width ) x = 0;
				if ( cur_y < height ) y = 0;

				vw = width - start_x;
				vh = height - start_y;
			}

			BLENDFUNCTION bf;
			bf.BlendOp = AC_SRC_OVER;
			bf.BlendFlags = 0;
			bf.SourceConstantAlpha = 255;
			bf.AlphaFormat = AC_SRC_ALPHA;
			AlphaBlend (
				hMemDC, x, y, vw, vh,
				hdc, start_x, start_y, width - start_x, height - start_y, bf );

			DeleteObject ( hBmp );
			DeleteDC ( hdc );

			BitBlt ( hPaintDC, 0, 0, cur_x, cur_y, hMemDC, 0, 0, SRCCOPY );

			DeleteObject ( hMemBmp );
			DeleteDC ( hMemDC );

		} else
			FillRect ( hPaintDC, &pt.rcPaint, (HBRUSH) (COLOR_BTNFACE + 1) );

		EndPaint ( widget, &pt );
	}


	void ImageView::OnHScroll ( WPARAM wParam, LPARAM lParam )
	{
		lParam;

		SCROLLINFO info;
		info.cbSize = sizeof(info);
		info.fMask = SIF_PAGE;
		GetScrollInfo ( widget, SB_HORZ, &info );

		int image_width = this_imageview->image_->getWidth();

		switch ( LOWORD(wParam) ) {
		case SB_LINERIGHT:
			start_x += 10;
			if ( start_x > image_width - (int)info.nPage ) start_x = image_width - info.nPage;
			break;

		case SB_LINELEFT:
			start_x -= 10;
			if ( start_x < 0 ) start_x = 0;
			break;

		case SB_PAGERIGHT:
			start_x += info.nPage;
			if ( start_x > image_width - (int)info.nPage ) start_x = image_width - info.nPage;
			break;

		case SB_PAGELEFT:
			start_x -= info.nPage;
			if ( start_x < 0 ) start_x = 0;
			break;

		case SB_LEFT:
			start_x = 0;
			break;

		case SB_RIGHT:
			start_x = image_width - info.nPage;
			break;

		case SB_THUMBTRACK:
			start_x = HIWORD(wParam);
			break;

		default:
			return;

		}

		info.fMask = SIF_POS;
		info.nPos = start_x;
		SetScrollInfo ( widget, SB_HORZ, &info, TRUE );

		InvalidateRect ( widget, NULL, TRUE );
		UpdateWindow ( widget );
	}


	void ImageView::OnVScroll ( WPARAM wParam, LPARAM lParam )
	{
		lParam;

		SCROLLINFO info;
		info.cbSize = sizeof(info);
		info.fMask = SIF_PAGE;
		GetScrollInfo ( widget, SB_VERT, &info );

		int image_height = this_imageview->image_->getHeight();

		switch ( LOWORD(wParam) ) {
		case SB_LINEDOWN:
			start_y += 10;
			if ( start_y > image_height - (int)info.nPage ) start_y = image_height - info.nPage;
			break;

		case SB_LINEUP:
			start_y -= 10;
			if ( start_y < 0 ) start_y = 0;
			break;

		case SB_PAGEDOWN:
			start_y += this_imageview->view_height_;
			if ( start_y > image_height - (int)info.nPage ) start_y = image_height - info.nPage;
			break;

		case SB_PAGEUP:
			start_y -= info.nPage;
			if ( start_y < 0 ) start_y = 0;
			break;

		case SB_TOP:
			start_y = 0;
			break;

		case SB_BOTTOM:
			start_y = image_height - info.nPage;
			break;

		case SB_THUMBTRACK:
			start_y = HIWORD(wParam);
			break;

		default:
			return;

		}

		info.fMask = SIF_POS;
		info.nPos = start_y;
		SetScrollInfo ( widget, SB_VERT, &info, TRUE );

		InvalidateRect ( widget, NULL, TRUE );
		UpdateWindow ( widget );
	}


	void ImageView::OnResize ( WPARAM wParam, LPARAM lParam )
	{
		wParam;

		if ( !this_imageview->scrolled_ ) return;

		int w = LOWORD(lParam);
		int h = HIWORD(lParam);

		SCROLLINFO info;
		info.cbSize = sizeof(info);
		info.fMask = SIF_PAGE;
		info.nPage = w;
		SetScrollInfo ( widget, SB_HORZ, &info, TRUE );
		info.nPage = h;
		SetScrollInfo ( widget, SB_VERT, &info, TRUE );
	}


	void ImageView::setAccessor ( xgui::Widget * w )
	{
		this_imageview = dynamic_cast<xgui::ImageView *>(w);
		ASSERT(this_imageview);

		Widget::setAccessor ( w );
	}
}
