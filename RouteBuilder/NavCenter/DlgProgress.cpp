// DlgProgress.cpp : implementation file
//

#include "stdafx.h"
#include "NavCenter.h"
#include "DlgProgress.h"
#include "afxdialogex.h"
#include "PictureEx.h"

// CDlgProgress dialog

IMPLEMENT_DYNAMIC(CDlgProgress, CDialogEx)

CDlgProgress::CDlgProgress(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgProgress::IDD, pParent)
{

}

CDlgProgress::~CDlgProgress()
{
}

void CDlgProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MYPICTURE, m_pict);
}


BEGIN_MESSAGE_MAP(CDlgProgress, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDlgProgress message handlers


BOOL CDlgProgress::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	if(m_pict.Load("D:\\Instityt\\IV kyrs\\II семестр\\Hamburg\\NavCenter2.0\\MFCComponentExt\\res\\Wait.gif"))
	{
		m_pict.Draw();
	}
	
	return TRUE;
}

void CDlgProgress::OnPaint()
{
	CPaintDC dc(this); 
	

}
