#include "StdAfx.h"
#include "DBService.h"
#include "DBExcelExport.h"

CDBService::CDBService(void)
{
	connected = false;
}


CDBService::~CDBService(void)
{

}

void CDBService::Close()
{
	dBase.Close();
}

/*
SELECT Relations.Route_ID, Routes.Name AS Route_Name, Routes.LocFirst_ID, Routes.LocFinish_ID,  
             Relations.Line_ID, PolyLines.Loc1_ID, PolyLines.Loc2_ID, PolyLines.Name AS Line_Name, PolyLines.Geom, PolyLines.Distance, PolyLines.Time, PolyLines.Type

 FROM ((Relations INNER JOIN Routes ON Routes.ID =  Relations.Route_ID) 
              INNER JOIN PolyLines On PolyLines.ID = Relations.Line_ID)
*/

list<CDBCombineRoute> CDBService::GetAllCombineRoutes()
{
	list<CDBCombineRoute> routes;
	try
	{
		ADODB::CADORecordset* pRs = new ADODB::CADORecordset(&dBase);
		CADOCommand* pCommand = new CADOCommand(&dBase);
		
		CString sql;
		sql = CString("SELECT Relations.Route_ID, Routes.Name AS Route_Name, Routes.LocFirst_ID, Routes.LocFinish_ID,")+
              CString("Relations.Line_ID, PolyLines.Loc1_ID, PolyLines.Loc2_ID, PolyLines.Name AS Line_Name, PolyLines.Geom,")+
			  CString("PolyLines.Distance, PolyLines.Time, PolyLines.Type")+
			  CString(" FROM ((Relations INNER JOIN Routes ON Routes.ID =  Relations.Route_ID) ")+
              CString("INNER JOIN PolyLines ON PolyLines.ID = Relations.Line_ID)");
		pCommand->SetText(sql);
		pCommand->SetType(CADOCommand::typeCmdText);
		if(pRs->Execute(pCommand))
		{
			if(!pRs->IsEof())
				 pRs->MoveFirst();
			while(!pRs->IsEof())
			{
				CDBCombineRoute route;
				pRs->GetFieldValue("Route_ID", route.Route_ID);
				pRs->GetFieldValue("Route_Name", route.Route_Name);
				pRs->GetFieldValue("LocFirst_ID", route.LocFirst_ID);
				pRs->GetFieldValue("LocFinish_ID", route.LocFinish_ID);
				pRs->GetFieldValue("Line_ID", route.Line_ID);
				pRs->GetFieldValue("Loc1_ID", route.Loc1_ID);
				pRs->GetFieldValue("Loc2_ID", route.Loc2_ID);
				pRs->GetFieldValue("Line_Name", route.Line_Name);
				pRs->GetFieldValue("Geom", route.Geom);
				pRs->GetFieldValue("Distance", route.Distance);
				pRs->GetFieldValue("Time", route.Time);
				pRs->GetFieldValue("Type",(int&) route.Type);
				pRs->MoveNext();
				routes.push_back(route);
			}
		}

		pRs->Close();
		delete pRs;
		delete pCommand;
	}
	catch(CADOException &ex)
	{
		return routes;
	}

	return routes;
}

bool CDBService::DeleteRelationsByRoute(int& routeID)
{
	try
	{
		if(dBase.IsOpen()==FALSE)
			throw CADOException("Open error!");
		ADODB::CADORecordset* pRs = new ADODB::CADORecordset(&dBase);
		CADOCommand* pCommand = new CADOCommand(&dBase);
		CADOParameter pParamIn1(CADORecordset::typeInteger, sizeof(int));
		pParamIn1.SetValue(routeID);
		pCommand->AddParameter(&pParamIn1);
		pCommand->SetText("DELETE FROM PolyLines where Route_ID = ? ");
		pCommand->SetType(CADOCommand::typeCmdText);
		bool f1 = pRs->Execute(pCommand);
		pRs->Close();
		delete pRs;
		delete pCommand;
	}
	catch(CADOException &ex)
	{
		return false;
	}
	return true;
}
bool  CDBService::AddRelations(list<CDBRelation>& relations)
{
	for(list<CDBRelation>::iterator i = relations.begin(); i!= relations.end(); i++)
	{
		AddRelation(*i);
	}
	return true;
}

