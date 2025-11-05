//
// C++ Interface: callback
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

namespace xgui
{
	//Callbacks Interfaces
	class Callback
	{
		protected:
			 Callback();

		public:
			 virtual int call(Widget *);
			 virtual ~Callback();
	};

	class TextCallback : public Callback
	{
		protected:
			 TextCallback();
	
		public:
			 virtual int call(Widget *, std::string const &text);
			 virtual ~TextCallback();
	};


	class TextStatusCallback : public Callback
	{
		protected:
			 TextStatusCallback();
	
		public:
			 virtual int call(Widget *, std::string const &text, int state);
			 virtual ~TextStatusCallback();

	};


	class TextPairCallback : public Callback
	{
		protected:
			 TextPairCallback();
	
		public:
			 virtual int call(Widget *, std::string const &text1, std::string const &text2);
			 virtual ~TextPairCallback();

	};

	class PyCallback : public xgui::Callback
	{
		public:
			PyCallback(PyObject * cb);
			virtual ~PyCallback();
			virtual int call(xgui::Widget *w);
	};
	
	class PyTextCallback : public xgui::TextCallback
	{
		public:
			PyTextCallback(PyObject * cb);
			virtual ~PyTextCallback();
			virtual int call(xgui::Widget *w, std::string const &str);
	};
	
	class PyTextStatusCallback : public xgui::TextStatusCallback
	{
		public:
			PyTextStatusCallback(PyObject * cb);
			virtual ~PyTextStatusCallback();
			virtual int call(xgui::Widget *w, std::string const &str, int status);
	};
	
	class PyTextPairCallback : public xgui::TextPairCallback
	{
		public:
			PyTextPairCallback(PyObject * cb);
			virtual ~PyTextPairCallback();
			virtual int call(xgui::Widget *w, std::string const &str, std::string const &str2);
	};
}

