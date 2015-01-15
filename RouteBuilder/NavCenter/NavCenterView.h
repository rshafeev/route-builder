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

// NavCenterView.h : интерфейс класса CNavCenterView
//

#pragma once

#include "NavCenterDoc.h"
#include "IGuiObserver.h"

class CNavCenterView : public CView
{
protected: // создать только из сериализации
	CNavCenterView();

	
// Атрибуты
public:
	CNavCenterDoc* GetDocument() const;

// Операции
public:

// Переопределение
public:
	virtual void OnDraw(CDC* pDC);  // переопределено для отрисовки этого представления
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void Init(CGuiSubject &guiSubject) = NULL;
protected:

// Реализация
public:
	virtual ~CNavCenterView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
};

#ifndef _DEBUG  // отладочная версия в NavCenterView.cpp
inline CNavCenterDoc* CNavCenterView::GetDocument() const
   { return reinterpret_cast<CNavCenterDoc*>(m_pDocument); }
#endif

