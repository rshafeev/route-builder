// DlgChangeRoute.cpp : implementation file
//

#include "stdafx.h"
#include "NavCenter.h"
#include "DlgChangeRoute.h"
#include "afxdialogex.h"

#include "OSMDoc.h"
#include "OSMView.h"
#include "MainFrm.h"
// CDlgChangeRoute dialog

IMPLEMENT_DYNAMIC(CDlgChangeRoute, CDialogEx)

BEGIN_MESSAGE_MAP(CDlgChangeRoute, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTONADDAFTER, &CDlgChangeRoute::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTONDELETE, &CDlgChangeRoute::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDOK, &CDlgChangeRoute::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTONADDFIRST, &CDlgChangeRoute::OnBnClickedButtonaddfirst)
	ON_BN_CLICKED(IDC_BUTTONADDLAST, &CDlgChangeRoute::OnBnClickedButtonaddlast)
	ON_MESSAGE_VOID(WM_KICKIDLE, OnKickIdle)
	ON_UPDATE_COMMAND_UI(IDC_BUTTONADDLAST, &CDlgChangeRoute::OnUpdateAddLastBtnClick)
    ON_UPDATE_COMMAND_UI(IDC_BUTTONADDAFTER, &CDlgChangeRoute::OnUpdateAddAfterBtnClick)
    ON_UPDATE_COMMAND_UI(IDC_BUTTONDELETE, &CDlgChangeRoute::OnUpdateDeleteBtnClick)

END_MESSAGE_MAP()

CDlgChangeRoute::CDlgChangeRoute(COSMCtrlEx& osmCtrl,CDBRoute& route, CWnd* pParent /*=NULL*/,bool isNew/*=true*/)
	: CDialogEx(CDlgChangeRoute::IDD, pParent),	
	  route(route), 
	  m_osmCtrl(osmCtrl),
	  m_isNew(isNew)
{
	for(int i=0;i  < osmCtrl.GetMarkerCount(); i++)
	{
		COSMCtrlMarkerEx* m  = dynamic_cast<COSMCtrlMarkerEx*>(&  osmCtrl.GetMarker(i));
		if(m == NULL)
			continue;
		locations.push_back( m->location);
	}
}

void CDlgChangeRoute::DeleteLocationFromList(int ID)
{
	for(int i=0;i < m_listLocs.GetItemCount();i++)
	{
	    CString idS =   m_listLocs.GetItemText(i,0);
		if(atoi(idS) == ID)
		{
			CString name = m_listLocs.GetItemText(i,1);
			m_comboLocs.AddString(name);
			m_listLocs.DeleteItem(i);

			return;
		}
	}
	

}
CDlgChangeRoute::~CDlgChangeRoute()
{
			
}

CString CDlgChangeRoute::GetFreeName()
{
	CString name;

	//for(int i=0;)
	

	return name;
}

void CDlgChangeRoute::OnKickIdle()
{
    UpdateDialogControls( this, FALSE );
}
void CDlgChangeRoute::OnUpdateAddLastBtnClick(CCmdUI* pCmdUI)
{
	CDBLocation loc;
	if(m_comboLocs.GetCurSel()<0||GetSelectedLocation(loc)==false)
		pCmdUI->Enable(false);
	else
		pCmdUI->Enable(true);

}
void CDlgChangeRoute::OnUpdateAddAfterBtnClick(CCmdUI* pCmdUI)
{
	CDBLocation loc;
	if(m_comboLocs.GetCurSel()<0||GetSelectedLocation(loc)==false)
		pCmdUI->Enable(false);
	else
		pCmdUI->Enable(true);
}
void CDlgChangeRoute::OnUpdateDeleteBtnClick(CCmdUI* pCmdUI)
{
	CDBLocation loc;
	if(GetSelectedLocation(loc)==false)
		pCmdUI->Enable(false);
	else
		pCmdUI->Enable(true);
}

bool CDlgChangeRoute::GetSelectedLocation(CDBLocation& location)
{
	POSITION pos = m_listLocs.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int ind = m_listLocs.GetNextSelectedItem(pos);
		CString idS = m_listLocs.GetItemText(ind,0);
		if(idS != "")
		{
			location.ID = atoi(idS);
			return true;
		}
	}
	return false;
}

