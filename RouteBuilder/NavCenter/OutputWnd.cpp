
#include "stdafx.h"

#include "OutputWnd.h"
#include "Resource.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

COutputWnd::COutputWnd()
{
}

COutputWnd::~COutputWnd()
{
}

BEGIN_MESSAGE_MAP(COutputWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int COutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create tabs window:
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("Failed to create output tab window\n");
		return -1;      // fail to create
	}

	// Create output panes:
	const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

	if (!m_wndOutputInfo.Create(dwStyle, rectDummy, &m_wndTabs, 2))
	{
		TRACE0("Failed to create output windows\n");
		return -1;      // fail to create
	}

	

	CString strTabName;
	BOOL bNameValid;

	// Attach list windows to tab:
	m_wndTabs.AddTab(&m_wndOutputInfo, "Info", (UINT)0);
	
	// Fill output tabs with some dummy text (nothing magic here)
	FillInfoWindow();
#ifdef _DEBUG	
	if (!m_wndOutputDebug.Create(dwStyle, rectDummy, &m_wndTabs, 3))
	{
		TRACE0("Failed to create output windows\n");
		return -1;      // fail to create
	}
	m_wndTabs.AddTab(&m_wndOutputDebug, "Debug", (UINT)0);
	FillDebugWindow();
#endif
	UpdateFonts();
	return 0;
}

void COutputWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// Tab control should cover the whole client area:
	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void COutputWnd::AdjustHorzScroll(CListBox& wndListBox)
{
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&afxGlobalData.fontRegular);

	int cxExtentMax = 0;

	for (int i = 0; i < wndListBox.GetCount(); i ++)
	{
		CString strItem;
		wndListBox.GetText(i, strItem);

		cxExtentMax = max(cxExtentMax, dc.GetTextExtent(strItem).cx);
	}

	wndListBox.SetHorizontalExtent(cxExtentMax);
	dc.SelectObject(pOldFont);
}

void COutputWnd::FillInfoWindow()
{
	m_wndOutputInfo.AddString(_T("Info output is being displayed here."));
}
#ifdef _DEBUG
void COutputWnd::FillDebugWindow()
{
	m_wndOutputDebug.AddString(_T("Debug output is being displayed here."));
	m_wndOutputDebug.AddString(_T("The output is being displayed in rows of a list view"));
	m_wndOutputDebug.AddString(_T("but you can change the way it is displayed as you wish..."));
}
#endif

void COutputWnd::UpdateFonts()
{
	m_wndOutputInfo.SetFont(&afxGlobalData.fontRegular);
#ifdef _DEBUG	
	m_wndOutputDebug.SetFont(&afxGlobalData.fontRegular);
#endif
}
#ifdef _DEBUG
void COutputWnd::AddDebugString(CString str)
{
	m_wndOutputDebug.AddString(str);
}
#endif
void COutputWnd::AddInfoString(CString str)
{
	m_wndOutputInfo.AddString(str);

}


/////////////////////////////////////////////////////////////////////////////
// COutputList1

COutputList::COutputList()
{
}

COutputList::~COutputList()
{
}

BEGIN_MESSAGE_MAP(COutputList, CListBox)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_WM_WINDOWPOSCHANGING()
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, &COutputList::OnUpdateEditClear)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// COutputList message handlers

void COutputList::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_OUTPUT_POPUP);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}

	SetFocus();
}


void COutputList::OnEditClear()
{
	this->ResetContent();
}

void COutputList::OnUpdateEditClear(CCmdUI *pCmdUI)
{
	if(this->GetCount()>0)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);

}
