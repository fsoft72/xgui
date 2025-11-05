//
// C++ Interface: entry
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
	struct EntryClass : public xgui::WidgetClass
	{
		EntryClass();
		std::string const &className();
		bool isInstanceable();
		xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);
	};


	class Entry : public xgui::Widget
	{
		protected:
			Entry ( xgui::Container * parent, std::string const &text, int maxlen, bool editable, bool password_mode );
			virtual ~Entry();

		public:
			void appendText ( const std::string & text );
			void prependText ( const std::string & text );
			void insertText ( const std::string & text, int pos );
	};
}

%extend xgui::Entry {
	%pythoncode {
		def __del__(self):
			if self.valid: self.delPyOwner(self)
	}
}

