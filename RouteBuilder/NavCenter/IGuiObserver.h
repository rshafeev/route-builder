#pragma once
#include "..\NavCenter.DB\DBModel.h"
#include "OSMCtrlEx.h"
class CGuiSubject;
class IGuiObserver
{
protected:
	IGuiObserver();
	CGuiSubject* subject;
	void SetSubject(CGuiSubject* subject);
public:
	virtual void OnAddMarker(CDBLocation&) = NULL;
	virtual void OnDeleteMarker(int &locID, CString &Name) = NULL;
	virtual void OnSelectMarker(int&){};

	virtual void OnAddPolyline(CDBPolyLine&){}
	virtual void OnDeletePolyline(int&){}
	virtual void OnSelectPolyline(int&){}
	virtual void OnUnselectedPolylines(bool unselectedEx){}

	virtual void OnRefreshData(COSMCtrlEx& ctrl, bool forcibly){}
	virtual void OnFreeAllData(){}

	virtual void OnOpenManualAddPolyline(TAddPolylineProps props){}
	virtual void OnCloseManualAddPolyline(){}

	virtual void OnAddRoute(CDBRoute&){}
	virtual void OnDeleteRoute(int&){}
	virtual void OnSelectRoute(int &){}
	virtual void OnChangePolylineToRoute(int &routeID, int &oldPolylineID,int &newPolylineID){}
public:
	virtual ~IGuiObserver(void);
};

