#pragma once

#include "..\MFCComponentExt\CListCtrl_Column_Picker.h"
#include "IGuiObserver.h"

// CDockableLocationsPane

class CDockableLocationsPane : public CDockablePane, public IGuiObserver
{
	DECLARE_DYNAMIC(CDockableLocationsPane)

	//CListCtrl_Column_Picker* m_listLocations;
	CListCtrl* m_listLocations;
	CButton* m_deleteBtn;
	CString lastDocFile;
public:
	CDockableLocationsPane(CGuiSubject &guiSubject);
   ~CDockableLocationsPane();
	bool		   GetSelectedLocation(int &ID, CString &name);

	CDocument* GetActiveDocument();
protected:
	
	
	virtual void OnAddMarker(CDBLocation& loc);
	virtual void OnDeleteMarker(int &locID, CString &Name);
	virtual void OnSelectMarker(int& locID);
	virtual void OnRefreshData(COSMCtrlEx& ctrl, bool forcibly);
	virtual void OnFreeAllData();
	virtual void OnOpenManualAddPolyline(TAddPolylineProps props);
	virtual void OnCloseManualAddPolyline();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnListClick(NMHDR * a,LRESULT * b);
	afx_msg void OnDeleteBtnClick();
	afx_msg void OnUpdateDeleteBtnClick(CCmdUI* pCmdUI);
};