void CDlgChangeRoute::AddLocationToList(CDBLocation &loc,int indAfter)
{
	int nItem = m_listLocs.InsertItem(LVIF_TEXT|LVIF_STATE,indAfter+1,loc.GetIdS(),	0, LVIS_SELECTED, 0, 0);
	m_listLocs.SetItemText(nItem, 1, loc.Name);
	m_listLocs.SetItemText(nItem, 2, loc.City);
	m_listLocs.SetItemText(nItem, 3, loc.Country);
	m_listLocs.SetItemText(nItem, 4, loc.GetLatS());
	m_listLocs.SetItemText(nItem, 5, loc.GetLonS());
	for(int i=0;i  < m_comboLocs.GetCount(); i++)
	{
		CString name;
		m_comboLocs.GetLBText(i,name);
		if(name.Compare(loc.Name)==0)
		{
			m_comboLocs.DeleteString(i);
			return;
		}
	}
}

void CDlgChangeRoute::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listLocs);
	DDX_Control(pDX, IDC_COMBO1, m_comboLocs);
	DDX_Control(pDX, IDC_EDIT2, m_editFirst);
	DDX_Control(pDX, IDC_EDIT1, m_editFinish);
	DDX_Control(pDX, IDC_EDIT_NAME2, m_editName);
	DDX_Control(pDX, IDC_STATICNAME, m_statictextName);
}





// CDlgChangeRoute message handlers

void CDlgChangeRoute::OnBnClickedButtonAdd()
{
	POSITION pos = m_listLocs.GetFirstSelectedItemPosition();
	int id1,id2;
	CString locName1,locName2;
	int ind;
	if(pos!=NULL)
	{
		ind = m_listLocs.GetNextSelectedItem(pos);
		CString idS = m_listLocs.GetItemText(ind,0);
		locName1 = m_listLocs.GetItemText(ind,1);
		id1 = atoi(idS);
	}
	else
		return;

	if(this->m_comboLocs.GetCurSel()<0)
		return;

	this->m_comboLocs.GetLBText(this->m_comboLocs.GetCurSel(),locName2);
	COSMCtrlMarkerEx* m = m_osmCtrl.GetMarkerByName(locName2);

	id2 = m->location.ID;
	COSMCtrlPolylineEx* line =  GetExistPolyline(id1,id2);
	
	this->m_editFirst.GetWindowTextA(locName2);
	if(line==NULL)
	{
		MessageBox("You can not add selected location, because you do not have polyline between '" + locName2 + "' and '" + locName1 +"'.");
		return;
	}
	else
	{
		AddLocationToList(m->location,ind);
	}

}


