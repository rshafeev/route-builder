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

// NavCenterView.cpp : ���������� ������ CNavCenterView
//

#include "stdafx.h"

// SHARED_HANDLERS ����� ���������� � ������������ �������� ��������� ���������� ������� ATL, �������
// � ������; ��������� ��������� ������������ ��� ��������� � ������ �������.
#ifndef SHARED_HANDLERS
#include "NavCenter.h"
#endif

#include "NavCenterView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNavCenterView


BEGIN_MESSAGE_MAP(CNavCenterView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// ��������/����������� CNavCenterView

CNavCenterView::CNavCenterView()
{
	// TODO: �������� ��� ��������

}

CNavCenterView::~CNavCenterView()
{
}

BOOL CNavCenterView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �������� ����� Window ��� ����� ����������� ���������
	//  CREATESTRUCT cs
	
	return CView::PreCreateWindow(cs);
}

// ��������� CNavCenterView

void CNavCenterView::OnDraw(CDC* /*pDC*/)
{
	CNavCenterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �������� ����� ��� ��������� ��� ����������� ������
}

void CNavCenterView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	//OnContextMenu(this, point);
}

void CNavCenterView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
/*
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif*/
}


// ����������� CNavCenterView

#ifdef _DEBUG
void CNavCenterView::AssertValid() const
{
	CView::AssertValid();
}

void CNavCenterView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CNavCenterDoc* CNavCenterView::GetDocument() const // �������� ������������ ������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNavCenterDoc)));
	return (CNavCenterDoc*)m_pDocument;
}
#endif //_DEBUG


// ����������� ��������� CNavCenterView


int CNavCenterView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
		
	return 0;
}


void CNavCenterView::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CView::OnClose();
}
