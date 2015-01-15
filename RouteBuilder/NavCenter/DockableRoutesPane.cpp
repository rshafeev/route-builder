// DockableRoutesPane.cpp : implementation file
//

#include "stdafx.h"
#include "NavCenter.h"
#include "DockableRoutesPane.h"
#include "GuiSubject.h"
#include "OSMView.h"
#include "OSMDoc.h"
#include "MainFrm.h"
#include "DlgChangeRoute.h"
// CDockableRoutesPane

IMPLEMENT_DYNAMIC(CDockableRoutesPane, CDockablePane)

CDockableRoutesPane::CDockableRoutesPane(CGuiSubject &guiSubject)
{
	SetSubject(&guiSubject);
	lastDocFile = "";
	subject->Attach(this);
	isManualAdd = false;
}

CDockableRoutesPane::~CDockableRoutesPane()
{
	subject->Detach(this);
}


BEGIN_MESSAGE_MAP(CDockableRoutesPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(ID_VIEWROUTES_COMBOA,OnSelectComboA)
	ON_CBN_SELCHANGE(ID_VIEWROUTES_COMBOB,OnSelectComboB)
	ON_CBN_SELCHANGE(ID_VIEW_COMBOPOLYLINES,OnSelectComboPolylines)
	
	ON_BN_CLICKED(ID_BUTTON_NEW, &CDockableRoutesPane::OnNewRouteBtnClick)
    ON_UPDATE_COMMAND_UI(ID_BUTTON_NEW, &CDockableRoutesPane::OnUpdateNewRouteBtnClick)
	ON_BN_CLICKED(ID_BUTTON_DELETE, &CDockableRoutesPane::OnDeleteRouteBtnClick)
    ON_UPDATE_COMMAND_UI(ID_BUTTON_DELETE, &CDockableRoutesPane::OnUpdateDeleteRouteBtnClick)
	ON_BN_CLICKED(ID_BUTTONCHANGE, &CDockableRoutesPane::OnChangeRouteBtnClick)
    ON_UPDATE_COMMAND_UI(ID_BUTTONCHANGE, &CDockableRoutesPane::OnUpdateChangeRouteBtnClick)
	
	ON_NOTIFY(NM_CLICK,ID_ROUTELIST,&CDockableRoutesPane::OnListRoutesClick)
	ON_NOTIFY(NM_CLICK,ID_RELATIONLIST,&CDockableRoutesPane::OnListRelationsClick)

	
END_MESSAGE_MAP()

void CDockableRoutesPane::OnSelectComboPolylines()
{
	int ind = m_comboPolylines.GetCurSel();
	if(ind<0)
		return;
	int id1,id2;
	int routeID;
	if(GetSelectedRelation(id1,id2)&&GetSelectedRouteID(routeID))
	{
		COSMView* view = dynamic_cast<COSMView*>(GetActiveView());
		COSMDoc* doc = dynamic_cast<COSMDoc*>(GetActiveDocument());
		if(view==NULL|| doc==NULL)
			return;
		COSMCtrlRoute* selectedRoute = view->m_ctrlOSM.GetRouteByID(routeID);
		if(selectedRoute==NULL)
			return;
		COSMCtrlPolylineEx* line1 = selectedRoute->GetLine(id1,id2);
		
		CString loc2Name;
		m_comboPolylines.GetLBText(ind,loc2Name);
		COSMCtrlPolylineEx* line2 = view->m_ctrlOSM.GetPolylineByName(loc2Name,id1,id2);
		if(line2==NULL)
			return;
		if(line1->m_dbPolyLine.ID == line2->m_dbPolyLine.ID)
			return;
		if(!doc->GetDBService()->DeleteRelation(CDBRelation(routeID,line1->m_dbPolyLine.ID)))
			return;
		if(!doc->GetDBService()->AddRelation(CDBRelation(routeID,line2->m_dbPolyLine.ID)))
			return;
		((CMainFrame*)theApp.m_pMainWnd)->GetGuiSubject().ChangePolylineToRoute(routeID,line1->m_dbPolyLine.ID, line2->m_dbPolyLine.ID);

	}

	//
}
bool CDockableRoutesPane::GetSelectedRelation(int &loc1ID, int& loc2ID)
{
	POSITION pos = m_listCtrlPoints->GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int ind = m_listCtrlPoints->GetNextSelectedItem(pos);
		loc1ID = atoi(m_listCtrlPoints->GetItemText(ind,2));
		loc2ID = atoi(m_listCtrlPoints->GetItemText(ind,3));
		return true;
	}

	return false;
}

