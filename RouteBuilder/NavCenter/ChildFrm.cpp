// ���� �������� ��� MFC Samples ������������� ���������������� ����������������� ���������� Fluent �� ������ MFC � Microsoft Office
// ("Fluent UI") � ��������������� ������������� ��� ���������� �������� � �������� ���������� �
// ����������� �� ������ Microsoft Foundation Classes � ��������� ����������� ������������,
// ���������� � ����������� ����������� ���������� MFC C++. 
// ������� ������������� ���������� �� �����������, ������������� ��� ��������������� Fluent UI �������� ��������. 
// ��� ��������� �������������� �������� � ����� ������������ ��������� Fluent UI �������� ���-����
// http://msdn.microsoft.com/officeui.
//
// (C) ���������� ���������� (Microsoft Corp.)
// ��� ����� ��������.

// ChildFrm.cpp : ���������� ������ CChildFrame
//

#include "stdafx.h"
#include "NavCenter.h"
#include "MainFrm.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
	ON_COMMAND(ID_FILE_NEW, &CChildFrame::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CChildFrame::OnFileOpen)
END_MESSAGE_MAP()

// ��������/����������� CChildFrame

CChildFrame::CChildFrame()
{
	// TODO: �������� ��� ������������� �����
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �������� ����� Window ��� ����� ����������� ��������� CREATESTRUCT cs
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// ����������� CChildFrame

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// ����������� ��������� CChildFrame


void CChildFrame::OnFileNew()
{
	dynamic_cast<CMainFrame*>(theApp.m_pMainWnd)->OnFileNew();
}


void CChildFrame::OnFileOpen()
{
	dynamic_cast<CMainFrame*>(theApp.m_pMainWnd)->OnFileOpen();
}
