//
// C++ Interface: progressbar
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
	struct ProgressbarClass : public xgui::WidgetClass
	{
		ProgressbarClass();
		std::string const &className();
		bool isInstanceable();
		xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);
	};

	class Progressbar : public xgui::Widget
	{
		private:
			Progressbar(xgui::Container * parent, int min, int max);
			virtual ~Progressbar();

		public:
			int getPos ();
			void setPos ( int pos );
	};
}

%extend xgui::Progressbar {
	%pythoncode {
		def __del__(self):
			if self.valid: self.delPyOwner(self)
	}
}

