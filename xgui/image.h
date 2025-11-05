//
// C++ Interface: image
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_IMAGE_H_
#define _XGUI_IMAGE_H_

#include "base.h"
#include "object.h"

namespace xguimpl { struct Image; }

namespace xgui
{
	struct ImageClass : public xgui::ObjectClass
	{
		DLLEXPORT ImageClass();
		DLLEXPORT std::string const &className();
		DLLEXPORT bool isInstanceable();
		DLLEXPORT xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);
	};

	class Image : public xgui::Object
	{
		protected:
			xguimpl::Image * impl_;
			int width_, height_;
			int references_;
			std::string path_;

			DLLEXPORT Image(std::string const &path);
			DLLEXPORT virtual ~Image();

		public:
			DLLEXPORT int ref();
			DLLEXPORT int unref();
			DLLEXPORT std::string const &getPath();

			DLLEXPORT int getWidth();
			DLLEXPORT int getHeight();

			DLLEXPORT xguimpl::Image * getImpl() { return impl_; }
			DLLEXPORT bool isValid();

			virtual std::string dump();

		friend class Master;
		friend struct xguimpl::Image;
	};
}

#endif
