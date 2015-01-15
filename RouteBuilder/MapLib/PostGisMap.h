#pragma once

#include "MapLib.h"
#include <PostGreSQL\libpq-fe.h>

namespace pgis
{

class MAPLIB_API CShortestPath
{
		
};
class MAPLIB_API CPostGisMap
{
	PGconn *mycon;
	string conninf;
public:
	CPostGisMap(void);
	~CPostGisMap(void);
	bool Open(string host, string user, string dbname, string port, string password);

};
}

