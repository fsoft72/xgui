//
// C++ Implementation: object
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
#	include "object.h"
#	include "widget.h"
#	include "model.h"
#	include "style.h"
#	include "master.h"
#	include "dynamic.h"
#endif

static void obj_prop_setter(xgui::Object * o, std::string const &prop, std::string const &val)
{
	if ( prop == "id" )
		o->setId(val);
}

static void obj_prop_getter(xgui::Object * o, std::string const &prop, std::string &val)
{
	if ( prop == "id" )
		val = o->id();
}

namespace xgui
{
	xgui::ObjectClass object_class_info;

	ObjectClass::ObjectClass() 
	: ClassInfo()
	{
		registerProperty("id", obj_prop_setter, obj_prop_getter, true);
	}

	xgui::Object * ObjectClass::create(xgui::Object * parent, xgui::StringMap &properties)
	{
		return 0;
	}

	void ObjectClass::finalize(xgui::Object * o) {}

	bool ObjectClass::isInstanceable() { return false; }
	bool ObjectClass::mustFinalize() { return false; }
	std::string const &ObjectClass::className() { static std::string const c_name = "xgui::Object"; return c_name; }
	bool ObjectClass::isContainer() { return false; }
	bool ObjectClass::isWidget() { return false; }
	bool ObjectClass::isModel() { return false; }
	bool ObjectClass::isStyleManager() { return false; }

	bool ObjectClass::canContain(xgui::ClassInfo * c) { return false; }
	bool ObjectClass::canBeContainedIn(xgui::ClassInfo * c) { return false; }
}

namespace xgui
{
	struct StoredObject 
	{
		xgui::Object * object;
		bool should_destroy;

		StoredObject() : object(0), should_destroy(false) {}
		StoredObject(xgui::Object * o, bool destroy) : object(o), should_destroy(destroy) {}
		~StoredObject() { if (should_destroy) Master::DestroyObject(object); }
	};
}

namespace xgui
{
	Object::Object(xgui::ClassInfo * cinfo)
	: cinfo_( cinfo ? cinfo : &object_class_info ), owner_destruction_notifier_(0) 
	{
		id_ = xgui::semantic_cast<std::string>(this);
	
		DMESSAGE("Creazione " << cinfo_->className() << " : " << this << " -> " << id_);
	}
	
	Object::~Object()
	{
		DMESSAGE("Distruzione " << cinfo_->className() << " : " << this << " -> " << id_);
	
		for(ObjectsMap::iterator obj = stored_objects_.begin(); obj != stored_objects_.end(); ++obj)
			delete obj->second;
	
		while(!owners_.empty()) {
			VoidSet::iterator owner_i = owners_.begin();
			void * owner = *owner_i;
			owners_.erase(owner_i);
			
			DMESSAGE("Notifico Owner " << owner << " della distruzione di un " << cinfo_->className());
			ASSERT(owner);
	
			if (owner_destruction_notifier_)
				owner_destruction_notifier_(owner, this);
		}

		DMESSAGE("Deallocazione metodi dinamici di " << this);
		for(MethodsMap::iterator i = mmap_.begin(); i != mmap_.end(); ++i) 
			delete i->second;
	}
	
	void Object::registerProperty(std::string const &prop, xgui::PropertySetter setter, xgui::PropertyGetter getter, bool dumpable)
	{
		properties_[prop] = PropertyInfo(setter, getter, dumpable);
	}

	std::vector<std::string> Object::properties()
	{
		std::vector<std::string> props = cinfo_->properties();

		for(PropertiesMap::iterator i = properties_.begin(); i != properties_.end(); ++i)
			props.push_back(i->first);

		return props;
	}

	xgui::PropertyType Object::hasProperty(std::string const &property)
	{
		PropertyType p_type;

		PropertiesMap::iterator i = properties_.find(property);
		
		if (i == properties_.end()) 
			p_type = xgui::PROPERTY_NOT_FOUND;
		else if (i->second.setter == 0)
			p_type = xgui::PROPERTY_READ_ONLY;
		else
			p_type = xgui::PROPERTY_READ_WRITE;

		if (p_type == xgui::PROPERTY_NOT_FOUND)
			p_type = cinfo_->hasProperty(property);

		return p_type;
	}

	bool Object::isPropertyDumpable(std::string const &property)
	{
		bool dumpable = false;

		PropertiesMap::iterator i = properties_.find(property);
		
 		if (i == properties_.end()) 
			dumpable = cinfo_->isPropertyDumpable(property);
		else if (i->second.dumpable)
			dumpable = true;

		return dumpable;
	}

	
	std::string const &Object::className() { return cinfo_->className(); }
	xgui::ClassInfo *Object::getClass() { return cinfo_; }


	bool Object::isIdentifiedAs(std::string const &id) { return id_ == id; }
	std::string const &Object::id() { return id_; }
	void Object::setId(std::string const &id) { id_ = id; }
	