void CDockableRoutesPane::OnSelectPolyline(int& lineID)
{
	POSITION pos = m_listCtrlRoutes->GetFirstSelectedItemPosition();
	while(pos!=NULL)
	{
		int ind  = this->m_listCtrlRoutes->GetNextSelectedItem(pos);
		m_listCtrlRoutes->SetItemState(ind, 0, LVIS_SELECTED);
	}	
	m_listCtrlPoints->DeleteAllItems();
	
}

void CDockableRoutesPane::OnListRelationsClick(NMHDR * a,LRESULT * b)
{
	int id1,id2;
	int routeID;
	this->m_comboPolylines.ResetContent();
	if(GetSelectedRelation(id1,id2)&&GetSelectedRouteID(routeID))
	{
		
		COSMView* view = dynamic_cast<COSMView*>(GetActiveView());
		if(view==NULL)
			return;
		COSMCtrlRoute* selectedRoute = view->m_ctrlOSM.GetRouteByID(routeID);
		if(selectedRoute==NULL)
			return;
		COSMCtrlPolylineEx* selectedPolyline = selectedRoute->GetLine(id1,id2);
			
		list<COSMCtrlPolylineEx*> lines = view->m_ctrlOSM.GetPolylinesBetwwenLocations(id1,id2);
		
		int ind = 0;
		int curInd = -1;
		m_comboPolylines.EnableWindow(TRUE);
		for(list<COSMCtrlPolylineEx*>::iterator i = lines.begin(); i!= lines.end(); i++)
		{
			if(selectedPolyline!=NULL&& (*i)->m_dbPolyLine.ID == selectedPolyline->m_dbPolyLine.ID)
			{
				curInd = ind;
			}
			m_comboPolylines.AddString((*i)->m_dbPolyLine.Name);
			ind++;
		}
		m_comboPolylines.SetCurSel(curInd);
		
	}
	else
	{
		m_comboPolylines.EnableWindow(FALSE);
	}
}

void CDockableRoutesPane::OnListRoutesClick(NMHDR * a,LRESULT * b)
{
	int selectedID;
	m_comboPolylines.ResetContent();
	m_comboPolylines.EnableWindow(FALSE);

	if(GetSelectedRouteID(selectedID)==true)
	{
		subject->SelectRoute(selectedID);
		COSMView* view = dynamic_cast<COSMView*>(GetActiveView());
		if(view==NULL)
			return;
		COSMCtrlRoute *route = view->m_ctrlOSM.GetRouteByID(selectedID);
		m_listCtrlPoints->DeleteAllItems();
		if(route!=NULL)
		{
			list<int> locsId = route->GetSortedIDLocations();
			vector<CDBLocation> locs;
			locs.reserve(locsId.size());
			for(list<int>::iterator i = locsId.begin(); i!= locsId.end(); i++)
			{
				COSMCtrlMarkerEx* m = view->m_ctrlOSM.GetMarkerByID(*i);
				if(m!=NULL)
					locs.push_back(m->location);
			}

			for(int i=0;i < locs.size()-1; i++)
			{
				int nItem = m_listCtrlPoints->InsertItem(LVIF_TEXT|LVIF_STATE,m_listCtrlPoints->GetItemCount(),locs[i].Name,0, LVIS_SELECTED, 0, 0);
				m_listCtrlPoints->SetItemText(nItem, 1, locs[i+1].Name);
				m_listCtrlPoints->SetItemText(nItem, 2, locs[i].GetIdS());
				m_listCtrlPoints->SetItemText(nItem, 3, locs[i+1].GetIdS());
			}
		}
	}
	else
	{
		subject->UnselectedPolylines(true);
		m_listCtrlPoints->DeleteAllItems();
	}
	
}
bool CDockableRoutesPane::GetSelectedRouteID(int &routeID)
{
	POSITION pos = this->m_listCtrlRoutes->GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int ind = this->m_listCtrlRoutes->GetNextSelectedItem(pos);
		CString idS = this->m_listCtrlRoutes->GetItemText(ind,0);
		if(idS != "")
		{
			routeID = atoi(idS);
			return true;
		}
	}
	return false;
}

