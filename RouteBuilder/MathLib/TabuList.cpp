#include "StdAfx.h"
#include "TabuList.h"


CTabuList::CTabuList(unsigned int MaxElems)
{
	this->MaxElems = MaxElems;
	i = j = 0 ;
	if(MaxElems>0)
	{
		this->ri_list.reserve(MaxElems);
		this->rj_list.reserve(MaxElems);
	}
}


CTabuList::~CTabuList(void)
{
}
