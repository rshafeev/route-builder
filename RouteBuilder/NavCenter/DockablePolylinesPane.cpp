// DockablePolylinesPane.cpp : implementation file
//

#include "stdafx.h"

#include<vector>

#include "NavCenter.h"
#include "DockablePolylinesPane.h"
#include "GuiSubject.h"
#include "DlgAddPolyline.h"
#include "OSMDoc.h"
#include "OSMView.h"
#include "MainFrm.h"
#include "..\InetLib\RouteCloudeMade.h"
// CDockablePolylinesPane

IMPLEMENT_DYNAMIC(CDockablePolylinesPane, CDockablePane)

BEGIN_MESSAGE_MAP(CDockablePolylinesPane, CDockablePane)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(ID_VIEWPOLYLINES_COMBOA,OnSelectComboA)
	ON_CBN_SELCHANGE(ID_VIEWPOLYLINES_COMBOB,OnSelectComboB)

	ON_WM_SIZE()

	ON_BN_CLICKED(ID_BUTTON_ADD, &CDockablePolylinesPane::OnAddBtnClick)
    ON_UPDATE_COMMAND_UI(ID_BUTTON_ADD, &CDockablePolylinesPane::OnUpdateAddBtnClick)

	ON_BN_CLICKED(ID_BUTTON_DELETE, &CDockablePolylinesPane::OnDeleteBtnClick)
    ON_UPDATE_COMMAND_UI(ID_BUTTON_DELETE, &CDockablePolylinesPane::OnUpdateDeleteBtnClick)

	ON_BN_CLICKED(ID_BUTTONSAVE, &CDockablePolylinesPane::OnSaveBtnClick)
    ON_UPDATE_COMMAND_UI(ID_BUTTONSAVE, &CDockablePolylinesPane::OnUpdateSaveBtnClick)

	ON_BN_CLICKED(ID_BUTTONCANCEL, &CDockablePolylinesPane::OnCancelBtnClick)
    ON_UPDATE_COMMAND_UI(ID_BUTTONCANCEL, &CDockablePolylinesPane::OnUpdateCancelBtnClick)

	ON_NOTIFY(NM_CLICK,ID_POLYLINELIST, &CDockablePolylinesPane::OnListPolylinesClick)
	ON_NOTIFY(NM_CLICK,ID_POINTLIST, &CDockablePolylinesPane::OnListPointsClick)

	ON_WM_DESTROY()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


CDockablePolylinesPane::CDockablePolylinesPane(CGuiSubject &guiSubject)
{
	SetSubject(&guiSubject);
	subject->Attach(this);
	isManualAdd = false;
	lastDocFile = "";
}
void CDockablePolylinesPane::LoadRoadFromCloudMade(TAddPolylineProps props)
{
	COSMView * view = dynamic_cast<COSMView*>(GetActiveView());
	
	try
	{
		if(view!=NULL)
		{
			COSMCtrlMarkerEx * mA = view->m_ctrlOSM.GetMarkerByName(props.agentA);
			COSMCtrlMarkerEx * mB = view->m_ctrlOSM.GetMarkerByName(props.agentB);
			if(mA==NULL || mB == NULL)
				return;
			CDBPolyLine addLine;
			CString geom;
			double distance;
			double time;
			list<TLocation> points;
			CRouteCloudeMade route(_T(theApp.GetProfileStringA("OptionsParam","CloudKey","")));
			double smooth = atof(theApp.GetProfileStringA("OptionsParam","Smoothing","-1"));
			route.GetGeom(mA->location.Lat,mA->location.Lon, mB->location.Lat, mB->location.Lon,geom,distance,time,smooth);
			if(geom == "")
				throw CRouteCloudeMadeException("CloudeMade was not build route.");
			addLine.Name = props.name;
			addLine.Geom = geom;
			addLine.Loc1_ID = mA->location.ID;
			addLine.Loc2_ID = mB->location.ID;
			addLine.Distance = distance;
			addLine.Type = props.type;
			addLine.Time = time;
			/*if(props.type!=CDBPolyLine::c_ship)
				addLine.Time = 0;
			else
				addLine.Time = props.time;*/
			if(view->GetDocument()->GetDBService()->AddPolyline(addLine)==false)
				throw exception("CDockablePolylinesPane::OnSaveBtnClick() : error!");
			dynamic_cast<CMainFrame*>(theApp.m_pMainWnd)->AddInfoString("Add polyline '"+props.name+"' between " + props.agentA +" and " + props.agentB);
			subject->AddPolyline(addLine);
		}
	}
	catch(CRouteCloudeMadeException &ex)
	{
		MessageBox(ex.GetErrorText());
	}
	catch(exception &ex)
	{
		TRACE(ex.what());
		MessageBox("Error! Do not save to database!");
	}
	catch(...)
	{
		TRACE("catch :: CDockablePolylinesPane::OnSaveBtnClick() :: unknown error");
		MessageBox("Error! Do not save to database!");
	}
}

