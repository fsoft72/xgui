//
// C++ Interface: style
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

%ignore xgui::Style::operator=;
%ignore xgui::StyleManager::operator[];

namespace xgui
{
	struct StyleManagerClass : public xgui::ObjectClass
	{
		  StyleManagerClass();
		  std::string const &className();
		  bool isStyleManager();
		  bool isInstanceable();
		  xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);

		  bool canContain(xgui::ClassInfo * c);
		  bool canBeContainedIn(xgui::ClassInfo * c);
	};

	class StyleManager : public Object
	{
		protected:
			unsigned int owncount_;
			std::map<std::string, xguimpl::Style *> styles_;
			std::map<std::string, xguimpl::Style *>::iterator cached_style_;
			xguimpl::StyleManager * sm_impl_;

			  StyleManager();
			  ~StyleManager();
			  void destroy();

		public:
			enum TextStyle {
				TxtItalic =	0x00000001,
				TxtBold =	0x00000002,
				TxtUnderline =	0x00000004,
				TxtStrikeOut =	0x00000008,
			};

			  xguimpl::StyleManager * getImpl();
			  int ref();
			  int unref();

			  int addStyle(std::string const &style_name, xgui::Image * icon, std::string const &fg_color, std::string const &bg_color, unsigned int text_style);
			  int delStyle(std::string const &style);

			  bool isTextBold(std::string const &style);
			  bool isTextItalic(std::string const &style);
			  bool isTextUnderlined(std::string const &style);
			  bool isTextStriked(std::string const &style);
			xgui::Image * getIcon(std::string const &style);
			std::string const &getFgColor(std::string const &style);
			std::string const &getBgColor(std::string const &style);

			unsigned int parseColorString(const std::string & color);

			virtual std::string dump();

		friend class Master;
	};
}
