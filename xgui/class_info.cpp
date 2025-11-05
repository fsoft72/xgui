//
// C++ Implementation: class_info
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
#	include "class_info.h"
#	include "object.h"
#	include "master.h"
#endif

namespace xgui
{
	ClassInfo::ClassInfo() {}

	ClassInfo::~ClassInfo() {}

	void ClassInfo::registerProperty(std::string const &prop, xgui::PropertySetter setter, xgui::PropertyGetter getter, bool dumpable)
	{
		properties_[prop] = PropertyInfo(setter, getter, dumpable);
	}

	std::vector<std::string> ClassInfo::properties()
	{
		std::vector<std::string> props;

		for(PropertiesMap::iterator i = properties_.begin(); i != properties_.end(); ++i)
			props.push_back(i->first);

		return props;
	}

	bool ClassInfo::isPropertyDumpable(std::string const &property)
	{
		PropertiesMap::iterator i = properties_.find(property);
		
 		if (i == properties_.end()) return false;

		if (i->second.dumpable) return true;

		return false;
	}


	xgui::PropertyType ClassInfo::hasProperty(std::string const &property)
	{
		PropertiesMap::iterator i = properties_.find(property);
		
		if (i == properties_.end()) return xgui::PROPERTY_NOT_FOUND;

		if (i->second.setter == 0) return xgui::PROPERTY_READ_ONLY;

		return xgui::PROPERTY_READ_WRITE;
	}

	xgui::PropertySetter ClassInfo::getPropertySetter(std::string const &prop)
	{
		PropertiesMap::iterator i = properties_.find(prop);

		if (i != properties_.end())
			return i->second.setter;
		else
			return 0;
	}

	xgui::PropertyGetter ClassInfo::getPropertyGetter(std::string const &prop)
	{
		PropertiesMap::iterator i = properties_.find(prop);

		if (i != properties_.end())
			return i->second.getter;
		else
			return 0;
	}

	void ClassInfo::registerMethod(std::string const &name, DMethod * m)
	{
		mmap_[name] = m;
	}

	DMethod * ClassInfo::findMethod(std::string const &name)
	{
		MethodsMap::iterator mi = mmap_.find(name);
		if (mi == mmap_.end()) 
			return 0;
		return mi->second;
	}

	std::vector<std::string> ClassInfo::methods()
	{
		std::vector<std::string> ms;

		for(MethodsMap::iterator i = mmap_.begin(); i != mmap_.end(); ++i)
			ms.push_back(i->first);

		return ms;
	}

	std::string const &ClassInfo::tag()
	{
		if (tag_.empty()) {
			tag_ = this->className();

			std::string::size_type dot_pos = tag_.find("::");
			if(dot_pos != std::string::npos)
				tag_.erase(0, dot_pos+2);

			std::transform(tag_.begin(), tag_.end(), tag_.begin(), (int(*)(int))::tolower);
		}

		return tag_;
	}

	const std::map<std::string, bool> & ClassInfo::initProperties()
	{
		return constr_props_;
	}

	void ClassInfo::registerInitProperty ( const std::string & name, bool required )
	{
		constr_props_[name] = required;
	}

	const std::string & ClassInfo::dtd()
	{
		if (!dtd_.empty()) return dtd_;
		dtd_ = "<!ELEMENT " + tag() + " (%xgui." + tag() + "_children;)*>\n";
		dtd_ += "<!ATTLIST " + tag() + "\n";

		const std::map<std::string, bool> & init_map = initProperties();
		for ( std::map<std::string, bool>::const_iterator iter = init_map.begin();
			iter != init_map.end(); ++iter ) {

			dtd_ += "   " + iter->first + " CDATA " + (iter->second ? "#REQUIRED" : "#IMPLIED") + "\n";

		}

		std::vector<std::string> prop_list = properties();
		for ( std::vector<std::string>::iterator iter = prop_list.begin();
			iter != prop_list.end(); ++iter ) {

			if ( (hasProperty(*iter) == PROPERTY_READ_WRITE) && (init_map.find(*iter) == init_map.end()) )
				dtd_ += "   " + (*iter) + " CDATA #IMPLIED\n";

		}

		dtd_ += ">\n";

		return dtd_;
	}
}