void CDockablePolylinesPane::OnAddBtnClick()
{
	CDlgAddPolyline *dlg = new CDlgAddPolyline(this,NULL);
	if(dlg->DoModal()==IDOK)
	{
		COSMView * view = dynamic_cast<COSMView*>(GetActiveView());
		if(view==NULL)
			return;
		TAddPolylineProps prop = dlg->m_addPolylineProps;
	
		prop.agentA = this->GetComboAText();
		prop.agentB = this->GetComboBText();
		if((prop.type == CDBPolyLine::c_road&&prop.IsCloudmade==false)||
			prop.type == CDBPolyLine::c_ship||
			prop.type == CDBPolyLine::c_train||
			prop.type == CDBPolyLine::c_air)
				subject->OpenManualAddPolyline(prop);
		else
        if(prop.type == CDBPolyLine::c_road&&prop.IsCloudmade==true)
				LoadRoadFromCloudMade(prop);
		else
			if(prop.type == CDBPolyLine::c_air)
			{
				try
				{
					if(view!=NULL)
					{
						COSMCtrlMarkerEx * mA = view->m_ctrlOSM.GetMarkerByName(prop.agentA);
						COSMCtrlMarkerEx * mB = view->m_ctrlOSM.GetMarkerByName(prop.agentB);
						if(mA==NULL || mB == NULL)
							 return;
						list<TLocation> points;
						points.push_back(TLocation(mA->location.Lat,mA->location.Lon));
						points.push_back(TLocation(mB->location.Lat,mB->location.Lon));
						CDBPolyLine addLine;
						addLine.Name = prop.name;
						addLine.Geom = CDBPolyLine::MasToGeom(points);
						addLine.Loc1_ID = mA->location.ID;
						addLine.Loc2_ID = mB->location.ID;
						addLine.Distance = COSMCtrlEx::GetDistance(points)/1000.0;
						addLine.Type = prop.type;
						if(view->GetDocument()->GetDBService()->AddPolyline(addLine)==false)
							throw exception("CDockablePolylinesPane::OnSaveBtnClick() : error!");
						subject->AddPolyline(addLine);
						dynamic_cast<CMainFrame*>(theApp.m_pMainWnd)->AddInfoString("Add polyline '"+prop.name+"' between " + prop.agentA +" and " + prop.agentB);
					}
				}
				catch(CRouteCloudeMadeException &ex)
				{
					MessageBox(ex.GetErrorText());
				}
				catch(exception &ex)
				{
					TRACE(ex.what());
					MessageBox("Error! Do not save to database!");
				}
				catch(...)
				{
					TRACE("catch :: CDockablePolylinesPane::OnSaveBtnClick() :: unknown error");
					MessageBox("Error! Do not save to database!");
				}
			}
		

	}
	delete dlg;
}
void CDockablePolylinesPane::OnUpdateAddBtnClick(CCmdUI* pCmdUI)
{
	int indA = m_comboBoxLocA.GetCurSel();
	int indB = m_comboBoxLocB.GetCurSel();
	
	if(isManualAdd==true||indA<0||indB<0||indA==indB)
		pCmdUI->Enable(false);
	else
		pCmdUI->Enable(true);
}
void CDockablePolylinesPane::OnDeleteBtnClick()
{
	int lineID;
	try
	{
		if(GetSelectedPolylineID(lineID))
		{
			COSMDoc * doc = dynamic_cast<COSMDoc*>(GetActiveDocument());
			if(doc==NULL)
				throw exception("CDockablePolylinesPane::OnSaveBtnClick() : error!");
			COSMView * view = dynamic_cast<COSMView*>(GetActiveView());
			if(view==NULL)
				return;
			COSMCtrlPolylineEx* oldLine =  view->m_ctrlOSM.GetPolylineByID(lineID);
			if(oldLine==NULL)
				return;
			list<COSMCtrlPolylineEx*> lines = view->m_ctrlOSM.GetPolylinesBetwwenLocations(oldLine->m_dbPolyLine.Loc1_ID, oldLine->m_dbPolyLine.Loc2_ID);
			
			list<COSMCtrlRoute*> routes = view->m_ctrlOSM.GetRoutesUsingPolyline(lineID);
			if(lines.size() <= 1)
			{
				for(list<COSMCtrlRoute*>::iterator i = routes.begin(); i!= routes.end(); i++)
				{
					if(doc->GetDBService()->DeleteRoute((*i)->dbroute.ID))
					{
						COSMCtrlMarkerEx* mA = view->m_ctrlOSM.GetMarkerByID((*i)->dbroute.LocFirst_ID);
						COSMCtrlMarkerEx* mB = view->m_ctrlOSM.GetMarkerByID((*i)->dbroute.LocFinish_ID);

						dynamic_cast<CMainFrame*>(theApp.m_pMainWnd)->AddInfoString("Delete route '"+(*i)->dbroute.Name+"' between"+
							mA->location.Name + " and " + mB->location.Name);
						subject->DeleteRoute((*i)->dbroute.ID);
					}
					
				}
			}
			else
			{
				  COSMCtrlPolylineEx* newLine = NULL;
				  for(list<COSMCtrlPolylineEx*>::iterator i = lines.begin(); i!= lines.end(); i++)
				  {
					  if((*i)->m_dbPolyLine.ID!=lineID)
					  {
						  newLine = *i;
						  break;
					  }
				  }
				  if(newLine!=NULL)
				  {
						for(list<COSMCtrlRoute*>::iterator i = routes.begin(); i!= routes.end(); i++)
						{

							(*i)->DeletePolyline(oldLine->m_dbPolyLine.ID);
							(*i)->AddPolyline(newLine);
							doc->GetDBService()->AddRelation(CDBRelation((*i)->dbroute.ID,newLine->m_dbPolyLine.ID));
							doc->GetDBService()->DeleteRelation(CDBRelation((*i)->dbroute.ID,oldLine->m_dbPolyLine.ID));

						}		  
				  }
			}

			if(doc->GetDBService()->DeletePolyline(lineID)==false)
				throw exception("CDockablePolylinesPane::OnSaveBtnClick() : error!");
			else
			{
				COSMCtrlMarkerEx* mA = view->m_ctrlOSM.GetMarkerByID(oldLine->m_dbPolyLine.Loc1_ID);
				COSMCtrlMarkerEx* mB = view->m_ctrlOSM.GetMarkerByID(oldLine->m_dbPolyLine.Loc2_ID);
				dynamic_cast<CMainFrame*>(theApp.m_pMainWnd)->AddInfoString("Delete polyline '"+oldLine->m_dbPolyLine.Name+"' between"+
										  mA->location.Name + " and " + mB->location.Name);
				subject->DeletePolyline(lineID);
			}
		}
	}
	catch(...)
	{
		MessageBox("Error! The polyline was not save  to database!");
	}
}
void CDockablePolylinesPane::OnUpdateDeleteBtnClick(CCmdUI* pCmdUI)
{
	if(m_listCtrlPolylines->GetSelectedCount()<=0||isManualAdd==true)
		pCmdUI->Enable(false);
	else
		pCmdUI->Enable(true);
}
void CDockablePolylinesPane::OnSaveBtnClick()
{
	COSMView * view = dynamic_cast<COSMView*>(GetActiveView());
	try
	{
		if(view!=NULL)
		{
			COSMCtrlMarkerEx * mA = view->m_ctrlOSM.GetMarkerByName(manualProp.agentA);
			COSMCtrlMarkerEx * mB = view->m_ctrlOSM.GetMarkerByName(manualProp.agentB);
			if(mA==NULL || mB == NULL)
				return;
			CDBPolyLine addLine;
			CString geom;
			list<TLocation> points = GetPointsFromList();
			points.push_front(TLocation(mA->location.Lat,mA->location.Lon));
			points.push_back(TLocation(mB->location.Lat,mB->location.Lon));

			addLine.Name = manualProp.name;
			addLine.Geom = CDBPolyLine::MasToGeom(points);
			addLine.Loc1_ID = mA->location.ID;
			addLine.Loc2_ID = mB->location.ID;
			addLine.Distance = COSMCtrlEx::GetDistance(points)/1000.0;
			addLine.Type = manualProp.type;
			if(manualProp.type!=CDBPolyLine::c_ship)
				addLine.Time = 0;
			else
				addLine.Time = manualProp.time;
			if(view->GetDocument()->GetDBService()->AddPolyline(addLine)==false)
				throw exception("CDockablePolylinesPane::OnSaveBtnClick() : error!");
			dynamic_cast<CMainFrame*>(theApp.m_pMainWnd)->AddInfoString("Add polyline '"+manualProp.name+"' between " + manualProp.agentA +" and " + manualProp.agentB);
			subject->AddPolyline(addLine);
			
			
			//subject->
		}
	}
	catch(exception &ex)
	{
		TRACE(ex.what());
		MessageBox("Error! Do not save to database!");
	}
	catch(...)
	{
		TRACE("catch :: CDockablePolylinesPane::OnSaveBtnClick() :: unknown error");
		MessageBox("Error! Do not save to database!");
	}
	subject->CloseManualAddPolyline();
}

