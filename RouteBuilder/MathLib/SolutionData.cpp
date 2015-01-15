#include "StdAfx.h"
#include "SolutionData.h"
#include "Relation.h"


CSolutionData::CSolutionData(CVRPTWData& rawdata)
	: data(rawdata)
{
	depots.reserve(data.depots.size());
	ques.reserve(data.ques.size());
	vehicles.reserve(data.vehicles.size());
	relations.reserve(data.relations.size());
	
	for(vector<TDepotData*>::iterator i = data.depots.begin(); i!=data.depots.end();i++ )
	{
		CDepot *t = new CDepot(**i,*this);
		this->depots.push_back(t);
		this->AllVertexes.push_back(t);
	}
	for(vector<TQueData*>::iterator i = data.ques.begin(); i!=data.ques.end();i++ )
	{
		CQue*  q = new CQue(**i,*this);
		this->ques.push_back(q);
		this->AllVertexes.push_back(q);
	}

	for(vector<TVehicleData*>::iterator i = data.vehicles.begin(); i!=data.vehicles.end();i++ )
	{
		CVehicle *v = new CVehicle(**i,*this);
		this->vehicles.push_back(v);
		this->AllVertexes.push_back(v);
	}
	for(vector<TRelationData*>::iterator i = data.relations.begin(); i!=data.relations.end();i++ )
	{
		IVertex *v1 = GetVertex((*i)->idBegin,(*i)->BeginType);
		IVertex *v2 = GetVertex((*i)->idEnd,(*i)->EndType);
		if(v1==NULL|| v2 == NULL)
			continue;
		relations.push_back(new CRelation(*v1,*v2,**i,*this));
	}

	for(vector<CRelation*>::iterator j=relations.begin();j!=relations.end();j++)
	{
		for(vector<IVertex*>::iterator i=AllVertexes.begin();i!=AllVertexes.end();i++)
		{
			if(&(*j)->GetBegin()==(*i))
			{
				(*i)->allOutputRelations.push_back(*j);
			}
			else
			if(&(*j)->GetEnd()==(*i))
				(*i)->allInputRelations.push_back(*j);
		}
	}
}
CRelation* CSolutionData::GetRelation(IVertex* vBegin, IVertex* vEnd)
{
	if(vBegin->allOutputRelations.empty())
		return NULL;
	for(vector<CRelation*>::iterator i = vBegin->allOutputRelations.begin(); i!= vBegin->allOutputRelations.end(); i++)
	{
		if( &(*i)->GetEnd() == vEnd)
			return *i;
	}
	return NULL;
}

IVertex* CSolutionData::GetVertex(int id, _VertexType type)
{
	if(type == c_Que)
	{
		for(unsigned int i=0;i< ques.size();i++)
			if(ques[i]->GetData().queID==id)
				return ques[i];
	}
	else
	if(type == c_Depot)
	{
		for(unsigned int i=0;i< depots.size();i++)
			if(depots[i]->GetData().depotID==id)
				return depots[i];
	}
	else
	if(type == c_Vehicle)
	{
		for(unsigned int i=0;i< vehicles.size();i++)
				if(vehicles[i]->GetData().vechID==id)
					return vehicles[i];
	}
	return NULL;
}

CSolutionData::~CSolutionData(void)
{
	for(vector<CDepot*>::iterator i = depots.begin(); i!=depots.end();i++ )
	{
	    delete *i;
	}
	for(vector<CQue*>::iterator i = ques.begin(); i!=ques.end();i++ )
	{
		delete *i;
	}

	for(vector<CVehicle*>::iterator i = vehicles.begin(); i!=vehicles.end();i++ )
	{
		delete *i;
	}
	for(vector<CRelation*>::iterator i = relations.begin(); i!=relations.end();i++ )
	{
		delete *i;
	}
	depots.clear();
	ques.clear();
	vehicles.clear();
	relations.clear();
}
CVRPTWData& CSolutionData::GetData()
{
	return data;
}

bool CSolutionData::IsQuesEmpty()
{
	for(unsigned int i=0;i < ques.size();i++)
		if(ques[i]->GetInput()==0)
			return false;
	return true;
}

vector<CVehicle*>& CSolutionData::GetVehicles()
{
	return this->vehicles;
}
vector<CDepot*>& CSolutionData::GetDepots()
{
	return this->depots;
}
vector<CQue*>& CSolutionData::GetQues()
{
	return this->ques;
}
vector<IVertex*>& CSolutionData::GetAllVertexes()
{
	return this->AllVertexes;
}
vector<CRelation*>& CSolutionData::GetRelations()
{
	return this->relations;
}