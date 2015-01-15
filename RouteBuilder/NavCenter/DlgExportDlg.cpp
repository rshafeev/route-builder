// DlgExportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NavCenter.h"
#include "DlgExportDlg.h"
#include "afxdialogex.h"

#include "OSMDoc.h"
// CDlgExportDlg dialog

IMPLEMENT_DYNAMIC(CDlgExportDlg, CDialogEx)

CDlgExportDlg::CDlgExportDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgExportDlg::IDD, pParent)
{
	m_fName = "";
}

CDlgExportDlg::~CDlgExportDlg()
{
}

void CDlgExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editFileName);
	DDX_Control(pDX, IDC_EDIT2, m_Air);
	DDX_Control(pDX, IDC_EDIT3, m_Train);
	DDX_Control(pDX, IDC_EDIT4, m_ship);
}


BEGIN_MESSAGE_MAP(CDlgExportDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgExportDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTONCHOOSE, &CDlgExportDlg::OnBnClickedButtonchoose)
END_MESSAGE_MAP()


// CDlgExportDlg message handlers


void CDlgExportDlg::OnBnClickedOk()
{
	if(m_fName=="")
	{
		MessageBox("Please, choose excel file.");
		return;
	}
	CString air,train,ship;
	m_Air.GetWindowTextA(air);
	m_Train.GetWindowTextA(train);
	m_ship.GetWindowTextA(ship);

	CMDIFrameWnd *pFrame =   (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild =   (CMDIChildWnd *) pFrame->GetActiveFrame();
	COSMDoc* doc = dynamic_cast<COSMDoc*>(pChild->GetActiveDocument());
	if(doc!=NULL)
	{
		IDBService *db = doc->GetDBService();
		TExcelExportProps props;
		props.excelFile = m_fName;
		props.freqAir = atoi(air);
		props.freqTrain = atoi(train);
		props.freqShip = atoi(ship);
		if(!db->ExportToExcel(props))
			MessageBox("Error! Program could not export to excel!");
	}
	else
	{
		MessageBox("Error! Program could not export to excel!");
	}
	CDialogEx::OnOK();
}

void CDlgExportDlg::OnBnClickedButtonchoose()
{
	CFileDialog * dlg = new CFileDialog(true,_T("*.xlsm"),0,0,	_T("2007 Excel file (*.xlsm)|*.xlsm"),0,0,1);
	if(dlg->DoModal()==IDOK)
	{	
		this->m_fName = dlg->GetPathName();
		this->m_editFileName.SetWindowTextA(m_fName);
	}

}


BOOL CDlgExportDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_Air.SetWindowTextA("2");
	m_Train.SetWindowTextA("3");
	m_ship.SetWindowTextA("24");
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