void CDockablePolylinesPane::OnUpdateSaveBtnClick(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(true);
}

void CDockablePolylinesPane::OnOpenManualAddPolyline(TAddPolylineProps props)
{
	m_btnCancelPolyline.ShowWindow(SW_SHOW);
	m_btnSavePolyline.ShowWindow(SW_SHOW);
	m_comboBoxLocA.EnableWindow(FALSE);
	m_comboBoxLocB.EnableWindow(FALSE);

	isManualAdd = true;
	this->manualProp = props;
	m_listCtrlPoints->DeleteAllItems();
}
void CDockablePolylinesPane::OnCloseManualAddPolyline()
{
	m_btnCancelPolyline.ShowWindow(SW_HIDE);
	m_btnSavePolyline.ShowWindow(SW_HIDE);
	m_listCtrlPoints->DeleteAllItems();
	m_comboBoxLocA.EnableWindow(TRUE);
	m_comboBoxLocB.EnableWindow(TRUE);
	isManualAdd = false;
}

void CDockablePolylinesPane::OnCancelBtnClick()
{
	subject->CloseManualAddPolyline();
}
CString CDockablePolylinesPane::GetComboAText()
{
	CString s = "";
	int ind = m_comboBoxLocA.GetCurSel();
	if(ind<0)
		return s;
	m_comboBoxLocA.GetLBText(ind,s);
	return s;
}

