//
// C++ Interface: toolbar
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
	class Image;

	struct ToolbarClass : public xgui::WidgetClass
	{
		ToolbarClass();
		std::string const &className();
		bool isInstanceable();
		xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);

		virtual bool canContain(xgui::ClassInfo * c);
	};

	class Toolbar : public xgui::Widget
	{
		protected:
			Toolbar(xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::Toolbar * widget);
			virtual ~Toolbar();

		public:	
			int addItem ( std::string const &id, std::string const &text, xgui::Image * icon, std::string const &tooltip, std::string const & type = "" );
			int delItem ( const std::string & name );

			void enableButton ( const std::string & name );
			void disableButton ( const std::string & name );
			void checkButton ( const std::string & name );
			void uncheckButton ( const std::string & name );
			bool isButtonChecked ( const std::string & name );
	};
}

%extend xgui::Toolbar {
	%pythoncode {
		def __del__(self):
			if self.valid: self.delPyOwner(self)
	}
}

