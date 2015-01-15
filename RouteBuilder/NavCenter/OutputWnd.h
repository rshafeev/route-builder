
#pragma once

/////////////////////////////////////////////////////////////////////////////
// COutputList window

class COutputList : public CListBox
{
// Construction
public:
	COutputList();

// Implementation
public:
	virtual ~COutputList();

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditClear();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnUpdateEditClear(CCmdUI *pCmdUI);
};
class COutputWnd : public CDockablePane
{
// Construction
public:
	COutputWnd();

	void UpdateFonts();

// Attributes
protected:
	CMFCTabCtrl	m_wndTabs;

	COutputList m_wndOutputInfo;
#ifdef _DEBUG
	COutputList m_wndOutputDebug;
#endif
protected:
	void FillInfoWindow();
#ifdef _DEBUG
	void FillDebugWindow();
#endif
	void AdjustHorzScroll(CListBox& wndListBox);

// Implementation
public:
	virtual ~COutputWnd();
#ifdef _DEBUG
	void AddDebugString(CString str);
#endif
	void AddInfoString(CString str);
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};

