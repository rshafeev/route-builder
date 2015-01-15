
#pragma once

#include <list>
using namespace std;

#ifdef NAVCENTERDB_EXPORTS
#define NAVCENTERDB_API __declspec(dllexport)
#else
#define NAVCENTERDB_API __declspec(dllimport)
#endif

struct TLocation
{
public:
	TLocation(double Lat, double Lon)
	{
		this->Lat = Lat;
		this->Lon = Lon;
	}
	double Lat;
	double Lon;
};


class NAVCENTERDB_API CDBPolyLine
{
 
public:
	enum _polyLineType
	{
		c_ship = 1,
		c_air = 2,
		c_road = 3,
		c_train = 4,
	};
public:
	int ID;
	int Loc1_ID;
	int Loc2_ID;
	CString Name;
	CString Geom;
	double Distance;
	long Time;
	_polyLineType Type;
	
	static list<TLocation> GeomToMas(CString geom);
	static CString MasToGeom(list<TLocation> mas);
	static _polyLineType GetType(CString type);
	static CString GetType(_polyLineType type);

	CString GetIdS();
	CString GetDistanceS();

};


