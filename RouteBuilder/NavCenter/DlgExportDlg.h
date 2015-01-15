#pragma once

#include "afxwin.h"

// CDlgExportDlg dialog

class CDlgExportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgExportDlg)
	CString m_fName;
public:
	CDlgExportDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgExportDlg();
// Dialog Data
	enum { IDD = IDD_DIALOG_EXPORT };
   
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonchoose();
	CEdit m_editFileName;
	CEdit m_Air;
	CEdit m_Train;
	CEdit m_ship;
	virtual BOOL OnInitDialog();
};
