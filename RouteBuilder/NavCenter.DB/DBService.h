#pragma once

#include "IDBService.h"

#include "..\ADOLib\ADOLib.h"

class CDBService : public IDBService
{
	CADODatabase dBase;
	bool connected;
public:
	
	CDBService(void);
	~CDBService(void);

	virtual BOOL Connect(LPCTSTR dbFileName, LPCTSTR user=NULL, LPCTSTR password=NULL);
	virtual bool IsConnect();
	virtual void Close();

	virtual list<CDBLocation> GetAllLocations();
	virtual bool			  AddLocation(CDBLocation &location);
	virtual CDBLocation*	  GetLocation(CString name);
	virtual bool              DeleteLocation(int ID);

	virtual list<CDBPolyLine> GetAllPolylines();
	virtual bool			  AddPolyline(CDBPolyLine &line);
	virtual CDBPolyLine*	  GetPolyline(CString name);
	virtual bool              DeletePolyline(int ID);

	virtual list<CDBRoute>    GetAllRoutes(bool withRelations=true);
	virtual list<CDBRelation> GetRelationsByRoute(int routeID);
	virtual bool              AddRoute(CDBRoute& route);
	virtual bool              DeleteRoute(int &ID);
	
		virtual CDBRoute*         GetRoute(CString &name, int& LocFirst_ID, int& LocFinish_ID);
	
	virtual bool			  DeleteRelation(CDBRelation& relation);
	virtual bool			  DeleteRelationsByRoute(int& routeID);
	virtual bool			  AddRelation(CDBRelation& relation);
	virtual bool              AddRelations(list<CDBRelation>& relations);


	virtual bool			  ExportToExcel(TExcelExportProps props);
	virtual list<CDBCombineRoute> GetAllCombineRoutes();



};

