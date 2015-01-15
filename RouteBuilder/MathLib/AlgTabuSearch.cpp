#include "StdAfx.h"
#include "AlgTabuSearch.h"
#include "TabuList.h"
#include <stdlib.h>
#include <time.h>

CAlgTabuSearch::CAlgTabuSearch(void)
{
}


CAlgTabuSearch::~CAlgTabuSearch(void)
{
	solutionData = NULL;
}


bool CAlgTabuSearch::Estimate_2opt(CRoute *route1, CRoute *route2, list<CRelation*>::iterator &ri,list<CRelation*>::iterator &rj,CTabuElem &bestNxSolutions)
{
	double c = 0.0;
	CRelation *r11_12, *r12_13, * r11_22, *r22_13;
	CRelation *r21_22, *r22_23, * r21_12, *r12_23;
	CRelation *after_r1=NULL, *after_r2=NULL;
	list<CRelation*> &rels1 = route1->GetAllRelations(); 
	list<CRelation*> &rels2 = route2->GetAllRelations(); 

	
try
{
			/*if(ri == rels1.end())
				throw CSolveVRPTWException(CSolveVRPTWException::c_iterator);*/
			r11_12 = *ri;
			ri++; 
			/*if(ri == rels1.end())
				throw CSolveVRPTWException(CSolveVRPTWException::c_iterator);*/
			r12_13 = *ri;
			ri--;

			/*if(rj == rels2.end())
				throw CSolveVRPTWException(CSolveVRPTWException::c_iterator);*/
			r21_22 = *rj;
			rj++;
			/*if(rj == rels2.end())
				throw CSolveVRPTWException(CSolveVRPTWException::c_iterator);*/
			r22_23 = *rj;

			rj--;
		if((route1->freightSize + ((CQue*)(&r21_22->GetEnd()))->GetData().LoadSize - ((CQue*)(&r11_12->GetEnd()))->GetData().LoadSize> route1->GetVehicle().GetData().MaxCapacity)||
			(route2->freightSize + ((CQue*)(&r11_12->GetEnd()))->GetData().LoadSize - ((CQue*)(&r21_22->GetEnd()))->GetData().LoadSize> route1->GetVehicle().GetData().MaxCapacity))
		{
			return false;
		}
		
			
		r11_22 = this->solutionData->GetRelation(&r11_12->GetBegin(),&r21_22->GetEnd());
		if(r11_22 == NULL)	
		{
			throw CSolveVRPTWException(CSolveVRPTWException::c_RelationNotFound);
			return false;
		}
		r22_13 = this->solutionData->GetRelation(&r21_22->GetEnd(),&r12_13->GetEnd());
		if(r22_13 == NULL)
		{
			//throw CSolveVRPTWException(CSolveVRPTWException::c_RelationNotFound);
			return false;
		}
		r21_12 = this->solutionData->GetRelation(&r21_22->GetBegin(),&r11_12->GetEnd());
		if(r21_12 == NULL)		
		{
		//	throw CSolveVRPTWException(CSolveVRPTWException::c_RelationNotFound);
			return false;
		}
		r12_23 = this->solutionData->GetRelation(&r11_12->GetEnd(),&r22_23->GetEnd());
		if(r12_23 == NULL)		
		{
		//	throw CSolveVRPTWException(CSolveVRPTWException::c_RelationNotFound);
			return false;
		}
	double c1;
	double c2;
	c1 = r11_12->GetCost(route1->GetVehicle()) + r12_13->GetCost(route1->GetVehicle()) - 
		r11_22->GetCost(route1->GetVehicle()) - r22_13->GetCost(route1->GetVehicle());
	c2 = r21_22->GetCost(route2->GetVehicle()) + r22_23->GetCost(route2->GetVehicle()) -
		r21_12->GetCost(route2->GetVehicle()) - r12_23->GetCost(route2->GetVehicle());
	if(c1 + c2 <0)
		return false;
	if(bestNxSolutions.cost < c1 + c2)
	{
		bestNxSolutions.cost = c1 + c2;
		bestNxSolutions.route1 = route1;
		bestNxSolutions.route2 = route2;
		bestNxSolutions.r1 = r11_12;
		bestNxSolutions.r2 = r21_22;
		bestNxSolutions.type = CTabuElem::c_two_opt; 
	}
}
catch(CSolveVRPTWException& ex)
{
	OutputDebugString("ERROR! CSolveVRPTW::Estimate_2opt()");
	OutputDebugString(ex.GetErrorString().c_str());

	return false;
}

	return true;
}

