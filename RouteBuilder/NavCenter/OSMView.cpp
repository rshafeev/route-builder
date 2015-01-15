#include "StdAfx.h"
#include "OSMView.h"
#include "resource.h"       // основные символы
#include "..\InetLib\RouteCloudeMade.h"
#include "GuiSubject.h"
#include "NavCenter.h"
#include "MainFrm.h"

COSMView::COSMView(void)

                                     
{
	subject = NULL;
	isActivatedAlready = false;
}


COSMView::~COSMView(void)
{
}

IMPLEMENT_DYNCREATE(COSMView, CNavCenterView)

BEGIN_MESSAGE_MAP(COSMView, CNavCenterView)

	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


COSMDoc* COSMView::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNavCenterDoc)));
	return (COSMDoc*)m_pDocument;
}

// диагностика CNavCenterView

#ifdef _DEBUG
void COSMView::AssertValid() const
{
	CNavCenterView::AssertValid();
}

void COSMView::Dump(CDumpContext& dc) const
{
	CNavCenterView::Dump(dc);
}


#endif //_DEBUG

void COSMView::OnChangePolylineToRoute(int &routeID, int &oldPolylineID,int &newPolylineID)
{
	COSMCtrlRoute* route = m_ctrlOSM.GetRouteByID(routeID);
	COSMCtrlPolylineEx* line2  = m_ctrlOSM.GetPolylineByID(newPolylineID);
	route->AddPolyline(line2);
	route->DeletePolyline(oldPolylineID);
	m_ctrlOSM.SelectRoute(routeID);
	m_ctrlOSM.RedrawWindow();
}

void COSMView::OnOpenManualAddPolyline(TAddPolylineProps props)
{
	m_ctrlOSM.OpenManualAddPolyline(props);
}
void COSMView::OnCloseManualAddPolyline()
{
	m_ctrlOSM.CloseManualAddPolyline();
}
int COSMView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CNavCenterView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rClient;
  GetClientRect(&rClient);
  if (!m_ctrlOSM.Create(_T(""), WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | SS_NOTIFY, rClient, this,ID_OSMCTRL))
  {
    TRACE(_T("COSMCtrlAppView::OnCreate, Failed to create OSM window\n")); 
	MessageBox("error!");
    return -1;
  }
  //Hook up the event handler
  m_ctrlOSM.SetEventHandler(this);


  
	m_ctrlOSM.SetDeltaMode(false);
	m_ctrlOSM.SetAllowMouseZoom(true);
	m_ctrlOSM.SetAllowKeyboard(true);
	m_ctrlOSM.SetDrawCenterCrossHairs(false);
	m_ctrlOSM.SetFocus();
	

COSMCtrlIcon  m_dummyDefaultIcon;
  HRESULT hr = LoadResourceImage(MAKEINTRESOURCE(IDB_DEFAULT_MARKER), _T("PNG"), AfxGetResourceHandle(), m_dummyDefaultIcon.m_pImage);
  if (FAILED(hr))
  {
    TRACE(_T("COSMCtrlAppView::OnCreate, Failed to load up icon1.png resource\n")); 
   // return -1;
  }
COSMCtrlIcon  m_dummySelectedtIcon;
hr = LoadResourceImage(MAKEINTRESOURCE(IDB_SELECTED_MARKER), _T("PNG"), AfxGetResourceHandle(), m_dummySelectedtIcon.m_pImage);
  if (FAILED(hr))
  {
    TRACE(_T("COSMCtrlAppView::OnCreate, Failed to load up icon1.png resource\n")); 
   // return -1;
  }
  m_ctrlOSM.SetDefaultIcon(m_dummyDefaultIcon);
  m_ctrlOSM.SetSelectedIcon(m_dummySelectedtIcon);

	 return 0;
}
void COSMView::OnAddRoute(CDBRoute& route)
{
	this->m_ctrlOSM.AddRoute(route);
}
void COSMView::OnDeleteRoute(int& routeID)
{
	this->m_ctrlOSM.RemoveRoute(routeID);
}

