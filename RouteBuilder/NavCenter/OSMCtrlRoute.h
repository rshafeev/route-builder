#pragma once

#include "..\NavCenter.DB\DBModel.h"
#include "OSMCtrlPolylineEx.h"


class COSMCtrlRoute
{
private: 
	bool GetNextLocs(int PrevID,int CurID, int &nextID);
	COSMCtrlPolylineEx* GetPolylineByID(int lineID);
	
public:
	CDBRoute dbroute;
	list<COSMCtrlPolylineEx*> lines;

	list<int> GetSortedIDLocations(); 
	COSMCtrlPolylineEx* GetLine(int &loc1ID, int &loc2ID);

	bool AddPolyline(COSMCtrlPolylineEx* line);
	bool DeletePolyline(int lineID);
	bool IsExistPolylineIntoRoute(int &lineID);
	//CDBRelation
	COSMCtrlRoute(void);
	~COSMCtrlRoute(void);
};

