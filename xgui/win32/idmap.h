//
// C++ Interface: idmap
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef _XGUI_IDMAP_H_
#define _XGUI_IDMAP_H_

namespace xguimpl
{
	class IdMap
	{
	public:
		DLLEXPORT IdMap() {}
		DLLEXPORT virtual ~IdMap() {}

		DLLEXPORT void set ( int id, const std::string & str );

		DLLEXPORT void remove ( int id );
		DLLEXPORT void remove ( const std::string & str );

		DLLEXPORT int get_id ( const std::string & str );
		DLLEXPORT const std::string & get_string ( int id );

	private:
		std::map<int, std::string> id2str;
		std::map<std::string, int> str2id;

	};
}

#endif
