#include "StdAfx.h"
#include "Vehicle.h"
#include "SolutionData.h"


CVehicle::CVehicle(TVehicleData& rawdata,CSolutionData &solutionData)
	:IVertex(c_Vehicle,solutionData),
	data(rawdata)
{
}


CVehicle::~CVehicle(void)
{

}
TVehicleData& CVehicle::GetData()
{
	return this->data;
}
