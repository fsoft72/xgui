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
			using Callback::call;
			DLLEXPORT virtual int call(Widget *, std::string const &text);
			DLLEXPORT virtual ~TextCallback();
	};


	class TextStatusCallback : public virtual Callback
	{
		protected:
			DLLEXPORT TextStatusCallback();

		public:
			using Callback::call;
			DLLEXPORT virtual int call(Widget *, std::string const &text, int state);
			DLLEXPORT virtual ~TextStatusCallback();

	};


	class TextPairCallback : public virtual Callback
	{
		protected:
			DLLEXPORT TextPairCallback();

		public:
			using Callback::call;
			DLLEXPORT virtual int call(Widget *, std::string const &text1, std::string const &text2);
			DLLEXPORT virtual ~TextPairCallback();

	};


	class IntCallback : public virtual Callback
	{
		protected:
			DLLEXPORT IntCallback();

		public:
			using Callback::call;
			DLLEXPORT virtual int call(Widget *, int value);
			DLLEXPORT virtual ~IntCallback();

	};


	class BoolCallback : public virtual Callback
	{
		protected:
			DLLEXPORT BoolCallback();

		public:
			using Callback::call;
			DLLEXPORT virtual int call(Widget *, bool value);
			DLLEXPORT virtual ~BoolCallback();

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

	class CppFIntCallback : public IntCallback
	{
		private:
			int (*func)(Widget*, int value);

		public:
			DLLEXPORT CppFIntCallback(int(*fptr)(Widget*, int));
			DLLEXPORT virtual ~CppFIntCallback();
			DLLEXPORT virtual int call(Widget *, int value);
	};

	class CppFBoolCallback : public BoolCallback
	{
		private:
			int (*func)(Widget*, bool value);

		public:
			DLLEXPORT CppFBoolCallback(int(*fptr)(Widget*, bool));
			DLLEXPORT virtual ~CppFBoolCallback();
			DLLEXPORT virtual int call(Widget *, bool value);
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

	template <typename ObjectType>
	class CppMIntCallback : public IntCallback
	{
		private:
			ObjectType * obj;
			int (ObjectType::*method)(Widget*, int value);

		public:
			CppMIntCallback(ObjectType * const o, int(ObjectType::*mptr)(Widget*, int))
			: IntCallback(), obj(o), method(mptr) {}
			virtual ~CppMIntCallback() {}
			virtual int call(Widget * w, int value) { return (obj->*method)(w, value); }
	};

	template <typename ObjectType>
	class CppMBoolCallback : public BoolCallback
	{
		private:
			ObjectType * obj;
			int (ObjectType::*method)(Widget*, bool value);

		public:
			CppMBoolCallback(ObjectType * const o, int(ObjectType::*mptr)(Widget*, bool))
			: BoolCallback(), obj(o), method(mptr) {}
			virtual ~CppMBoolCallback() {}
			virtual int call(Widget * w, bool value) { return (obj->*method)(w, value); }
	};



}

#endif
