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
	class Object;

	struct ClassInfo
	{
		protected:
			xgui::PropertiesMap properties_;
			xgui::MethodsMap mmap_;
			std::string tag_;
			std::map<std::string, bool> constr_props_;
			std::string dtd_;

		public:
			DLLEXPORT ClassInfo();
			DLLEXPORT virtual ~ClassInfo();

			virtual bool isInstanceable() = 0;
			virtual bool mustFinalize() = 0;
			virtual std::string const &className() = 0;
			DLLEXPORT std::string const &tag();
			DLLEXPORT virtual std::string const &dtd();

			virtual bool isContainer() = 0;
			virtual bool isWidget() = 0;
			virtual bool isModel() = 0;
			virtual bool isStyleManager() = 0;

			virtual bool canContain(xgui::ClassInfo * c) = 0;
			virtual bool canBeContainedIn(xgui::ClassInfo * c) = 0;

			DLLEXPORT void registerProperty(std::string const &prop, xgui::PropertySetter setter, xgui::PropertyGetter getter, bool dumpable);
			DLLEXPORT xgui::PropertySetter getPropertySetter(std::string const &prop);
			DLLEXPORT xgui::PropertyGetter getPropertyGetter(std::string const &prop);
			DLLEXPORT std::vector<std::string> properties();
			DLLEXPORT xgui::PropertyType hasProperty(std::string const &property);
			DLLEXPORT bool isPropertyDumpable(std::string const &property);

			DLLEXPORT const std::map<std::string, bool> & initProperties();
			DLLEXPORT void registerInitProperty ( const std::string & name, bool required );

			DLLEXPORT void registerMethod(std::string const &name, DMethod * m);
			DLLEXPORT DMethod * findMethod(std::string const &name);
			DLLEXPORT std::vector<std::string> methods();

			virtual xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties) = 0;
			virtual void finalize(xgui::Object * obj) = 0;
	};
}

#endif
