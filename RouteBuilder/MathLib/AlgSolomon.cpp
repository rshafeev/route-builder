#include "StdAfx.h"
#include "AlgSolomon.h"
#include <stdlib.h>
#include <time.h>

CAlgSolomon::CAlgSolomon(void)
{
}


CAlgSolomon::~CAlgSolomon(void)
{
}

vector<CQue*> GetRandomQuers(vector<CQue*>& ques)
{
	vector<CQue*> r;
	int n = ques.size();
	r.reserve(n);
	vector<CQue*> temp;
	temp.reserve(n);
	for(int i=0;i<n;i++)
		temp.push_back(ques[i]);
	int t = 0;
	while(t<=n-1)
	{
		int ind = (n-t-1)*rand()/RAND_MAX;
		r.push_back(temp[ind]);
		temp.erase(temp.begin()+ind);
		t++;
	}
	return r;
}
int CAlgSolomon::Solve(CSolution& solution )
{
	srand ((unsigned int) (time(NULL)/2) );
	// инициализация первой вершины
	vector<CRoute*> routes  = solution.GetRoutes();
	CSolutionData& data = solution.GetSolutionData();
	
	
	//vector<CQue*> freeQues = GetRandomQuers( data.GetQues());
	vector<CQue*> freeQues = data.GetQues();//!!!!!!!!!!!!!!!!!ROMA
	int i = 0;
	while(freeQues.empty()==false)
	{
	for(vector<CRoute*>::iterator  route = routes.begin();route!=routes.end();route++)
	{
		CQue* q = freeQues.back();
		CRelation* r1;
		if((*route)->GetAllRelations().empty())
			r1 = data.GetRelation(&(*route)->GetVehicle(),q);
		else
			r1 = data.GetRelation(&(*route)->GetAllRelations().back()->GetEnd(),q);
		if( (*route)->freightSize + q->GetData().LoadSize<= (*route)->GetVehicle().GetData().MaxCapacity)
		{
			(*route)->InsertRelationToEnd(r1);
			(*route)->freightSize += q->GetData().LoadSize;
			freeQues.pop_back();
		}
		if(freeQues.empty()==true) break;
	}
	break;
	}
	
	for(vector<CRoute*>::iterator  route = routes.begin();route!=routes.end();route++)
	{
		CRelation* r1;
		if((*route)->GetAllRelations().empty())
			r1 = data.GetRelation(&(*route)->GetVehicle(),data.GetDepots().back());
		else
			r1 = data.GetRelation(&(*route)->GetAllRelations().back()->GetEnd(),data.GetDepots().back());

		(*route)->InsertRelationToEnd(r1);
	}
	//return 1;
	// Решение методом Соломона
	double a1=0.9,a2=0.1, lamda = 0.1, mu = 0.9;
	int iter = 0;
	while(freeQues.empty()==false)
	{
		iter++;
		if(iter>1000)
			return 0;
		for(vector<CRoute*>::iterator  route= routes.begin();route!=routes.end();route++)
		{
			double best_Cost = 0;
			vector<CQue*>::iterator  best_U;
			list<CRelation*>::iterator best_relation;
			CRelation* best_Ur1=NULL, *best_Ur2;
			for(vector<CQue*>::iterator u = freeQues.begin(); u!=freeQues.end();u++)
			{
				double d = sqrt((*u)->GetData().LatA*(*u)->GetData().LatA+(*u)->GetData().LonA*(*u)->GetData().LonA);
				if( (*route)->freightSize + (*u)->GetData().LoadSize> (*route)->GetVehicle().GetData().MaxCapacity)
				{
					continue;
				}
				for(list<CRelation*>::iterator r12 = (*route)->GetAllRelations().begin(); r12!=(*route)->GetAllRelations().end(); r12++)
				{
					CRelation* ur1,* ur2 ;
					ur1 = data.GetRelation(&(*r12)->GetBegin(),*u);
					ur2 = data.GetRelation(*u,&(*r12)->GetEnd());
					if(ur1==NULL||ur2==NULL) continue;
					
					double c11 = ur1->GetCost((*route)->GetVehicle()) + (ur2)->GetCost((*route)->GetVehicle()) - mu*(*r12)->GetCost((*route)->GetVehicle());
					double c12 ; // изменение времени приезда в конечную вершину 2
					c12  = (*r12)->GetEnd().GetReal_tB() +  ur1->GetRelationData().time + ((CQue*)&ur1->GetEnd())->tAB + ur2->GetRelationData().time;
					c12 -= (*r12)->GetEnd().GetReal_tA();
					c12 = 0.0;
					
					double c1 = a1*c11 + a2*c12;
					double c2 =  lamda*d - c1;
					if((best_Ur1!=NULL&& best_Cost< c2)|| best_Ur1==NULL)
					{
						best_Cost = c2;
						best_Ur1 = ur1;
						best_Ur2 = ur2;
						best_relation = r12;
						best_U = u;
					}
				}
			}
			if(best_Ur1!=NULL)
			{
				(*route)->freightSize +=(*best_U)->GetData().LoadSize;
				(*route)->Replace(best_Ur1,best_Ur2,best_relation);
				best_U  = freeQues.erase(best_U);
			}	
		}

	}

	return 1;
}
CSolution* CAlgSolomon::Solve(CSolutionData& solutionData)
{
	CSolution *currentSolution = new CSolution(solutionData);
	int r = Solve(*currentSolution);
	if(r==0) 
		return NULL;
	return currentSolution;
}