void CDockableRoutesPane::OnAddRoute(CDBRoute& route)
{
	AddRouteToList(route);
}
void CDockableRoutesPane::OnDeleteRoute(int& routeID)
{
	for(int i=0;i < this->m_listCtrlRoutes->GetItemCount(); i++)
	{
		CString idS = m_listCtrlRoutes->GetItemText(i,0);
		if(atoi(idS)== routeID)
		{
			m_listCtrlRoutes->DeleteItem(i);
			m_listCtrlPoints->DeleteAllItems();
			m_comboPolylines.ResetContent();
			m_comboPolylines.EnableWindow(FALSE);
			break;
		}
	}
}
bool CDockableRoutesPane::GetLocationIDFromCombo(CComboBox& combo, int& locID)
{
	CString locName;
	int ind = combo.GetCurSel();
	if(ind<0)
		return false;
	combo.GetLBText(ind,locName);
	COSMView * view = dynamic_cast<COSMView*>(GetActiveView());
	if(view==NULL)
		return false;
	COSMCtrlMarkerEx  *m =  view->m_ctrlOSM.GetMarkerByName(locName);
	if(m==NULL)
		return false;
	locID = m->location.ID;
	return true;
}

void CDockableRoutesPane::OnNewRouteBtnClick()
{
	int LocFirst_ID,LocFinish_ID;
	COSMView * view = dynamic_cast<COSMView*>(GetActiveView());
	if(view==NULL)
		return ;
	if(!GetLocationIDFromCombo(m_comboBoxLocA,LocFirst_ID)||
	   !GetLocationIDFromCombo(m_comboBoxLocB,LocFinish_ID))
	  return;

	CDBRoute route;
	route.LocFirst_ID =LocFirst_ID;
	route.LocFinish_ID = LocFinish_ID;
	CDlgChangeRoute *dlg = new CDlgChangeRoute(view->m_ctrlOSM,route,this);
	dlg->DoModal();
	delete dlg;

}
void CDockableRoutesPane::OnDeleteRouteBtnClick()
{
	POSITION pos = m_listCtrlRoutes->GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int ind = m_listCtrlRoutes->GetNextSelectedItem(pos);
        CString s =   m_listCtrlRoutes->GetItemText(ind,0);
		if(s!="")
		{
			int ID = atoi(s);
			COSMDoc * doc = dynamic_cast<COSMDoc*>(GetActiveDocument());
			CString name;
			CString locAname,locBname;
			name =  m_listCtrlRoutes->GetItemText(ind,1);
			m_comboBoxLocA.GetLBText(m_comboBoxLocA.GetCurSel(),locAname);
			m_comboBoxLocB.GetLBText(m_comboBoxLocB.GetCurSel(),locBname);

			if(doc->GetDBService()->DeleteRoute(ID) == true)
			{
				dynamic_cast<CMainFrame*>(theApp.m_pMainWnd)->AddInfoString("Delete route '"+name+"' between " + locAname +" and " +locBname);
				dynamic_cast<CMainFrame*>(theApp.m_pMainWnd)->GetGuiSubject().DeleteRoute(ID);

			}
		}
	}
	dynamic_cast<CMainFrame*>(theApp.m_pMainWnd)->GetGuiSubject().UnselectedPolylines(true);
}

