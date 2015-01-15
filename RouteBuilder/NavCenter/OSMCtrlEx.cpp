#include "StdAfx.h"
#include <GdiPlus.h>
#include "OSMCtrlEx.h"
#include "NavCenter.h"
#include "OSMCtrlMarkerEx.h"
#include "DlgAddLocation.h"
#include "MainFrm.h"
COSMCtrlEx::COSMCtrlEx()
	: COSMCtrl(),
	   m_nDefaultMarkerIconIndex(-1)
{
	m_isOpenManualAddPolyline = false;
}


COSMCtrlEx::~COSMCtrlEx(void)
{
}
BEGIN_MESSAGE_MAP(COSMCtrlEx, COSMCtrl)
	ON_WM_RBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_POPUPMENU_ADDLOCATION, &COSMCtrlEx::OnPopupmenuAddlocation)
END_MESSAGE_MAP()


void COSMCtrlEx::UnselectAllPolylines()
{
	for(int i=0;i  < this->m_Polylines.GetSize();i++)
	{
		COSMCtrlPolylineEx *m = dynamic_cast<COSMCtrlPolylineEx*>(this->m_Polylines[i]);
		if(m!=NULL)
		    this->m_Polylines[i]->m_bVisible = false;
	}
}
COSMCtrlPolylineEx* COSMCtrlEx::GetPolylineByName(CString &name, int& loc1ID, int& loc2ID)
{
	for(int i=0;i < this->GetPolylineCount(); i++)
	{
		COSMCtrlPolylineEx* line = dynamic_cast<COSMCtrlPolylineEx*>( &GetPolyline(i) ) ;
		if(line!=NULL)
		{
			if(((line->m_dbPolyLine.Loc1_ID == loc1ID &&line->m_dbPolyLine.Loc2_ID == loc2ID )||
			(line->m_dbPolyLine.Loc1_ID == loc2ID &&line->m_dbPolyLine.Loc2_ID == loc1ID ))&&
			(line->m_dbPolyLine.Name == name))
			{
				return line;
			}
		}
	}
	return NULL;
}
list<COSMCtrlRoute*> COSMCtrlEx::GetRoutesUsingPolyline(int &lineID)
{
	list<COSMCtrlRoute*> r;
	for(list<COSMCtrlRoute*>::iterator i = routes.begin(); i!= routes.end(); i++)
	{
		if((*i)->IsExistPolylineIntoRoute(lineID))
			r.push_back(*i);
	}
	//return routes;
	return r;
}

list<COSMCtrlPolylineEx*> COSMCtrlEx::GetPolylinesBetwwenLocations(int &loc1ID, int& loc2ID)
{
	list<COSMCtrlPolylineEx*> lines;
	for(int i=0;i < this->GetPolylineCount(); i++)
	{
		COSMCtrlPolylineEx* line = dynamic_cast<COSMCtrlPolylineEx*>( &GetPolyline(i) ) ;
		if(line!=NULL)
		{
			if((line->m_dbPolyLine.Loc1_ID == loc1ID &&line->m_dbPolyLine.Loc2_ID == loc2ID )||
			(line->m_dbPolyLine.Loc1_ID == loc2ID &&line->m_dbPolyLine.Loc2_ID == loc1ID ))
			{
				lines.push_back(line);
			}
		}
	}
	return lines;
}

void COSMCtrlEx::SelectRoute(int & routeID)
{
	COSMCtrlRoute* route =   GetRouteByID(routeID);
	if(route==NULL)
		return;
	UnselectAllPolylines();
	for(list<COSMCtrlPolylineEx*>::iterator i = route->lines.begin(); i!= route->lines.end(); i++)
	{
		(*i)->m_bVisible = true;
		(*i)->m_fLinePenWidth = 2.0;
	}	
}

void COSMCtrlEx::AddRoute(CDBRoute& dbroute)
{
	COSMCtrlRoute* r = new COSMCtrlRoute();
	r->dbroute = dbroute;
	for(list<CDBRelation>::iterator i = dbroute.relations.begin(); i!= dbroute.relations.end() ; i++)
	{
		COSMCtrlPolylineEx* line =  this->GetPolylineByID(i->Line_ID);
		if(line != NULL && line->m_dbPolyLine.ID && i->Route_ID == dbroute.ID)
		{
			r->lines.push_back(line);
		}
	}
	this->routes.push_back(r);
}
void COSMCtrlEx::AddRoute(COSMCtrlRoute* route)
{
	this->routes.push_back(route);
}
COSMCtrlRoute* COSMCtrlEx::GetRouteByID(int routeID)
{
	for(list<COSMCtrlRoute*>::iterator i = this->routes.begin(); i!=routes.end();i++)
	{
		if((*i)->dbroute.ID == routeID)
			return *i;
	}
	return NULL;
}