	void Object::store(std::string const &name, xgui::Object * o, bool destroy) 
	{
		ObjectsMap::iterator obj_i = stored_objects_.find(name);
		if (obj_i != stored_objects_.end()) 
			delete obj_i->second;
		stored_objects_[name] = new xgui::StoredObject(o, destroy); 
	}

	Object * Object::retrieve(std::string const &name) 
	{
		ObjectsMap::iterator obj = stored_objects_.find(name);
		if (obj != stored_objects_.end()) 
			return obj->second->object;
		else
			return 0;
	}

	void Object::unstore(std::string const &name)
	{
		ObjectsMap::iterator obj_i = stored_objects_.find(name);
		if (obj_i != stored_objects_.end()) {
			delete obj_i->second;
			stored_objects_.erase(obj_i);
		}
	}

	//Properties management
	bool Object::set(std::string const &name, std::string const &val)
	{
		PropertySetter setter = 0;

		PropertiesMap::iterator i = properties_.find(name);
		if (i != properties_.end())
			setter = i->second.setter;
			
		if (!setter)
			setter = cinfo_->getPropertySetter(name);
	
		if (setter) {
			setter(this, name, val);
			return true;
		}

		return false;
	}
	
	bool Object::get(std::string const &name, std::string &dest)
	{
		PropertyGetter getter = 0;

		PropertiesMap::iterator i = properties_.find(name);
		if (i != properties_.end())
			getter = i->second.getter;
			
		if (!getter)
			getter = cinfo_->getPropertyGetter(name);
	
		if (getter) {
			getter(this, name, dest);
			return true;
		}

		return false;
	}
	
	//Ownership management for external bindings
	void Object::addOwner(void *o)
	{
		ASSERT(o);
		DMESSAGE("Aggiunta Owner: " << o << " di un " << cinfo_->className());
		owners_.insert(o);
	}
	
	void Object::delOwner(void *o)
	{
		ASSERT(o);
		DMESSAGE("Rimozione Owner: " << o << " di un " <<  cinfo_->className());
		owners_.erase(o);
	}
	
	void Object::setOwnerDestructionNotifier(void (*notifier)(void *, Object *))
	{
		owner_destruction_notifier_ = notifier; 
	}
	
	//Dynamic Methods Management
	std::string Object::dumpProperties()
	{
		std::string xml = " ";

		std::vector<std::string> prop_list = properties();
		for ( std::vector<std::string>::iterator iter = prop_list.begin();
			iter != prop_list.end(); ++iter ) {
			if (isPropertyDumpable(*iter)) {
				std::string value;
				get(*iter, value);

				if(!value.empty()) {
					xml += *iter;
					xml += "=\"";
					xml += value;
					xml += "\" ";
				}
			}
		}

		return xml;
	}

	std::string Object::dump()
	{
		std::string xml = "<";
		xml+=cinfo_->tag();
		xml+=dumpProperties();
		xml+="/>\n";

		return xml;
	}

	DMethod * Object::findMethod(std::string const &name)
	{
		DMethod * m = 0;

		MethodsMap::iterator mi = mmap_.find(name);
		if (mi != mmap_.end()) 
			m = mi->second;
		else
			m = cinfo_->findMethod(name);

		return m;
	}

	void Object::registerMethod(std::string const &name, DMethod * m)
	{
		mmap_[name] = m;
	}

	std::vector<std::string> Object::methods()
	{
		std::vector<std::string> mv = cinfo_->methods();

		for(MethodsMap::iterator mi = mmap_.begin(); mi != mmap_.end(); ++mi)
			mv.push_back(mi->first);

		return mv;
	}

	std::string Object::call(std::string const &method)
	{
		DMethod *m = findMethod(method);
		if (m)
			return m->call(this);
		return "";
	}
	
	std::string Object::call(std::string const &method, std::string const &p1)
	{
		DMethod *m = findMethod(method);
		if (m)
			return m->call(this, p1);
		return "";
	}
	
	std::string Object::call(std::string const &method, std::string const &p1, std::string const &p2)
	{
		DMethod *m = findMethod(method);
		if (m)
			return m->call(this, p1, p2);
		return "";
	}

	std::string Object::call(std::string const &method, std::string const &p1, std::string const &p2, std::string const &p3)
	{
		DMethod *m = findMethod(method);
		if (m)
			return m->call(this, p1, p2, p3);
		return "";
	}

	std::string Object::call(std::string const &method, std::string const &p1, std::string const &p2, std::string const &p3, std::string const &p4)
	{
		DMethod *m = findMethod(method);
		if (m)
			return m->call(this, p1, p2, p3, p4);
		return "";
	}

	std::string Object::call(std::string const &method, std::string const &p1, std::string const &p2, std::string const &p3, std::string const &p4, std::string const &p5)
	{
		DMethod *m = findMethod(method);
		if (m)
			return m->call(this, p1, p2, p3, p4, p5);
		return "";
	}

	Object * Object::Unpack(std::string const &address)
	{
		return xgui::semantic_cast<xgui::Object*>(address);
	}

	std::string Object::pack()
	{
		return xgui::semantic_cast<std::string>(this);
	}
}




