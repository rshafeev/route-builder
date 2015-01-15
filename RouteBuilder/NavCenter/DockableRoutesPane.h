#pragma once

#include "IGuiObserver.h"
#include "..\MFCComponentExt\CListCtrl_Column_Picker.h"
// CDockableRoutesPane

class CDockableRoutesPane : public CDockablePane, public IGuiObserver
{
	DECLARE_DYNAMIC(CDockableRoutesPane)

	CString lastDocFile;
	bool isManualAdd;
	CListCtrl_Column_Picker* m_listCtrlRoutes;
	CListCtrl_Column_Picker* m_listCtrlPoints;
	CButton m_btnNewRoute, m_btnDeleteRoute;
    CButton m_btnChangeRoute;
	CComboBox m_comboBoxLocA,m_comboBoxLocB;
	CComboBox m_comboPolylines;

	void FillRoutesList();
	CDocument* GetActiveDocument();
	CView* GetActiveView();
	void AddRouteToList(CDBRoute& route);
	bool GetLocationIDFromCombo(CComboBox& combo, int& locID);

	bool GetSelectedRouteID(int &routeID);
	bool GetSelectedRelation(int &loc1ID, int& loc2ID);
public:
	CDockableRoutesPane(CGuiSubject &guiSubject);
	virtual ~CDockableRoutesPane();

protected:
	virtual void OnAddMarker(CDBLocation& loc);
	virtual void OnDeleteMarker(int &locID, CString &Name);
	virtual void OnFreeAllData();
	virtual void OnRefreshData(COSMCtrlEx& ctrl, bool forcibly);
	virtual void OnOpenManualAddPolyline(TAddPolylineProps props);
	virtual void OnCloseManualAddPolyline();
	virtual void OnAddRoute(CDBRoute& route);
	virtual void OnDeleteRoute(int& routeID);
	virtual void OnSelectPolyline(int& lineID);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnSelectComboA();
	afx_msg void OnSelectComboB();
	afx_msg void OnSelectComboPolylines();

	afx_msg void OnNewRouteBtnClick();
	afx_msg void OnDeleteRouteBtnClick();
	afx_msg void OnChangeRouteBtnClick();
	afx_msg void OnUpdateNewRouteBtnClick(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDeleteRouteBtnClick(CCmdUI* pCmdUI);
	afx_msg void OnUpdateChangeRouteBtnClick(CCmdUI* pCmdUI);
	
	afx_msg void OnListRoutesClick(NMHDR * a,LRESULT * b);
	afx_msg void OnListRelationsClick(NMHDR * a,LRESULT * b);
};