void CDlgChangeRoute::OnBnClickedButtonDelete()
{
	POSITION pos = m_listLocs.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int ind = m_listLocs.GetNextSelectedItem(pos);
		CString idS = m_listLocs.GetItemText(ind,0);
		DeleteLocationFromList(atoi(idS));
	}
}
bool CDlgChangeRoute::OnOkAdd()
{
	CString name;
	m_editName.GetWindowTextA(name);
	
	if(name=="")
	{
		MessageBox("Please, input name of new route!");
		return false;
	}
	
	for(list<COSMCtrlRoute*>::iterator i = m_osmCtrl.routes.begin(); i!= m_osmCtrl.routes.end(); i++)
	{
		if( (*i)->dbroute.Name.Compare(name)==0&& 
			((route.LocFirst_ID == (*i)->dbroute.LocFirst_ID && route.LocFinish_ID == (*i)->dbroute.LocFinish_ID)||
			 (route.LocFinish_ID == (*i)->dbroute.LocFirst_ID && route.LocFirst_ID == (*i)->dbroute.LocFinish_ID))
	      )
		{
				MessageBox("Name '" + name + "' of  the route already exist. Please, input other name!");
				return false;
		}
	}

	CMDIFrameWnd *pFrame =   (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild =   (CMDIChildWnd *) pFrame->GetActiveFrame();
	COSMDoc* doc = dynamic_cast<COSMDoc*>(pChild->GetActiveDocument());
	if(doc!=NULL)
	{
		route.Name = name;
		CString error;
		list<CDBRelation> relations = GetRelations(error);
		if(relations.empty())
		{
				MessageBox(error);
				return false;
		}

		if(doc->GetDBService()->AddRoute(route)==true)
		{
			for(list<CDBRelation>::iterator i = relations.begin(); i!= relations.end(); i++)
				i->Route_ID = route.ID;
			if(doc->GetDBService()->AddRelations(relations)==true)
			{
				route.relations = relations;
				COSMCtrlMarkerEx* mA = m_osmCtrl.GetMarkerByID(route.LocFirst_ID);
				COSMCtrlMarkerEx* mB = m_osmCtrl.GetMarkerByID(route.LocFinish_ID);
				if(mA!=NULL&& mB!=NULL)
				{
					dynamic_cast<CMainFrame*>(theApp.m_pMainWnd)->AddInfoString("Add route '"+route.Name+"' between " + mA->location.Name +" and " + mB->location.Name);
				}((CMainFrame*)theApp.m_pMainWnd)->GetGuiSubject().AddRoute(route);
				return true;
			}
			else
			{
				doc->GetDBService()->DeleteRoute(route.ID);
				MessageBox("Error! The route was not saved to database.");
				return false;
			}
		}
		else
		{
			MessageBox("Error! The route was not saved to database.");
			return false;
		}
	}
	else
	{
		MessageBox("Error of insert of route to database.");
		return false;
	}
	return false;
}
bool CDlgChangeRoute::OnOkChange()
{
	
	CMDIFrameWnd *pFrame =   (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild =   (CMDIChildWnd *) pFrame->GetActiveFrame();
	COSMDoc* doc = dynamic_cast<COSMDoc*>(pChild->GetActiveDocument());
	if(doc!=NULL)
	{
		CString error;
		list<CDBRelation> relations = GetRelations(error);
		if(relations.empty())
		{
				MessageBox(error);
				return false;
		}
		for(list<CDBRelation>::iterator i = relations.begin(); i!= relations.end(); i++)
			i->Route_ID = route.ID;
		if(doc->GetDBService()->DeleteRelationsByRoute(route.ID) && doc->GetDBService()->AddRelations(relations))
		{
			((CMainFrame*)theApp.m_pMainWnd)->GetGuiSubject().DeleteRoute(route.ID);
			route.relations = relations;
	 	    ((CMainFrame*)theApp.m_pMainWnd)->GetGuiSubject().AddRoute(route);
			return true;
		}
		else
		{
			MessageBox("Error! The route was not saved to database.");
			return false;
		}

	}
	return false;
}

COSMCtrlPolylineEx* CDlgChangeRoute::GetExistPolyline(int idA, int idB)
{
	CMDIFrameWnd *pFrame =   dynamic_cast<CMDIFrameWnd*>(AfxGetApp()->m_pMainWnd);
	CMDIChildWnd *pChild =   (CMDIChildWnd *) pFrame->GetActiveFrame();
	COSMView * view = dynamic_cast<COSMView*>(pChild->GetActiveView());
	if(view==NULL)
		return false;

	for(int i=0;i < view->m_ctrlOSM.GetPolylineCount(); i++ )
	{
		COSMCtrlPolylineEx* line =  dynamic_cast<COSMCtrlPolylineEx*>(&view->m_ctrlOSM.GetPolyline(i));
		if(line==NULL)
			continue;
		if((line->m_dbPolyLine.Loc1_ID == idA && line->m_dbPolyLine.Loc2_ID == idB)||
			(line->m_dbPolyLine.Loc2_ID == idA && line->m_dbPolyLine.Loc1_ID == idB))
		{
			return line;
		}
	}
	return NULL;
}

list<CDBRelation> CDlgChangeRoute::GetRelations(CString &errorString)
{
	vector<int> locsIDmas;
	list<CDBRelation> relations;
	locsIDmas.reserve(m_listLocs.GetItemCount()+2);

	locsIDmas.push_back(this->route.LocFirst_ID);
	for(int i=0;i < this->m_listLocs.GetItemCount(); i++)
	{
		locsIDmas.push_back(atoi(m_listLocs.GetItemText(i,0)));
	}
	locsIDmas.push_back(route.LocFinish_ID);

	for(int i=0;i < locsIDmas.size()-1; i++)
	{
		COSMCtrlPolylineEx* line = GetExistPolyline(locsIDmas[i], locsIDmas[i+1]);
		if( line==NULL)
		{
			CString name1,name2;
			COSMCtrlMarkerEx* m1 = m_osmCtrl.GetMarkerByID(locsIDmas[i]);
			COSMCtrlMarkerEx* m2 = m_osmCtrl.GetMarkerByID(locsIDmas[i+1]);
			if(m1!=NULL && m2 !=NULL)
			{
				name1 = m1->location.Name;
				name2 = m2->location.Name;
			}
			errorString.Format("You can not add selected location, because you do not have polyline between '%s' and '%s'.",name1,name2);
			relations.clear();
			return relations;
		}
		else
		{
			CDBRelation relation;
			relation.Line_ID = line->m_dbPolyLine.ID;
			relations.push_back(relation);
		}
	}
	return relations;
}

void CDlgChangeRoute::OnBnClickedOk()
{
	bool f = false;
	
	CString error;
	if(m_isNew==true)
		f = OnOkAdd();
	else
		f = OnOkChange();
	if(f)
		CDialogEx::OnOK();
	return;
}


BOOL CDlgChangeRoute::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	COSMCtrlMarkerEx* mA = m_osmCtrl.GetMarkerByID(route.LocFirst_ID);
	COSMCtrlMarkerEx* mB = m_osmCtrl.GetMarkerByID(route.LocFinish_ID);
	if(mA==NULL|| mB==NULL)
		return FALSE;
	
	m_editFirst.SetWindowTextA(mA->location.Name);
	m_editFinish.SetWindowTextA(mB->location.Name);
	
	m_listLocs.SetExtendedStyle(m_listLocs.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	m_listLocs.ModifyStyle(LVS_SINGLESEL, 0);
	m_listLocs.InsertColumn(0,_T("ID"),LVCFMT_LEFT, 70);
	m_listLocs.InsertColumn(1,_T("Name"),LVCFMT_LEFT, 70);
	m_listLocs.InsertColumn(2,_T("City"),LVCFMT_LEFT, 70);
	m_listLocs.InsertColumn(3,_T("Country"),LVCFMT_LEFT, 70);
	m_listLocs.InsertColumn(4,_T("Lat"),LVCFMT_LEFT, 70);
	m_listLocs.InsertColumn(5,_T("Lon"),LVCFMT_LEFT, 70);

	list<int> locsID;
	if(m_isNew)
	{
		m_editName.EnableWindow(TRUE);
	}
	else
	{
		COSMCtrlRoute* r =  m_osmCtrl.GetRouteByID(route.ID);
		if(r==NULL)
			return FALSE;

		m_editName.SetWindowTextA(this->route.Name);
		m_editName.EnableWindow(FALSE);

		list<COSMCtrlMarkerEx*> routeLocs;
		locsID = r->GetSortedIDLocations();
		for(list<int>::iterator i = locsID.begin(); i!= locsID.end(); i++)
		{
			COSMCtrlMarkerEx* marker = m_osmCtrl.GetMarkerByID(*i);
			if(marker!=NULL)
			routeLocs.push_back(marker);
		}
		routeLocs.pop_back();
		routeLocs.pop_front();
		for(list<COSMCtrlMarkerEx*>::iterator i = routeLocs.begin(); i != routeLocs.end(); i++)
		{
			AddLocationToList((*i)->location,m_listLocs.GetItemCount());
		}
	}
	for(list<CDBLocation>::iterator i = locations.begin(); i!= locations.end(); i++)
	{
		if(i->ID == route.LocFinish_ID|| i->ID == route.LocFirst_ID)
			continue;
		bool isExist = false;
		for(list<int>::iterator j = locsID.begin(); j!= locsID.end(); j++)
		{
			if(i->ID == *j)
			{
				isExist = true;
				break;
			}
		}
		if(isExist==true)
			continue;
		this->m_comboLocs.AddString(i->Name);
	}

	if(m_comboLocs.GetCount()>0)
		m_comboLocs.SetCurSel(0);
	return TRUE;  
}


void CDlgChangeRoute::OnBnClickedButtonaddfirst()
{
	if(this->m_comboLocs.GetCurSel()<0)
		return;
	CString locName1,locName2;
	this->m_comboLocs.GetLBText(this->m_comboLocs.GetCurSel(),locName1);
	COSMCtrlMarkerEx* m = m_osmCtrl.GetMarkerByName(locName1);

	int id1 = m->location.ID;
	int id2 = this->route.LocFirst_ID;
	COSMCtrlPolylineEx* line =  GetExistPolyline(id1,id2);
	
	this->m_editFirst.GetWindowTextA(locName2);
	if(line==NULL)
	{
		MessageBox("You can not add selected location, because you do not have polyline between '" + locName2 + "' and '" + locName1 +"'.");
		return;
	}
	else
	{
		AddLocationToList(m->location,-1);
	}

}


void CDlgChangeRoute::OnBnClickedButtonaddlast()
{
	// TODO: Add your control notification handler code here
}