%{
namespace xgui 
{
	class PyCallback : public xgui::Callback
	{
		private:
			PyObject * callback;
	
		public:
			PyCallback(PyObject * cb) 
			: xgui::Callback(), callback(cb) 
			{ 
				if (!PyCallable_Check(cb)) {
					PyErr_SetString(PyExc_TypeError, "Non callable object to PyCallback");
					PyErr_Print();
				}
	
				Py_INCREF(cb); 
			}
			
			virtual ~PyCallback() 
			{ 
				Py_DECREF(callback); 
			}
	
			virtual int call(xgui::Widget *w) 
			{
				int rv = 0;
				
				bool should_allow = PythonThreadsManager::DenyThreads();
	
				if (PyCallable_Check(callback)) {
					PyObject *pw =  XGUIPyObjectCreate(w, false);
					PyObject *args = Py_BuildValue("(O)", pw);
					PyObject *result = PyEval_CallObject(callback, args);
					Py_XDECREF(args);
					Py_XDECREF(pw);
	
					if (result == 0) {
						PyErr_Print();
					}
					else if (result == Py_None) {
						Py_XDECREF(result);
					}
					else if (PyInt_Check(result)) {
						rv = PyInt_AsLong(result);
						Py_XDECREF(result);
					}
					else {
						Py_XDECREF(result);
						PyErr_SetString(PyExc_TypeError, "Expected Integer or None as return value of callback");
						PyErr_Print();
					}
				}
				else {
					PyErr_SetString(PyExc_TypeError, "Non callable object to PyCallback");
					PyErr_Print();
				}
	
				if ( should_allow ) PythonThreadsManager::AllowThreads();
				
				return rv;
			}
	};
	
	class PyTextCallback : public xgui::TextCallback
	{
		private:
			PyObject * callback;
	
		public:
			PyTextCallback(PyObject * cb) 
			: xgui::TextCallback(), callback(cb) 
			{ 
				if (!PyCallable_Check(cb)) {
					PyErr_SetString(PyExc_TypeError, "Non callable object to PyCallback");
					PyErr_Print();
				}
	
				Py_INCREF(cb); 
			}
			
			virtual ~PyTextCallback() 
			{ 
				Py_DECREF(callback); 
			}
	
	
			virtual int call(xgui::Widget *w, std::string const &text) 
			{
				int rv = 0;
	
				bool should_allow = PythonThreadsManager::DenyThreads();
				
				if (PyCallable_Check(callback)) {
					PyObject *pw =  XGUIPyObjectCreate(w, false);
					PyObject *args = Py_BuildValue("(Os)", pw, text.c_str());
					PyObject *result = PyEval_CallObject(callback, args);
					Py_XDECREF(args);
					Py_XDECREF(pw);
					
					if (result == 0) {
						PyErr_Print();
					}
					else if (result == Py_None) {
						Py_XDECREF(result);
					}
					else if (PyInt_Check(result)) {
						rv = PyInt_AsLong(result);
						Py_XDECREF(result);
					}
					else {
						Py_XDECREF(result);
						PyErr_SetString(PyExc_TypeError, "Expected Integer or None as return value of callback");
						PyErr_Print();
					}
				}
				else {
					PyErr_SetString(PyExc_TypeError, "Non callable object to PyCallback");
					PyErr_Print();
				}
	
				if ( should_allow ) PythonThreadsManager::AllowThreads();
				
				return rv;
			}
	};
	
	class PyTextStatusCallback : public xgui::TextStatusCallback
	{
		private:
			PyObject * callback;
	
		public:
			PyTextStatusCallback(PyObject * cb) 
			: xgui::TextStatusCallback(), callback(cb) 
			{ 
				if (!PyCallable_Check(cb)) {
					PyErr_SetString(PyExc_TypeError, "Non callable object to PyCallback");
					PyErr_Print();
				}
	
				Py_INCREF(cb); 
			}
			
			virtual ~PyTextStatusCallback() 
			{ 
				Py_DECREF(callback); 
			}
	
	
			virtual int call(xgui::Widget *w, std::string const &text, int status) 
			{
				int rv = 0;
	
				bool should_allow = PythonThreadsManager::DenyThreads();
				
				if (PyCallable_Check(callback)) {
					PyObject *pw =  XGUIPyObjectCreate(w, false);
					PyObject *args = Py_BuildValue("(Osi)", pw, text.c_str(), status);
					PyObject *result = PyEval_CallObject(callback, args);
					Py_XDECREF(args);
					Py_XDECREF(pw);
					
					if (result == 0) {
						PyErr_Print();
					}
					else if (result == Py_None) {
						Py_XDECREF(result);
					}
					else if (PyInt_Check(result)) {
						rv = PyInt_AsLong(result);
						Py_XDECREF(result);
					}
					else {
						Py_XDECREF(result);
						PyErr_SetString(PyExc_TypeError, "Expected Integer or None as return value of callback");
						PyErr_Print();
					}
				}
				else {
					PyErr_SetString(PyExc_TypeError, "Non callable object to PyCallback");
					PyErr_Print();
				}
	
				if ( should_allow ) PythonThreadsManager::AllowThreads();
	
				return rv;
			}
	};
	
	class PyTextPairCallback : public xgui::TextPairCallback
	{
		private:
			PyObject * callback;
	
		public:
			PyTextPairCallback(PyObject * cb) 
			: xgui::TextPairCallback(), callback(cb) 
			{ 
				if (!PyCallable_Check(cb)) {
					PyErr_SetString(PyExc_TypeError, "Non callable object to PyCallback");
					PyErr_Print();
				}
	
				Py_INCREF(cb); 
			}
			
			virtual ~PyTextPairCallback() 
			{ 
				Py_DECREF(callback); 
			}
	
	
			virtual int call(xgui::Widget *w, std::string const &text, std::string const &text2) 
			{
				int rv = 0;
	
				bool should_allow = PythonThreadsManager::DenyThreads();
				
				if (PyCallable_Check(callback)) {
					PyObject *pw =  XGUIPyObjectCreate(w, false);
					PyObject *args = Py_BuildValue("(Oss)", pw, text.c_str(), text2.c_str());
					PyObject *result = PyEval_CallObject(callback, args);
					Py_XDECREF(args);
					Py_XDECREF(pw);
					
					if (result == 0) {
						PyErr_Print();
					}
					else if (result == Py_None) {
						Py_XDECREF(result);
					}
					else if (PyInt_Check(result)) {
						rv = PyInt_AsLong(result);
						Py_XDECREF(result);
					}
					else {
						Py_XDECREF(result);
						PyErr_SetString(PyExc_TypeError, "Expected Integer or None as return value of callback");
						PyErr_Print();
					}
				}
				else {
					PyErr_SetString(PyExc_TypeError, "Non callable object to PyCallback");
					PyErr_Print();
				}
	
				if ( should_allow ) PythonThreadsManager::AllowThreads();
	
				return rv;
			}
	};
}
%}

