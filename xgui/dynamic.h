//
// C++ Interface: dynamic
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_DYNAMIC_H_
#define _XGUI_DYNAMIC_H_

#include "object.h"

namespace xgui
{
	template < class PtrT >
	struct SemanticCastActuator<PtrT *, std::string>
	{
		PtrT *operator()(const std::string &source)
		{
			std::istringstream ss(source);
			void * p = 0;
			ss >> std::hex >> p;
			return (PtrT*)p;
		}
	};

	template < class PtrT >
	struct SemanticCastActuator<std::string, PtrT *>
	{
		std::string operator()(PtrT * source)
		{
			void * p = (void*)(source);

			if (p == 0) 
				return "";
			else {
				std::ostringstream ss;
				ss << p;
				return ss.str();
			}
		}
	};

	template <>
	struct SemanticCastActuator<xgui::Object *, std::string>
	{
		xgui::Object *operator()(const std::string &source)
		{
			std::istringstream ss(source);
			void * o = 0;
			ss >> std::hex >> o;
			return (xgui::Object*)o;
		}
	};

	template <>
	struct SemanticCastActuator<std::string, xgui::Object *>
	{
		std::string operator()(xgui::Object * source)
		{
			void * p = dynamic_cast<void*>(source);

			if (p == 0) 
				return "";
			else {
				std::ostringstream ss;
				ss << p;
				return ss.str();
			}
		}
	};

	template <typename T>
	struct Type
	{
		typedef T Tmp_T;
		typedef T Real_T;
	};
	
	template <typename T>
	struct Ref
	{
		typedef typename T::Tmp_T Tmp_T;
		typedef typename T::Real_T &Real_T;
	};
	
	template <typename T>
	struct Const
	{
		typedef typename T::Tmp_T Tmp_T;
		typedef typename T::Real_T const Real_T; 
	};
	
	template <typename T>
	struct Ptr
	{
		typedef typename T::Tmp_T  *Tmp_T;
		typedef typename T::Real_T *Real_T;
	};

	template <typename T>
	struct TypeOf
	{
		typedef Type<T> Type;
	};

	template <typename T>
	struct TypeOf<T&>
	{
		typedef Ref< typename TypeOf<T>::Type > Type;
	};

	template <typename T>
	struct TypeOf<const T>
	{
		typedef Const< typename TypeOf<T>::Type > Type;
	};

	template <typename T>
	struct TypeOf<T *>
	{
		typedef Ptr< typename TypeOf<T>::Type > Type;
	};
	
	struct DMethod
	{
		public:
			DLLEXPORT virtual unsigned int numberOfParameters() = 0;
			DLLEXPORT virtual std::string call(Object *) { XGUI_ERROR("Calling dynamic method with 0 parameters"); return ""; }
			DLLEXPORT virtual std::string call(Object *, std::string const &) { XGUI_ERROR("Calling dynamic method with 1 parameter"); return ""; }
			DLLEXPORT virtual std::string call(Object *, std::string const &, std::string const &) { XGUI_ERROR("Calling dynamic method with 2 parameters"); return ""; }
			DLLEXPORT virtual std::string call(Object *, std::string const &, std::string const &, std::string const &) { XGUI_ERROR("Calling dynamic method with 3 parameters"); return ""; }
			DLLEXPORT virtual std::string call(Object *, std::string const &, std::string const &, std::string const &, std::string const &) { XGUI_ERROR("Calling dynamic method with 4 parameters"); return ""; }
			DLLEXPORT virtual std::string call(Object *, std::string const &, std::string const &, std::string const &, std::string const &, std::string const &) { XGUI_ERROR("Calling dynamic method with 5 parameters"); return ""; }
	};

	template <typename RT, typename OType>
	struct DMethod0 : DMethod
	{
		typedef typename RT::Real_T (OType::*FType)();
		FType method;
	
		DLLEXPORT DMethod0(FType f_ptr) : DMethod(), method(f_ptr) {}

		DLLEXPORT unsigned int numberOfParameters() { return 0; }
	
		DLLEXPORT std::string call(Object * o)
		{
			FType m = (FType)(method);		
			OType * real_o = dynamic_cast<OType*>(o);
			std::string returned_str;
			if (real_o) {
				typename RT::Tmp_T rv = (real_o->*m)();
				returned_str = semantic_cast<std::string>(rv); 
			}
			return returned_str;
		}
	};

	template <typename RT, typename OType, typename T1>
	struct DMethod1 : DMethod
	{
		typedef typename RT::Real_T (OType::*FType)(typename T1::Real_T);
		FType method;
	
		DLLEXPORT DMethod1(FType f_ptr) : DMethod(), method(f_ptr) {}

		DLLEXPORT unsigned int numberOfParameters() { return 1; }
	
		DLLEXPORT std::string call(Object * o, std::string const &param)
		{
			FType m = (FType)(method);
			typename T1::Tmp_T p1 = semantic_cast<typename T1::Tmp_T>(param);
			
			OType * real_o = dynamic_cast<OType*>(o);
			std::string returned_str;
			if (real_o) {
				typename RT::Tmp_T rv = (real_o->*m)(p1);
				returned_str = semantic_cast<std::string>(rv); 
			}
			return returned_str;
		}
	};
	