void  COSMCtrlEx::OpenManualAddPolyline(TAddPolylineProps props)
{
	COSMCtrlMarkerEx* markerA = GetMarkerByName(props.agentA);
	COSMCtrlMarkerEx* markerB = GetMarkerByName(props.agentB);
	this->m_manualProps = props;
	if(markerA == NULL|| markerB == NULL)
	  return;			
	
	m_isOpenManualAddPolyline = true;
	m_manualAddline->m_bVisible = true;
	m_manualAddline->m_Nodes.RemoveAll();
	m_manualAddline->m_Nodes.Add(COSMCtrlNode(markerA->location.Lon,markerA->location.Lat));
	m_manualAddline->m_Nodes.Add(COSMCtrlNode(markerB->location.Lon,markerB->location.Lat));

	if(props.type==CDBPolyLine::c_road)
	{
		m_manualAddline->m_colorPen = Gdiplus::Color::Black;
	}
	else
	if(props.type==CDBPolyLine::c_ship)
	{
		m_manualAddline->m_colorPen = Gdiplus::Color::DarkGreen;

	}
	else
	if(props.type==CDBPolyLine::c_train)
	{
		m_manualAddline->m_colorPen = Gdiplus::Color::Brown;
	}
	this->RedrawWindow();
}
void  COSMCtrlEx::CloseManualAddPolyline()
{
	m_isOpenManualAddPolyline = false;
	m_manualAddline->m_bVisible = false;
	this->RedrawWindow();
}
double COSMCtrlEx::GetDistance(list<TLocation>& points)
{
	double distance = 0.0;
	for(list<TLocation>::iterator i = points.begin(); i!= points.end(); i++)
	{
		list<TLocation>::iterator j = i;
		j++;
		if(j==points.end())
			break;
		distance += COSMCtrlHelper::DistanceBetweenPoints(COSMCtrlPosition(i->Lon,i->Lat),COSMCtrlPosition(j->Lon,j->Lat),NULL,NULL);
	}
	return distance;
}

void COSMCtrlEx::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	 POINT cursor_pos;
	 ::GetCursorPos (&cursor_pos);
	CRect r;
	GetClientRect(r);
	this->ClientToPosition(Gdiplus::PointF(point.x,point.y),r,m_rightClickPos);

	if(m_isOpenManualAddPolyline==true)
	{
		//this->ManualAddline->m_Nodes.Add();
		int ind = this->m_manualAddline->m_Nodes.GetCount()-1;
		this->m_manualAddline->m_Nodes.InsertAt(ind,COSMCtrlNode(m_rightClickPos.m_fLongitude,m_rightClickPos.m_fLatitude),1);
		dynamic_cast<CMainFrame*>(theApp.m_pMainWnd)->GetDockablePolylinesPane().AddPoint(m_rightClickPos.m_fLatitude,m_rightClickPos.m_fLongitude);
		this->RedrawWindow();
	}
	else
	{
		m_ContextMenu.GetSubMenu (0)->TrackPopupMenu( nFlags, cursor_pos.x, cursor_pos.y,this);
	}
	COSMCtrl::OnRButtonDown(nFlags, point);
}
COSMCtrlMarkerEx* COSMCtrlEx::GetMarkerByName(CString name)
{
	for(int i=0;i  < this->m_Markers.GetSize();i++)
	{
		COSMCtrlMarkerEx *m = dynamic_cast<COSMCtrlMarkerEx*>(this->m_Markers[i]);
		if(m->location.Name.Compare(name)==0)
		{
			return m;
		}
		else
		    this->m_Markers[i]->m_nIconIndex = this->m_nDefaultMarkerIconIndex;
	}
	return NULL;
}


COSMCtrlPolylineEx* COSMCtrlEx::GetPolylineByID(int& lineID)
{
	for(int i=0;i < this->m_Polylines.GetCount(); i++)
	{
		COSMCtrlPolylineEx* line = dynamic_cast<COSMCtrlPolylineEx*>(this->m_Polylines[i]);
		if(line==NULL)
			continue;
		if(line->m_dbPolyLine.ID == lineID)
		   return line;
	}
	return NULL;
}
COSMCtrlMarkerEx* COSMCtrlEx::GetMarkerByID(int locationID)
{
	for(int i=0;i  < this->m_Markers.GetSize();i++)
	{
		COSMCtrlMarkerEx *m = dynamic_cast<COSMCtrlMarkerEx*>(this->m_Markers[i]);
		if(m==NULL)
			continue;
		if(m->location.ID == locationID)
		{
			return m;
		}
		else
		    this->m_Markers[i]->m_nIconIndex = this->m_nDefaultMarkerIconIndex;
	}
	return NULL;
}

