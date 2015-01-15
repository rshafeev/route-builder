// Этот исходный код MFC Samples демонстрирует функционирование пользовательского интерфейса Fluent на основе MFC в Microsoft Office
// ("Fluent UI") и предоставляется исключительно как справочный материал в качестве дополнения к
// справочнику по пакету Microsoft Foundation Classes и связанной электронной документации,
// включенной в программное обеспечение библиотеки MFC C++. 
// Условия лицензионного соглашения на копирование, использование или распространение Fluent UI доступны отдельно. 
// Для получения дополнительных сведений о нашей лицензионной программе Fluent UI посетите веб-узел
// http://msdn.microsoft.com/officeui.
//
// (C) Корпорация Майкрософт (Microsoft Corp.)
// Все права защищены.

// NavCenterView.cpp : реализация класса CNavCenterView
//

#include "stdafx.h"

// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
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

// создание/уничтожение CNavCenterView

CNavCenterView::CNavCenterView()
{
	// TODO: добавьте код создания

}

CNavCenterView::~CNavCenterView()
{
}

BOOL CNavCenterView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs
	
	return CView::PreCreateWindow(cs);
}

// рисование CNavCenterView

void CNavCenterView::OnDraw(CDC* /*pDC*/)
{
	CNavCenterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: добавьте здесь код отрисовки для собственных данных
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


// диагностика CNavCenterView

#ifdef _DEBUG
void CNavCenterView::AssertValid() const
{
	CView::AssertValid();
}

void CNavCenterView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CNavCenterDoc* CNavCenterView::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNavCenterDoc)));
	return (CNavCenterDoc*)m_pDocument;
}
#endif //_DEBUG


// обработчики сообщений CNavCenterView


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
