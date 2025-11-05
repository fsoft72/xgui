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
		ImageClass();
		std::string const &className();
		bool isInstanceable();
		xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);
	};

	class Image : public xgui::Object
	{
		protected:
			Image(std::string const &path);
			virtual ~Image();

		public:
			int ref();
			int unref();
			std::string const &getPath();

			int getWidth();
			int getHeight();

			xguimpl::Image * getImpl() { return impl_; }
			bool isValid();
	};
}

#endif
