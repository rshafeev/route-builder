#pragma once
#include "MathLib.h"
#include "IVertex.h"
#include "Route.h"
#include <queue>
#include "SolutionData.h"
#include "SolveVRPTWException.h"
#include "Solution.h"
namespace VRPTW
{

class CTabuElem
{
public:
	enum _tabuElemType
	{
		c_two_opt = 1,
		c_or_opt = 2
	};
public:
	CTabuElem()
	{
		route1 = NULL;
		route2 = NULL;
		r1 = NULL;
		r2 = NULL;
		cost = -10000;
	}
	CRoute* route1;
	CRoute* route2;
	CRelation* r1;
	CRelation* r2;
	_tabuElemType type;
	double cost;
	bool IsValid()
	{
		if(route1==NULL || route2 == NULL)
			return false;
		return true;
	}
	void Execute()
	{
		if(type== c_two_opt)
		{
			Execute2opt();
		}
		else
			if(type== c_or_opt)
				ExecuteORopt();
	}
private:
	bool ExecuteORopt()
	{
		list<CRelation*>::iterator iter1;
		list<CRelation*>::iterator iter2;
		CRelation *r11_12, * r11_22, *r22_12;
		CRelation *r21_22, *r22_23, * r21_23;
		try
		{
		list<CRelation*> &rels1 = route1->GetAllRelations(); 
		list<CRelation*> &rels2 = route2->GetAllRelations(); 

		CSolutionData& solutionData = route1->GetVehicle().GetSolutionData();
		bool f= false;
		
		iter1 = find(rels1.begin(),rels1.end(),r1);
		if(iter1==route1->GetAllRelations().end())
			throw CSolveVRPTWException(CSolveVRPTWException::c_iterator);
		iter2 = find(rels2.begin(),rels2.end(),r2);
		if(iter2==route2->GetAllRelations().end())
			throw CSolveVRPTWException(CSolveVRPTWException::c_iterator);



		if(iter1 == rels1.end())
			throw CSolveVRPTWException(CSolveVRPTWException::c_iterator);
		r11_12 = *iter1;
	
		if(iter2 == rels2.end())
			throw CSolveVRPTWException(CSolveVRPTWException::c_iterator);
		r21_22 = *iter2;
		iter2++; 
		if(iter2==route2->GetAllRelations().end())
			throw CSolveVRPTWException(CSolveVRPTWException::c_iterator);
		r22_23 = *iter2;
		iter2--;
	
		r11_22 = solutionData.GetRelation(&r11_12->GetBegin(),&r21_22->GetEnd());
		if(r11_22 == NULL)
		{
			throw CSolveVRPTWException(CSolveVRPTWException::c_RelationNotFound);
			return false;
		}
		r22_12 = solutionData.GetRelation(&r21_22->GetEnd(),&r11_12->GetEnd());
		if(r22_12 == NULL)		
		{
			throw CSolveVRPTWException(CSolveVRPTWException::c_RelationNotFound);
			return false;
		}
		r21_23 = solutionData.GetRelation(&r21_22->GetBegin(),&r22_23->GetEnd());
		if(r21_23 == NULL)	
		{
			throw CSolveVRPTWException(CSolveVRPTWException::c_RelationNotFound);
			return false;
		}
		

		iter1 = rels1.insert(iter1,r22_12);
		iter1 = rels1.insert(iter1,r11_22);
		if(iter1==rels1.end())
			throw CSolveVRPTWException(CSolveVRPTWException::c_iterator);
		iter1++;
		if(iter1==rels1.end())
			throw CSolveVRPTWException(CSolveVRPTWException::c_iterator);
		iter1++;
		if(iter1==rels1.end())
			throw CSolveVRPTWException(CSolveVRPTWException::c_iterator);
		iter1 = route1->GetAllRelations().erase(iter1);

		iter2 = route2->GetAllRelations().insert(iter2,r21_23);
		if(iter2==rels2.end())
			throw CSolveVRPTWException(CSolveVRPTWException::c_iterator);
		iter2++;
		if(iter2==rels2.end())
			throw CSolveVRPTWException(CSolveVRPTWException::c_iterator);
		iter2 = route2->GetAllRelations().erase(iter2);
		if(rels2.empty())
			throw CSolveVRPTWException(CSolveVRPTWException::c_iterator);
		iter2 = route2->GetAllRelations().erase(iter2);
		
		route1->freightSize += ((CQue*)(&r21_22->GetEnd()))->GetData().LoadSize;
		route2->freightSize -= ((CQue*)(&r21_22->GetEnd()))->GetData().LoadSize;

		}
		catch(CSolveVRPTWException& ex)
		{
			OutputDebugString("ERROR! CSolveVRPTW::Estimate_ORopt()");
			OutputDebugString(ex.GetErrorString().c_str());
			return false;
		}

		return true;
	}
	bool Execute2opt()
	{
		list<CRelation*>::iterator iter1;
		list<CRelation*>::iterator iter2;
		CRelation *r11_12, *r12_13, * r11_22, *r22_13;
		CRelation *r21_22, *r22_23, * r21_12, *r12_23;
		list<CRelation*> &rels1 = route1->GetAllRelations(); 
		list<CRelation*> &rels2 = route2->GetAllRelations(); 	
		try
		{
		CSolutionData& solutionData = route1->GetVehicle().GetSolutionData();
		
		iter1 = find(route1->GetAllRelations().begin(),route1->GetAllRelations().end(),r1);
		if(iter1==route1->GetAllRelations().end())
			throw CSolveVRPTWException(CSolveVRPTWException::c_iterator);
		iter2 = find(route2->GetAllRelations().begin(),route2->GetAllRelations().end(),r2);
		if(iter2==route2->GetAllRelations().end())
			throw CSolveVRPTWException(CSolveVRPTWException::c_iterator);

		if(iter1 == rels1.end())
			throw CSolveVRPTWException(CSolveVRPTWException::c_iterator);
		r11_12 = *iter1;
		iter1++; 
		if(iter1 == rels1.end())
			throw CSolveVRPTWException(CSolveVRPTWException::c_iterator);
		r12_13 = *iter1;
		iter1--;

		if(iter2 == rels2.end())
			throw CSolveVRPTWException(CSolveVRPTWException::c_iterator);
		r21_22 = *iter2;
		iter2++;
		if(iter2 == rels2.end())
			throw CSolveVRPTWException(CSolveVRPTWException::c_iterator);
		r22_23 = *iter2;

		iter2--;
	
		r11_22 = solutionData.GetRelation(&r11_12->GetBegin(),&r21_22->GetEnd());
		if(r11_22 == NULL)		
			return false;
		r22_13 = solutionData.GetRelation(&r21_22->GetEnd(),&r12_13->GetEnd());
		if(r22_13 == NULL)		
			return false;
		r21_12 = solutionData.GetRelation(&r21_22->GetBegin(),&r11_12->GetEnd());
		if(r21_12 == NULL)		
			return false;
		r12_23 = solutionData.GetRelation(&r11_12->GetEnd(),&r22_23->GetEnd());
		if(r12_23 == NULL)		
			return false;

		iter1 = route1->GetAllRelations().insert(iter1,r22_13);
		iter1 = route1->GetAllRelations().insert(iter1,r11_22);
		if(iter1==rels1.end())
			throw CSolveVRPTWException(CSolveVRPTWException::c_iterator);
		iter1++;
		if(iter1==rels1.end())
			throw CSolveVRPTWException(CSolveVRPTWException::c_iterator);
		iter1++;
		if(iter1==rels1.end())
			throw CSolveVRPTWException(CSolveVRPTWException::c_iterator);
		iter1 = route1->GetAllRelations().erase(iter1);
		if(iter1==rels1.end())
			throw CSolveVRPTWException(CSolveVRPTWException::c_iterator);
		iter1 = route1->GetAllRelations().erase(iter1);

		iter2 = route2->GetAllRelations().insert(iter2,r12_23);
		iter2 = route2->GetAllRelations().insert(iter2,r21_12);
		if(iter2==rels2.end())
			throw CSolveVRPTWException(CSolveVRPTWException::c_iterator);
		iter2++;
		if(iter2==rels2.end())
			throw CSolveVRPTWException(CSolveVRPTWException::c_iterator);
		iter2++;
		if(iter2==rels2.end())
			throw CSolveVRPTWException(CSolveVRPTWException::c_iterator);
		iter2 = route2->GetAllRelations().erase(iter2);
		if(iter2==rels2.end())
			throw CSolveVRPTWException(CSolveVRPTWException::c_iterator);
		iter2 = route2->GetAllRelations().erase(iter2);

		route1->freightSize += ((CQue*)(&r21_22->GetEnd()))->GetData().LoadSize - ((CQue*)(&r11_12->GetEnd()))->GetData().LoadSize;
		route2->freightSize += ((CQue*)(&r11_12->GetEnd()))->GetData().LoadSize - ((CQue*)(&r21_22->GetEnd()))->GetData().LoadSize;

		}
		catch(CSolveVRPTWException& ex)
		{
			OutputDebugString("ERROR! CSolveVRPTW::Estimate_2opt()");
			OutputDebugString(ex.GetErrorString().c_str());
			throw CSolveVRPTWException(CSolveVRPTWException::c_iterator);
			return false;
		}
		
		return true;
	}
};

class CTabuList
{
	unsigned int MaxElems;
	vector<CRelation*>  ri_list;
	vector<CRelation*>  rj_list;
	int i,j;

