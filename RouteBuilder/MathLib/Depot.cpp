#include "StdAfx.h"
#include "Depot.h"


CDepot::CDepot(TDepotData& rawdata,CSolutionData& solutionData)
	:IVertex(c_Depot,solutionData),
	data(rawdata)
{
}


CDepot::~CDepot(void)
{
}
TDepotData& CDepot::GetData()
{
	return this->data;
}
