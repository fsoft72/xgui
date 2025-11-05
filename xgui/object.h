//
// C++ Interface: object
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_OBJECT_H_
#define _XGUI_OBJECT_H_

#include "class_info.h"

namespace xgui
{
	struct StoredObject;

	struct ObjectClass : public xgui::ClassInfo
	{
		DLLEXPORT ObjectClass();
		DLLEXPORT xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);
		DLLEXPORT void finalize(xgui::Object *);

		DLLEXPORT virtual bool canContain(xgui::ClassInfo * c);
		DLLEXPORT virtual bool canBeContainedIn(xgui::ClassInfo * c);
	
		DLLEXPORT bool isInstanceable();
		DLLEXPORT bool mustFinalize();
		DLLEXPORT virtual std::string const &className();
		DLLEXPORT bool isContainer();
		DLLEXPORT bool isWidget();
		DLLEXPORT bool isModel();
		DLLEXPORT bool isStyleManager();
	};

	class Object
	{
		protected:
			typedef std::map<std::string, StoredObject *> ObjectsMap;

			ClassInfo * cinfo_;
			
			std::string id_;

			ObjectsMap stored_objects_;
			VoidSet owners_;
			void (*owner_destruction_notifier_)(void *, xgui::Object*);

			xgui::PropertiesMap properties_;
			xgui::MethodsMap mmap_;

			Object(xgui::ClassInfo * cinfo);
			DLLEXPORT virtual ~Object();

			DLLEXPORT virtual std::string dumpProperties();

		public:
			//Dynamic type management
			DLLEXPORT std::string const &className();
			DLLEXPORT xgui::ClassInfo *getClass();

			DLLEXPORT bool isIdentifiedAs(std::string const &id);
			DLLEXPORT std::string const &id();
			DLLEXPORT void setId(std::string const &id);
			
			//Storable data management
			DLLEXPORT void store(std::string const &name, xgui::Object * o, bool destroy = false);
			DLLEXPORT xgui::Object * retrieve(std::string const &name);
			DLLEXPORT void unstore(std::string const &name);

			//Properties management
			DLLEXPORT bool set(std::string const &name, std::string const &val);
			DLLEXPORT bool get(std::string const &name, std::string &dest);
			DLLEXPORT void registerProperty(std::string const &prop, xgui::PropertySetter setter, xgui::PropertyGetter getter, bool dumpable);
			DLLEXPORT std::vector<std::string> properties();
			DLLEXPORT xgui::PropertyType hasProperty(std::string const &property);
			DLLEXPORT bool isPropertyDumpable(std::string const &property);

			//Ownership management for external bindings
			DLLEXPORT void addOwner(void *o);
			DLLEXPORT void delOwner(void *o);
			DLLEXPORT void setOwnerDestructionNotifier(void (*notifier)(void *, xgui::Object *));

			//Dynamic Objects
			DLLEXPORT virtual std::string dump();

			DLLEXPORT static xgui::Object * Unpack(std::string const &obj);
			DLLEXPORT std::string pack();
			
			DLLEXPORT std::vector<std::string> methods();
			DLLEXPORT DMethod * findMethod(std::string const &name);
			DLLEXPORT void registerMethod(std::string const &name, DMethod * m);
			DLLEXPORT std::string call(std::string const &method);
			DLLEXPORT std::string call(std::string const &method, std::string const &p1);
			DLLEXPORT std::string call(std::string const &method, std::string const &p1, std::string const &p2);
			DLLEXPORT std::string call(std::string const &method, std::string const &p1, std::string const &p2, std::string const &p3);
			DLLEXPORT std::string call(std::string const &method, std::string const &p1, std::string const &p2, std::string const &p3, std::string const &p4);
			DLLEXPORT std::string call(std::string const &method, std::string const &p1, std::string const &p2, std::string const &p3, std::string const &p4, std::string const &p5);


		friend class Master;
	};
}

#endif
