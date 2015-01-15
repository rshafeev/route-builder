#pragma once
#include "..\inetlib\osmctrl.h"
#include "..\NavCenter.DB\DBModel.h"
#include "OSMCtrlMarkerEx.h"
#include "OSMCtrlPolylineEx.h"
#include "OSMCtrlRoute.h"
enum OSMCtrlAppTileProvider
{
  Mapnik,
  Osmarender,
  Cyclemap,
  MapquestOSM,
  MapquestOpenAerial
};
struct TAddPolylineProps
{
		CString name;
		bool IsCloudmade;
		long time;
		CDBPolyLine::_polyLineType type;
		CString agentA;
		CString agentB;
} ;
class COSMCtrlEx :	public COSMCtrl
{
  CMenu m_ContextMenu;
  COSMCtrlMapnikTileProvider             m_MapnikTileProvider;
  COSMCtrlIcon                           m_dummyDefaultIcon,m_dummySelectedIcon;
  INT_PTR                                m_nDefaultMarkerIconIndex,m_nSelectedMarkerIconIndex;	

  COSMCtrlOsmarenderTileProvider         m_OsmarenderTileProvider;
  COSMCtrlCyclemapTileProvider           m_CyclemapTileProvider;
  COSMCtrlMapquestOSMTileProvider        m_MapquestOSMTileProvider;
  COSMCtrlMapquestOpenAerialTileProvider m_MapquestOpenAerialTileProvider;
  COSMCtrlPosition m_rightClickPos;
  

private:
  bool m_isOpenManualAddPolyline;
  COSMCtrlPolyline* m_manualAddline;
  TAddPolylineProps m_manualProps;
public:
	list<COSMCtrlRoute*> routes;
public:
	bool IsOpenManualAddPolyline();
	TAddPolylineProps GetManualProps();
	list<TLocation> GetManualPointsList();
	static double GetDistance (list<TLocation>& points);

	void OpenManualAddPolyline(TAddPolylineProps props);
	void CloseManualAddPolyline();

	void SetDefaultIcon(COSMCtrlIcon m_dummyDefaultIcon);
	void SetSelectedIcon(COSMCtrlIcon m_dummySelectedIcon);
	
	void AddMarker(CDBLocation &location);
	void RemoveMarker(int locationID);
	void SelectMarker(int locationID);
	COSMCtrlMarkerEx* GetMarkerByID(int locationID);
	COSMCtrlMarkerEx* GetMarkerByName(CString name);

	void AddPolyline(CDBPolyLine& polyLine);
	void RemovePolyline(int& lineID);
	void SelectPolyline(int& lineID);
	void UnselectAllPolylines();
    COSMCtrlPolylineEx* GetPolylineByID(int& lineID);
	COSMCtrlPolylineEx* GetPolylineByName(CString &name, int& loc1ID, int& loc2ID);
	list<COSMCtrlPolylineEx*> GetPolylinesBetwwenLocations(int &loc1ID, int& loc2ID);
	list<COSMCtrlRoute*> GetRoutesUsingPolyline(int &lineID);
	void AddRoute(CDBRoute& dbroute);
	void AddRoute(COSMCtrlRoute* route);
	COSMCtrlRoute* GetRouteByID(int routeID);
	void RemoveRoute(int &routeID);
    void SelectRoute(int & routeID);

public:
	COSMCtrlEx();
	~COSMCtrlEx(void);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPopupmenuAddlocation();
};