void  COSMView::OnAddPolyline(CDBPolyLine& polyLine)
{
	m_ctrlOSM.AddPolyline(polyLine);
	m_ctrlOSM.RedrawWindow();
}
void  COSMView::OnDeletePolyline(int& lineID)
{
	m_ctrlOSM.RemovePolyline(lineID);
	m_ctrlOSM.RedrawWindow();
}
void  COSMView::OnSelectPolyline(int& lineID)
{
	m_ctrlOSM.SelectPolyline(lineID);
	m_ctrlOSM.RedrawWindow();
}
void COSMView::OnSelectRoute(int & routeID)
{
	m_ctrlOSM.SelectRoute(routeID);
	m_ctrlOSM.RedrawWindow();

}

HRESULT COSMView::LoadResourceImage(LPCTSTR pName, LPCTSTR pType, HMODULE hInst, Gdiplus::Image*& pImage)
{
  //Find the resource
  HRSRC hResource = FindResource(hInst, pName, pType);
  if (!hResource)
    return ATL::AtlHresultFromWin32(GetLastError());
    
  //Get the size of the resource
  DWORD dwResourceSize = SizeofResource(hInst, hResource);
  if (dwResourceSize == 0)
    return ATL::AtlHresultFromWin32(GetLastError());

  //Load up the resource
  HGLOBAL hImage = LoadResource(hInst, hResource);
  if (hImage == NULL)
    return ATL::AtlHresultFromWin32(GetLastError());
  LPVOID pImageData = LockResource(hImage);
  if (pImageData == NULL)
    return ATL::AtlHresultFromWin32(GetLastError());

  //What will be the return value from this function (assume the best)
  HRESULT hr = S_OK;

  //Create a buffer for the stream
  HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwResourceSize);
  if (hGlobal)
  {
    LPVOID pGlobal = GlobalLock(hGlobal);
    if (pGlobal)
    {
      //Transfer the resource to the buffer
      memcpy(pGlobal, pImageData, dwResourceSize);

      //Create the stream from the buffer
      IStream* pStream = NULL;
      hr = CreateStreamOnHGlobal(hGlobal, TRUE, &pStream);
      if (hr == S_OK)
        pImage = Gdiplus::Image::FromStream(pStream);

      GlobalUnlock(hGlobal);
    }
    else
      hr = ATL::AtlHresultFromWin32(GetLastError());
  }
  else
    hr = ATL::AtlHresultFromWin32(GetLastError());

  return hr;
}


void COSMView::OnSize(UINT nType, int cx, int cy)
{

	__super::OnSize(nType, cx, cy);
 //Always make the OSM ctrl fill the full client area
  CRect rClient;
  GetClientRect(&rClient);
  m_ctrlOSM.MoveWindow(rClient);
}


void COSMView::OnDestroy()
{
    __super::OnDestroy();
	if(subject!=NULL)
	{
		subject->Detach(this);
		int count = ((CMainFrame*)(theApp.m_pMainWnd))->GetOSMDocumentCount();
		if(count<=1)
		{
			((CMainFrame*)(theApp.m_pMainWnd))->GetGuiSubject().FreeAllData();
		}
	}
}


void COSMView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_ctrlOSM.OnKeyDown(nChar, nRepCnt, nFlags);
	__super::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL COSMView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	return m_ctrlOSM.OnMouseWheel(nFlags, zDelta, pt);
	
}

void COSMView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	__super::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if(bActivate)
	{

		subject->Attach(this);
		if(GetDocument()->GetDBService()!=NULL&& GetDocument()->GetDBService()->IsConnect())
			subject->RefreshData(this->m_ctrlOSM,!isActivatedAlready);
		if(isActivatedAlready==false)
			isActivatedAlready = true;

		TRACE("OnActivateView: true\n");

			int count = ((CMainFrame*)(theApp.m_pMainWnd))->GetOSMDocumentCount();
			CString s;
			s.Format("OnActive: %d\n",count);
			TRACE(s);
		if(pActivateView!=pDeactiveView)
		{
			COSMView* v = dynamic_cast<COSMView*>(pDeactiveView);
			if(v!=NULL)
			{
				TRACE("OnActivateView: false\n");
				v->subject->Detach(this);
			}
		}
	}
	else
	{
		

		
	}
	
}

