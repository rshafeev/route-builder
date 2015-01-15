
#include "stdafx.h"
#include "DockableLocationsPane.h"
#include "NavCenter.h"

#include "OSMCtrlMarkerEx.h"
#include "OSMDoc.h"
#include "GuiSubject.h"
// CDockableLocationsPane

IMPLEMENT_DYNAMIC(CDockableLocationsPane, CDockablePane)


CDockableLocationsPane::CDockableLocationsPane(CGuiSubject &guiSubject)
{
	SetSubject(&guiSubject);
	subject->Attach(this);
	lastDocFile = "";
}

CDockableLocationsPane::~CDockableLocationsPane()
{
	subject->Detach(this);
}
void CDockableLocationsPane::OnOpenManualAddPolyline(TAddPolylineProps props)
{
	
	EnableWindow(FALSE);
}
void CDockableLocationsPane::OnCloseManualAddPolyline()
{
	EnableWindow(TRUE);
}

BEGIN_MESSAGE_MAP(CDockableLocationsPane, CDockablePane)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	
	ON_NOTIFY(NM_CLICK,ID_LOCATIONVIEW_LISTVIEW, &CDockableLocationsPane::OnListClick)
	ON_BN_CLICKED(ID_LOCATIONVIEW_BTNDELETE, &CDockableLocationsPane::OnDeleteBtnClick)
    ON_UPDATE_COMMAND_UI(ID_LOCATIONVIEW_BTNDELETE, &CDockableLocationsPane::OnUpdateDeleteBtnClick)
END_MESSAGE_MAP()



// CDockableLocationsPane message handlers
void CDockableLocationsPane::OnUpdateDeleteBtnClick(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(true);
}

bool CDockableLocationsPane::GetSelectedLocation(int &ID, CString &name)
{
    POSITION pos = m_listLocations->GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int ind = m_listLocations->GetNextSelectedItem(pos);
        CString s =   m_listLocations->GetItemText(ind,0);
		if(s!="")
		{
			ID = atof(s);
			name =  m_listLocations->GetItemText(ind,1);
			return true;
		}
	}
	return false;
}
CDocument* CDockableLocationsPane::GetActiveDocument()
{
	CMDIFrameWnd *pFrame =   (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild =   (CMDIChildWnd *) pFrame->GetActiveFrame();
	
	return pChild->GetActiveDocument();
}

void CDockableLocationsPane::OnDeleteBtnClick()
{
	int ID;
	CString Name;
	if(GetSelectedLocation(ID,Name)==true)
	{
		if(dynamic_cast<COSMDoc*>(GetActiveDocument())!=NULL)
		{
			COSMDoc* doc = (COSMDoc*)GetActiveDocument();
			if(doc->GetDBService()->DeleteLocation(ID))
			{
				
				subject->DeleteMarker(ID,Name);
			}
		}
	}
}

void  CDockableLocationsPane::OnListClick(NMHDR * a,LRESULT * b)
{
	int selectedID;
	CString name;
	if(GetSelectedLocation(selectedID,name))
		subject->SelectMarker(selectedID);
}

void CDockableLocationsPane::OnPaint()
{
	CPaintDC dc(this); 
	CRect r;
	GetClientRect(r);
	dc.Draw3dRect(r, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
	dc.Rectangle(0,0,r.Width()+5,r.Height()+5);
}


void CDockableLocationsPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	CRect r;
	GetClientRect(r);
	m_listLocations->MoveWindow(10,10,r.Width()-20,r.Height()-60);

	CRect r_DeleteBtn;
	m_deleteBtn->GetWindowRect(r_DeleteBtn);
	m_deleteBtn->MoveWindow(r.Width()-80,r.Height()-40,r_DeleteBtn.Width(),r_DeleteBtn.Height());
}

int CDockableLocationsPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_listLocations = new CListCtrl;//CListCtrl_Column_Picker
	m_listLocations->Create( WS_CHILD|WS_VISIBLE|WS_BORDER|LVS_REPORT, CRect(10,10,200,200),this,ID_LOCATIONVIEW_LISTVIEW);
    m_listLocations->SetExtendedStyle(m_listLocations->GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	m_listLocations->ModifyStyle(LVS_SINGLESEL, 0);
	m_listLocations->InsertColumn(0,_T("ID"),LVCFMT_LEFT, 100);
	m_listLocations->InsertColumn(1,_T("Name"), LVCFMT_LEFT,100);
	m_listLocations->InsertColumn(2,_T("City"),LVCFMT_LEFT, 100);
	m_listLocations->InsertColumn(3,_T("Country"),LVCFMT_LEFT, 100);
	m_listLocations->InsertColumn(4,_T("Lat"),LVCFMT_LEFT, 100);
	m_listLocations->InsertColumn(5,_T("Lon"),LVCFMT_LEFT, 100);
	//m_listLocations->ShowColumn(0,false);
	
	m_deleteBtn = new CButton();
	m_deleteBtn->Create("Delete",WS_CHILD|WS_VISIBLE| BS_PUSHBUTTON,CRect(0,0,70,30),this,ID_LOCATIONVIEW_BTNDELETE);
	m_deleteBtn->ShowWindow(TRUE);
	m_deleteBtn->UpdateWindow();

	return 0;
}

void CDockableLocationsPane::OnFreeAllData()
{
	m_listLocations->DeleteAllItems();
}
void CDockableLocationsPane::OnAddMarker(CDBLocation& loc)
{
	int nItem = m_listLocations->InsertItem(0,loc.GetIdS());
	m_listLocations->SetItemText(nItem, 1, loc.Name);
	m_listLocations->SetItemText(nItem, 2, loc.City);
	m_listLocations->SetItemText(nItem, 3, loc.Country);
	m_listLocations->SetItemText(nItem, 4, loc.GetLatS());
	m_listLocations->SetItemText(nItem, 5, loc.GetLonS());
}
void CDockableLocationsPane::OnDeleteMarker(int &locID, CString &Name)
{
	for(int i=0; i < m_listLocations->GetItemCount(); i++)
	{
		CString locIDS = m_listLocations->GetItemText(i,0);
		if(atoi(locIDS)==locID)
		{
			m_listLocations->DeleteItem(i);
			break;
		}
	}
}
void CDockableLocationsPane::OnSelectMarker(int& locID)
{

}
void CDockableLocationsPane::OnRefreshData(COSMCtrlEx& ctrl,bool forcibly)
{
	COSMDoc* doc = dynamic_cast<COSMDoc*>(GetActiveDocument());
	if(doc==NULL||(lastDocFile == doc->GetFileName()&&forcibly==false))
		return;
	lastDocFile = doc->GetFileName();
	m_listLocations->DeleteAllItems();
	for(int i=0;i < ctrl.GetMarkerCount(); i++)
	{
			int nItem;
			COSMCtrlMarkerEx* m = dynamic_cast<COSMCtrlMarkerEx*>(&ctrl.GetMarker(i));
			if(m==NULL)continue;
			nItem = m_listLocations->InsertItem(0, m->location.GetIdS());
			m_listLocations->SetItemText(nItem, 1, m->location.Name);
			m_listLocations->SetItemText(nItem, 2, m->location.City);
			m_listLocations->SetItemText(nItem, 3, m->location.Country);
			m_listLocations->SetItemText(nItem, 4, m->location.GetLatS());
			m_listLocations->SetItemText(nItem, 5, m->location.GetLonS());
	}
	if(ctrl.IsOpenManualAddPolyline())
	{
		this->EnableWindow(FALSE);
	}

}