	void AddToIList(CRelation* r)
	{
		if(ri_list.size() < MaxElems)
		{
			ri_list.push_back(r);
		}
		else
		{
			if(MaxElems==0)
				return;
			ri_list[i] = r;
			i++;
			if(i==ri_list.size())
				i = 0;
		}
	}
	void AddToJList(CRelation* r)
	{
		if(rj_list.size() < MaxElems)
		{
			rj_list.push_back(r);
		}
		else
		{
			if(MaxElems==0)
				return;
			rj_list[j] = r;
			j++;
			if(j==rj_list.size())
				j = 0;
		}
	}

	bool IsExistInLists(CRelation *ri, CRelation *rj )
	{
		int ni = ri_list.size();
		int nj = rj_list.size();
		for(int i=0;i < ni; i++)
			if(ri_list[i] == ri||ri_list[i] == rj)
				return false;
		
		for(int j=0;j < nj; j++)
			if(rj_list[j] == rj||rj_list[j] == ri)
				return false;
		return true;
	}
public:
	bool AddToList(CTabuElem &elem, CSolution& solution)
	{
		if(elem.route1==NULL)
		{
			AddToIList(NULL);
		}
		list<CRelation*> &rels1 = elem.route1->GetAllRelations(); 
		list<CRelation*> &rels2 = elem.route2->GetAllRelations(); 
		list<CRelation*>::iterator iter1;
		list<CRelation*>::iterator iter2;	

		AddToIList(elem.r1);
		AddToJList(elem.r2);
		
		iter2 = find(rels2.begin(),rels2.end(),elem.r2);
		if(elem.r2 != elem.route2->GetAllRelations().front())
		{
			iter2--;
			AddToJList(*iter2);
			iter2++;
		}
		if(elem.r2 != elem.route2->GetAllRelations().back())
		{
			iter2++;
			AddToJList(*iter2);
		}
		if(elem.type == CTabuElem::c_two_opt)
		{
			iter1 = find(rels1.begin(),rels1.end(),elem.r1);
			if(elem.r1 != elem.route1->GetAllRelations().front())
			{
				iter1--;
				AddToIList(*iter1);
				iter1++;
			}
			if(elem.r1 != elem.route1->GetAllRelations().back())
			{
				iter1++;
				AddToIList(*iter1);
			}
		}
		return true;
	}

	bool IsORopt(CRelation *ri, CRelation *rj )
	{
		return IsExistInLists(ri,rj);
	}
	bool Is2Opt(CRelation *ri, CRelation *rj)
	{
		return IsExistInLists(ri,rj);
	}
	CTabuList(unsigned int MaxElems);
	~CTabuList(void);
};

}

/*
class CTabuElem
{

public:
	enum _oper
	{
		c_add,
		c_del
	};
	struct TTabuElemData
	{
		CRoute* route;
		vector<IVertex*> v;
		vector<CRelation*> r;
		IVertex* after_v;
		CRelation* after_r;

		_oper operation; // 0 - delete 1 - add
	};
public:
	vector<TTabuElemData> actions;

	void Execute()
	{
		for(vector<CTabuElem::TTabuElemData>::iterator action = actions.begin(); action!= actions.end();action++)
		{
				if(action->operation == CTabuElem::c_add)
					action->route->AddAfter(action->after_v,action->after_r,action->v,action->r);
				else
				if(action->operation == CTabuElem::c_del)
					action->route->DeleteRV(action->v,action->r);

		}
	}
	double cost;
};*/