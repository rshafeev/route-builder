#pragma once
#include "navcenterview.h"
#include "OSMDoc.h"
#include "OSMCtrlEx.h"


class COSMView : 	public CNavCenterView, IOSMCtrlHandler, public IGuiObserver
{

public: // создать только из сериализации
	COSMView();
	DECLARE_DYNCREATE(COSMView)

    COSMCtrlEx      m_ctrlOSM;

// Атрибуты

public:
	COSMDoc* GetDocument() const;
	virtual void Init(CGuiSubject &guiSubject);
// Операции
public:
	bool isActivatedAlready;
// Переопределение
public:
protected:

// Реализация
public:
	virtual ~COSMView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	#ifdef COSMCTRL_NOD2D
		HRESULT LoadResourceImage(LPCTSTR pName, LPCTSTR pType, HMODULE hInst, Gdiplus::Image*& pImage);
	#endif	
// Созданные функции схемы сообщений
protected:
	virtual void OnAddMarker(CDBLocation& loc);
	virtual void OnDeleteMarker(int &locID, CString &Name);
	virtual void OnSelectMarker(int &locID);
	virtual void OnOpenManualAddPolyline(TAddPolylineProps props);
	virtual void OnCloseManualAddPolyline();

	virtual void OnAddPolyline(CDBPolyLine& polyLine);
	virtual void OnDeletePolyline(int& lineID);
	virtual void OnSelectPolyline(int& lineID);
	virtual void OnUnselectedPolylines(bool unselectedEx);

	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);

	virtual void OnAddRoute(CDBRoute& route);
	virtual void OnDeleteRoute(int& routeID);
	virtual void OnSelectRoute(int & routeID);
	virtual void OnChangePolylineToRoute(int &routeID, int &oldPolylineID,int &newPolylineID);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

};


