//
// C++ Interface: callback
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_CALLBACK_H_
#define _XGUI_CALLBACK_H_

#include "base.h"

namespace xgui
{
	//Callbacks Interfaces
	class Callback
	{
		protected:
			DLLEXPORT Callback();

		public:
			DLLEXPORT virtual int call(Widget *);
			DLLEXPORT virtual ~Callback();
	};

	class TextCallback : public virtual Callback
	{
		protected:
			DLLEXPORT TextCallback();
	
		public:
			DLLEXPORT virtual int call(Widget *, std::string const &text);
			DLLEXPORT virtual ~TextCallback();
	};


	class TextStatusCallback : public virtual Callback
	{
		protected:
			DLLEXPORT TextStatusCallback();
	
		public:
			DLLEXPORT virtual int call(Widget *, std::string const &text, int state);
			DLLEXPORT virtual ~TextStatusCallback();

	};


	class TextPairCallback : public virtual Callback
	{
		protected:
			DLLEXPORT TextPairCallback();
	
		public:
			DLLEXPORT virtual int call(Widget *, std::string const &text1, std::string const &text2);
			DLLEXPORT virtual ~TextPairCallback();

	};


	//Cpp Callbacks Implementation
	class CppFCallback : public Callback
	{
		private:
			int (*func)(Widget*);

		public:
			DLLEXPORT CppFCallback(int(*fptr)(Widget*));
			DLLEXPORT virtual ~CppFCallback();
			DLLEXPORT virtual int call(Widget *);
			
	};

	class CppFTextCallback : public TextCallback
	{
		private:
			int (*func)(Widget*, std::string const &text);

		public:
			DLLEXPORT CppFTextCallback(int(*fptr)(Widget*, std::string const &));
			DLLEXPORT virtual ~CppFTextCallback();
			DLLEXPORT virtual int call(Widget *, std::string const &text);
	};

	class CppFTextStatusCallback : public TextStatusCallback
	{
		private:
			int (*func)(Widget*, std::string const &text, int state);

		public:
			DLLEXPORT CppFTextStatusCallback(int(*fptr)(Widget*, std::string const &, int));
			DLLEXPORT virtual ~CppFTextStatusCallback();
			DLLEXPORT virtual int call(Widget *, std::string const &text, int state);
	};

	class CppFTextPairCallback : public TextPairCallback
	{
		private:
			int (*func)(Widget*, std::string const &text1, std::string const &text2);

		public:
			DLLEXPORT CppFTextPairCallback(int(*fptr)(Widget*, std::string const &, std::string const &));
			DLLEXPORT virtual ~CppFTextPairCallback();
			DLLEXPORT virtual int call(Widget *, std::string const &text1, std::string const &text2);
	};

	template <typename ObjectType>
	class CppMCallback : public Callback
	{
		private:
			ObjectType * obj;
			int (ObjectType::*method)(Widget*);

		public:
			CppMCallback(ObjectType * const o, int(ObjectType::*mptr)(Widget*)) : Callback(), obj(o), method(mptr) {}
			virtual ~CppMCallback() {}
			virtual int call(Widget * w) { return (obj->*method)(w); }
	};

	template <typename ObjectType>
	class CppMTextStatusCallback : public TextStatusCallback
	{
		private:
			ObjectType * obj;
			int (ObjectType::*func)(Widget*, std::string const &text, int state);

		public:
			CppMTextStatusCallback(ObjectType * const o, int(ObjectType::*fptr)(Widget*, std::string const &, int)) 
			: TextStatusCallback(), obj(o), func(fptr) {}
			virtual ~CppMTextStatusCallback() {}
			virtual int call(Widget * w, std::string const &text, int state) { return (obj->*func)(w, text, state); }
	};



}

#endif
