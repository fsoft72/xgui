%module xgui
%include "std_string.i"
%include "std_vector.i"

%{
#include "xgui.h"
#include "dynamic.h"
#include <algorithm>
#include <iostream>
#include <string>

#ifdef WIN32

#include <windows.h>
#include "../xgui/win32/master_impl.h"

#undef CreateWindow
#undef LoadImage

#pragma warning(disable: 4101)
#pragma warning(disable: 4800)

#endif

class PythonThreadsManager
{
	public:
		#ifdef WIN32
			static DWORD tls_index;
		#else
			static __thread PyThreadState * thread_state;
		#endif

		static void SetThreadState(PyThreadState * ts)
		{
		#ifdef WIN32
			TlsSetValue(tls_index, ts);
		#else
			thread_state = ts;
		#endif
		}
		
		static PyThreadState * GetThreadState()
		{
		#ifdef WIN32
			return (PyThreadState *)TlsGetValue(tls_index);
		#else
			return thread_state;
		#endif
		}
		
		static void AllowThreads()
		{
			if (GetThreadState() == 0)
				SetThreadState(PyEval_SaveThread());
			else {
				PyErr_SetString(PyExc_RuntimeError, "XGUI was unable to save thread state");
				PyErr_Print();
				return;
			}
		}

		static bool DenyThreads()
		{
			PyThreadState * ts = GetThreadState();
			if (ts != 0) {
				PyEval_RestoreThread(ts);
				SetThreadState(0);
				return true;
			}
			else {
				//PyErr_SetString(PyExc_RuntimeError, "XGUI was unable to repristinate thread state");
				//PyErr_Print();
				return false;
			}
		}
};

#ifndef WIN32
__thread PyThreadState * PythonThreadsManager::thread_state = 0;
#else
DWORD PythonThreadsManager::tls_index = 0;
#endif
%}

%init %{
	#ifdef WIN32
		PythonThreadsManager::tls_index = TlsAlloc();
	#endif
%}

%{
void XGUIPyObjectDestructionNotify(PyObject * pyobj, xgui::Object * w)
{
        PyObject *n = PyLong_FromLong(0);

	int ptrlen = sizeof(void*)*2;
	std::string ptr = "_";
	for(int i=0; i <ptrlen; ++i)
		ptr += '0';
	ptr+="DestroyedObject";
	PyObject *this_ptr = PyUnicode_FromString(ptr.c_str());

        PyObject_SetAttrString(pyobj,(char*)"thisown",n);
	PyObject_SetAttrString(pyobj, (char*)"this", this_ptr);
	PyObject_SetAttrString(pyobj, (char*)"valid", n);

        Py_DECREF(n);
	Py_DECREF(this_ptr);
}

PyObject * XGUIPyObjectCreate(xgui::Object * w, bool constructing)
{
	if (!w) {
		Py_INCREF(Py_None);
		return Py_None;
	}

	std::string swig_class_name = "_p_";
	std::string class_name = w->className();
	std::replace(class_name.begin(), class_name.end(), ':', '_');

	swig_class_name += class_name;
	swig_type_info * swig_class = SWIG_TypeQuery(swig_class_name.c_str());
	if (!swig_class) {
		swig_class = SWIGTYPE_p_xgui__Widget;
	}

	PyObject *pyobj = SWIG_NewPointerObj(w, swig_class, 0);
	PyObject *n = PyLong_FromLong(1);
	PyObject_SetAttrString(pyobj, (char*)"valid", n);
        Py_DECREF(n);

	if (pyobj) w->addOwner(pyobj);

	if (constructing) {
		w->setOwnerDestructionNotifier( (void(*)(void*, xgui::Object*))XGUIPyObjectDestructionNotify );
	}

	return pyobj;
}

PyObject * XGUIPyObjectReturn(xgui::Object * caller, PyObject * pycaller, xgui::Widget * wid)
{
	if ( (caller == wid) ) {
		Py_INCREF(pycaller);
		return pycaller;
	}
	else {
		return XGUIPyObjectCreate(wid, false);
	}
}
%}

%include "definitions.i"

namespace std {
   %template(StringVector) vector<std::string>;
   %template(FileExtensionsVector) vector<xgui::FileExtension>;
}

%include "class_info.i"
%include "object.i"
%include "widget.i"

%include "model.i"

%include "container.i"

%include "view.i"
%include "image.i"

%include "style.i"

%include "button.i"
%include "window.i"
%include "label.i"
%include "hbox.i"
%include "vbox.i"
%include "entry.i"
%include "list.i"
%include "tree.i"
%include "toolbar.i"
%include "combobox.i"
%include "space.i"
%include "checkbox.i"
%include "progressbar.i"
%include "slider.i"
%include "menu.i"
%include "tab.i"
%include "spin.i"
%include "imageview.i"
%include "frame.i"
%include "calendar.i"

%include "master.i"

%include "callback.i"
