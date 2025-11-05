//
// C++ Interface: menu
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

	struct MenuClass : public xgui::WidgetClass
	{
		MenuClass();
		std::string const &className();
		bool isInstanceable();
		xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);

		virtual bool canContain(xgui::ClassInfo * c);
		virtual bool canBeContainedIn(xgui::ClassInfo * c);
	};

	class Menu : public xgui::Widget
	{
		protected:
			Menu(xgui::Window * parent, bool visible);
			virtual ~Menu();

		public:	
			void addItem(std::string const &parent_id, std::string const &id, std::string const &text, xgui::Image * icon = 0, std::string const & type = "" );
			void delItem(std::string const &id);

			void popupMenu(std::string const &id);

			void enableItem(std::string const &id);
			void disableItem(std::string const &id);
			bool isItemEnabled(std::string const &id);
			
			void checkItem(std::string const &id);
			void uncheckItem(std::string const &id);
			bool isItemChecked(std::string const &id);
			
			void setItemText(std::string const &id, std::string const &text);
			std::string getItemText(std::string const &id);
	};
}

%extend xgui::Menu {
	%pythoncode {
		def __del__(self):
			if self.valid: self.delPyOwner(self)
	}
}

