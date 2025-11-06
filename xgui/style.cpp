//
// C++ Implementation: style
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "precomp.h"

#ifndef WIN32
#	include "style.h"
#	include "master.h"
#	include "image.h"
#	include "dynamic.h"
#endif

namespace xguimpl
{
	#ifndef WIN32
		struct StyleManager { };
	#endif

	Style::Style() : icon_(0), text_style_(0) {}
	
	Style::Style(xgui::Image * icon, std::string const &fg_color, std::string const &bg_color, unsigned int text_style)
	: icon_(icon), fg_color_(fg_color), bg_color_(bg_color), text_style_(text_style) { }
	
	void Style::swap(Style &other)
	{
		std::swap( icon_, other.icon_ );
		std::swap( fg_color_, other.fg_color_ );
		std::swap( bg_color_, other.bg_color_ );
		std::swap( text_style_, other.text_style_ );
	}
	
	Style &Style::operator=(xguimpl::Style const &other)
	{
		Style tmp(other);
		tmp.swap(*this);
		return (*this);
	}
	
	bool Style::isTextBold()
	{
		return (text_style_ & xgui::StyleManager::TxtBold) != 0;
	}
	
	bool Style::isTextItalic()
	{
		return (text_style_ & xgui::StyleManager::TxtItalic) != 0;
	}
	
	bool Style::isTextUnderlined()
	{
		return (text_style_ & xgui::StyleManager::TxtUnderline) != 0;
	}
	
	bool Style::isTextStriked()
	{
		return (text_style_ & xgui::StyleManager::TxtStrikeOut) != 0;
	}
	
	xgui::Image * Style::getIcon()
	{
		return icon_;
	}
	
	std::string const &Style::getFgColor()
	{
		return fg_color_;
	}
	
	std::string const &Style::getBgColor()
	{
		return bg_color_;
	}
}

namespace xgui
{
	xgui::StyleManagerClass smanager_class_info;

	StyleManagerClass::StyleManagerClass() 
	: ObjectClass()
	{
		registerMethod("addStyle", new DMethod5< Type<int>, StyleManager, TypeOf<std::string const &>::type, TypeOf<xgui::Image*>::type, TypeOf<std::string const &>::type, TypeOf<std::string const &>::type, TypeOf<unsigned int>::type >(&StyleManager::addStyle));
	}

	std::string const &StyleManagerClass::className() { static std::string const c_name = "xgui::StyleManager"; return c_name; }

	bool StyleManagerClass::isInstanceable() { return true; }

	bool StyleManagerClass::isStyleManager() { return true; }

	xgui::Object * StyleManagerClass::create(xgui::Object * parent, xgui::StringMap &properties)
	{
		StyleManager * sm = xgui::Master::CreateStyleManager();
		Master::AddToPool(sm);
		return sm;
	}
}

namespace xgui
{
	StyleManager::StyleManager() 
	: Object(&smanager_class_info),	owncount_(0), 
	#ifdef WIN32
	sm_impl_(new xguimpl::StyleManager(&styles_))
	#else
	sm_impl_(0)
	#endif
	{
	}	
	
	StyleManager::~StyleManager()
	{
		for(std::map<std::string, xguimpl::Style *>::iterator i = styles_.begin(); i != styles_.end(); ++i)
			delete i->second;

		delete sm_impl_;
	}
	
	void StyleManager::destroy()
	{
		if (owncount_ <= 0) delete this;
	}
	
	int StyleManager::ref() { return ++owncount_; }
	int StyleManager::unref() { if (--owncount_ <= 0) { delete this; return 0; } else { return owncount_; } }

	int StyleManager::addStyle(std::string const &style_name, xgui::Image * icon, std::string const &fg_color, std::string const &bg_color, unsigned int text_style)
	{
		if (styles_.find(style_name) != styles_.end())
			return 0;

		styles_[style_name] = new xguimpl::Style(icon, fg_color, bg_color, text_style);
		cached_style_ = styles_.end();
		return 1;
	}

	int StyleManager::delStyle(std::string const &style_name)
	{
		std::map<std::string, xguimpl::Style *>::iterator s = styles_.find(style_name);
		if (s == styles_.end())
			return 0;

		delete s->second;
		styles_.erase(s);
		cached_style_ = styles_.end();
		return 1;
	}

	bool StyleManager::isTextBold(std::string const &style)
	{
		if( (cached_style_ != styles_.end()) && (cached_style_->first == style) )
			return cached_style_->second->isTextBold();

		cached_style_ = styles_.find(style);
		if(cached_style_ != styles_.end())
			return cached_style_->second->isTextBold();
		else
			return false;
	}
	
