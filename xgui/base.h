//
// C++ Interface: base
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_BASE_H_
#define _XGUI_BASE_H_

#ifndef WIN32
#	define DLLEXPORT
#else
#	define DLLEXPORT __declspec(dllexport)
#endif

#include <map>
#include <vector>
#include <set>
#include <list>
#include <sstream>
#include <typeinfo>
#include <cstdlib>
#include <algorithm>

#include "definitions.h"
#include "xguifwd.h"
#include "debug.h"

namespace xgui
{
	typedef std::set<void*> VoidSet;
	typedef std::set<std::string> StrSet;
	typedef std::vector<std::string> StrVector;
	typedef std::map<std::string, void*>  StrVoidMap;
	typedef std::map<std::string, xgui::Callback*> CallbackMap;
	typedef std::vector<void*> VoidVector;
	typedef std::map<std::string, std::string> StringMap;
	typedef std::vector<xgui::FileExtension> FileExtensionsVector;
	typedef std::vector<xgui::Widget*> WidgetsVector;
	typedef std::map<std::string, xgui::Widget*> WidgetsMap;
	typedef std::set<xgui::Widget*> WidgetsSet;
	typedef std::map<std::string, xgui::Image*> ImagesMap;

	struct DMethod;
	typedef void (*PropertySetter)(xgui::Object *, std::string const &, std::string const &);
	typedef void (*PropertyGetter)(xgui::Object *, std::string const &, std::string &);

	struct PropertyInfo
	{
		xgui::PropertySetter setter;
		xgui::PropertyGetter getter;
		bool dumpable;

		PropertyInfo() : setter(0), getter(0), dumpable(false) {}

		PropertyInfo(xgui::PropertySetter set, xgui::PropertyGetter get, bool dump)
		: setter(set), getter(get), dumpable(dump) {}
	};

	typedef std::map<std::string, PropertyInfo> PropertiesMap;
	typedef std::map<std::string, DMethod*> MethodsMap;

	template < typename A, typename B >
	std::ostream &operator<<(std::ostream &os, std::pair<A, B> const &p)
	{
		return os << p.first << " " << p.second;
	}

	template < typename A, typename B >
	std::istream &operator>>(std::istream &is, std::pair<A, B> &p)
	{
		return is >> p.first >> p.second;
	}

	template < class TargetT, class SourceT >
	struct SemanticCastActuator
	{
		TargetT operator()(const SourceT &source)
		{
			throw std::bad_cast();
		}
	};
	
	template < class TargetT, class SourceT >
	TargetT semantic_cast(const SourceT &orig)
	{
		try {
			return SemanticCastActuator<TargetT, SourceT>()(orig);
		}
		catch(...) {
			throw;
		}
	}

	template < class SourceT >
	struct SemanticCastActuator<std::string, SourceT>
	{
		std::string operator()(const SourceT &source)
		{
			std::ostringstream ss;
			ss << source;
			return ss.str();
		}
	};

	template < class T >
	struct SemanticCastActuator<T, T>
	{
		T operator()(const T &source)
		{
			return source;
		}
	};

	template < >
	struct SemanticCastActuator<std::string, std::string>
	{
		std::string operator()(const std::string &source)
		{
			return source;
		}
	};
	
	template < class TargetT >
	struct SemanticCastActuator<TargetT, std::string>
	{
		TargetT operator()(const std::string &source)
		{
			TargetT retv;
			std::istringstream ss(source);
			
			if (!(ss >> retv))
				throw std::bad_cast();
			else
				return retv;
		}
	};

	#define XGUI_ERROR(message) std::cerr << "XGUI ERROR: " << message << std::endl;
}

#endif
