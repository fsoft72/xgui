//
// C++ Implementation: idmap
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

using namespace xguimpl;


void IdMap::set ( int id, const std::string & str )
{
	id2str[id] = str;
	str2id[str] = id;
}


void IdMap::remove ( int id )
{
	std::map<int, std::string>::iterator iter = id2str.find(id);
	if (iter == id2str.end()) return;
	std::string str = id2str[id];
	str2id.erase(str);
	id2str.erase(id);
}


void IdMap::remove ( const std::string & str )
{
	std::map<std::string, int>::iterator iter = str2id.find(str);
	if (iter == str2id.end()) return;
	int id = str2id[str];
	id2str.erase(id);
	str2id.erase(str);
}


int IdMap::get_id ( const std::string & str )
{
	std::map<std::string, int>::iterator iter = str2id.find(str);
	if (iter == str2id.end()) return -1;
	return str2id[str];
}


const std::string & IdMap::get_string ( int id )
{
	static const std::string empty = "";
	std::map<int, std::string>::iterator iter = id2str.find(id);
	if (iter == id2str.end()) return empty;
	return id2str[id];
}
