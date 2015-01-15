#pragma once
#include "afxwin.h"


// CDlgOptions dialog

class CDlgOptions : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgOptions)

public:
	CDlgOptions(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgOptions();

// Dialog Data
	enum { IDD = IDD_DIALOG_OPTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editKey;
	CEdit m_editSmooth;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