void COSMCtrlEx::SelectMarker(int locationID)
{
	for(int i=0;i  < this->m_Markers.GetSize();i++)
	{
		COSMCtrlMarkerEx *m = dynamic_cast<COSMCtrlMarkerEx*>(this->m_Markers[i]);
		if(m!=NULL && m->location.ID == locationID)
			this->m_Markers[i]->m_nIconIndex = this->m_nSelectedMarkerIconIndex;
		else
		    this->m_Markers[i]->m_nIconIndex = this->m_nDefaultMarkerIconIndex;
	}
}
void COSMCtrlEx::RemoveMarker(int locationID)
{
	for(int i=0;i  < this->m_Markers.GetSize();i++)
	{
		COSMCtrlMarkerEx *m = dynamic_cast<COSMCtrlMarkerEx*>(this->m_Markers[i]);
		if( m!=NULL && m->location.ID == locationID)
		{
			this->m_Markers.RemoveAt(i);
			break;
		}
	}
}
bool COSMCtrlEx::IsOpenManualAddPolyline()
{
	return m_isOpenManualAddPolyline;
}
TAddPolylineProps COSMCtrlEx::GetManualProps()
{
	return m_manualProps;
}
list<TLocation> COSMCtrlEx::GetManualPointsList()
{
	list<TLocation> locs;
	for(int i=1;i < m_manualAddline->m_Nodes.GetSize()-1; i++)
	{
		locs.push_back(TLocation(m_manualAddline->m_Nodes[i].m_Position.m_fLatitude,m_manualAddline->m_Nodes[i].m_Position.m_fLongitude));
	}
	return locs;
}

void  COSMCtrlEx::AddMarker(CDBLocation &location)
{
	COSMCtrlMarkerEx *marker = new COSMCtrlMarkerEx();
	marker->location = CDBLocation( location);
	marker->m_Position = COSMCtrlPosition(location.Lon,location.Lat);
	marker->m_nIconIndex = this->m_nDefaultMarkerIconIndex;
	this->m_Markers.Add(marker);
	return;
}
void COSMCtrlEx::AddPolyline(CDBPolyLine& polyLine)
{
	COSMCtrlPolylineEx* line = new COSMCtrlPolylineEx();
	line->m_dbPolyLine = polyLine;
	
	list<TLocation> points = CDBPolyLine::GeomToMas(polyLine.Geom);
	for(list<TLocation>::iterator i = points.begin(); i!= points.end();i++)
	{
		line->m_Nodes.Add(COSMCtrlNode(i->Lon,i->Lat));
		
	}

	if(polyLine.Type==CDBPolyLine::c_road)
	{
		line->m_colorPen = Gdiplus::Color::Black;
	}
	else
	if(polyLine.Type==CDBPolyLine::c_ship)
	{
		line->m_colorPen = Gdiplus::Color::DarkGreen;

	}
	else
	if(polyLine.Type==CDBPolyLine::c_train)
	{
		line->m_colorPen = Gdiplus::Color::Brown;
	}
	else
	if(polyLine.Type==CDBPolyLine::c_ship)
	{
		line->m_colorPen = Gdiplus::Color::MistyRose;
	}
	line->m_bVisible = false;
	this->m_Polylines.Add(line);

}
void COSMCtrlEx::RemovePolyline(int& lineID)
{
	for(int i=0;i  < this->m_Polylines.GetSize();i++)
	{
		COSMCtrlPolylineEx *m = dynamic_cast<COSMCtrlPolylineEx*>(this->m_Polylines[i]);
		if(m!=NULL&&m->m_dbPolyLine.ID == lineID)
		{
			this->m_Polylines.RemoveAt(i);
			break;
		}
	}
}
void COSMCtrlEx::RemoveRoute(int &routeID)
{
	for(list<COSMCtrlRoute*>::iterator i = routes.begin(); i!= routes.end(); i++)
	{
		if((*i)->dbroute.ID == routeID)
		{
			routes.erase(i);
			return;
		}
	}
}

