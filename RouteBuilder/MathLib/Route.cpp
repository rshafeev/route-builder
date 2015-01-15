#include "StdAfx.h"
#include "Route.h"
#include "Vehicle.h"
#include "Solution.h"
CRoute::CRoute(CVehicle& v)
	:vehicle(v)
{
	relations.reverse();
	freightSize = 0; 
}
CRoute::CRoute(const CRoute& copy)
	:vehicle(copy.vehicle)
{
	relations.resize(copy.relations.size());
	std::copy(copy.relations.begin(),copy.relations.end(),relations.begin());
	this->freightSize = copy.freightSize;

}

CRoute::~CRoute(void)
{
	relations.clear();
}

double CRoute::GetCost()
{
	double cost = 0;
	for(list<CRelation*>::iterator it=relations.begin();it!=relations.end();it++)
	{
		cost+=(*it)->GetCost(this->vehicle);
	}
	return cost;
}
CVehicle& CRoute::GetVehicle()
{
	return this->vehicle;
}

bool CRoute::IsValid()
{
	int n = relations.size();
	if(n==0)
		return false;
	if(&relations.front()->GetBegin()!=&this->vehicle)
		return false;
	if(relations.back()->GetEnd().GetType()!=c_Depot)
		return false;
	int i=0;
	if(n==1)
	{
		return true;
	}

	for(list<CRelation*>::iterator it = relations.begin(); it!= relations.end(); it++)
	{
		if(it == relations.end())
			return false;
		IVertex* v1 = &(*it)->GetEnd(); it++;
		if(it == relations.end())
			return false;
		IVertex* v2 = &(*it)->GetBegin();it--;
		if(v1!=v2)
			return false;
		i++;
		if(i==n-1)
			break;
	}
	return true;
}
bool CRoute::InsertAfter(list<CRelation*>::iterator ind, CRelation* value)
{
	return true;
}

bool CRoute::DeleteAfter(list<CRelation*>::iterator ind,int count)
{
	return true;
}
bool CRoute::InsertAfter( CRelation* after, CRelation* value)
{
	if(after==NULL||relations.empty()==true)
		this->relations.push_front(value);
	else
	{
		for(list<CRelation*>::iterator r = relations.begin(); r!= relations.end(); r++)
			if((*r) == after)
			{
				relations.insert(r,value);
				break;
			}
	}
	return true;
}

bool CRoute::DeleteAfter( CRelation* after, int count)
{
	if(relations.empty()==true)
		return false;
	if(after==NULL)
		this->relations.pop_front();
	else
	{
		for(list<CRelation*>::iterator r = relations.begin(); r!= relations.end(); r++)
			if((*r) == after)
			{
				r++;
				for(int i = 0;i < count ; i++)
				{
					
					r = relations.erase(r);
				}
				break;
			}
	}
	return true;
}
bool CRoute::InsertRelationToEnd(CRelation* relation)
{
	relations.push_back(relation);
	return true;
}
bool CRoute::Replace(CRelation* Insert_r1,CRelation* Insert_r2,list<CRelation*>::iterator Delete_r)
{
	if(Delete_r == this->relations.end())
		return false;
	Delete_r = relations.insert(Delete_r,Insert_r2);
	Delete_r = relations.insert(Delete_r,Insert_r1);
	Delete_r++;
	Delete_r++;
	relations.erase(Delete_r);
	return true;
}

bool CRoute::Replace(CRelation* Insert_r1,CRelation* Insert_r2,CRelation* Delete_r)
{
	for(list<CRelation*>::iterator it = relations.begin();it!=relations.end();it++)
	{
		if(*it == Delete_r)
		{
			it = relations.erase(it);
			it = relations.insert(it,Insert_r2);
			relations.insert(it,Insert_r1);
			return true;
		}
	}
	return false;
}
list<CRelation*>& CRoute::GetAllRelations()
{
	return this->relations;
}