bool CDBService::AddRoute(CDBRoute& route)
{
	try
	{
		if(dBase.IsOpen()==FALSE)
			throw CADOException("Open error!");
		ADODB::CADORecordset* pRs = new ADODB::CADORecordset(&dBase);
		if(pRs->Open("Routes", CADORecordset::openTable))
		{
			pRs->AddNew();
			pRs->SetFieldValue("Name", route.Name);
			pRs->SetFieldValue("LocFirst_ID", route.LocFirst_ID);
			pRs->SetFieldValue("LocFinish_ID", route.LocFinish_ID);
			pRs->Update();
			pRs->Close();
		}
		delete pRs;

		CDBRoute* r = GetRoute(route.Name,route.LocFirst_ID,route.LocFinish_ID);
		if(r==NULL)
		{
			throw CADOException(" CDBService::AddLocation(): add error.");
		}
		route.ID = r->ID;
		delete r;
	}
	catch(CADOException &ex)
	{
		return false;
	}
	return true;	
}
CDBRoute*   CDBService::GetRoute(CString &name, int& LocFirst_ID, int& LocFinish_ID)
{
	CDBRoute *route = new CDBRoute();
	try
	{
		ADODB::CADORecordset* pRs = new ADODB::CADORecordset(&dBase);
		
		if(pRs->Open("Routes", CADORecordset::openTable))
		{
			CString filter;
			filter.Format("Name='%s' AND LocFirst_ID = %d AND LocFinish_ID = %d",name,LocFirst_ID,LocFinish_ID);
			pRs->SetFilter(filter);
			if(!pRs->IsEof())
				 pRs->MoveFirst();
			if(!pRs->IsEof())
			{
					pRs->GetFieldValue("ID", route->ID);
					pRs->GetFieldValue("Name", route->Name);
					pRs->GetFieldValue("LocFirst_ID", route->LocFirst_ID);
					pRs->GetFieldValue("LocFinish_ID", route->LocFinish_ID);
			}
			pRs->Close();
		}
		delete pRs;
	}
	catch(CADOException &ex)
	{
		delete route;
		return NULL;
	}
	return route;
}

CDBLocation* CDBService::GetLocation(CString name)
{
	CDBLocation *location = new CDBLocation();
	try
	{
		ADODB::CADORecordset* pRs = new ADODB::CADORecordset(&dBase);
		
		if(pRs->Open("Locations", CADORecordset::openTable))
		{
			pRs->SetFilter(CString("Name = '"+name+"'"));
			if(!pRs->IsEof())
				 pRs->MoveFirst();
			if(!pRs->IsEof())
			{
					pRs->GetFieldValue("ID", location->ID);
					pRs->GetFieldValue("Name", location->Name);
					pRs->GetFieldValue("Lat", location->Lat);
					pRs->GetFieldValue("Lon", location->Lon);
					pRs->GetFieldValue("Country", location->Country);
					pRs->GetFieldValue("City", location->City);
			}
			pRs->Close();
		}
		delete pRs;
	}
	catch(CADOException &ex)
	{
		delete location;
		return NULL;
	}
	return location;
}