	bool StyleManager::isTextItalic(std::string const &style)
	{
		if( (cached_style_ != styles_.end()) && (cached_style_->first == style) )
			return cached_style_->second->isTextItalic();

		cached_style_ = styles_.find(style);
		if(cached_style_ != styles_.end())
			return cached_style_->second->isTextItalic();
		else
			return false;
	}
	
	bool StyleManager::isTextUnderlined(std::string const &style)
	{
		if( (cached_style_ != styles_.end()) && (cached_style_->first == style) )
			return cached_style_->second->isTextUnderlined();

		cached_style_ = styles_.find(style);
		if(cached_style_ != styles_.end())
			return cached_style_->second->isTextUnderlined();
		else
			return false;
	}
	
	bool StyleManager::isTextStriked(std::string const &style)
	{
		if( (cached_style_ != styles_.end()) && (cached_style_->first == style) )
			return cached_style_->second->isTextStriked();

		cached_style_ = styles_.find(style);
		if(cached_style_ != styles_.end())
			return cached_style_->second->isTextStriked();
		else
			return false;
	}
	
	xgui::Image * StyleManager::getIcon(std::string const &style)
	{
		if( (cached_style_ != styles_.end()) && (cached_style_->first == style) )
			return cached_style_->second->getIcon();

		cached_style_ = styles_.find(style);
		if(cached_style_ != styles_.end())
			return cached_style_->second->getIcon();
		else
			return 0;
	}
	
	std::string const &StyleManager::getFgColor(std::string const &style)
	{
		static std::string null_str = "";

		if( (cached_style_ != styles_.end()) && (cached_style_->first == style) )
			return cached_style_->second->getFgColor();

		cached_style_ = styles_.find(style);
		if(cached_style_ != styles_.end())
			return cached_style_->second->getFgColor();
		else
			return null_str;
	}
	
	std::string const &StyleManager::getBgColor(std::string const &style)
	{
		static std::string null_str = "";

		if( (cached_style_ != styles_.end()) && (cached_style_->first == style) )
			return cached_style_->second->getBgColor();

		cached_style_ = styles_.find(style);
		if(cached_style_ != styles_.end())
			return cached_style_->second->getBgColor();
		else
			return null_str;
	}
	
	unsigned int StyleManager::parseColorString(const std::string & color)
	{
		if (color.empty()) return 0;
	
		std::string rgb = color;
		if (rgb[0] == '#') rgb = color.substr(1, 6);
	
		rgb.insert(4, " ");
		rgb.insert(2, " ");
	
		std::istringstream is(rgb);
	
		unsigned int r, g, b;
		is >> std::hex >> r;
		is >> std::hex >> g;
		is >> std::hex >> b;
	
		unsigned int retval = 0;
	
		unsigned char * pr = (unsigned char *)&retval;
		pr[0] = (unsigned char)r;
		pr[1] = (unsigned char)g;
		pr[2] = (unsigned char)b;
	
		return retval;
	}
	
	xguimpl::StyleManager * StyleManager::getImpl() { return sm_impl_; }

	std::string StyleManager::dump()
	{
		std::string xml = "<";
		xml+=cinfo_->tag();
		xml+=dumpProperties();
		xml+=">\n";

		for(std::map<std::string, xguimpl::Style *>::iterator s = styles_.begin(); s!=styles_.end(); ++s) {
			xguimpl::Style * style = dynamic_cast<xguimpl::Style *>(s->second);
		
			xml+="<style id=\"";
			xml+=s->first;
			xml+="\" ";

			
			xgui::Image * icon = style->getIcon();
			if(icon) {
				xml+="icon=\"";
				xml+=icon->getPath();
				xml+="\" ";
			}

			std::string fgcolor = style->getFgColor();
			if(!fgcolor.empty()) {
				xml+="fgcolor=\"";
				xml+=fgcolor;
				xml+="\" ";
			}

			std::string bgcolor = style->getBgColor();
			if(!bgcolor.empty()) {
				xml+="bgcolor=\"";
				xml+=bgcolor;
				xml+="\" ";
			}

			xml+="bold=\"";
			xml+=xgui::semantic_cast<std::string>(style->isTextBold());
			xml+="\" ";

			xml+="italic=\"";
			xml+=xgui::semantic_cast<std::string>(style->isTextItalic());
			xml+="\" ";

			xml+="underline=\"";
			xml+=xgui::semantic_cast<std::string>(style->isTextUnderlined());
			xml+="\" ";

			xml+="strikeout=\"";
			xml+=xgui::semantic_cast<std::string>(style->isTextStriked());
			xml+="\" ";

			xml+="/>\n";
		}

		xml+="</";
		xml+=cinfo_->tag();
		xml+=">\n";

		return xml;
	}
}
