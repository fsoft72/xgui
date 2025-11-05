//
// C++ Interface: imageview
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_IMAGEVIEW_H_
#define _XGUI_IMAGEVIEW_H_

#include "xguifwd.h"
#include "widget.h"

namespace xguimpl { struct ImageView; }

namespace xgui
{
	class Image;

	struct ImageViewClass : public xgui::WidgetClass
	{
		DLLEXPORT ImageViewClass();
		DLLEXPORT std::string const &className();
		DLLEXPORT bool isInstanceable();
		DLLEXPORT xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);
	};

	class ImageView : public xgui::Widget
	{
		protected:
			xgui::Image * image_;
			int view_width_, view_height_;
			bool scrolled_;

			xguimpl::ImageView * imageview_impl_;

			DLLEXPORT ImageView ( xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::ImageView * widget );
			DLLEXPORT ImageView ( xgui::Container * parent, xgui::Image * img );
			DLLEXPORT void init();

			DLLEXPORT virtual ~ImageView();

		public:
			DLLEXPORT xgui::Image * getImage();
			DLLEXPORT int setImage ( xgui::Image * img );
			DLLEXPORT std::pair<int, int> getViewSize();
			DLLEXPORT bool isScrolled();

		friend class Master;
		friend struct xguimpl::ImageView;
	};
}

#endif
