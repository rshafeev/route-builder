#pragma once
#include "afxcmn.h"
#include "PictureEx.h"
#include "afxwin.h"

// CDlgProgress dialog

class CDlgProgress : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgProgress)

public:
	CDlgProgress(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgProgress();

// Dialog Data
	enum { IDD = IDD_DIALOG_PROGRESS };
	CPictureEx pict;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	CPictureEx m_pict;
};