bool CDBService::AddLocation(CDBLocation &location)
{
	try
	{
		if(dBase.IsOpen()==FALSE)
			throw CADOException("Open error!");
		ADODB::CADORecordset* pRs = new ADODB::CADORecordset(&dBase);
		if(pRs->Open("Locations", CADORecordset::openTable))
		{
			pRs->AddNew();
			pRs->SetFieldValue("Name", location.Name);
			pRs->SetFieldValue("Lat", location.Lat);
			pRs->SetFieldValue("Lon", location.Lon);
			pRs->SetFieldValue("Country", location.Country);
			pRs->SetFieldValue("City", location.City);
			pRs->Update();
			pRs->Close();
		}
		delete pRs;

		CDBLocation* loc = GetLocation(location.Name);
		if(loc==NULL)
		{
			throw CADOException(" CDBService::AddLocation(): add error.");
		}
		location.ID = loc->ID;
		delete loc;
	}
	catch(CADOException &ex)
	{
		return false;
	}
	return true;
}
bool CDBService::DeleteRoute(int &ID)
{
	try
	{
		ADODB::CADORecordset* pRs = new ADODB::CADORecordset(&dBase);
		CADOCommand* pCommand = new CADOCommand(&dBase);
		CADOParameter pParamIn(CADORecordset::typeInteger, sizeof(int));
		pParamIn.SetValue(ID);
		pCommand->AddParameter(&pParamIn);
		pCommand->SetText("DELETE FROM Routes where ID = ?");
		pCommand->SetType(CADOCommand::typeCmdText);
		bool f1 = pRs->Execute(pCommand);
		pRs->Close();
		delete pRs;
		delete pCommand;
	}
	catch(CADOException &ex)
	{
		return false;
	}
	return true;
}

bool CDBService::DeleteLocation(int ID)
{
	try
	{
		ADODB::CADORecordset* pRs = new ADODB::CADORecordset(&dBase);
		CADOCommand* pCommand = new CADOCommand(&dBase);
		
		CADOParameter pParamIn(CADORecordset::typeInteger, sizeof(int));
		pParamIn.SetValue(ID);
		pCommand->AddParameter(&pParamIn);
		pCommand->SetText("DELETE FROM Locations where ID = ?");
		pCommand->SetType(CADOCommand::typeCmdText);
		bool f1 = pRs->Execute(pCommand);
		pRs->Close();
		delete pRs;
		delete pCommand;
	}
	catch(CADOException &ex)
	{
		return false;
	}
	return true;
}
BOOL CDBService::Connect(LPCTSTR dbFileName, LPCTSTR user, LPCTSTR password)
{
		CString strConnection = _T("Provider=Microsoft.ACE.OLEDB.12.0; Data Source=")+(CString)dbFileName+"; User Id="+user +"; Password="+password+";";
		try
		{
			dBase.SetConnectionString(strConnection);
			connected = dBase.Open();
		}
		catch(...)
		{
			
			return FALSE;
		}

		return TRUE;
}
bool CDBService::IsConnect()
{
	return this->connected;
}
list<CDBRoute>     CDBService::GetAllRoutes(bool withRelations)
{
	list<CDBRoute> routes;
	try
	{
		ADODB::CADORecordset* pRs = new ADODB::CADORecordset(&dBase);
		if(pRs->Open("Routes", CADORecordset::openTable))
		{
			if(!pRs->IsEof())
				 pRs->MoveFirst();
			while(!pRs->IsEof())
			{
					CDBRoute route;
					pRs->GetFieldValue("ID", route.ID);
					pRs->GetFieldValue("Name", route.Name);
					pRs->GetFieldValue("LocFirst_ID", route.LocFirst_ID);
					pRs->GetFieldValue("LocFinish_ID", route.LocFinish_ID);
					pRs->MoveNext();
					routes.push_back(route);
			}
			pRs->Close();
		}
		delete pRs;
		if(withRelations)
		{
			for(list<CDBRoute>::iterator i = routes.begin(); i!= routes.end(); i++)
				i->relations = GetRelationsByRoute(i->ID);
		}
	}
	catch(CADOException &ex)
	{
	}
	return routes;
}

