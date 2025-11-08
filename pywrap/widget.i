//
// C++ Interface: widget
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

%typemap(out) xgui::Window * { $result = XGUIPyObjectCreate($1, false); }
%typemap(out) xgui::Container * { $result = XGUIPyObjectCreate($1, false); }

namespace xgui
{
	struct WidgetClass : public xgui::ObjectClass
	{
		  WidgetClass();
		  bool isWidget();
		  std::string const &className();

		  virtual bool canContain(xgui::ClassInfo * c);
		  virtual bool canBeContainedIn(xgui::ClassInfo * c);
	};

	class Widget : public xgui::Object
	{
		private:
			Widget(xgui::Container * parent, xgui::ClassInfo * cinfo = 0, xguimpl::Widget * i = 0);
			virtual ~Widget();

		public:
			xguimpl::Widget * getImpl() { return impl_; }

			void setAsFloating();
			xgui::Container * getParent();
			xgui::Window * getContext();
			std::string getPath();

			bool linkEvent( std::string const &name, xgui::Callback * cb );
			xgui::Callback * getEvent( std::string const &name );
			bool hasEvent ( std::string const &name );

			void recalcLayout();

			std::pair<int, int> getMousePos();
			void redraw();
	};
}

%extend xgui::Widget {
	void delPyOwner(PyObject * pyobj) {
		self->delOwner(pyobj);
	}

	%pythoncode {
		def __del__(self):
			if self.valid: self.delPyOwner(self)
	}
}

%clear xgui::Container *;
%clear xgui::Window *;