#include "StdAfx.h"
#include "Que.h"


CQue::CQue(TQueData& rawdata,CSolutionData& sData)
	:IVertex(c_Que,sData),
	tA(data.tA),
	dtA(data.dtA),
	tAB(data.tAB),
	data(rawdata)
{
}


CQue::~CQue(void)
{
}

TQueData& CQue::GetData()
{
	return this->data;
}

void  CQue::SetReal_tA(int time)
{
	this->real_tA = time;
	real_tB = real_tA + this->tAB;
}