CString CDockablePolylinesPane::GetComboBText()
{
	CString s = "";
	int ind = m_comboBoxLocB.GetCurSel();
	if(ind<0)
		return s;
	m_comboBoxLocB.GetLBText(ind,s);
	return s;
}

void CDockablePolylinesPane::OnUpdateCancelBtnClick(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(true);
}


CDocument* CDockablePolylinesPane::GetActiveDocument()
{
	CMDIFrameWnd *pFrame =   (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild =   (CMDIChildWnd *) pFrame->GetActiveFrame();
	
	return pChild->GetActiveDocument();
}
CView* CDockablePolylinesPane::GetActiveView()
{
	CMDIFrameWnd *pFrame =   (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild =   (CMDIChildWnd *) pFrame->GetActiveFrame();
	return pChild->GetActiveView();
}
void CDockablePolylinesPane::OnAddPolyline(CDBPolyLine& polyLine)
{
	AddPolylineToList(polyLine);
	

}
void CDockablePolylinesPane::OnDeletePolyline(int& lineID)
{
	m_listCtrlPoints->DeleteAllItems();

	for(int i=0;i < m_listCtrlPolylines->GetItemCount();i++)
	{
	    CString idS =   m_listCtrlPolylines->GetItemText(i,0);
		if(atoi(idS) == lineID)
		{
			m_listCtrlPolylines->DeleteItem(i);
			return;
		}
	}
}
void CDockablePolylinesPane::OnSelectRoute(int & routeID)
{
	POSITION pos = m_listCtrlPolylines->GetFirstSelectedItemPosition();
	while(pos!=NULL)
	{
		int ind  = this->m_listCtrlPolylines->GetNextSelectedItem(pos);
		m_listCtrlPolylines->SetItemState(ind, 0, LVIS_SELECTED);
	}
	m_listCtrlPoints->DeleteAllItems();
}

CDockablePolylinesPane::~CDockablePolylinesPane()
{
	subject->Detach(this);
}
bool CDockablePolylinesPane::GetSelectedPolylineID(int &ID)
{
	POSITION pos = m_listCtrlPolylines->GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int ind = m_listCtrlPolylines->GetNextSelectedItem(pos);
        CString s =   m_listCtrlPolylines->GetItemText(ind,0);
		if(s!="")
		{
			ID = atoi(s);
			return true;
		}
	}
	return false;
}
void CDockablePolylinesPane::AddPolylineToList(CDBPolyLine& line)
{
	int nItem = m_listCtrlPolylines->InsertItem(LVIF_TEXT|LVIF_STATE,m_listCtrlPolylines->GetItemCount(),line.GetIdS(),	0, LVIS_SELECTED, 0, 0);
	m_listCtrlPolylines->SetItemText(nItem, 1, line.Name);
	m_listCtrlPolylines->SetItemText(nItem, 2, CDBPolyLine::GetType(line.Type));
	m_listCtrlPolylines->SetItemText(nItem, 3, line.GetDistanceS());
}

void CDockablePolylinesPane::FillPolylinesList()
{
	CString locNameA, locNameB;
	int indA = m_comboBoxLocA.GetCurSel();
	int indB = m_comboBoxLocB.GetCurSel();

	if(indA<0||indB<0)
		return;
	m_comboBoxLocA.GetLBText( indA, locNameA);
	m_comboBoxLocB.GetLBText( indB, locNameB);


	m_listCtrlPolylines->EnableWindow(true);
	m_listCtrlPolylines->DeleteAllItems();
	m_listCtrlPoints->DeleteAllItems();
	COSMView * view = dynamic_cast<COSMView*>(GetActiveView());
	if(view==NULL)
		return;
	COSMCtrlMarkerEx  *mA =  view->m_ctrlOSM.GetMarkerByName(locNameA);
	COSMCtrlMarkerEx  *mB =  view->m_ctrlOSM.GetMarkerByName(locNameB);
	if(mA==NULL|| mB == NULL)
		return;
	int idA = mA->location.ID;
	int idB = mB->location.ID;
	
	for(int i=0;i < view->m_ctrlOSM.GetPolylineCount(); i++)
	{
		COSMCtrlPolylineEx* line = dynamic_cast<COSMCtrlPolylineEx*>(&view->m_ctrlOSM.GetPolyline(i));
		if(line!=NULL)
		{
			if((line->m_dbPolyLine.Loc1_ID == idA && line->m_dbPolyLine.Loc2_ID == idB )||
			   (line->m_dbPolyLine.Loc1_ID == idB && line->m_dbPolyLine.Loc2_ID == idA))
			   this->AddPolylineToList(line->m_dbPolyLine);
		}
	}
}

bool  CDockablePolylinesPane::SelectCombo(CComboBox &m_combo,CString locName)
{
	for(int i=0;i < m_combo.GetCount();i++)
	{
		CString name;
		m_combo.GetLBText(i,name);
		if(name.Compare(locName)==0)
		{
			m_combo.SetCurSel(i);
			return true;
		}
	}
	return false;
}

list<TLocation> CDockablePolylinesPane::GetPointsFromList()
{
	list<TLocation> points;
	for(int i=0;i < m_listCtrlPoints->GetItemCount();i++)
	{
		CString latS = m_listCtrlPoints->GetItemText(i,0);
		CString lonS = m_listCtrlPoints->GetItemText(i,1);
		if(latS=="" || lonS =="")
			continue;
		points.push_back(TLocation(atof(latS),atof(lonS)));
	}

	return points;
}

void CDockablePolylinesPane::AddPoint(double lat, double lon)
{
	CString latS,lonS;
	latS.Format("%f",lat);
	lonS.Format("%f",lon);
	int nItem = m_listCtrlPoints->InsertItem(LVIF_TEXT|LVIF_STATE,m_listCtrlPoints->GetItemCount(),latS,0, LVIS_SELECTED, 0, 0);
	m_listCtrlPoints->SetItemText(nItem, 1, lonS);

}

bool CDockablePolylinesPane::IsExistPolylineNameIntoListCtrl(CString &polylineName)
{
	
	for(int i=0;i <m_listCtrlPolylines->GetItemCount();i++ )
	{
        CString name =   m_listCtrlPolylines->GetItemText(i,1);
		if(name.Compare(polylineName)==0)
			return true;
	}
	return false;
}

void CDockablePolylinesPane::OnSelectComboB()
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
	FillPolylinesList();
}
void CDockablePolylinesPane::OnSelectComboA()
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
	FillPolylinesList();
}

