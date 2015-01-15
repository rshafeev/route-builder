// DlgAddPolyline.cpp : implementation file
//

#include "stdafx.h"
#include "NavCenter.h"
#include "DlgAddPolyline.h"
#include "afxdialogex.h"
#include "..\NavCenter.DB\DBModel.h"

// CDlgAddPolyline dialog

IMPLEMENT_DYNAMIC(CDlgAddPolyline, CDialogEx)

CDlgAddPolyline::CDlgAddPolyline(CDockablePolylinesPane *dockablePolylinesPane,CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgAddPolyline::IDD, pParent)
{
	this->p_dockablePolylinesPane = dockablePolylinesPane;
}

CDlgAddPolyline::~CDlgAddPolyline()
{

}

void CDlgAddPolyline::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITNAME, m_editName);
	DDX_Control(pDX, IDC_COMBOPOLYLINETYPE, m_comboType);
	DDX_Control(pDX, IDC_RADIOMANUAL, m_radioManual);
	DDX_Control(pDX, IDC_RADIOCLOUDMADE, m_radioCloudmade);
	DDX_Control(pDX, IDC_EDITTIME, m_editTime);
}


BEGIN_MESSAGE_MAP(CDlgAddPolyline, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBOPOLYLINETYPE, &CDlgAddPolyline::OnCbnSelchangeCombopolylinetype)
	ON_BN_CLICKED(IDOK, &CDlgAddPolyline::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgAddPolyline message handlers


void CDlgAddPolyline::OnCbnSelchangeCombopolylinetype()
{
	int ind = m_comboType.GetCurSel();
	if(ind < 0)
		return;
	CString comboText;
	m_comboType.GetLBText(ind,comboText);

	if(CDBPolyLine::GetType(comboText)==CDBPolyLine::c_road)
	{
		m_radioCloudmade.EnableWindow(true);
		m_radioManual.EnableWindow(true);
		m_editTime.EnableWindow(false);
	}
	else
	if(CDBPolyLine::GetType(comboText)==CDBPolyLine::c_ship)
	{
		m_radioCloudmade.EnableWindow(false);
		m_radioManual.EnableWindow(false);
		m_editTime.EnableWindow(true);

	}
	else
	if(CDBPolyLine::GetType(comboText)==CDBPolyLine::c_train)
	{
		m_radioCloudmade.EnableWindow(false);
		m_radioManual.EnableWindow(false);
		m_editTime.EnableWindow(false);
	}
	else
	if(CDBPolyLine::GetType(comboText)==CDBPolyLine::c_air)
	{
		m_radioCloudmade.EnableWindow(false);
		m_radioManual.EnableWindow(false);
		m_editTime.EnableWindow(false);
	}
}


BOOL CDlgAddPolyline::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_comboType.AddString(CDBPolyLine::GetType(CDBPolyLine::c_road));
	m_comboType.AddString(CDBPolyLine::GetType(CDBPolyLine::c_ship));
	m_comboType.AddString(CDBPolyLine::GetType(CDBPolyLine::c_train));
	m_comboType.AddString(CDBPolyLine::GetType(CDBPolyLine::c_air));

	m_comboType.SetCurSel(1);
	m_editTime.EnableWindow(false);
	m_editTime.SetWindowTextA("1");
	m_editName.SetWindowTextA("Line_"+p_dockablePolylinesPane->GetComboAText()+"_"+
		                       p_dockablePolylinesPane->GetComboBText());

	m_radioManual.SetCheck(false);
	m_radioCloudmade.SetCheck(true);
	return TRUE; 
}


void CDlgAddPolyline::OnBnClickedOk()
{
	CString lineName;
	m_editName.GetWindowTextA(lineName);
	if(lineName.GetLength() == 0)
	{
		MessageBox("Please, put name of polyline.");
		return;
	}
	if(p_dockablePolylinesPane->IsExistPolylineNameIntoListCtrl(lineName))
	{
		MessageBox("Name: '" + lineName + "' already exist.Pleasy, input other name!");
		return;
	}
	m_addPolylineProps.name = lineName;
	
	//double time

	int ind = m_comboType.GetCurSel();
	if(ind < 0)
		throw exception();
	
	CString comboText;
	m_comboType.GetLBText(ind,comboText);
	m_addPolylineProps.type = CDBPolyLine::GetType(comboText);
	m_addPolylineProps.IsCloudmade = m_radioCloudmade.GetCheck();
	if(m_addPolylineProps.type==CDBPolyLine::c_ship)
	{
		CString timeS;
		m_editTime.GetWindowTextA(timeS);
		m_addPolylineProps.time = (long)(86400*atof(timeS));
		if(m_addPolylineProps.time<=0)
		{
			MessageBox("Please, input time");
			return;
		}
	}
	else
	  m_addPolylineProps.time = 0;
	
	CDialogEx::OnOK();
}
