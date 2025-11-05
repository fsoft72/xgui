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

%typemap(python, in, numinputs=0) std::string &dest (std::string temp) {
	$1 = &temp;
}

%typemap(python, argout) std::string &dest {
	Py_XDECREF($result);
	if (result) {
		char const * cstrdata = $1->c_str();
		unsigned long int cstrlen = $1->size();
		$result = PyString_FromStringAndSize(cstrdata, cstrlen);
	}
	else {
		Py_INCREF(Py_None);
		$result = Py_None;
	}
}

%typemap(python,out) xgui::Object * { $result = XGUIPyObjectCreate($1, false); }

namespace xgui
{
	struct StoredObject;

	struct ObjectClass : public xgui::ClassInfo
	{
		ObjectClass();
		xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);
		void finalize(xgui::Object *);

		virtual bool canContain(xgui::ClassInfo * c);
		virtual bool canBeContainedIn(xgui::ClassInfo * c);
	
		bool isInstanceable();
		bool mustFinalize();
		virtual std::string const &className();
		bool isContainer();
		bool isWidget();
		bool isModel();
		bool isStyleManager();
	};

	class Object
	{
		protected:
			Object(xgui::ClassInfo * cinfo);
			virtual ~Object();

		public:
			//Dynamic type management
			std::string const &className();
			xgui::ClassInfo *getClass();

			bool isIdentifiedAs(std::string const &id);
			std::string const &id();
			void setId(std::string const &id);
			
			//Storable data management
			void store(std::string const &name, xgui::Object * o, bool destroy = false);
			xgui::Object * retrieve(std::string const &name);
			void unstore(std::string const &name);

			//Properties management
			bool set(std::string const &name, std::string const &val);
			bool get(std::string const &name, std::string &dest);
			void registerProperty(std::string const &prop, xgui::PropertySetter setter, xgui::PropertyGetter getter, bool dumpable);
			std::vector<std::string> properties();
			xgui::PropertyType hasProperty(std::string const &property);

			//Dynamic Objects
			static xgui::Object * Unpack(std::string const &obj);
			std::string pack();
			
			xgui::DMethod * findMethod(std::string const &name);
			void registerMethod(std::string const &name, xgui::DMethod * m);
			std::string call(std::string const &method, std::string const &p1);
			std::string call(std::string const &method, std::string const &p1, std::string const &p2);
	};
}

%clear xgui::Object *;
%clear std::string &outvals;