void COSMView::Init(CGuiSubject &guiSubject)
{
  SetSubject(&guiSubject);
  if(GetDocument()->GetDBService()!=NULL&& GetDocument()->GetDBService()->IsConnect())
  {
	    list<CDBLocation> locs =  GetDocument()->GetDBService()->GetAllLocations();
		for(list<CDBLocation>::iterator i = locs.begin();i != locs.end(); i++)
			this->m_ctrlOSM.AddMarker(*i);

		list<CDBPolyLine> lines =  GetDocument()->GetDBService()->GetAllPolylines();
		for(list<CDBPolyLine>::iterator i = lines.begin();i != lines.end(); i++)
			this->m_ctrlOSM.AddPolyline(*i);

		list<CDBRoute> routes =  GetDocument()->GetDBService()->GetAllRoutes(true);
		for(list<CDBRoute>::iterator i = routes.begin(); i!= routes.end(); i++)
		{
			this->m_ctrlOSM.AddRoute(*i);
		}
  }

  //subject->RefreshData(m_ctrlOSM,true);
}
void COSMView::OnUnselectedPolylines(bool unselectedEx)
{
	if(unselectedEx)
	{
		m_ctrlOSM.UnselectAllPolylines();
		m_ctrlOSM.RedrawWindow();
	}
}

void COSMView::OnAddMarker(CDBLocation& loc)
{
	m_ctrlOSM.AddMarker(loc);
	m_ctrlOSM.RedrawWindow();
}
void COSMView::OnDeleteMarker(int &locID, CString &Name)
{
	
	dynamic_cast<CMainFrame*>(theApp.m_pMainWnd)->AddInfoString("Delete location '"+Name + "'");
	for(int i=0;i < m_ctrlOSM.GetPolylineCount(); i++)
	{
		COSMCtrlPolylineEx* line  = dynamic_cast<COSMCtrlPolylineEx*>(& m_ctrlOSM.GetPolyline(i));
		if(line!=NULL)
		{
			
			if(line->m_dbPolyLine.Loc1_ID == locID || line->m_dbPolyLine.Loc2_ID==locID)
			{
				COSMCtrlMarkerEx* mA = m_ctrlOSM.GetMarkerByID(line->m_dbPolyLine.Loc1_ID);
				COSMCtrlMarkerEx* mB = m_ctrlOSM.GetMarkerByID(line->m_dbPolyLine.Loc2_ID);

				m_ctrlOSM.RemovePolyline(line->m_dbPolyLine.ID);
				dynamic_cast<CMainFrame*>(theApp.m_pMainWnd)->AddInfoString("Delete polyline '"+line->m_dbPolyLine.Name + 
					"' between '" + mA->location.Name + "' and '"+mB->location.Name+"'");
				i--;
			}
		}
	}
	for(list<COSMCtrlRoute*>::iterator i = m_ctrlOSM.routes.begin(); i!= m_ctrlOSM.routes.end();)
	{
		if ((*i)->dbroute.LocFinish_ID == locID|| (*i)->dbroute.LocFirst_ID == locID)
		{
			COSMCtrlMarkerEx* mA = m_ctrlOSM.GetMarkerByID((*i)->dbroute.LocFirst_ID);
			COSMCtrlMarkerEx* mB = m_ctrlOSM.GetMarkerByID((*i)->dbroute.LocFinish_ID);
			dynamic_cast<CMainFrame*>(theApp.m_pMainWnd)->AddInfoString("Delete route '"+(*i)->dbroute.Name + 
					"' between '" + mA->location.Name + "' and '"+mB->location.Name+"'");
			i = m_ctrlOSM.routes.erase(i);
		}
		else
			i++;
	}
	m_ctrlOSM.RemoveMarker(locID);
	m_ctrlOSM.RedrawWindow();
}
void COSMView::OnSelectMarker(int &locID)
{
	m_ctrlOSM.SelectMarker(locID);
	COSMCtrlMarkerEx * m = m_ctrlOSM.GetMarkerByID(locID);
	if(m!=NULL)
		m_ctrlOSM.SetCenter(m->m_Position,true);
	m_ctrlOSM.RedrawWindow();
}


