
#include "stdafx.h"
#include "DBPolyline.h"

CString CDBPolyLine::GetIdS()
{
	CString s;
	s.Format("%d",this->ID);
	return s;
}
CString CDBPolyLine::GetDistanceS()
{
	CString s;
	s.Format("%f",this->Distance);
	return s;
}

list<TLocation> CDBPolyLine::GeomToMas(CString geom)
{

		list<TLocation> mas;
		while(geom.GetLength()!=0)
		{
			int ind1 = geom.Find("(");
			int ind2 = geom.Find(")");
			if (ind1<0|| ind2<0)
				return mas;
			CString latLonS = geom.Mid(ind1+1, ind2 - ind1-1);
			geom.Delete(0,ind2+2);
			ind1 = latLonS.Find(";");
			if(ind1<0)
				return mas;
			double lat,lon;
			lat = atof(latLonS.Mid(0,ind1));
			lon = atof(latLonS.Mid(ind1+1,latLonS.GetLength()-ind1-1));
			mas.push_back(TLocation(lat,lon));

		}
		return mas;
}
 CString CDBPolyLine::MasToGeom(list<TLocation> mas)
{
		CString geom="";
		for(list<TLocation>::iterator i = mas.begin(); i!= mas.end(); i++)
		{
			CString add;
			add.Format("(%f;%f)",i->Lat,i->Lon);
			geom+=add + ",";
		}
		if(geom!="")
			geom.Delete(geom.GetLength()-1,1);
		return geom;
}
 CDBPolyLine::_polyLineType CDBPolyLine::GetType(CString type)
{
		if(type=="Ship")
			return CDBPolyLine::c_ship;
		else
		if(type=="Air")
			return CDBPolyLine::c_air;
		else
		if(type=="Road")
			return CDBPolyLine::c_road;
		else
		if(type=="Train")
			return CDBPolyLine::c_train;
		else
			throw exception("Error _polyLineType value ");
}
CString CDBPolyLine::GetType(_polyLineType type)
{
		switch(type)
		{
			case c_ship: return "Ship";
			case c_air: return "Air";
			case c_road: return "Road";
			case c_train: return "Train";
		}
		return NULL;
}	