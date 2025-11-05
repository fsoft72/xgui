//
// C++ Interface: window
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

%typemap(python,out) xgui::Widget * { $result = XGUIPyObjectCreate($1, false); }
%typemap(python,out) xgui::Menu * { $result = XGUIPyObjectCreate($1, false); }

namespace xgui
{
	class Menu;
	class Image;

	struct WindowClass : public xgui::ContainerClass
	{
		WindowClass();
		std::string const &className();
		bool isInstanceable();
		xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);

		virtual bool canBeContainedIn(xgui::ClassInfo * c);
	};

	class Window : public xgui::Container
	{
		private:
			Window(xgui::Window * parent);
			virtual ~Window();

		public:
			void show();
			bool isModal();
			void modalReturn(int response);

			xgui::Widget * getRootChild();
			xgui::Menu * getMenu();

			void maximize();
			void minimize();
			void restore();

			void setIcon ( Image * img );
			xgui::Image * getIcon();
	};
}

%extend xgui::Window {
        int modalRun() {
                PythonThreadsManager::AllowThreads();
                int rv = self->modalRun();
                PythonThreadsManager::DenyThreads();
                return rv;
        }

        %pythoncode {
                def __del__(self):
                        if self.valid: self.delPyOwner(self)
        }
}
%clear xgui::Widget *;
%clear xgui::Menu *;      
