//
// C++ Interface: list
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
	class TextPairCallback;

	struct ListClass : public xgui::ViewClass
	{
		ListClass();
		bool isInstanceable();
		std::string const &className();
		xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);

		virtual bool canContain(xgui::ClassInfo * c);
	};

	class List : public xgui::View
	{
		protected:
			List(xgui::Container * parent, xgui::StyleManager * sm);
			virtual ~List();

		public:
			void addColumn ( int cpos, const std::string & cid, const std::string & cname, int cwidth = 100, const std::string & calign = "left" );
			void delColumn ( const std::string & id );
			void sort ( const std::string & column_id, xgui::TextPairCallback * callback );

			virtual bool isChecked ( xgui::Model * parent, int child_pos );
			virtual void check ( xgui::Model * parent, int child_pos, bool state );
			virtual void checkAll ( xgui::Model * parent, bool state );
	};
}

%extend xgui::List {
	%pythoncode {
		def __del__(self):
			if self.valid: self.delPyOwner(self)
	}
}

