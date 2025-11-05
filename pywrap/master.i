namespace xgui
{
	%nodefault;
	class Master
	{
		public:
			static xgui::Master * Instance();
			static void Quit();
			static void RegisterIdleCallback ( xgui::Callback * cbk, xgui::Widget * wid );

			static void InitDynamicClasses();

			static xgui::Model * CreateModel();
			static void DestroyModel(xgui::Model * model);

			static xgui::StyleManager * CreateStyleManager();
			static void DestroyStyleManager(xgui::StyleManager * sm);
			
			%typemap(python,out) xgui::Object * { $result = XGUIPyObjectCreate($1, true); }
			%typemap(python,out) xgui::Window * { $result = XGUIPyObjectCreate($1, true); }
			%typemap(python,out) xgui::Button * { $result = XGUIPyObjectCreate($1, true); }
			%typemap(python,out) xgui::Label * { $result = XGUIPyObjectCreate($1, true); }
			%typemap(python,out) xgui::Entry * { $result = XGUIPyObjectCreate($1, true); }
			%typemap(python,out) xgui::VBox * { $result = XGUIPyObjectCreate($1, true); }
			%typemap(python,out) xgui::HBox * { $result = XGUIPyObjectCreate($1, true); }
			%typemap(python,out) xgui::ImageView * { $result = XGUIPyObjectCreate($1, true); }
			%typemap(python,out) xgui::List * { $result = XGUIPyObjectCreate($1, true); }
			%typemap(python,out) xgui::Toolbar * { $result = XGUIPyObjectCreate($1, true); }
			%typemap(python,out) xgui::Combobox * { $result = XGUIPyObjectCreate($1, true); }
			%typemap(python,out) xgui::Space * { $result = XGUIPyObjectCreate($1, true); }
			%typemap(python,out) xgui::Checkbox * { $result = XGUIPyObjectCreate($1, true); }
			%typemap(python,out) xgui::Progressbar * { $result = XGUIPyObjectCreate($1, true); }
			%typemap(python,out) xgui::Slider * { $result = XGUIPyObjectCreate($1, true); }
			%typemap(python,out) xgui::Menu * { $result = XGUIPyObjectCreate($1, true); }
			%typemap(python,out) xgui::Tab * { $result = XGUIPyObjectCreate($1, true); }
			%typemap(python,out) xgui::Spin * { $result = XGUIPyObjectCreate($1, true); }
			%typemap(python,out) xgui::Frame * { $result = XGUIPyObjectCreate($1, true); }
			%typemap(python,out) xgui::Calendar * { $result = XGUIPyObjectCreate($1, true); }
			%typemap(python,out) xgui::Tree * { $result = XGUIPyObjectCreate($1, true); }
			
			static xgui::Image * LoadImage(std::string const &path);
			static xgui::Image * GetImage(std::string const &path);
			static xgui::Image * NullImage();

			static xgui::Window * FindContext(std::string const &cname);
			static xgui::Window * CreateWindow(xgui::Window * parent = 0);
			static xgui::Button * CreateButton(xgui::Container * parent, std::string const &text, xgui::Image * image = 0, bool toggle_button = false);
			static xgui::Label * CreateLabel(xgui::Container * parent, std::string const &text);
			static xgui::Entry * CreateEntry(xgui::Container * parent, std::string const &text, int maxlen=0, bool editable=true, bool password_mode=false);
			static xgui::VBox * CreateVBox(xgui::Container * parent, int space = 0, bool border = false);
			static xgui::HBox * CreateHBox(xgui::Container * parent, int space = 0, bool border = false);
			static xgui::ImageView * CreateImageView(xgui::Container * parent, xgui::Image * img = 0);
 			static xgui::List * CreateList(xgui::Container * parent);
			static xgui::Toolbar * CreateToolbar(xgui::Container * parent);
			static xgui::Combobox * CreateCombobox(xgui::Container * parent, bool editable = 0);
			static xgui::Space * CreateSpace(xgui::Container * parent, int width = 1, int height = 1);
			static xgui::Checkbox * CreateCheckbox(xgui::Container * parent, std::string const &text);
			static xgui::Progressbar * CreateProgressbar(xgui::Container * parent, int min = 0, int max = 100);
			static xgui::Slider * CreateSlider(xgui::Container * parent, int min = 0, int max = 100, bool vertical = false);
			static xgui::Menu * CreateMenu(xgui::Window * parent, bool visible = true );
			static xgui::Tab * CreateTab(xgui::Container * parent, const std::string & alignment = "top");
			static xgui::Spin * CreateSpin(xgui::Container * parent, int min = 0, int max = 100);
			static xgui::Frame * CreateFrame(xgui::Container * parent, const std::string & label);
			static xgui::Calendar * CreateCalendar(xgui::Container * parent);
			static xgui::Tree * CreateTree(xgui::Container * parent);
			
			static xgui::Object * LoadXml(const std::string & file_name);
			
			%clear xgui::Object *;
			%clear xgui::Calendar *;
			%clear xgui::Window *;
			%clear xgui::Button *;
			%clear xgui::Label *;
			%clear xgui::Entry *;
			%clear xgui::VBox *;
			%clear xgui::HBox *;
			%clear xgui::ImageView *;
			%clear xgui::List *;
			%clear xgui::Toolbar *;
			%clear xgui::Combobox *;
			%clear xgui::Space *;
			%clear xgui::Checkbox *;
			%clear xgui::Progressbar *;
			%clear xgui::Slider *;
			%clear xgui::Menu *;
			%clear xgui::Tab *;
			%clear xgui::Spin *;
			%clear xgui::Frame *;
			%clear xgui::Tree *;

			static void DestroyWidget(xgui::Widget * w);
			
	};
	%makedefault;
}

