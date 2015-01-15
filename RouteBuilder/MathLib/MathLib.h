// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MATHLIB_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MATHLIB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#pragma once
#ifdef MATHLIB_EXPORTS
#define MATHLIB_API __declspec(dllexport)
#else
#define MATHLIB_API __declspec(dllimport)
#endif


#include <vector>
#include <string>
using namespace std;
// This class is exported from the MathLib.dll
namespace VRPTW
{
enum _algType
{
	c_Solomon  = 1,
	c_TabuSearch = 2
};
enum _VertexType
{
	c_Depot,
	c_Vehicle,
	c_Que
};
struct MATHLIB_API TSettingsSolveVRPTW
{
	_algType algType;
	double SoftTimeWindowCoef; // [0,1] if SoftTimeWindowCoef < 0 => HardTimeWindow
	double ReserveTimeCoef; // [0,1] if ReserveTimeCoef< 0 => reserve time не учитывается
	double Pr_kr; //[0,1]
	int NStop;    // >=1
	int SolutionCount; //>=1
	int TabuListCount; //>=0
	bool IsMinimumCars;
};
struct TRelationData
{
	TRelationData()
	{

	}
	TRelationData(int idBegin, int idEnd,double length, int time,_VertexType BeginType, _VertexType EndType)
	{
		this->idBegin = idBegin;
		this->idEnd = idEnd;
		this->length = length;
		this->time = time;
		this->BeginType = BeginType;
		this->EndType = EndType;
	}
	int idBegin;
	int idEnd;
	double length;
	int time;
	_VertexType BeginType; // 0 - car, 1 - que
	_VertexType EndType;   // 0 - que, 1 - depot

};
struct MATHLIB_API TQueData
{
	TQueData()
	{

	}
	TQueData(int ID, int tA, int dtA, int tAB, double LatA,double LonA, double LatB, double LonB,unsigned int LoadSize)
	{
		this->queID = ID;
		this->tA = tA;
		this->dtA = dtA;
		this->tAB;
		this->LatA = LatA;
		this->LatB = LatB;
		this->LonA = LonA;
		this->LonB = LonB;
		this->LoadSize = LoadSize;
	}
	int queID;
	int tA;  // теоретическое время, к которому нужно обслужить данную заявку
	int dtA; // время ожидания клиентом АТС
	int tAB; // время, затраченное на выполнение заявки(для таксопарка: время передвижения из пункта А в пункт В)
	double LatA, LonA; // положение пункта А
	double LatB, LonB; // положение пункта В
	unsigned int LoadSize; // объем груза, необходимого для доставки
};
struct MATHLIB_API TDepotData
{
	TDepotData()
	{
	}
	TDepotData(int id,double Lat, double Lon)
	{
		this->depotID = id;
		this->Lat = Lat;
		this->Lon = Lon;

	}
	int depotID;
	double Lat,Lon;
};
struct MATHLIB_API TVehicleData
{
	TVehicleData()
	{

	}
	TVehicleData(int id, double Lat, double Lon,int tFree,int MaxCapacity)
	{
		this->vechID = id;
		this->Lat = Lat;
		this->Lon = Lon;
		this->tFree = tFree;
		this->MaxCapacity = MaxCapacity;
	}
	int vechID;
	double Lat;
	double Lon;
	int tFree;
	int MaxCapacity;
};

struct CLocation
{
	double Lat;
	double Lon;
};
}