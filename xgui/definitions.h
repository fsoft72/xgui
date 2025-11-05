//
// C++ Interface: definitions
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_DEFINITIONS_H_
#define _XGUI_DEFINITIONS_H_

#include "xguifwd.h"
#include <string>

namespace xgui
{
	enum
	{
		keyShiftMask = 0x00000001,
		keyControlMask = 0x00000002,
		keyAltMask = 0x00000004,

		mouseLeftMask = 0x00000008,
		mouseRightMask = 0x00000010,
		mouseMiddleMask = 0x00000020,
	};

	bool const EVT_BLOCK = false;
	bool const EVT_PROPAGATE = true;

	enum PropertyType { PROPERTY_NOT_FOUND, PROPERTY_READ_ONLY, PROPERTY_READ_WRITE };

	struct FileExtension {
		std::string name;
		std::string extension;
		DLLEXPORT FileExtension(std::string const &n, std::string const &ext) : name(n), extension(ext) {}
		DLLEXPORT FileExtension() : name("All files", "*") {}
	};
}

#endif