void COSMCtrlEx::SelectPolyline(int& lineID)
{
	for(int i=0;i  < this->m_Polylines.GetSize();i++)
	{
		COSMCtrlPolylineEx *m = dynamic_cast<COSMCtrlPolylineEx*>(this->m_Polylines[i]);
		if(m!=NULL && m->m_dbPolyLine.ID == lineID)
		{
			this->m_Polylines[i]->m_bVisible = true;
			this->m_Polylines[i]->m_fLinePenWidth = 2.0;

		}
		else
		if(m!=NULL)
		{
			this->m_Polylines[i]->m_fLinePenWidth = 1.0;
		    this->m_Polylines[i]->m_bVisible = false;
		}
	}
}
int COSMCtrlEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COSMCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	 m_ContextMenu.LoadMenu(IDR_MENUPOPUP);
	 
	 SetMenu(NULL);
	 SetMenu(&m_ContextMenu);
   
	 TCHAR szPath[_MAX_PATH];
  if (!SHGetSpecialFolderPath(NULL, szPath, CSIDL_LOCAL_APPDATA, TRUE))
  {
    TRACE(_T("COSMCtrlAppView::OnCreate, Failed to get CDISL_LOCAL_APPDATA folder location\n")); 
    return -1;
  }
  CString sCacheDirectory(szPath);
  sCacheDirectory += _T("\\OSMCtrlApp");
  CreateDirectory(sCacheDirectory, NULL);
  CWinApp* pApp = AfxGetApp();
  OSMCtrlAppTileProvider tileProvider = static_cast<OSMCtrlAppTileProvider>(pApp->GetProfileInt(_T("General"), _T("TileProvider"), Mapnik));
   switch (tileProvider)
  {
    case Osmarender:
    {
      sCacheDirectory += _T("\\Osmarender");
      SetTileProvider(&m_OsmarenderTileProvider);
      break;
    }
    case Cyclemap:
    {
      SetTileProvider(&m_CyclemapTileProvider);
      sCacheDirectory += _T("\\Cyclemap");
      break;
    }
    case MapquestOSM:
    {
      SetTileProvider(&m_MapquestOSMTileProvider);
      sCacheDirectory += _T("\\MapquestOSM");
      break;
    }
    case MapquestOpenAerial:
    {
      SetTileProvider(&m_MapquestOpenAerialTileProvider);
      sCacheDirectory += _T("\\MapquestOpenAerial");
      break;
    }
    case Mapnik: //deliberate fallthrough
    default:
    {
      SetTileProvider(&m_MapnikTileProvider);
      sCacheDirectory += _T("\\Mapnik");
      break;
    }
  }
  CreateDirectory(sCacheDirectory, NULL);
  SetCacheDirectory(sCacheDirectory);
    //as a convenience, restore the various settings
  CString sZoom;
  sZoom.Format(_T("%f"), GetZoom());
  sZoom = pApp->GetProfileString(_T("General"), _T("ZoomS"), sZoom);
  double fZoom = _tstof(sZoom);
  CString sDefault;
  COSMCtrlPosition centerPosition(GetCenter());
  sDefault.Format(_T("%f"), centerPosition.m_fLongitude);
  CString sValue = pApp->GetProfileString(_T("General"), _T("LastLongitude"),sDefault);
  double dLongitude = _tstof(sValue);
  sDefault.Format(_T("%f"), centerPosition.m_fLatitude);
  sValue = pApp->GetProfileString(_T("General"), _T("LastLatitude"), sDefault);
  double dLatitude = _tstof(sValue);

  SetDoAnimations(false);
  SetCenterAndZoom(COSMCtrlPosition(dLongitude, dLatitude), fZoom, TRUE);
  SetDrawScrollRose(pApp->GetProfileInt(_T("General"), _T("DrawScrollRose"), TRUE));
  SetDrawZoomBar(pApp->GetProfileInt(_T("General"), _T("DrawZoomBar"), TRUE));
  SetDrawScaleBar(pApp->GetProfileInt(_T("General"), _T("DrawScaleBar"), TRUE));
  SetUseTransparencyForZoomBar(pApp->GetProfileInt(_T("General"), _T("UseTransparencyForZoomBar"), TRUE));
  SetDrawTileOutlines(pApp->GetProfileInt(_T("General"), _T("DrawTileOutlines"), FALSE));
  SetDownloadTiles(pApp->GetProfileInt(_T("General"), _T("DownloadTiles"), TRUE));
  SetAllowUseNextZoomSqueeze(pApp->GetProfileInt(_T("General"), _T("UseNextZoomSqueeze"), TRUE));
  SetAllowUsePreviousZoomStretch(pApp->GetProfileInt(_T("General"), _T("UsePrevZoomStretch"), TRUE));
  SetDrawZoomBarAsSlider(pApp->GetProfileInt(_T("General"), _T("DrawZoomBarAsSlider"), TRUE));
  SetDrawCenterCrossHairs(pApp->GetProfileInt(_T("General"), _T("DrawCenterCrossHairs"), TRUE));
  SetDeltaMode(pApp->GetProfileInt(_T("General"), _T("DeltaMode"), FALSE));
  
  m_manualAddline = new COSMCtrlPolyline();
  m_manualAddline->m_bVisible = false;
  this->m_Polylines.Add(m_manualAddline);

  return 0;

}

