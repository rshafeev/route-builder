#pragma once
#include "IVertex.h"
#include "mathLib.h"
namespace VRPTW
{
class CSolutionData;
class MATHLIB_API CDepot : public IVertex
{
	TDepotData& data;
public:
	CDepot(TDepotData& rawdata,CSolutionData& solutionData);
	~CDepot(void);
	TDepotData& GetData();
};
}

