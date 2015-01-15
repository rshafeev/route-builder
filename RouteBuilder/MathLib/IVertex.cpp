#include "StdAfx.h"
#include "IVertex.h"
#include "Relation.h"
#include "SolutionData.h"

IVertex::IVertex(_VertexType _type,CSolutionData &sData)
	:solutionData(sData)
	/*:	input(0),
		output(0)*/
{
	this->type = _type;
	input = output = 0;
	this->real_tA = this->real_tB = 0;
	this->allInputRelations.reserve(100);
	this->allOutputRelations.reserve(100);

}



IVertex::~IVertex(void)
{
}
CSolutionData& IVertex::GetSolutionData()
{
	return this->solutionData;
}
_VertexType IVertex::GetType()
{
	return this->type;
}

void IVertex::PushInput()
{
	input++;
}
void IVertex::PopInput()
{
	input--;
}
void IVertex::PushOutput()
{
	
	output++;
}
void IVertex::PopOutput()
{
	output--;
}

int IVertex::GetInput()
{
	return input;
}
int IVertex::GetOutput()
{
	return output;
}
void  IVertex::Initialize(vector<CRelation*> allInputRelations,vector<CRelation*> allOutputRelations)
{
	this->allInputRelations = allInputRelations;
	this->allOutputRelations = allOutputRelations;
}
vector<CRelation*>&  IVertex::GetAllInputRelations()
{
	return this->allInputRelations;
}
vector<CRelation*>&  IVertex::GetAllOutputRelations()
{
	return this->allOutputRelations;
}

CRelation*  IVertex::GetRelationByEndVertex(IVertex* endVertex,int bizyType)
{
	vector<CRelation*>::iterator it;
	if(bizyType==2)
	{
		for(it = allOutputRelations.begin();it!=this->allOutputRelations.end();it++)
		{
		if(&(*it)->GetEnd()==endVertex)
			return (*it);
		}
	}
	else
	{
		for(it = this->allOutputRelations.begin();it!=this->allOutputRelations.end();it++)
		{
		if((int)(*it)->IsBuzy()==bizyType&&(&(*it)->GetEnd())==endVertex)
			return (*it);
		}
	}
	return NULL;
}
CRelation*  IVertex::GetRelationByBeginVertex(IVertex* beginVertex,int bizyType)
{
	vector<CRelation*>::iterator it;
	if(bizyType==2)
	{
		for(it = allInputRelations.begin();it!=this->allInputRelations.end();it++)
		{
			if(&(*it)->GetBegin()==beginVertex)
				return (*it);
		}
	}
	else
	{
		for(it = allInputRelations.begin();it!=this->allInputRelations.end();it++)
		{
			if((int)(*it)->IsBuzy()==bizyType&&(&(*it)->GetBegin())==beginVertex)
				return (*it);
		}
	}
	return NULL;
}

int  IVertex::GetReal_tA()
{
	return this->real_tA;
}
int  IVertex::GetReal_tB()
{
	return this->real_tB;
}