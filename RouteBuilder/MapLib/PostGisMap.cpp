#include "StdAfx.h"
#include "PostGisMap.h"

CPostGisMap::CPostGisMap(void)
{

}


CPostGisMap::~CPostGisMap(void)
{

}
bool CPostGisMap::Open(string host, string user, string dbname, string port, string password)
{
	this->conninf="host="+host+" user="+user+" dbname="+ dbname +" port="+port;
	if(!password.empty())
	this->conninf+=" password="+password;
	mycon =PQconnectdb(conninf.c_str());

	ConnStatusType type =PQstatus(mycon);
	if(type==CONNECTION_BAD)
		return false;
	return true;

}
