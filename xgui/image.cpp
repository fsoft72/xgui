//
// C++ Implementation: image
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
#	include "image.h"
#	ifdef OPENSTEP
#		include "openstep/image_impl.h"
#	else
#		include "gtk/image_impl.h"
#	endif
#	include "master.h"
#	include "dynamic.h"
#endif

namespace xgui
{
	xgui::ImageClass img_class_info;

	ImageClass::ImageClass() : ObjectClass()
	{
		registerInitProperty("path", true);

		registerMethod("ref", new DMethod0< Type<int>, xgui::Image >(&xgui::Image::ref));
		registerMethod("unref", new DMethod0< Type<int>, xgui::Image >(&xgui::Image::unref));
		registerMethod("getPath", new DMethod0< Ref< Const< Type<std::string> > >, xgui::Image >(&xgui::Image::getPath));

		registerMethod("getWidth", new DMethod0< Type<int>, xgui::Image >(&xgui::Image::getWidth));
		registerMethod("getHeight", new DMethod0< Type<int>, xgui::Image >(&xgui::Image::getHeight));

		registerMethod("isValid", new DMethod0< Type<bool>, xgui::Image >(&xgui::Image::isValid));
	}

	std::string const &ImageClass::className() { static std::string const c_name = "xgui::Image"; return c_name; }

	bool ImageClass::isInstanceable() { return true; }

	xgui::Object * ImageClass::create(xgui::Object * parent, xgui::StringMap &properties)
	{
		xgui::StringMap::iterator prop_i;
		xgui::Image * w = 0;

		std::string path = "";

		if ( (prop_i = properties.find("path")) != properties.end()) {
			path = prop_i->second;
			properties.erase(prop_i);
		}

		w = xgui::Master::LoadImage(path);

		return w;
	}
}

namespace xgui
{
	Image::Image(std::string const &path)
	: Object(&img_class_info), impl_(new xguimpl::Image(path)), width_(0), height_(0), references_(0), path_(path)
	{
		impl_->setAccessor(this);
	}

	Image::~Image()	
	{ 
		xgui::ImagesMap &images_cache = xgui::Master::Instance()->images_cache_;

		xgui::ImagesMap::iterator i = images_cache.find(path_);
		if (i != images_cache.end())
			images_cache.erase(i);

		delete impl_; 
	}

	int Image::ref() { return ++references_; }
	int Image::unref()
	{
		int rc = --references_;
	
		if (rc <= 0) 
			delete this;
	
		return rc;
	}

	std::string const &Image::getPath() { return path_; }

	int Image::getWidth() { return width_; }
	int Image::getHeight() { return height_; }
	bool Image::isValid() { return impl_->isValid(); }

	std::string Image::dump()
	{
		std::string xml = "<";
		xml+=cinfo_->tag();

		xml+=dumpProperties();

		xml+="path=\"";
		xml+=getPath();
		xml+="\"";

		xml+="/>\n";

		return xml;
	}
}
