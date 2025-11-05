//
// C++ Interface: style_impl
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <windows.h>
#include <map>

struct _IMAGELIST;

namespace xgui { class Image; }

namespace xguimpl
{
	struct StyleManager
	{
		std::map<std::string, Style *> * styles_;
		std::map<xgui::Image *, int> img_map_;
		_IMAGELIST * hImgListSmall_;
		_IMAGELIST * hImgListBig_;

		DLLEXPORT StyleManager(std::map<std::string, Style *> * styles);
		DLLEXPORT ~StyleManager();

		DLLEXPORT int getIconIndex ( const std::string & style_name );
		DLLEXPORT _IMAGELIST * getImageListSmall();
		DLLEXPORT _IMAGELIST * getImageListBig();
	};
}