#include "StdAfx.h"
#include "Solution.h"



CSolution::CSolution(CSolutionData& sData)
	:data(sData)
{
	vector<CVehicle*> vehicles = data.GetVehicles(); 
	routes.reserve(vehicles.size());

	for(vector<CVehicle*>::iterator v = vehicles.begin(); v!=vehicles.end();v++)
		routes.push_back(new CRoute(*(*v)));
	
}
CSolution::CSolution(const CSolution& copy)
	:data(copy.data)	
{
	routes.resize(copy.routes.size());

	for(unsigned int i=0;i < copy.routes.size();i++)
	{
		routes[i] = new  CRoute(*copy.routes[i]);
	}
}
CSolutionData& CSolution::GetSolutionData()
{
	return this->data;
}

CSolution::~CSolution(void)
{
	for(vector<CRoute*>::iterator v = routes.begin(); v!=routes.end();v++)
		delete *v;
	routes.clear();
}

vector<CRoute*>& CSolution::GetRoutes()
{
	return routes;
}


bool  CSolution::IsValidRoutes()
{
	int n = routes.size();
	for(int i=0;i < n;i++)
	{
		if(routes[i]->IsValid()==false)
			return false;
	}
	return true;
}

double CSolution::GetWholeCost()
{
	double cost = 0.0;

	for(vector<CRoute*>::iterator  i= routes.begin();i!=routes.end();i++)
	{
		cost +=(*i)->GetCost();
	}
	return cost;
}