void COSMCtrlEx::SetSelectedIcon(COSMCtrlIcon m_dummySelectedIcon)
{
  this->m_dummySelectedIcon = m_dummySelectedIcon;
  m_nSelectedMarkerIconIndex = m_Icons.Add(&this->m_dummySelectedIcon); 
  this->m_dummySelectedIcon.m_ptAnchor.x = 10;
  this-> m_dummySelectedIcon.m_ptAnchor.y = 24;
}
void COSMCtrlEx::SetDefaultIcon(COSMCtrlIcon m_dummyDefaultIcon)
{
  this->m_dummyDefaultIcon = m_dummyDefaultIcon;
  m_nDefaultMarkerIconIndex = m_Icons.Add(&this->m_dummyDefaultIcon); 
  this->m_dummyDefaultIcon.m_ptAnchor.x = 10;
  this-> m_dummyDefaultIcon.m_ptAnchor.y = 24;
}

void COSMCtrlEx::OnDestroy()
{
  CString sZoom;
  sZoom.Format(_T("%f"), GetZoom());
  CWinApp* pApp = AfxGetApp();
  pApp->WriteProfileString(_T("General"), _T("ZoomS"), sZoom);
  CString sValue;
  COSMCtrlPosition centerPosition(GetCenter());
  sValue.Format(_T("%f"), centerPosition.m_fLongitude);
  pApp->WriteProfileString(_T("General"), _T("LastLongitude"), sValue);
  sValue.Format(_T("%f"), centerPosition.m_fLatitude);
  pApp->WriteProfileString(_T("General"), _T("LastLatitude"), sValue);
  IOSMCtrlTileProvider* pTileProvider = GetTileProvider();
  OSMCtrlAppTileProvider tileProvider = Mapnik;
  if (pTileProvider == &m_OsmarenderTileProvider)
    tileProvider = Osmarender;
  else if (pTileProvider == &m_CyclemapTileProvider)
    tileProvider = Cyclemap;
  else if (pTileProvider == &m_MapquestOSMTileProvider)
    tileProvider = MapquestOSM;
  else if (pTileProvider == &m_MapquestOpenAerialTileProvider)
    tileProvider = MapquestOpenAerial;
  pApp->WriteProfileInt(_T("General"), _T("TileProvider"), tileProvider);
  pApp->WriteProfileInt(_T("General"), _T("DrawScrollRose"), GetDrawScrollRose());
  pApp->WriteProfileInt(_T("General"), _T("DrawZoomBar"), GetDrawZoomBar());
  pApp->WriteProfileInt(_T("General"), _T("DrawScaleBar"), GetDrawScaleBar());
  pApp->WriteProfileInt(_T("General"), _T("UseTransparencyForZoomBar"), GetUseTransparencyForZoomBar());
  pApp->WriteProfileInt(_T("General"), _T("DrawTileOutlines"), GetDrawTileOutlines());
  pApp->WriteProfileInt(_T("General"), _T("DownloadTiles"), GetDownloadTiles());
  pApp->WriteProfileInt(_T("General"), _T("UseNextZoomSqueeze"), GetAllowUseNextZoomSqueeze());
  pApp->WriteProfileInt(_T("General"), _T("UsePrevZoomStretch"), GetAllowUsePreviousZoomStretch());
  pApp->WriteProfileInt(_T("General"), _T("DrawZoomBarAsSlider"), GetDrawZoomBarAsSlider());
  pApp->WriteProfileInt(_T("General"), _T("DrawCenterCrossHairs"), GetDrawCenterCrossHairs());
  pApp->WriteProfileInt(_T("General"), _T("DeltaMode"), GetDeltaMode());

  //Tidy up the marker resources
  if (m_dummyDefaultIcon.m_pImage != NULL)
  {
    delete m_dummyDefaultIcon.m_pImage;
    m_dummyDefaultIcon.m_pImage = NULL;
  }

	COSMCtrl::OnDestroy();
}


void COSMCtrlEx::OnPopupmenuAddlocation()
{
	CDlgAddLocation dlg(this,this->m_rightClickPos,this);
	dlg.DoModal();
}