void CDockableRoutesPane::OnChangeRouteBtnClick()
{
	int routeID;
	if(!GetSelectedRouteID(routeID))
	  return;
	COSMView * view = dynamic_cast<COSMView*>(GetActiveView());
	if(view==NULL)
		return ;
	COSMCtrlRoute * route = view->m_ctrlOSM.GetRouteByID(routeID);
	CDlgChangeRoute dlg(view->m_ctrlOSM,route->dbroute,this,false);
	dlg.DoModal();
	m_listCtrlPoints->DeleteAllItems();
}
void CDockableRoutesPane::OnUpdateNewRouteBtnClick(CCmdUI* pCmdUI)
{
	int indA = m_comboBoxLocA.GetCurSel();
	int indB = m_comboBoxLocB.GetCurSel();
	
	if(isManualAdd==true||indA<0||indB<0||indA==indB)
		pCmdUI->Enable(false);
	else
		pCmdUI->Enable(true);
}

void CDockableRoutesPane::OnUpdateDeleteRouteBtnClick(CCmdUI* pCmdUI)
{
	if(m_listCtrlRoutes->GetSelectedCount()<=0||isManualAdd==true)
		pCmdUI->Enable(false);
	else
		pCmdUI->Enable(true);
}
void CDockableRoutesPane::OnUpdateChangeRouteBtnClick(CCmdUI* pCmdUI)
{
	if(m_listCtrlRoutes->GetSelectedCount()<=0||isManualAdd==true)
		pCmdUI->Enable(false);
	else
		pCmdUI->Enable(true);
}

