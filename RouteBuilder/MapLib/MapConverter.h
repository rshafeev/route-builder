#pragma once

//Export2DB(std::string host, std::string user, std::string dbname, std::string port, std::string password);

class MAPLIB_API IConverterReport
{
public:
	IConverterReport(){}
	~IConverterReport(){}

};
class MAPLIB_API TOsmDBExportProp
{
public:
	string file;
	string cFile;
	string host;
	string user;
	string port;
	string dbname;
	string passwd;
	bool skipnodes;
	bool clean;
	bool IsCreateSchema;
	bool IsClearingData;
	TOsmDBExportProp();
	~TOsmDBExportProp();
};
class MAPLIB_API MapConverter
{
//	IConverterReport* exportReport;
	
public:
	MapConverter(IConverterReport* exportReport);
	bool ExportOsm2PostGis(TOsmDBExportProp* prop);
	~MapConverter(void);
};