%extend xgui::Master
{
	static void InitThreads()
	{
		xgui::Master::InitThreads();
	}
	
	static void QuitThreads()
	{
	%#ifdef WIN32
		TlsFree(PythonThreadsManager::tls_index);
	%#endif
		xgui::Master::QuitThreads();
	}

	static void GuiLock()
	{
		Py_BEGIN_ALLOW_THREADS
		xgui::Master::GuiLock();
		Py_END_ALLOW_THREADS
	}

	static void GuiUnlock()
	{
		Py_BEGIN_ALLOW_THREADS
		xgui::Master::GuiUnlock();
		Py_END_ALLOW_THREADS
	}

	static int Run()
	{
		PythonThreadsManager::AllowThreads();
		xgui::Master::Run();
		PythonThreadsManager::DenyThreads();
		return 0;
	}
	
	static std::vector<std::string> OpenFileDialog(xgui::Window * parent, const std::vector<xgui::FileExtension> &filter, std::string const &title, std::string const &path = "", bool multiselection = false)
	{
		std::vector<std::string> retval;
		
		Py_BEGIN_ALLOW_THREADS
		retval = xgui::Master::OpenFileDialog(parent, filter, title, path, multiselection);
		Py_END_ALLOW_THREADS
		
		return retval;
	}
	
	static std::string SaveFileDialog(xgui::Window * parent, const std::vector<xgui::FileExtension> &filter, std::string const &title, std::string const &path = "", std::string const &file_name = "")
	{
		std::string retval;
		
		Py_BEGIN_ALLOW_THREADS
		retval = xgui::Master::SaveFileDialog(parent, filter, title, path, file_name);
		Py_END_ALLOW_THREADS
		
		return retval;
	}
	
	static int MessageDialog(xgui::Window * parent, const std::string & title, const std::string & text, const std::string & type = "info", const std::string & buttons = "ok" )
	{
		int retval;
		
		Py_BEGIN_ALLOW_THREADS
		retval = xgui::Master::MessageDialog(parent, title, text, type, buttons);
		Py_END_ALLOW_THREADS
		
		return retval;
	}
}
