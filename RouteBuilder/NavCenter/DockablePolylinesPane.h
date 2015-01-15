#pragma once
#include "IGuiObserver.h"
#include "..\MFCComponentExt\CListCtrl_Column_Picker.h"

#include<list>

using namespace std;
// CDockablePolylinesPane

class CDockablePolylinesPane : public CDockablePane, public IGuiObserver
{
	CString lastDocFile;
	bool isManualAdd;
	TAddPolylineProps manualProp;
private:
	DECLARE_DYNAMIC(CDockablePolylinesPane)
	
	CListCtrl_Column_Picker* m_listCtrlPolylines;
	CListCtrl_Column_Picker* m_listCtrlPoints;
	CButton m_btnAddPolyline, m_btnDeletePolyline;
    CButton m_btnSavePolyline, m_btnCancelPolyline;
	CComboBox m_comboBoxLocA,m_comboBoxLocB;
	
	bool m_enableAddBtn,m_enableDeleteBtn;
	bool SelectCombo(CComboBox &m_combo,CString locName);

	void LoadRoadFromCloudMade(TAddPolylineProps props);
protected:
	virtual void OnAddMarker(CDBLocation& loc);
	virtual void OnDeleteMarker(int &locID, CString &Name);
	virtual void OnRefreshData(COSMCtrlEx& ctrl, bool forcibly);
	virtual void OnAddPolyline(CDBPolyLine& polyLine);
	virtual void OnDeletePolyline(int& lineID);

	virtual void OnFreeAllData();
	virtual void OnOpenManualAddPolyline(TAddPolylineProps props);
	virtual void OnCloseManualAddPolyline();
	virtual void OnSelectRoute(int & routeID);

public:
	CString GetComboAText();
	CString GetComboBText();
	bool GetSelectedPolylineID(int &ID);
	bool IsExistPolylineNameIntoListCtrl(CString &polylineName);
	CDockablePolylinesPane(CGuiSubject &guiSubject);

	void AddPoint(double lat, double lon);
	void AddPolylineToList(CDBPolyLine& line);
	
	list<TLocation> GetPointsFromList();
	virtual ~CDockablePolylinesPane();
protected:

	void FillPolylinesList();
	CDocument* GetActiveDocument();
	CView* GetActiveView();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelectComboA();
	afx_msg void OnSelectComboB();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnAddBtnClick();
	afx_msg void OnUpdateAddBtnClick(CCmdUI* pCmdUI);
	afx_msg void OnDeleteBtnClick();
	afx_msg void OnUpdateDeleteBtnClick(CCmdUI* pCmdUI);
	afx_msg void OnSaveBtnClick();
	afx_msg void OnUpdateSaveBtnClick(CCmdUI* pCmdUI);
	afx_msg void OnCancelBtnClick();
	afx_msg void OnUpdateCancelBtnClick(CCmdUI* pCmdUI);
	afx_msg void OnListPolylinesClick(NMHDR * a,LRESULT * b);
	afx_msg void OnListPointsClick(NMHDR * a,LRESULT * b);
	afx_msg void OnDestroy();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};


