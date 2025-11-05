//
// C++ Interface: class_info
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_CLASS_INFO_H_
#define _XGUI_CLASS_INFO_H_

#include "base.h"

namespace xgui
{
	struct ClassInfo
	{
		public:
			ClassInfo();
			virtual ~ClassInfo();

			virtual bool isInstanceable() = 0;
			virtual bool mustFinalize() = 0;
			virtual std::string const &className() = 0;
			std::string const &tag();
			virtual std::string const &dtd();

			virtual bool isContainer() = 0;
			virtual bool isWidget() = 0;
			virtual bool isModel() = 0;
			virtual bool isStyleManager() = 0;

			virtual bool canContain(xgui::ClassInfo * c) = 0;
			virtual bool canBeContainedIn(xgui::ClassInfo * c) = 0;

			void registerProperty(std::string const &prop, xgui::PropertySetter setter, xgui::PropertyGetter getter, bool dumpable);
			xgui::PropertySetter getPropertySetter(std::string const &prop);
			xgui::PropertyGetter getPropertyGetter(std::string const &prop);
			std::vector<std::string> properties();
			xgui::PropertyType hasProperty(std::string const &property);

			const std::map<std::string, bool> & initProperties();
			void registerInitProperty ( const std::string & name, bool required );

			void registerMethod(std::string const &name, xgui::DMethod * m);
			xgui::DMethod * findMethod(std::string const &name);

			virtual xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties) = 0;
			virtual void finalize(xgui::Object * obj) = 0;
	};
}

#endif