void CDockableRoutesPane::OnOpenManualAddPolyline(TAddPolylineProps props)
{
	isManualAdd = true;
	EnableWindow(FALSE);
}
void CDockableRoutesPane::OnCloseManualAddPolyline()
{
	isManualAdd = false;
	EnableWindow(TRUE);
}
void CDockableRoutesPane::AddRouteToList(CDBRoute& route)
{
	int nItem = m_listCtrlRoutes->InsertItem(LVIF_TEXT|LVIF_STATE,m_listCtrlRoutes->GetItemCount(),route.GetIdS(),	0, LVIS_SELECTED, 0, 0);
	m_listCtrlRoutes->SetItemText(nItem, 1, route.Name);
}
// CDockableRoutesPane message handlers
void CDockableRoutesPane::FillRoutesList()
{
	CString locNameA, locNameB;
	int indA = m_comboBoxLocA.GetCurSel();
	int indB = m_comboBoxLocB.GetCurSel();

	if(indA<0||indB<0)
		return;
	m_comboBoxLocA.GetLBText( indA, locNameA);
	m_comboBoxLocB.GetLBText( indB, locNameB);
	
    
	m_listCtrlRoutes->EnableWindow(true);
	m_listCtrlRoutes->DeleteAllItems();
	
	COSMView * view = dynamic_cast<COSMView*>(GetActiveView());
	if(view==NULL)
		return;
	COSMCtrlMarkerEx  *mA =  view->m_ctrlOSM.GetMarkerByName(locNameA);
	COSMCtrlMarkerEx  *mB =  view->m_ctrlOSM.GetMarkerByName(locNameB);
	if(mA==NULL|| mB == NULL)
		return;
	int idA = mA->location.ID;
	int idB = mB->location.ID;
	
	
	for(list<COSMCtrlRoute*>::iterator i = view->m_ctrlOSM.routes.begin(); i!= view->m_ctrlOSM.routes.end(); i++ )
	{
		if((*i)!=NULL)
		{
			if(((*i)->dbroute.LocFirst_ID == idA && (*i)->dbroute.LocFinish_ID == idB )||
			   ((*i)->dbroute.LocFirst_ID == idB && (*i)->dbroute.LocFinish_ID == idA))
			   this->AddRouteToList((*i)->dbroute);
		}
	}
}
CDocument* CDockableRoutesPane::GetActiveDocument()
{
	CMDIFrameWnd *pFrame =   (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild =   (CMDIChildWnd *) pFrame->GetActiveFrame();
	
	return pChild->GetActiveDocument();
}
CView* CDockableRoutesPane::GetActiveView()
{
	CMDIFrameWnd *pFrame =   (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild =   (CMDIChildWnd *) pFrame->GetActiveFrame();
	return pChild->GetActiveView();
}
void CDockableRoutesPane::OnSelectComboB()
{
	CString locName;
	int ind = m_comboBoxLocB.GetCurSel();
	if(ind<0)
		return;
	if(m_comboBoxLocA.GetCount()>0)
	{
		if(m_comboBoxLocA.GetCurSel()<0)
		{
			m_comboBoxLocA.SetCurSel(m_comboBoxLocB.GetCount()-1);
			OnSelectComboA();	
		}
	}
	FillRoutesList();
}
void CDockableRoutesPane::OnSelectComboA()
{
	CString locName;
	int ind = m_comboBoxLocA.GetCurSel();
	if(ind<0)
		return;
	if(m_comboBoxLocB.GetCount()>0)
	{
		if(m_comboBoxLocB.GetCurSel()<0)
		{
			m_comboBoxLocB.SetCurSel(m_comboBoxLocB.GetCount()-1);
			OnSelectComboB();	
			RedrawWindow();
		}
	}
	FillRoutesList();
}
int CDockableRoutesPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_comboBoxLocA.Create( WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST,CRect(15,25,200,50),this,ID_VIEWROUTES_COMBOA);
	m_comboBoxLocB.Create( WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST,CRect(15,80,200,50),this,ID_VIEWROUTES_COMBOB);
	m_comboBoxLocA.EnableWindow(FALSE);
	m_comboBoxLocB.EnableWindow(FALSE);

	m_listCtrlRoutes = new CListCtrl_Column_Picker;
	m_listCtrlRoutes->Create( WS_CHILD|WS_VISIBLE|WS_BORDER|LVS_REPORT, CRect(15,120,200,200),this,ID_ROUTELIST);
	m_listCtrlRoutes->SetExtendedStyle(m_listCtrlRoutes->GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	m_listCtrlRoutes->ModifyStyle(LVS_SINGLESEL, 0);
	m_listCtrlRoutes->InsertColumn(0,_T("ID"),LVCFMT_LEFT, 70);
	m_listCtrlRoutes->InsertColumn(1,_T("Name"),LVCFMT_LEFT, 100);
	m_listCtrlRoutes->ShowColumn(0,false);

	m_btnNewRoute.Create("New",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,CRect(0,230,100,280),this,ID_BUTTON_NEW);
	m_btnDeleteRoute.Create("Delete",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,CRect(0,230,100,280),this,ID_BUTTON_DELETE);
	m_btnChangeRoute.Create("Change...",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,CRect(),this,ID_BUTTONCHANGE);

	m_listCtrlPoints =  new CListCtrl_Column_Picker;
	m_listCtrlPoints->Create( WS_CHILD|WS_VISIBLE|WS_BORDER|LVS_REPORT, CRect(15,120,200,200),this,ID_RELATIONLIST);
	m_listCtrlPoints->SetExtendedStyle(m_listCtrlRoutes->GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	m_listCtrlPoints->ModifyStyle(LVS_SINGLESEL, 0);
	m_listCtrlPoints->InsertColumn(0,_T("Loc1"),LVCFMT_LEFT, 80);
	m_listCtrlPoints->InsertColumn(1,_T("Loc2"),LVCFMT_LEFT, 80);
	m_listCtrlPoints->InsertColumn(2,_T("Loc1_ID"),LVCFMT_LEFT, 80);
	m_listCtrlPoints->InsertColumn(3,_T("Loc2_ID"),LVCFMT_LEFT, 80);

	m_comboPolylines.Create( WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST,CRect(15,25,200,50),this,ID_VIEW_COMBOPOLYLINES);
	m_comboPolylines.EnableWindow(FALSE);
	return 0;
}
void CDockableRoutesPane::OnAddMarker(CDBLocation& loc)
{
	m_comboBoxLocA.AddString(loc.Name);
	m_comboBoxLocB.AddString(loc.Name);
	m_comboBoxLocA.EnableWindow(TRUE);
	m_comboBoxLocB.EnableWindow(TRUE);

}
void CDockableRoutesPane::OnDeleteMarker(int &locID, CString &Name)
{
	for(int i=0;i < m_comboBoxLocA.GetCount() ; i++)
	{
		CString locName;
		m_comboBoxLocA.GetLBText(i,locName);
		if(locName.Compare(Name)==0)
		{
			m_comboBoxLocA.DeleteString(i);
			break;
		}
	}
	for(int i=0;i < m_comboBoxLocB.GetCount() ; i++)
	{
		CString locName;
		m_comboBoxLocB.GetLBText(i,locName);
		if(locName.Compare(Name)==0)
		{
			m_comboBoxLocB.DeleteString(i);
			break;
		}
	}
}
void CDockableRoutesPane::OnFreeAllData()
{
	m_comboBoxLocA.EnableWindow(FALSE);
	m_comboBoxLocB.EnableWindow(FALSE);
	m_comboBoxLocA.ResetContent();
	m_comboBoxLocB.ResetContent();
	m_listCtrlPoints->DeleteAllItems();
	m_listCtrlRoutes->DeleteAllItems();
	
	m_comboPolylines.ResetContent();
	m_comboPolylines.EnableWindow(FALSE);

}
void CDockableRoutesPane::OnRefreshData(COSMCtrlEx& ctrl, bool forcibly)
{
	COSMDoc* doc = dynamic_cast<COSMDoc*>(GetActiveDocument());
	if(doc==NULL||(lastDocFile == doc->GetFileName() && forcibly==false))
		return;
	lastDocFile = doc->GetFileName();
	m_comboBoxLocA.Clear();
	m_comboBoxLocB.Clear();
	m_comboBoxLocA.ResetContent();
	m_comboBoxLocB.ResetContent();
	
	m_listCtrlRoutes->DeleteAllItems();
	
	vector<CDBLocation*> locs;
	locs.reserve(ctrl.GetMarkerCount());
	for(int i=0;i < ctrl.GetMarkerCount(); i++)
	{
		COSMCtrlMarkerEx* m =  dynamic_cast<COSMCtrlMarkerEx*>(&ctrl.GetMarker(i));
		if(m==NULL)continue;
		locs.push_back(&m->location);
	}
	// fill comboboxes
	for(vector<CDBLocation*>::iterator i = locs.begin(); i!= locs.end();i++)
	{
		m_comboBoxLocA.AddString((*i)->Name);
		m_comboBoxLocB.AddString((*i)->Name);
	}
	if(locs.size()>0)
	{
		m_comboBoxLocA.SetCurSel(0);
		OnSelectComboA();
		m_comboBoxLocA.EnableWindow(true);
		m_comboBoxLocB.EnableWindow(true);
		m_comboBoxLocA.RedrawWindow();
		m_comboBoxLocB.RedrawWindow();
	}
	if(ctrl.IsOpenManualAddPolyline())
	{
		this->EnableWindow(FALSE);
	}
}
void CDockableRoutesPane::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
	CRect r;
	GetClientRect(r);
	m_listCtrlRoutes->MoveWindow(15,135,r.Width()-130,120); 
	m_btnNewRoute.MoveWindow(r.Width()- 100,135,80,25); 
	m_btnDeleteRoute.MoveWindow(r.Width() - 100,175,80,25); 
	m_listCtrlPoints->MoveWindow(15,310,r.Width()-130,r.Height()-370); 
	m_btnChangeRoute.MoveWindow(r.Width()-100,310,80,25); 
	
	m_comboPolylines.MoveWindow(15,r.Height()-50,r.Width()-130,40);
	m_btnDeleteRoute.RedrawWindow();
	m_btnChangeRoute.RedrawWindow();
	RedrawWindow();

}


void CDockableRoutesPane::OnPaint()
{
	CPaintDC dc(this); 
	CRect r;
	GetClientRect(r);
	dc.Draw3dRect(r, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
	dc.Rectangle(0,0,r.Width()+5,r.Height()+5);
	dc.RoundRect(5,290,r.Width()-5, r.Height()-5,5,5);
	CFont Font;
	Font.CreateFont(14, 0, 0, 0, FW_BOLD, false, 0, 0, 
                                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
                                CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
                                DEFAULT_PITCH | FF_DONTCARE, "Arial");
	dc.SelectObject(&Font);
	dc.SetBkMode(TRANSPARENT);
	dc.TextOutA(15,5,"Location A:");
	dc.TextOutA(15,60,"Location B:");
	dc.TextOutA(15,115,"Routes:");
	dc.TextOutA(15,295,"Polylines:");
}
