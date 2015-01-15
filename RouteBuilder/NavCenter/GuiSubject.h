#pragma once

#include "..\NavCenter.DB\DBService.h"
#include "OSMCtrlEx.h"

#include <list>
using namespace std;

class IGuiObserver;
class CGuiSubject 
{
public:
	list<IGuiObserver*> *m_observers;
public:
//
	bool IsAttached(IGuiObserver* obs );
	void Attach(IGuiObserver* obs );
    void Detach(IGuiObserver* obs);

//
	void AddMarker(CDBLocation& loc);
	void DeleteMarker(int &locID, CString &Name);
	void SelectMarker(int locID);

	void AddPolyline(CDBPolyLine& line);
	void DeletePolyline(int lineID);
	void SelectPolyline(int lineID);
	void UnselectedPolylines(bool unselectedEx);

	void RefreshData(COSMCtrlEx& ctrl,bool forcibly);
	void FreeAllData();

	void OpenManualAddPolyline(TAddPolylineProps props);
	void CloseManualAddPolyline();


	void AddRoute(CDBRoute& route);
	void DeleteRoute(int &routeID);
	void SelectRoute(int &routeID);
	void ChangePolylineToRoute(int &routeID, int &oldPolylineID,int &newPolylineID);

	CGuiSubject(void);
	~CGuiSubject(void);
};

