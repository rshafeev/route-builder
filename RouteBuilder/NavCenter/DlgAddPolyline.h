#pragma once
#include "afxwin.h"
#include "DockablePolylinesPane.h"

// CDlgAddPolyline dialog

class CDlgAddPolyline : public CDialogEx
{
public:
	TAddPolylineProps m_addPolylineProps;

private:
	DECLARE_DYNAMIC(CDlgAddPolyline)
	CDockablePolylinesPane *p_dockablePolylinesPane;
public:
	CDlgAddPolyline(CDockablePolylinesPane *dockablePolylinesPane,CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAddPolyline();
	bool IsCloudmade;
// Dialog Data
	enum { IDD = IDD_DIALOG_ADDPOLYLINE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editName;
	CComboBox m_comboType;
	CButton m_radioManual,m_radioCloudmade;
	afx_msg void OnCbnSelchangeCombopolylinetype();
	virtual BOOL OnInitDialog();
	CEdit m_editTime;
	afx_msg void OnBnClickedOk();
};
