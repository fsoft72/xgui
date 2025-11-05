//
// C++ Implementation: style_impl
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
	StyleManager::StyleManager(std::map<std::string, Style *> * styles)
	: styles_(styles), hImgListBig_(ImageList_Create(32, 32, ILC_COLOR32, 0, 1)), 
	  hImgListSmall_(ImageList_Create(16, 16, ILC_COLOR32, 0, 1))
	{
		unsigned char pixel_and = 0xFF;
		unsigned char pixel_xor = 0x00;
	
		HICON hIcon = CreateIcon ( MAIN_HINSTANCE, 1, 1, 1, 1, (BYTE*)&pixel_and, (BYTE*)&pixel_xor );
	
		ImageList_SetBkColor ( hImgListBig_, GetSysColor ( COLOR_WINDOW ) );
		ImageList_SetBkColor ( hImgListSmall_, GetSysColor ( COLOR_WINDOW ) );
	
		ImageList_AddIcon ( hImgListSmall_, hIcon );
		ImageList_AddIcon ( hImgListBig_, hIcon );
	
		DeleteObject ( hIcon );
	}
	
	StyleManager::~StyleManager() 
	{
		ImageList_Destroy(hImgListBig_);
		ImageList_Destroy(hImgListSmall_);
	}

	int StyleManager::getIconIndex ( const std::string & style_name )
	{
		std::map<std::string, Style *>::iterator si = styles_->find(style_name);
		if (si == styles_->end()) return -1;
	
		xgui::Image * img = si->second->getIcon();
	
		int retval = -1;
	
		std::map<xgui::Image *, int>::iterator ii = img_map_.find(img);
		if (ii == img_map_.end()) {
			HICON hIcon = img->getImpl()->getIcon();
			retval = ImageList_AddIcon(hImgListSmall_, hIcon);
			ImageList_AddIcon(hImgListBig_, hIcon);
			DeleteObject ( hIcon );
			img_map_.insert(std::make_pair(img, retval));
		} else
			retval = ii->second;
	
		return retval;
	}

	_IMAGELIST * StyleManager::getImageListSmall() { return hImgListSmall_; }
	_IMAGELIST * StyleManager::getImageListBig() { return hImgListBig_; }
}
