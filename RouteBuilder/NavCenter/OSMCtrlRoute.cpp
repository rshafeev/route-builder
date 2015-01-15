#include "StdAfx.h"
#include "OSMCtrlRoute.h"


COSMCtrlRoute::COSMCtrlRoute(void)
{
}


COSMCtrlRoute::~COSMCtrlRoute(void)
{
	
}
bool COSMCtrlRoute::IsExistPolylineIntoRoute(int &lineID)
{
	for(list<COSMCtrlPolylineEx*>::iterator i = lines.begin(); i!= lines.end(); i++)
	{
		if((*i)->m_dbPolyLine.ID == lineID)
			return true;
	}
	return false;
}

bool COSMCtrlRoute::AddPolyline(COSMCtrlPolylineEx *line)
{

	dbroute.relations.push_back(CDBRelation(this->dbroute.ID,line->m_dbPolyLine.ID));
	lines.push_back(line);

	return true;
}

bool COSMCtrlRoute::DeletePolyline(int lineID)
{
	bool f  = false;
	for(list<COSMCtrlPolylineEx*>::iterator i = lines.begin(); i!= lines.end(); i++)
	{
		if((*i)->m_dbPolyLine.ID == lineID)
		{
			lines.erase(i);
			f = true;
			break;
		}
	}
	if(!f)
		return false;
	f = false;
	for(list<CDBRelation>::iterator i =  dbroute.relations.begin(); i!= dbroute.relations.end(); i++)
	{
		if(i->Line_ID == lineID)
		{
			f = true;
			dbroute.relations.erase(i);
			break;
		}
	}
	if(!f)
		return false;
	return true;
}

COSMCtrlPolylineEx* COSMCtrlRoute::GetPolylineByID(int lineID)
{
	
	for(list<COSMCtrlPolylineEx*>::iterator i = lines.begin(); i!= lines.end(); i++)
	{
		if((*i)->m_dbPolyLine.ID == lineID)
			return *i;
	}
	
	return NULL;

}

bool COSMCtrlRoute::GetNextLocs(int PrevID,int CurID, int &nextID)
{
	for(list<CDBRelation>::iterator i = dbroute.relations.begin(); i!= dbroute.relations.end(); i++)
	{
		COSMCtrlPolylineEx* line =  GetPolylineByID(i->Line_ID);
		if(line==NULL)
			continue;
		if(line->m_dbPolyLine.Loc1_ID ==CurID&& PrevID!=line->m_dbPolyLine.Loc2_ID)
		{
			nextID = line->m_dbPolyLine.Loc2_ID;
			return true;
		}
		else
		if(line->m_dbPolyLine.Loc2_ID ==CurID&& PrevID!=line->m_dbPolyLine.Loc1_ID)
		{
			nextID = line->m_dbPolyLine.Loc1_ID;
			return true;
		}
	}
	return false;
}

list<int> COSMCtrlRoute::GetSortedIDLocations()
{
	list<int> locs;
	int curID = dbroute.LocFirst_ID;
	int nextID = -1;
	int prevID = dbroute.LocFirst_ID;
	locs.push_back(curID);
	while(GetNextLocs(prevID,curID,nextID))
	{
		prevID = curID;
		locs.push_back(nextID);
		curID = nextID;
		if(curID== dbroute.LocFinish_ID)
			break;
	}
	return locs;
}


COSMCtrlPolylineEx* COSMCtrlRoute::GetLine(int &loc1ID, int &loc2ID)
{
	for(list<COSMCtrlPolylineEx*>::iterator i = lines.begin(); i!= lines.end(); i++)
	{
		if(((*i)->m_dbPolyLine.Loc1_ID == loc1ID && (*i)->m_dbPolyLine.Loc2_ID == loc2ID )||
			((*i)->m_dbPolyLine.Loc1_ID == loc2ID && (*i)->m_dbPolyLine.Loc2_ID == loc1ID )
			)
		{
			return *i;
		}
	}
	return NULL;
}
