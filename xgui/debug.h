//
// C++ Interface: debug
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_DEBUG_H_
#define _XGUI_DEBUG_H_

#include "base.h"

#ifdef _DEBUG
#include <iostream>

#define DMESSAGE(msg) std::cerr << ">>> XGui Debug Message: " << msg << std::endl

#ifdef WIN32
#define BreakPoint() __asm { int 3 }
#else
#define BreakPoint() exit(-1)
#endif

#define ASSERT(condition) if (!(condition)) { std::cerr << ">>> XGui Failed Assertion: FILE: " << __FILE__ << " LINE: " << __LINE__ << " ASSERTION: (" << #condition << ")" << std::endl; BreakPoint(); }


template<bool> struct CompileTimeAssertion { CompileTimeAssertion(...); };
template<> struct CompileTimeAssertion<false> { };
#define STATIC_ASSERT(condition, message) \
	{\
		class ERROR_##message {};\
		(void)sizeof((CompileTimeAssertion<(condition)>)CompileTimeAssertion<(condition)>((ERROR_##message())));\
	}

#else

#define ASSERT(condition) ((void)(condition))
#define STATIC_ASSERT(condition, message) ((void)(condition))
#define DMESSAGE(msg) ((void)(0))

#endif

#endif
