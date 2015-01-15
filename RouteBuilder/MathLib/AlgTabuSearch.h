#pragma once
#include "SolutionData.h"
#include "Solution.h"
#include "TabuList.h"
#include "MathLib.h"
namespace VRPTW
{
class CAlgTabuSearch
{
	CSolutionData* solutionData;
	bool Estimate_2opt(CRoute *route1, CRoute *route2, list<CRelation*>::iterator &ri,list<CRelation*>::iterator &rj,CTabuElem &bestNxSolutions);
	bool Estimate_ORopt(CRoute *route1, CRoute *route2, list<CRelation*>::iterator &ri,list<CRelation*>::iterator &rj,CTabuElem &bestNxSolutions,bool isCostFactor);
public:
	CAlgTabuSearch(void);
	CSolution* Solve(CSolution& initSolution);
	~CAlgTabuSearch(void);
};

}