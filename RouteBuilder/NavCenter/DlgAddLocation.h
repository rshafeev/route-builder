#pragma once
#include "OSMCtrlEx.h"
#include "afxwin.h"

// CDlgAddLocation dialog

class CDlgAddLocation : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAddLocation)
	COSMCtrlPosition m_rightClickPos;
	COSMCtrlEx* m_OSMCtrlEx;

public:
	CDlgAddLocation(COSMCtrlEx* OSMCtrlEx,COSMCtrlPosition m_rightClickPos,CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAddLocation();

// Dialog Data
	enum { IDD = IDD_DIALOG_ADDLOCATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	
	CEdit m_editLon;
	CEdit m_editLat;
	CEdit m_editName;
	CEdit m_editCity;
	CEdit m_editCountry;
};
