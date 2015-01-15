#include "StdAfx.h"
#include "IDBService.h"
#include "DBService.h"

IDBService::IDBService(void)
{
}


IDBService::~IDBService(void)
{
}

IDBService* IDBService::CreateObject()
{
	return (new CDBService);
}
