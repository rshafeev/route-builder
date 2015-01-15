// DlgOptions.cpp : implementation file
//

#include "stdafx.h"
#include "NavCenter.h"
#include "DlgOptions.h"
#include "afxdialogex.h"


// CDlgOptions dialog

IMPLEMENT_DYNAMIC(CDlgOptions, CDialogEx)

CDlgOptions::CDlgOptions(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgOptions::IDD, pParent)
{

}

CDlgOptions::~CDlgOptions()
{
}

void CDlgOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT3, m_editKey);
	DDX_Control(pDX, IDC_EDIT2, m_editSmooth);
}


BEGIN_MESSAGE_MAP(CDlgOptions, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgOptions::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgOptions message handlers


void CDlgOptions::OnBnClickedOk()
{
	CString key,smooth;
	m_editKey.GetWindowTextA(key);
	m_editSmooth.GetWindowTextA(smooth);
	if(key.Compare("Please, insert key!")!=0)
		theApp.WriteProfileStringA("OptionsParam","CloudKey",key);
	theApp.WriteProfileStringA("OptionsParam","Smoothing",smooth);
	CDialogEx::OnOK();
}


BOOL CDlgOptions::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_editKey.SetWindowTextA(theApp.GetProfileStringA("OptionsParam","CloudKey","Please, insert key!"));
	m_editSmooth.SetWindowTextA(theApp.GetProfileStringA("OptionsParam","Smoothing","-1"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
