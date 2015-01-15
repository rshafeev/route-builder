#pragma once
#include "IDBService.h"
#include "..\ADOLib\ADOLib.h"

class CDBExcelExport
{
public:

private:
	CString excelFile;
	IDBService* service;
	CADODatabase *excelBase;
	bool Connect();
	TExcelExportProps props;
public:
	CDBExcelExport(IDBService* service, TExcelExportProps props);
	bool Start();
	~CDBExcelExport(void);
};