bool	 CDBService::DeleteRelation(CDBRelation& relation)
{
	try
	{
		if(dBase.IsOpen()==FALSE)
			throw CADOException("Open error!");
		ADODB::CADORecordset* pRs = new ADODB::CADORecordset(&dBase);
		CADOCommand* pCommand = new CADOCommand(&dBase);
		
		CADOParameter pParamIn1(CADORecordset::typeInteger, sizeof(int));
		CADOParameter pParamIn2(CADORecordset::typeInteger, sizeof(int));

		pParamIn1.SetValue(relation.Route_ID);
		pParamIn2.SetValue(relation.Line_ID);

		pCommand->AddParameter(&pParamIn1);
		pCommand->AddParameter(&pParamIn2);

		pCommand->SetText("DELETE FROM Relations where Route_ID = ? and Line_ID = ?");
		pCommand->SetType(CADOCommand::typeCmdText);
		bool f1 = pRs->Execute(pCommand);
		pRs->Close();
		delete pRs;
		delete pCommand;
	}
	catch(CADOException &ex)
	{
		return false;
	}
	return true;
}
bool	 CDBService::AddRelation(CDBRelation& relation)
{
	try
	{
		if(dBase.IsOpen()==FALSE)
			throw CADOException("Open error!");
		ADODB::CADORecordset* pRs = new ADODB::CADORecordset(&dBase);
		if(pRs->Open("Relations", CADORecordset::openTable))
		{
			pRs->AddNew();
			pRs->SetFieldValue("Line_ID", relation.Line_ID);
			pRs->SetFieldValue("Route_ID", relation.Route_ID);
			pRs->Update();
			pRs->Close();
		}
		delete pRs;
	}
	catch(CADOException &ex)
	{
		return false;
	}
	return true;
}

list<CDBRelation>  CDBService::GetRelationsByRoute(int routeID)
{
	list<CDBRelation> relations;
	CString routeIDS;
	routeIDS.Format("%d",routeID);
	try
	{
		ADODB::CADORecordset* pRs = new ADODB::CADORecordset(&dBase);
		if(pRs->Open("Relations", CADORecordset::openTable))
		{
			pRs->SetFilter(CString("Route_ID = "+routeIDS));
			if(!pRs->IsEof())
				 pRs->MoveFirst();
			while(!pRs->IsEof())
			{
					CDBRelation relation;
					pRs->GetFieldValue("Route_ID", relation.Route_ID);
					pRs->GetFieldValue("Line_ID", relation.Line_ID);
					pRs->MoveNext();
					relations.push_back(relation);
			}
			pRs->Close();
		}
		delete pRs;
	}
	catch(CADOException &ex)
	{

	}

	return relations;
}
list<CDBLocation> CDBService::GetAllLocations()
{
		list<CDBLocation> locs;
		try
		{
			
		ADODB::CADORecordset* pRs = new ADODB::CADORecordset(&dBase);
		if(pRs->Open("Locations", CADORecordset::openTable))
		{
			if(!pRs->IsEof())
				 pRs->MoveFirst();
			while(!pRs->IsEof())
			{
					CDBLocation location;
					pRs->GetFieldValue("ID", location.ID);
					pRs->GetFieldValue("Name", location.Name);
					pRs->GetFieldValue("Lat", location.Lat);
					pRs->GetFieldValue("Lon", location.Lon);
					pRs->GetFieldValue("Country", location.Country);
					pRs->GetFieldValue("City", location.City);
					pRs->MoveNext();
					locs.push_back(location);
			}
			pRs->Close();
		}
		delete pRs;
		}
		catch(CADOException &ex)
		{

		}
		return locs;
}


