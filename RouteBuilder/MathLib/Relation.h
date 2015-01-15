#pragma once
#include "MathLib.h"
#include "IVertex.h"

namespace VRPTW
{
class CVehicle;
class MATHLIB_API CRelation
{
	IVertex& begin;
	IVertex& end;
	TRelationData& data;
	CSolutionData& solutionData;
	bool isBuzy;
public:
	bool SetRelationData(TRelationData &data);
	bool SetBegin(IVertex& begin);
	bool SetEnd(IVertex& end);

	IVertex& GetBegin();
	IVertex& GetEnd();
	TRelationData& GetRelationData();
	CRelation(IVertex& b,IVertex& e, TRelationData& rawdata,CSolutionData& sData);
	~CRelation(void);
	double GetCost(CVehicle& vehicle); // стоимость дуги при проезде по ней машины vehicle
	bool IsBuzy(); 
	void SetState(bool isBuzy);
};

}

