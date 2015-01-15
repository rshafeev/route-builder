#pragma once

#ifdef NAVCENTERDB_EXPORTS
#define NAVCENTERDB_API __declspec(dllexport)
#else
#define NAVCENTERDB_API __declspec(dllimport)
#endif

#include "DBModel.h"

#include <list>
#include <vector> 
using namespace std;

struct TExcelExportProps
{
	CString excelFile;
	double freqAir;
	double freqShip;
	double freqTrain;
};

class NAVCENTERDB_API IDBService
{
public:
	static IDBService* CreateObject();
	virtual BOOL Connect(LPCTSTR dbFileName, LPCTSTR user=NULL, LPCTSTR password=NULL)=NULL;
	virtual void Close()=NULL;
	virtual bool IsConnect()=NULL;

	virtual bool			  AddLocation(CDBLocation &location)=NULL;
	virtual CDBLocation*	  GetLocation(CString name)=NULL;
	virtual bool			  DeleteLocation(int ID)=NULL;
	virtual list<CDBLocation> GetAllLocations()=NULL;
	
	virtual list<CDBPolyLine> GetAllPolylines()=NULL;
	virtual bool			  AddPolyline(CDBPolyLine &line)=NULL;
	virtual CDBPolyLine*	  GetPolyline(CString name)=NULL;
	virtual bool              DeletePolyline(int ID)=NULL;

	virtual bool              AddRoute(CDBRoute& route)=NULL;
	virtual list<CDBRoute>    GetAllRoutes(bool withRelations=true)=NULL;
	virtual list<CDBRelation> GetRelationsByRoute(int routeID)=NULL;
	virtual CDBRoute*         GetRoute(CString &name, int& LocFirst_ID, int& LocFinish_ID)=NULL;
	virtual bool              DeleteRoute(int &ID)=NULL;

	virtual bool			  DeleteRelation(CDBRelation& relation)=NULL;
	virtual bool			  DeleteRelationsByRoute(int& routeID)=NULL;
	virtual bool			  AddRelation(CDBRelation& relation)=NULL;
	virtual bool              AddRelations(list<CDBRelation>& relations)=NULL;

	virtual bool			  ExportToExcel(TExcelExportProps props)=NULL;
	virtual list<CDBCombineRoute> GetAllCombineRoutes()=NULL;
	IDBService(void);
	~IDBService(void);
};