list<CDBPolyLine> CDBService::GetAllPolylines()
{
	list<CDBPolyLine> lines;
	try
	{
		ADODB::CADORecordset* pRs = new ADODB::CADORecordset(&dBase);
		if(pRs->Open("PolyLines", CADORecordset::openTable))
		{
			if(!pRs->IsEof())
				 pRs->MoveFirst();
			while(!pRs->IsEof())
			{
					CDBPolyLine line;
					int type;

					pRs->GetFieldValue("ID", line.ID);
					pRs->GetFieldValue("Name", line.Name);
					pRs->GetFieldValue("Loc1_ID", line.Loc1_ID);
					pRs->GetFieldValue("Loc2_ID", line.Loc2_ID);
					pRs->GetFieldValue("Geom", line.Geom);
					pRs->GetFieldValue("Distance", line.Distance);
					pRs->GetFieldValue("Time", line.Time);
					pRs->GetFieldValue("Type", type);
					
					line.Type = (CDBPolyLine::_polyLineType) type;
					lines.push_back(line);
					pRs->MoveNext();

			}
			pRs->Close();
		}
		delete pRs;
	}
	catch(CADOException &ex)
	{

	}
	return lines;
}
bool	CDBService::AddPolyline(CDBPolyLine &line)
{
	try
	{
		if(dBase.IsOpen()==FALSE)
			throw CADOException("Open error!");
		ADODB::CADORecordset* pRs = new ADODB::CADORecordset(&dBase);
		if(pRs->Open("PolyLines", CADORecordset::openTable))
		{
			int type = (int)line.Type;
			pRs->AddNew();
					pRs->SetFieldValue("Name", line.Name);
					pRs->SetFieldValue("Loc1_ID", line.Loc1_ID);
					pRs->SetFieldValue("Loc2_ID", line.Loc2_ID);
					pRs->SetFieldValue("Geom", line.Geom);
					pRs->SetFieldValue("Distance", line.Distance);
					pRs->SetFieldValue("Time", line.Time);
					pRs->SetFieldValue("Type", type);
			pRs->Update();
			pRs->Close();
		}
		delete pRs;
		CDBPolyLine* l = GetPolyline(line.Name);
		if(l == NULL)
		{
			throw CADOException("CDBService::AddPolyline: add error.");
		}
		line.ID = l->ID;
		delete l;
	}
	catch(CADOException &ex)
	{
		return false;
	}
	return true;		
}
CDBPolyLine*  CDBService::GetPolyline(CString name)
{
	CDBPolyLine *line = new CDBPolyLine();
	
	try
	{
		if(dBase.IsOpen()==FALSE)
			throw CADOException("Open error!");
		ADODB::CADORecordset* pRs = new ADODB::CADORecordset(&dBase);
		if(pRs->Open("PolyLines", CADORecordset::openTable))
		{
			pRs->SetFilter(CString("Name = '"+name+"'"));
			if(!pRs->IsEof())
				 pRs->MoveFirst();
			if(!pRs->IsEof())
			{
					int type;
					pRs->GetFieldValue("ID", line->ID);
					pRs->GetFieldValue("Name", line->Name);
					pRs->GetFieldValue("Loc1_ID", line->Loc1_ID);
					pRs->GetFieldValue("Loc2_ID", line->Loc2_ID);
					pRs->GetFieldValue("Geom", line->Geom);
					pRs->GetFieldValue("Distance", line->Distance);
					pRs->GetFieldValue("Time", line->Time);
					pRs->GetFieldValue("Type", type);
					line->Type = (CDBPolyLine::_polyLineType) type;
			}
			pRs->Close();
		}
		delete pRs;
	}
	catch(CADOException &ex)
	{
		delete line;
		return NULL;
	}
	return line;
}
bool  CDBService::DeletePolyline(int ID)
{
	try
	{
		if(dBase.IsOpen()==FALSE)
			throw CADOException("Open error!");
		ADODB::CADORecordset* pRs = new ADODB::CADORecordset(&dBase);
		CADOCommand* pCommand = new CADOCommand(&dBase);
		
		CADOParameter pParamIn(CADORecordset::typeInteger, sizeof(int));
		pParamIn.SetValue(ID);
		pCommand->AddParameter(&pParamIn);
		pCommand->SetText("DELETE FROM PolyLines where ID = ?");
		pCommand->SetType(CADOCommand::typeCmdText);
		bool f1 = pRs->Execute(pCommand);
		pRs->Close();
		delete pRs;
		delete pCommand;
	}
	catch(CADOException &ex)
	{
		return false;
	}
	return true;
}

bool CDBService::ExportToExcel(TExcelExportProps props)
{
	CDBExcelExport xls(this,props);
	return xls.Start();
}