	template <typename RT, typename OType, typename T1, typename T2>
	struct DMethod2 : DMethod
	{
		typedef typename RT::Real_T (OType::*FType)(typename T1::Real_T, typename T2::Real_T);
		FType method;
	
		DLLEXPORT DMethod2(FType f_ptr) : DMethod(), method(f_ptr) {}

		DLLEXPORT unsigned int numberOfParameters() { return 2; }
	
		DLLEXPORT std::string call(Object * o, std::string const &pa1, std::string const &pa2)
		{
			FType m = (FType)(method);
			typename T1::Tmp_T p1 = semantic_cast<typename T1::Tmp_T>(pa1);
			typename T2::Tmp_T p2 = semantic_cast<typename T2::Tmp_T>(pa2);
			
			OType * real_o = dynamic_cast<OType*>(o);
			std::string returned_str;
			if (real_o) {
				typename RT::Tmp_T rv = (real_o->*m)(p1, p2);
				returned_str = semantic_cast<std::string>(rv); 
			}
			return returned_str;
		}
	};

	template <typename RT, typename OType, typename T1, typename T2, typename T3>
	struct DMethod3 : DMethod
	{
		typedef typename RT::Real_T (OType::*FType)(typename T1::Real_T, typename T2::Real_T, typename T3::Real_T);
		FType method;
	
		DLLEXPORT DMethod3(FType f_ptr) : DMethod(), method(f_ptr) {}

		DLLEXPORT unsigned int numberOfParameters() { return 3; }
	
		DLLEXPORT std::string call(Object * o, std::string const &pa1, std::string const &pa2, std::string const &pa3)
		{
			FType m = (FType)(method);
			typename T1::Tmp_T p1 = semantic_cast<typename T1::Tmp_T>(pa1);
			typename T2::Tmp_T p2 = semantic_cast<typename T2::Tmp_T>(pa2);
			typename T3::Tmp_T p3 = semantic_cast<typename T3::Tmp_T>(pa3);
			
			OType * real_o = dynamic_cast<OType*>(o);
			std::string returned_str;
			if (real_o) {
				typename RT::Tmp_T rv = (real_o->*m)(p1, p2, p3);
				returned_str = semantic_cast<std::string>(rv); 
			}
			return returned_str;
		}
	};

	template <typename RT, typename OType, typename T1, typename T2, typename T3, typename T4>
	struct DMethod4 : DMethod
	{
		typedef typename RT::Real_T (OType::*FType)(typename T1::Real_T, typename T2::Real_T, typename T3::Real_T, typename T4::Real_T);
		FType method;
	
		DLLEXPORT DMethod4(FType f_ptr) : DMethod(), method(f_ptr) {}

		DLLEXPORT unsigned int numberOfParameters() { return 4; }
	
		DLLEXPORT std::string call(Object * o, std::string const &pa1, std::string const &pa2, std::string const &pa3, std::string const &pa4)
		{
			FType m = (FType)(method);
			typename T1::Tmp_T p1 = semantic_cast<typename T1::Tmp_T>(pa1);
			typename T2::Tmp_T p2 = semantic_cast<typename T2::Tmp_T>(pa2);
			typename T3::Tmp_T p3 = semantic_cast<typename T3::Tmp_T>(pa3);
			typename T4::Tmp_T p4 = semantic_cast<typename T4::Tmp_T>(pa4);
			
			OType * real_o = dynamic_cast<OType*>(o);
			std::string returned_str;
			if (real_o) {
				typename RT::Tmp_T rv = (real_o->*m)(p1, p2, p3, p4);
				returned_str = semantic_cast<std::string>(rv); 
			}
			return returned_str;
		}
	};

	template <typename RT, typename OType, typename T1, typename T2, typename T3, typename T4, typename T5>
	struct DMethod5 : DMethod
	{
		typedef typename RT::Real_T (OType::*FType)(typename T1::Real_T, typename T2::Real_T, typename T3::Real_T, typename T4::Real_T, typename T5::Real_T);
		FType method;
	
		DLLEXPORT DMethod5(FType f_ptr) : DMethod(), method(f_ptr) {}

		DLLEXPORT unsigned int numberOfParameters() { return 5; }
	
		DLLEXPORT std::string call(Object * o, std::string const &pa1, std::string const &pa2, std::string const &pa3, std::string const &pa4, std::string const &pa5)
		{
			FType m = (FType)(method);
			typename T1::Tmp_T p1 = semantic_cast<typename T1::Tmp_T>(pa1);
			typename T2::Tmp_T p2 = semantic_cast<typename T2::Tmp_T>(pa2);
			typename T3::Tmp_T p3 = semantic_cast<typename T3::Tmp_T>(pa3);
			typename T4::Tmp_T p4 = semantic_cast<typename T4::Tmp_T>(pa4);
			typename T5::Tmp_T p5 = semantic_cast<typename T5::Tmp_T>(pa5);
			
			OType * real_o = dynamic_cast<OType*>(o);
			std::string returned_str;
			if (real_o) {
				typename RT::Tmp_T rv = (real_o->*m)(p1, p2, p3, p4, p5);
				returned_str = semantic_cast<std::string>(rv); 
			}
			return returned_str;
		}
	};
}

#endif
