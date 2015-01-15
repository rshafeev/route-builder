
#pragma once

#include "DBPolyline.h"


class CDBLocation
{

public:
	int ID;
	CString Name;
	double Lat;
	double Lon;
	CString Country;
	CString	City;
	CDBLocation()
	{
		
	}
	CDBLocation( const CDBLocation& dv)
	{
		ID = dv.ID;
		Lat = dv.Lat;
		Lon = dv.Lon;
		Country = dv.Country;
		City = dv.City;
        Name = dv.Name;
	}
	CString GetIdS()
	{
		CString s;
		s.Format("%d",ID);
		return s;
	}
	CString GetLatS()
	{
		CString s;
		s.Format("%f",Lat);
		return s;
	}
	
	CString GetLonS()
	{
		CString s;
		s.Format("%f",Lon);
		return s;
	}
};


class CDBRelation
{
public:
	CDBRelation(){}
	CDBRelation(int Route_ID, int Line_ID)
	{
		this->Route_ID = Route_ID;
		this->Line_ID  = Line_ID; 
	}
	int Route_ID;
	int Line_ID;
};



class NAVCENTERDB_API CDBCombineRoute
{
 public:
   int     Route_ID;
   CString Route_Name;
   int     LocFirst_ID;
   int     LocFinish_ID;
   int     Line_ID;
   int     Loc1_ID;
   int     Loc2_ID;
   CString Line_Name;
   CString Geom;
   double  Distance;
   long    Time;
   CDBPolyLine::_polyLineType     Type;

   CDBCombineRoute();
   ~CDBCombineRoute();
};


class CDBRoute
{
public:
	int ID;
	CString Name;
	int LocFirst_ID;
	int LocFinish_ID;
    list<CDBRelation> relations;
    
    CString GetIdS()
	{
		CString s;
		s.Format("%d", ID);
		return s;
	}
};
