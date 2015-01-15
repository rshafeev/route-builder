#include "StdAfx.h"
#include "SolveVRPTW.h"
#include "SolveVRPTWException.h"
#include "AlgTabuSearch.h"
#include "AlgSolomon.h"
CSolveVRPTW::CSolveVRPTW(CVRPTWData &data)
	:rowData(data)
{
		solutionData = NULL;
		currentSolution = NULL;
}

CSolveVRPTW::~CSolveVRPTW(void)
{
	if(solutionData)
	delete solutionData;
	if(currentSolution)
		delete currentSolution;
	currentSolution = NULL;
	solutionData = NULL;
}

bool CSolveVRPTW::AddQue(TQueData que)
{
	TQueData *copy = new TQueData(que);
	rowData.ques.push_back(copy);
	return true;
}
bool CSolveVRPTW::AddVehicle(TVehicleData vehicle)
{
	TVehicleData* copy = new TVehicleData(vehicle);
	rowData.vehicles.push_back(copy);
	return true;
}
bool CSolveVRPTW::AddDepot(TDepotData depot)
{
	TDepotData* copy = new TDepotData(depot);
	rowData.depots.push_back(copy);
	return true;
}
bool CSolveVRPTW::AddRelation(TRelationData relation)
{
	TRelationData* copy = new TRelationData(relation);
	rowData.relations.push_back(copy);
	return true;
}
void CSolveVRPTW::InitSolutionData()
{
	if(solutionData)
		delete solutionData;
	if(currentSolution)
		delete currentSolution;
	currentSolution = NULL;
	solutionData = new CSolutionData(rowData);
}

CVRPTWData CSolveVRPTW::LoadStandardProblem(string fName)
{
	TSettingsSolveVRPTW set;
	set.algType = c_Solomon;
	CVRPTWData data(set);
	
	return data;
}
void  CSolveVRPTW::Initialize()
{
	InitSolutionData();
}
CSolution** CSolveVRPTW::GetCurrentSolution()
{
	return &currentSolution;
}
CSolution** CSolveVRPTW::Solve()
{
	TSettingsSolveVRPTW &sets = this->solutionData->GetData().GetSettings();
	if( sets.algType == c_TabuSearch)
	{
		CAlgSolomon solomon;
		CAlgTabuSearch tabuSearch;
		CSolution *bestSolution = NULL;
		vector<CSolution*> sols;
		int CarCount = -1;
		for(int i=0;i<max(sets.SolutionCount/2,1); i++)
		{
			CSolution* SolomonSolution = solomon.Solve(*solutionData);
			if(SolomonSolution==NULL)
				continue;
			CSolution* tabuSolution    = tabuSearch.Solve(*SolomonSolution);
			delete SolomonSolution;
			if(tabuSolution==NULL)
				continue;
			    if(CarCount<0||CarCount > tabuSolution->GetRoutes().size())
					CarCount = tabuSolution->GetRoutes().size();
				sols.push_back(new CSolution(*tabuSolution));
				delete tabuSolution;

		}
		if(sets.IsMinimumCars==true)
		{
			for(unsigned int i=0;i<sols.size(); i++)
			{
				if(sols[i]->GetRoutes().size() > CarCount+1)
				{
					sols.erase(sols.begin()+i);
					i--;
				}
			}
		}
		sets.IsMinimumCars = false;
		for(unsigned int i=0;i<sols.size(); i++)
		{
			CSolution* tabuSolution    = tabuSearch.Solve(*sols[i]);
			if(tabuSolution==NULL)
				continue;
			if(bestSolution==NULL)
				bestSolution = tabuSolution;
			else
				if(bestSolution->GetWholeCost() > tabuSolution->GetWholeCost())
				{
					delete bestSolution;
					bestSolution = tabuSolution;
				}
				else
					delete tabuSolution;

		}

		currentSolution = bestSolution;
	}
	else
		if( sets.algType == c_Solomon)
	{
		CAlgSolomon solomon;
		CSolution* solomonSolution = solomon.Solve(*solutionData);
		currentSolution = solomonSolution;
	}
	if(currentSolution==NULL)
		return NULL;
	return & currentSolution;

}