void CDockablePolylinesPane::OnFreeAllData()
{
	m_comboBoxLocA.EnableWindow(false);
	m_comboBoxLocB.EnableWindow(false);
	m_comboBoxLocA.ResetContent();
	m_comboBoxLocB.ResetContent();
	m_listCtrlPolylines->DeleteAllItems();
	m_listCtrlPoints->DeleteAllItems();
	m_btnCancelPolyline.ShowWindow(SW_HIDE);
	m_btnSavePolyline.ShowWindow(SW_HIDE);
}

void CDockablePolylinesPane::OnAddMarker(CDBLocation& loc)
{
	m_comboBoxLocA.AddString(loc.Name);
	m_comboBoxLocB.AddString(loc.Name);
	m_comboBoxLocA.EnableWindow(TRUE);
	m_comboBoxLocB.EnableWindow(TRUE);
}
void CDockablePolylinesPane::OnDeleteMarker(int &locID, CString &Name)
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

void CDockablePolylinesPane::OnPaint()
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
	dc.TextOutA(15,115,"Polylines:");
	dc.TextOutA(15,295,"Points:");
     	
}


int CDockablePolylinesPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_comboBoxLocA.Create( WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST,CRect(15,25,200,50),this,ID_VIEWPOLYLINES_COMBOA);
	m_comboBoxLocB.Create( WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST,CRect(15,80,200,50),this,ID_VIEWPOLYLINES_COMBOB);
	m_comboBoxLocA.EnableWindow(false);
	m_comboBoxLocB.EnableWindow(false);
	
	m_listCtrlPolylines = new CListCtrl_Column_Picker;
	m_listCtrlPolylines->Create( WS_CHILD|WS_VISIBLE|WS_BORDER|LVS_REPORT, CRect(15,120,200,200),this,ID_POLYLINELIST);
	m_listCtrlPolylines->SetExtendedStyle(m_listCtrlPolylines->GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	m_listCtrlPolylines->ModifyStyle(LVS_SINGLESEL, 0);
	m_listCtrlPolylines->InsertColumn(0,_T("ID"),LVCFMT_LEFT, 70);
	m_listCtrlPolylines->InsertColumn(1,_T("Name"),LVCFMT_LEFT, 100);
	m_listCtrlPolylines->InsertColumn(2,_T("Type"),LVCFMT_LEFT, 80);
	m_listCtrlPolylines->InsertColumn(3,_T("Distance(km)"),LVCFMT_LEFT, 100);
	m_listCtrlPolylines->ShowColumn(0,false);

	m_btnAddPolyline.Create("Add",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,CRect(0,230,100,280),this,ID_BUTTON_ADD);
	m_btnDeletePolyline.Create("Delete",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,CRect(0,230,100,280),this,ID_BUTTON_DELETE);
	m_btnSavePolyline.Create("Save",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,CRect(),this,ID_BUTTONSAVE);
	m_btnCancelPolyline.Create("Cancel",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,CRect(),this,ID_BUTTONCANCEL);

	m_listCtrlPoints =  new CListCtrl_Column_Picker;
	m_listCtrlPoints->Create( WS_CHILD|WS_VISIBLE|WS_BORDER|LVS_REPORT, CRect(15,120,200,200),this,ID_POINTLIST);
	m_listCtrlPoints->SetExtendedStyle(m_listCtrlPolylines->GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	m_listCtrlPoints->ModifyStyle(LVS_SINGLESEL, 0);
	m_listCtrlPoints->InsertColumn(0,_T("Lat"),LVCFMT_LEFT, 80);
	m_listCtrlPoints->InsertColumn(1,_T("Lon"),LVCFMT_LEFT, 80);

	m_btnCancelPolyline.ShowWindow(SW_HIDE);
	m_btnSavePolyline.ShowWindow(SW_HIDE);

	
	return 0;
}
void CDockablePolylinesPane::OnRefreshData(COSMCtrlEx& ctrl, bool forcibly)
{
	COSMDoc* doc = dynamic_cast<COSMDoc*>(GetActiveDocument());
	if(doc==NULL||(lastDocFile == doc->GetFileName() && forcibly==false))
		return;
	lastDocFile = doc->GetFileName();
	m_comboBoxLocA.Clear();
	m_comboBoxLocB.Clear();
	m_comboBoxLocA.ResetContent();
	m_comboBoxLocB.ResetContent();
	m_listCtrlPoints->DeleteAllItems();
	m_listCtrlPolylines->DeleteAllItems();
	
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
		SelectCombo(m_comboBoxLocA,this->manualProp.agentA);
		SelectCombo(m_comboBoxLocB,this->manualProp.agentB);
		FillPolylinesList();	
		list<TLocation> points = ctrl.GetManualPointsList();
		OnOpenManualAddPolyline(ctrl.GetManualProps());	
		for(list<TLocation>::iterator i  = points.begin(); i!=points.end();i++)
		{
			AddPoint(i->Lat,i->Lon);
		}
	}
}
void CDockablePolylinesPane::OnListPolylinesClick(NMHDR * a,LRESULT * b)
{
	int selectedID;
	if(GetSelectedPolylineID(selectedID)==true)
	{
		subject->SelectPolyline(selectedID);
		COSMView* view = dynamic_cast<COSMView*>(GetActiveView());
		COSMCtrlPolylineEx *line = view->m_ctrlOSM.GetPolylineByID(selectedID);
		m_listCtrlPoints->DeleteAllItems();
		if(line!=NULL)
		{
			list<TLocation> points =  CDBPolyLine::GeomToMas(line->m_dbPolyLine.Geom);
			int ind=-1;
			int n = points.size();
			for(list<TLocation>::iterator i = points.begin(); i!= points.end(); i++)
			{
				ind++;
				if(ind==0|| ind==n-1)
					continue;
				AddPoint(i->Lat,i->Lon);
			}
		}
	}
	else
	{
		subject->UnselectedPolylines(true);
		m_listCtrlPoints->DeleteAllItems();
	}
}
void CDockablePolylinesPane::OnListPointsClick(NMHDR * a,LRESULT * b)
{


}
void CDockablePolylinesPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	
	CRect r;
	GetClientRect(r);
	m_listCtrlPolylines->MoveWindow(15,135,r.Width()-30,120); 
	m_btnAddPolyline.MoveWindow(r.Width()-90 - 100,260,80,25); 
	m_btnDeletePolyline.MoveWindow(r.Width() - 100,260,80,25); 
	m_listCtrlPoints->MoveWindow(15,310,r.Width()-130,r.Height()-350); 

	m_btnSavePolyline.MoveWindow(r.Width()-100,310,80,25); 
	m_btnCancelPolyline.MoveWindow(r.Width()-100,350,80,25); 

	m_btnDeletePolyline.RedrawWindow();
	RedrawWindow();
}

void CDockablePolylinesPane::OnDestroy()
{
	__super::OnDestroy();

}


void CDockablePolylinesPane::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState, pWndOther, bMinimized);

	// TODO: Add your message handler code here
}
