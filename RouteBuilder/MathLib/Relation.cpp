#include "StdAfx.h"
#include "Relation.h"
#include "Vehicle.h"
#include "Que.h"
#include "SolutionData.h"
CRelation::CRelation(IVertex& b,IVertex& e, TRelationData& rawdata,CSolutionData& sData)
	:isBuzy(false),
	 data(rawdata),
	 solutionData(sData),
	 begin(b),
	 end(e)
{
}

CRelation::~CRelation(void)
{

}

IVertex& CRelation::GetBegin()
{
	return this->begin;
}

IVertex& CRelation::GetEnd()
{
	return this->end;

}
TRelationData& CRelation::GetRelationData()
{
	return this->data;

}
bool  CRelation::IsBuzy()
{
	return isBuzy;
}
void  CRelation::SetState(bool isBuzy)
{
	this->isBuzy  = isBuzy;
}

double  CRelation::GetCost(CVehicle& vehicle)
{
	double tReserve;
	double tSoft;
	if(this->GetEnd().GetType()==c_Depot)
		return this->data.length;
	TSettingsSolveVRPTW& sets = solutionData.GetData().GetSettings();
	if(sets.ReserveTimeCoef>0)
	{
		double t1, t2; // время освобеждения в начальной вершине и время приезда в конечную
		if(this->GetBegin().GetType()== c_Vehicle)
		{
			CVehicle* v = (CVehicle*)&this->GetBegin();
			
			t1 = v->GetData().tFree;
		}
		else
			if(this->GetBegin().GetType()== c_Que)
			{
				CQue* q = (CQue*)&this->GetBegin();
				t1 = q->GetReal_tB();
			}
		
		if(this->GetEnd().GetType()== c_Que)
		{
			CQue* q = (CQue*)&this->GetBegin();
			t2 =  q->GetReal_tA();
		}
		else
			throw exception("конечная вершина может быть только заявокй или депо.");
		tReserve = sets.ReserveTimeCoef*(t2 - t1);
	}
	else
	   tReserve = 0.0;
	if(sets.SoftTimeWindowCoef>0)
	{
		if(this->GetEnd().GetType()== c_Que)
		{
			CQue* q = (CQue*)&this->GetEnd();
			tSoft = sets.SoftTimeWindowCoef* (q->GetReal_tA() +q->dtA - q->tA );
			if(tSoft<0)
				tSoft = 0.0;
		}
		else
			throw exception("конечная вершина может быть только заявокй или депо.");
	}
	else
		tSoft = 0.0;
	
	return this->data.length+tReserve+tSoft;
}
