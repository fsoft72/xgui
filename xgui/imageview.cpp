//
// C++ Implementation: imageview
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

#ifndef WIN32
#	include "imageview.h"
#	ifdef OPENSTEP
#		include "openstep/imageview_impl.h"
#	else
#		include "gtk/imageview_impl.h"
#	endif
#	include "master.h"
#	include "image.h"
#	include "container.h"
#	include "dynamic.h"
#endif

static void ImageView_set(xgui::Object * o, std::string const &name, std::string const &val)
{
	xgui::ImageView * self = dynamic_cast<xgui::ImageView*>(o);
	xguimpl::ImageView * impl = dynamic_cast<xguimpl::ImageView*>(self->getImpl());

	if ( name == "image-path" )
		self->setImage ( xgui::Master::LoadImage(val) );
	else
		impl->set_imageview_prop(name, val);
}

static void ImageView_get(xgui::Object * o, std::string const &name, std::string &dest)
{
	xgui::ImageView * self = dynamic_cast<xgui::ImageView*>(o);
	xguimpl::ImageView * impl = dynamic_cast<xguimpl::ImageView*>(self->getImpl());

	if ( name == "image-path" ) {
		xgui::Image * img = self->getImage();
		dest = img ? img->getPath() : "";
	}
	else if ( name == "view-size" ) {
		dest = xgui::semantic_cast<std::string>(self->getViewSize());
	}
	else if ( name == "scrolled" ) {
		dest = self->isScrolled() ? "1" : "0";
	}
	else
		impl->get_imageview_prop(name, dest);
}

namespace xgui
{
	xgui::ImageViewClass imgview_class_info;

	ImageViewClass::ImageViewClass() : WidgetClass() 
	{
		registerProperty("image-path", ImageView_set, ImageView_get, true);
		registerProperty("alignment", ImageView_set, ImageView_get, true);
		registerProperty("view-size", ImageView_set, ImageView_get, true);
		registerProperty("scrolled", ImageView_set, ImageView_get, true);

		registerInitProperty("image-path", true);

		registerMethod("getImage", new DMethod0< Ptr< Type<xgui::Image> >, xgui::ImageView >(&xgui::ImageView::getImage));
		registerMethod("setImage", new DMethod1< Type<int>, xgui::ImageView, Ptr< Type<xgui::Image> > >(&xgui::ImageView::setImage));

		registerMethod("getViewSize", new DMethod0< Type< std::pair<int, int> >, xgui::ImageView >(&xgui::ImageView::getViewSize));
		registerMethod("isScrolled", new DMethod0< Type<bool>, xgui::ImageView >(&xgui::ImageView::isScrolled));
	}

	std::string const &ImageViewClass::className() { static std::string const c_name = "xgui::ImageView"; return c_name; }

	bool ImageViewClass::isInstanceable() { return true; }

	xgui::Object * ImageViewClass::create(xgui::Object * parent, xgui::StringMap &properties)
	{
		xgui::StringMap::iterator prop_i;
		xgui::Container * wparent = dynamic_cast<xgui::Container*>( parent );
		xgui::ImageView * w = 0;

		if (!parent) {
			XGUI_ERROR("Cannot create a ImageView without parent");
			return 0;
		}

		if (!wparent) {
			XGUI_ERROR("Cannot create a ImageView as a child of a " << parent->className());
			return 0;
		}

		xgui::Image * image = 0;

		if ( (prop_i = properties.find("image-path")) != properties.end()) {
			image = xgui::Master::LoadImage(prop_i->second);
			properties.erase(prop_i);
		}

		w = xgui::Master::CreateImageView(wparent, image);

		return w;
	}
}

namespace xgui
{
	void ImageView::init()
	{
		imageview_impl_ = dynamic_cast<xguimpl::ImageView *>(impl_);
		ASSERT(imageview_impl_);
	}

	ImageView::ImageView ( xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::ImageView * widget )
	: Widget ( parent, cinfo, widget ),
	image_(0), view_width_(0), view_height_(0), scrolled_(false)
	{
		init();
	}

	ImageView::ImageView ( xgui::Container * parent, xgui::Image * img )
	: Widget ( parent, &imgview_class_info, new xguimpl::ImageView ( parent ) ),
	image_(0), view_width_(0), view_height_(0), scrolled_(false)
	{
		impl_->setAccessor(this);
		post_construction();
		init();

		setImage ( img );
	}

	ImageView::~ImageView() 
	{
		if (image_) image_->unref();
	}

	xgui::Image * ImageView::getImage()
	{
		return image_;
	}

	int ImageView::setImage ( xgui::Image * img )
	{
		if (img) 
			img->ref();

		if (image_) {
			image_->unref();
			image_ = 0;
		}
	
		if (img) {
			view_width_ = img->getWidth();
			view_height_ = img->getHeight();
			imageview_impl_->setImage ( img );
			image_ = img;
		}

		return (img != 0);
	}

	std::pair<int, int> ImageView::getViewSize() { return std::make_pair(view_width_, view_height_); }
	bool ImageView::isScrolled() { return scrolled_; }
}