bool CAlgTabuSearch::Estimate_ORopt(CRoute *route1, CRoute *route2, list<CRelation*>::iterator &ri,list<CRelation*>::iterator &rj,CTabuElem &bestNxSolutions,bool isCostFactor)
{
	double c = 0.0;
	CRelation *r11_12, * r11_22, *r22_12;
	CRelation *r21_22, *r22_23,*r21_23;
	list<CRelation*> &rels1 = route1->GetAllRelations(); 
	list<CRelation*> &rels2 = route2->GetAllRelations(); 

try
{
	/*if(ri == rels1.end())
		throw CSolveVRPTWException(CSolveVRPTWException::c_iterator);*/
	r11_12 = *ri;
	
	/*if(rj == rels2.end())
		throw CSolveVRPTWException(CSolveVRPTWException::c_iterator);*/
	r21_22 = *rj;
	rj++; 
	/*if(rj==route2->GetAllRelations().end())
		throw CSolveVRPTWException(CSolveVRPTWException::c_iterator);*/
	r22_23 = *rj;
	rj--;


	r11_22 = this->solutionData->GetRelation(&r11_12->GetBegin(),&r21_22->GetEnd());
	if(r11_22 == NULL)
	{
		return false;
		//throw CSolveVRPTWException(CSolveVRPTWException::c_RelationNotFound);
	}
	r22_12 = this->solutionData->GetRelation(&r21_22->GetEnd(),&r11_12->GetEnd());
	if(r22_12 == NULL)	
	{
		return false;
		//throw CSolveVRPTWException(CSolveVRPTWException::c_RelationNotFound);
	}
	r21_23 = this->solutionData->GetRelation(&r21_22->GetBegin(),&r22_23->GetEnd());
	if(r21_23 == NULL)
	{
		return false;
		//throw CSolveVRPTWException(CSolveVRPTWException::c_RelationNotFound);
	}
	// проверяем ограничене - "грузоподъемность"
	if(r21_22->GetEnd().GetType()==c_Que)
		if(route1->freightSize + ((CQue*)(&r21_22->GetEnd()))->GetData().LoadSize> route1->GetVehicle().GetData().MaxCapacity)
			return false;

	double c1;
	double c2;
	c1 = r11_12->GetCost(route1->GetVehicle()) - r11_22->GetCost(route1->GetVehicle()) - r22_12->GetCost(route1->GetVehicle());
	c2 = r21_22->GetCost(route2->GetVehicle()) + r22_23->GetCost(route2->GetVehicle()) - r21_23->GetCost(route2->GetVehicle());

	if(bestNxSolutions.cost < c1 + c2 || bestNxSolutions.route1 == NULL||isCostFactor==false)
	{
		bestNxSolutions.cost = c1 + c2;
		bestNxSolutions.route1 = route1;
		bestNxSolutions.route2 = route2;
		bestNxSolutions.r1 = r11_12; 
		bestNxSolutions.r2 = r21_22;
		bestNxSolutions.type = CTabuElem::c_or_opt; 
	}
}
catch(CSolveVRPTWException& ex)
{
	OutputDebugString("ERROR! CSolveVRPTW::Estimate_ORopt()");
	OutputDebugString(ex.GetErrorString().c_str());

	return false;
}
	return true;
}


