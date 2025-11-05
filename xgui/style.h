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

#ifndef _XGUI_STYLE_H_
#define _XGUI_STYLE_H_

#include "object.h"
#include "image.h"

namespace xguimpl
{
	struct StyleManager;

	class Style
	{
		public:


		private:
			xgui::Image * icon_;
			std::string fg_color_;
			std::string bg_color_;
			unsigned int text_style_;

		public:
			DLLEXPORT Style();
			DLLEXPORT Style(xgui::Image * icon, std::string const &fg_color, std::string const &bg_color, unsigned int text_style);
			
			DLLEXPORT void swap(Style &other);
			DLLEXPORT Style &operator=(xguimpl::Style const &other);

			DLLEXPORT bool isTextBold();
			DLLEXPORT bool isTextItalic();
			DLLEXPORT bool isTextUnderlined();
			DLLEXPORT bool isTextStriked();
			DLLEXPORT xgui::Image * getIcon();
			DLLEXPORT std::string const &getFgColor();
			DLLEXPORT std::string const &getBgColor();

		friend struct StyleManager;
	};
}

namespace xgui
{
	struct StyleManagerClass : public xgui::ObjectClass
	{
		DLLEXPORT StyleManagerClass();
		DLLEXPORT std::string const &className();
		DLLEXPORT bool isStyleManager();
		DLLEXPORT bool isInstanceable();
		DLLEXPORT xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);

		DLLEXPORT bool canContain(xgui::ClassInfo * c);
		DLLEXPORT bool canBeContainedIn(xgui::ClassInfo * c);
	};

	class StyleManager : public Object
	{
		protected:
			unsigned int owncount_;
			std::map<std::string, xguimpl::Style *> styles_;
			std::map<std::string, xguimpl::Style *>::iterator cached_style_;
			xguimpl::StyleManager * sm_impl_;

			DLLEXPORT StyleManager();
			DLLEXPORT ~StyleManager();
			DLLEXPORT void destroy();

		public:
			enum TextStyle {
				TxtItalic =	0x00000001,
				TxtBold =	0x00000002,
				TxtUnderline =	0x00000004,
				TxtStrikeOut =	0x00000008,
			};

			DLLEXPORT xguimpl::StyleManager * getImpl();
			DLLEXPORT int ref();
			DLLEXPORT int unref();

			DLLEXPORT int addStyle(std::string const &style_name, xgui::Image * icon, std::string const &fg_color, std::string const &bg_color, unsigned int text_style);
			DLLEXPORT int delStyle(std::string const &style);

			DLLEXPORT bool isTextBold(std::string const &style);
			DLLEXPORT bool isTextItalic(std::string const &style);
			DLLEXPORT bool isTextUnderlined(std::string const &style);
			DLLEXPORT bool isTextStriked(std::string const &style);
			DLLEXPORT xgui::Image * getIcon(std::string const &style);
			DLLEXPORT std::string const &getFgColor(std::string const &style);
			DLLEXPORT std::string const &getBgColor(std::string const &style);

			DLLEXPORT unsigned int parseColorString(const std::string & color);

			virtual std::string dump();

		friend class Master;
	};
}

#endif
