#pragma once
#include "afxwin.h"

#include "OSMCtrlEx.h"
// CDlgChangeRoute dialog

class CDlgChangeRoute : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgChangeRoute)
	CDBRoute& route;
	COSMCtrlEx& m_osmCtrl;
	bool m_isNew;
	list<CDBLocation> locations;
public:
	CDlgChangeRoute(COSMCtrlEx& osmCtrl,CDBRoute& route, CWnd* pParent = NULL,bool isNew = true);   // standard constructor
	virtual ~CDlgChangeRoute();

// Dialog Data
	enum { IDD = IDD_DIALOG_ROUTECHANGE };
protected:
	bool OnOkAdd();
	bool OnOkChange();
	COSMCtrlPolylineEx* GetExistPolyline(int idA, int idB);
	void AddLocationToList(CDBLocation &loc,int indAfter);
	void DeleteLocationFromList(int ID);
	bool GetSelectedLocation(CDBLocation& location);
	list<CDBRelation> GetRelations(CString &errorString);
	CString GetFreeName();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listLocs;
	CComboBox m_comboLocs;
	CEdit m_editFirst;
	CEdit m_editFinish;
	CEdit m_editName;
	CStatic m_statictextName;

	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedOk();

	 afx_msg void OnKickIdle();

	afx_msg void OnBnClickedButtonaddfirst();
	afx_msg void OnBnClickedButtonaddlast();

	afx_msg void OnUpdateAddLastBtnClick(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddAfterBtnClick(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDeleteBtnClick(CCmdUI* pCmdUI);

};
