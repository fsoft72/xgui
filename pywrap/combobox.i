//
// C++ Interface: combobox
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
	struct ComboboxClass : public xgui::ViewClass
	{
		ComboboxClass();
		bool isInstanceable();
		std::string const &className();
		xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);
	
		virtual bool canContain(xgui::ClassInfo * c);
	};

	class Combobox : public xgui::View
	{
		private:
			Combobox(xgui::Container * parent, bool editable);
			virtual ~Combobox();

		public:
			void insertText(std::string const &text, unsigned int pos);
			void appendText(std::string const &text);
			void prependText(std::string const &text);

			void popup();
			void popdown();

			bool isEditable();
			void setAsEditable(bool editable);

		friend class Master;
		friend struct xguimpl::Combobox;
	};
}

%extend xgui::Combobox {
	%pythoncode {
		def __del__(self):
			if self.valid: self.delPyOwner(self)
	}
}

