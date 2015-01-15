#include "StdAfx.h"
#include "DBExcelExport.h"

CDBExcelExport::CDBExcelExport(IDBService* service, TExcelExportProps props)
{
	this->service = service;
	this->excelFile = excelFile;
	excelBase = NULL;
	this->props = props;
}


CDBExcelExport::~CDBExcelExport(void)
{
	if(excelBase)
	{
		if(excelBase->IsOpen())
			excelBase->Close();
		delete excelBase;
		excelBase = NULL;
	}
}

bool CDBExcelExport::Connect()
{
	CString strConnection = _T("Provider=Microsoft.ACE.OLEDB.12.0; Data Source=")+(CString)props.excelFile+"; Extended Properties=\"Excel 12.0; HDR=YES\"";
	bool connected = false;
	
	excelBase = new ADODB::CADODatabase();
	try
	{
			excelBase->SetConnectionString(strConnection);
			connected = excelBase->Open();
	}
	catch(...)
	{
		return false;
	}
	return connected;
}
CDBLocation* GetLocationByID(list<CDBLocation>& locations, int ID)
{
	for(list<CDBLocation>::iterator i = locations.begin(); i!= locations.end(); i++)
	{
		if(i->ID==ID)
			return &(*i);
	}
	return NULL;
}
bool CDBExcelExport::Start()
{
	if(!Connect())
		return false;
	ADODB::CADORecordset* pRs = new ADODB::CADORecordset(excelBase);
	CADOCommand* pCommand = new CADOCommand(excelBase);
	pCommand->SetType(CADOCommand::typeCmdText);

// fill locations
	bool f1 = false;
	pCommand->SetText("DROP TABLE GeoLocations");
	f1 = pRs->Execute(pCommand);
	pCommand->SetText("CREATE TABLE GeoLocations (ID NUMBER, Name TEXT, Lat FLOAT, Lon FLOAT, Country TEXT, City TEXT)");
	 f1 = pRs->Execute(pCommand);

	list<CDBLocation> locations = service->GetAllLocations();
	for(list<CDBLocation>::iterator i = locations.begin(); i!= locations.end(); i++)
	{
		CString sql;
		sql.Format("INSERT INTO GeoLocations (ID,Name,Lat,Lon,Country,City) VALUES (%d,'%s',%f,%f,'%s','%s')",
			i->ID,i->Name,i->Lat, i->Lon,i->Country, i->City);
		pCommand->SetText(sql);
		f1 = pRs->Execute(pCommand);
		if(f1==false)
		{ 
			pRs->Close();
			delete pRs;
			delete pCommand;
			return false;
		}
	}



// fill GeoData
	pCommand->SetText("DROP TABLE GeoData");
	f1 = pRs->Execute(pCommand);
	pCommand->SetText("CREATE TABLE GeoData (Route_ID NUMBER, Route_Name TEXT, LocFirst_ID NUMBER, LocFinish_ID NUMBER, Line_ID NUMBER, Loc1_ID NUMBER, Loc2_ID NUMBER, " + 
		      CString("Line_Name TEXT, Geom TEXT, Line_Distance FLOAT, Line_Time NUMBER, Line_Type NUMBER)"));
	
	f1 = pRs->Execute(pCommand);
	list<CDBCombineRoute> routes = service->GetAllCombineRoutes();
	for(list<CDBCombineRoute>::iterator i = routes.begin(); i!= routes.end(); i++)
	{
		CString sql;
		sql.Format("INSERT INTO GeoData (Route_ID,Route_Name,LocFirst_ID,LocFinish_ID,Line_ID,Loc1_ID,Loc2_ID,Line_Name,Geom,Line_Distance,Line_Time,Line_Type)"+
			           CString(" VALUES (%d      ,'%s'      ,%d         ,%d          ,%d     ,%d     ,%d     ,'%s'     ,'%s',%f      ,%d  ,%d)"),
			i->Route_ID, i->Route_Name, i->LocFirst_ID, i->LocFinish_ID, i->Line_ID, i->Loc1_ID, i->Loc2_ID,
			i->Line_Name, i->Geom,i->Distance, i->Time,i->Type);
		pCommand->SetText(sql);
		f1 = pRs->Execute(pCommand);
		if(f1==false)
		{
			pRs->Close();
			delete pRs;
			delete pCommand;	
			return false;
		}
	}


// fill Frequency
	/*
	pCommand->SetText("DROP TABLE Frequency");
	f1 = pRs->Execute(pCommand);
	pCommand->SetText("CREATE TABLE Frequency (LocFirst TEXT, LocFinish TEXT, Line_Name TEXT, Line_Type TEXT, Frequency FLOAT) ");
	f1 = pRs->Execute(pCommand);

	for(list<CDBCombineRoute>::iterator i = routes.begin(); i!= routes.end(); i++)
	{
		CString sql;
		CString LocFirst,LocFinish;
		CDBLocation* loc1 = GetLocationByID(locations,i->Loc1_ID);
		CDBLocation* loc2 = GetLocationByID(locations,i->Loc2_ID);
		if(loc1==NULL|| loc2==NULL)
			continue;
		LocFirst = loc1->Name;
		LocFinish = loc2->Name;
		float freq;
		if(i->Type == CDBPolyLine::c_air)
			freq = props.freqAir;
		else
		if(i->Type == CDBPolyLine::c_ship)
			freq = props.freqShip;
		else
		if(i->Type == CDBPolyLine::c_train)
			freq = props.freqTrain;
		else
			continue;
		CString typeS = CDBPolyLine::GetType(i->Type);
		sql.Format("INSERT INTO Frequency (LocFirst,LocFinish,Line_Name,Line_Type,Frequency)"+
			                CString(" VALUES ('%s'         ,'%s','%s'      ,'%s'      ,%f)"),
							LocFirst, LocFinish, i->Line_Name, typeS , freq);
		pCommand->SetText(sql);
		f1 = pRs->Execute(pCommand);
		if(f1==false)
		{
			pRs->Close();
			delete pRs;
			delete pCommand;	
			return false;
		}
	}
	*/
	pRs->Close();
	delete pRs;

	return true;
}
