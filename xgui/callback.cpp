//
// C++ Implementation: callback
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
#	include "callback.h"
#	include "widget.h"

#	include <iostream>
#endif

namespace xgui
{
	Callback::Callback() {}
	int Callback::call(Widget * w)
	{
		std::cerr << "XGUI Warning: DEFAULT CALLBACK CALLED ON " << w->className() << ":" << w << std::endl;
		return 0;
	}
	Callback::~Callback() {}
	
	TextCallback::TextCallback() : Callback() {}
	int TextCallback::call(Widget *w, std::string const &text)
	{
		std::cerr << "XGUI Warning: DEFAULT CALLBACK CALLED ON " << w->className() << ":" << w << " with text parameter: " << text << std::endl;
		return 0;
	}
	TextCallback::~TextCallback() {}
	
	TextStatusCallback::TextStatusCallback() : Callback() {}
	int TextStatusCallback::call(Widget *w, std::string const &text, int state)
	{
		std::cerr << "XGUI Warning: DEFAULT CALLBACK CALLED ON " << w->className() << ":" << w << " with parameters: " << text  << ", " << state << std::endl;
		return 0;
	}
	TextStatusCallback::~TextStatusCallback() {}
	
	TextPairCallback::TextPairCallback() : Callback() {}
	int TextPairCallback::call(Widget *w, std::string const &text1, std::string const &text2)
	{
		std::cerr << "XGUI Warning: DEFAULT CALLBACK CALLED ON " << w->className() << ":" << w << " with parameters: " << text1  << ", " << text2 << std::endl;
		return 0;
	}
	TextPairCallback::~TextPairCallback() {}
	
	
	CppFCallback::CppFCallback(int(*fptr)(Widget*)) : Callback(), func(fptr) {}
	int CppFCallback::call(Widget *w)
	{
		return func(w);
	}
	CppFCallback::~CppFCallback() {}
	
	CppFTextCallback::CppFTextCallback(int(*fptr)(Widget*, std::string const &)) : TextCallback(), func(fptr) {}
	int CppFTextCallback::call(Widget *w, std::string const &text)
	{
		return func(w, text);
	}
	CppFTextCallback::~CppFTextCallback() {}
	
	CppFTextStatusCallback::CppFTextStatusCallback(int(*fptr)(Widget*, std::string const &, int)) : TextStatusCallback(), func(fptr) {}
	int CppFTextStatusCallback::call(Widget *w, std::string const &text, int state)
	{
		return func(w, text, state);
	}
	CppFTextStatusCallback::~CppFTextStatusCallback() {}
	
	CppFTextPairCallback::CppFTextPairCallback(int(*fptr)(Widget*, std::string const &, std::string const &)) : TextPairCallback(), func(fptr) {}
	int CppFTextPairCallback::call(Widget *w, std::string const &text1, std::string const &text2)
	{
		return func(w, text1, text2);
	}
	CppFTextPairCallback::~CppFTextPairCallback() {}
}
