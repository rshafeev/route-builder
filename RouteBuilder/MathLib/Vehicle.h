#pragma once

#include "IVertex.h"
#include "MathLib.h"
namespace  VRPTW
{
class CSolutionData;
class MATHLIB_API CVehicle : public IVertex
{
	TVehicleData& data;
public:
	CVehicle(TVehicleData& rawdata,CSolutionData& solutionData);
	~CVehicle(void);

	TVehicleData& GetData();
	void AddRelation();

	

};
}