CSolution* CAlgTabuSearch::Solve(CSolution& initSolution)
{
	// выполняем поиск с запретами
	TSettingsSolveVRPTW &sets = initSolution.GetSolutionData().GetData().GetSettings();
	solutionData = &initSolution.GetSolutionData();

	CSolution *xt = new  CSolution(initSolution); // текущее ршение
	CSolution *xA = new  CSolution(initSolution);
	CTabuList tabuList(sets.TabuListCount);

	int stop_iter = 0;
	double p_cr = sets.Pr_kr;
	int iter = 0;
	
	if(!xt->IsValidRoutes())
			throw CSolveVRPTWException(CSolveVRPTWException::c_iterator);
	while(stop_iter< sets.NStop)
	{
		// пробегаемся по окрестности
		CTabuElem bestNxSolutions; // лучшее решение в окрестности(а точнее, необходимые изменения решения xt, чтобы получить минимум в окрестности)
		int shift =0;
		bestNxSolutions.cost = -0.1;
		srand ((unsigned int) (time(NULL)/2) );
		try
		{
		bool findFlag = false;
		for(vector<CRoute*>::iterator routei = xt->GetRoutes().begin(); routei!=xt->GetRoutes().end(); routei++)
		{
			shift++;
			if(shift>=xt->GetRoutes().size())
				break;
			for(vector<CRoute*>::iterator routej = xt->GetRoutes().begin()+shift; routej!=xt->GetRoutes().end(); routej++)
			{
				if(routei == xt->GetRoutes().end()||routej == xt->GetRoutes().end())
					throw CSolveVRPTWException(CSolveVRPTWException::c_iterator);
				if(*routei != *routej)
				{

					// пробегаемся по
					int n_ri = (*routei)->GetAllRelations().size();
					int n_rj = (*routej)->GetAllRelations().size();
					int u=0,v;
					for(list<CRelation*>::iterator ri = (*routei)->GetAllRelations().begin(); ri!= (*routei)->GetAllRelations().end(); ri++ )
					{
						v = 0;
						if(ri == (*routei)->GetAllRelations().end())
							throw CSolveVRPTWException(CSolveVRPTWException::c_iterator);

						for(list<CRelation*>::iterator rj = (*routej)->GetAllRelations().begin(); rj!= (*routej)->GetAllRelations().end(); rj++ )
						{
							// смотрим на список запретов...
							double p = rand()/(1.0*RAND_MAX);
							if(p>p_cr)
							{
								v++;
								if(v==n_rj-1) break;
								continue;
							}
							if(n_rj>1&&tabuList.IsORopt(*ri,*rj)==true)
							{
								
								if(n_ri > 1)
								{
									Estimate_2opt(*routei,*routej,ri,rj,bestNxSolutions);
								}
								bool isCostFactor;
								if(sets.IsMinimumCars==true&&n_rj==2)
									isCostFactor = false;
								else
									isCostFactor = true;
								bool r = Estimate_ORopt(*routei,*routej,ri,rj,bestNxSolutions,isCostFactor);
								if(sets.IsMinimumCars==true&&r==true&& n_rj==2)
								{
									findFlag = true;
									break;
								}
							}
							else
							if(n_ri>1&&tabuList.IsORopt(*rj,*ri)==true)
							{
								bool isCostFactor;
								if(sets.IsMinimumCars==true&&n_ri==2)
									isCostFactor = false;
								else
									isCostFactor = true;

								bool r = Estimate_ORopt(*routej,*routei,rj,ri,bestNxSolutions,isCostFactor);
								if(sets.IsMinimumCars==true&&r==true&& n_ri==2)
								{
									findFlag = true;
									break;
								}
							}
							v++;
							if(v==n_rj-1) break;
						}
						u++;
						if(u==n_ri-1) break;
						if(findFlag) break;
					}
					if(findFlag) break;
				}
					if(findFlag) break;
			}
		     if(findFlag) break;
		}
		// сизменяем текущее лучшее решение xt на локальный минимум(если он найден)
		if(bestNxSolutions.IsValid()== true)
		{
			tabuList.AddToList(bestNxSolutions,*xt);
			bestNxSolutions.Execute();
			for(vector<CRoute*>::iterator route = xt->GetRoutes().begin(); route!=xt->GetRoutes().end();)
			{
				if((*route)->GetAllRelations().size()<=1)
				{
					route = xt->GetRoutes().erase(route);
				}
				else
					 route++;
			}
		}
		else
		{
			bestNxSolutions.r1 = NULL;
			bestNxSolutions.r2 = NULL;
			bestNxSolutions.route1 = NULL;
			bestNxSolutions.route2 = NULL;
			//tabuList.AddToList(bestNxSolutions,*xt);
		}
		if(xt->GetWholeCost() < xA->GetWholeCost()||(sets.IsMinimumCars&&xA->GetRoutes().size() > xt->GetRoutes().size()))
		{
			delete xA;
			xA = new  CSolution(*xt);
			stop_iter = 0;
		}
		else
			stop_iter++;
		
		}
		catch(CSolveVRPTWException& ex)
		{
			OutputDebugString("ERROR! CSolveVRPTW::Solve()");
			OutputDebugString(ex.GetErrorString().c_str());

		}
		catch(...)
		{
			OutputDebugString("ERROR! CSolveVRPTW::Solve()");
		}
	}
	delete xt;
	return xA;

}


