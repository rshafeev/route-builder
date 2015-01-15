// DlgAddLocation.cpp : implementation file
//

#include "stdafx.h"
#include "NavCenter.h"
#include "DlgAddLocation.h"
#include "afxdialogex.h"
#include "..\InetLib\OSMCtrlMarker.h"
#include "OSMDoc.h"
#include "MainFrm.h"
// CDlgAddLocation dialog

IMPLEMENT_DYNAMIC(CDlgAddLocation, CDialogEx)

CDlgAddLocation::CDlgAddLocation(COSMCtrlEx* OSMCtrlEx,COSMCtrlPosition m_rightClickPos,CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgAddLocation::IDD, pParent)

{
	this->m_rightClickPos = m_rightClickPos;
	this->m_OSMCtrlEx =OSMCtrlEx;
}

CDlgAddLocation::~CDlgAddLocation()
{
}

void CDlgAddLocation::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITLON, m_editLon);
	DDX_Control(pDX, IDC_EDITLAT, m_editLat);
	DDX_Control(pDX, IDC_EDITNAME, m_editName);
	DDX_Control(pDX, IDC_EDITCITY, m_editCity);
	DDX_Control(pDX, IDC_EDITCOUNTRY, m_editCountry);
}


BEGIN_MESSAGE_MAP(CDlgAddLocation, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgAddLocation::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgAddLocation message handlers


void CDlgAddLocation::OnBnClickedOk()
{
	CString name, city,country;
	m_editName.GetWindowTextA(name);
	m_editCity.GetWindowTextA(city);
	m_editCountry.GetWindowTextA(country);
	
	if(name==""|| city=="" || country=="")
	{
		MessageBox("Please, input all data!");
		return;
	}
	for(int i=0;i <m_OSMCtrlEx->GetMarkerCount();i++)
	{
		COSMCtrlMarkerEx *marker = dynamic_cast<COSMCtrlMarkerEx*>(&(m_OSMCtrlEx->GetMarker(i)));
		if(marker!=NULL)
		{
			if(marker->location.Name.Compare(name)==0)
			{
				MessageBox("Name '" + name + "' of location already exist. Please, input other name!");
				return;
			}
		}
	}
	CMDIFrameWnd *pFrame =   (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild =   (CMDIChildWnd *) pFrame->GetActiveFrame();
	COSMDoc* doc = dynamic_cast<COSMDoc*>(pChild->GetActiveDocument());
	if(doc!=NULL)
	{
			CDBLocation loc;
			loc.Lat = m_rightClickPos.m_fLatitude;
			loc.Lon = m_rightClickPos.m_fLongitude;
			loc.Name = name;
			loc.City = city;
			loc.Country = country;
			if(doc->GetDBService()->AddLocation(loc)==true)
			{
				dynamic_cast<CMainFrame*>(theApp.m_pMainWnd)->AddInfoString("Add location '"+name + "'");
				((CMainFrame*)theApp.m_pMainWnd)->GetGuiSubject().AddMarker(loc);
			}
			else
			{
				MessageBox("Error of insert of location to database.");
				return;
			}
				//subject->DeleteMarker(ID);
	}
	else
	{
				MessageBox("Error of insert of location to database.");
				return;
	}

	CDialogEx::OnOK();
}


BOOL CDlgAddLocation::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString lonS;
	lonS.Format("%f",m_rightClickPos.m_fLongitude);
	m_editLon.SetWindowTextA(lonS);

	CString latS;
	latS.Format("%f",m_rightClickPos.m_fLatitude);
	m_editLat.SetWindowTextA(latS);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
