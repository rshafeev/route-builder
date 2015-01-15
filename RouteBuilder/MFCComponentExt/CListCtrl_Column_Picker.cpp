#include "CListCtrl_Column_Picker.h"
#include "stdafx.h"
#define MFCCOMPONENTEXT_API
#include "CListCtrl_Column_Picker.h"



CListCtrl_Column_Picker::ColumnState::ColumnState()
			:m_Visible(false)
			,m_OrgWidth(0)
			,m_OrgPosition(-1)
		{}

BEGIN_MESSAGE_MAP(CListCtrl_Column_Picker, CListCtrl)
	ON_MESSAGE(LVM_DELETECOLUMN, OnDeleteColumn)
	ON_MESSAGE(LVM_INSERTCOLUMN, OnInsertColumn)
	ON_MESSAGE(LVM_SETCOLUMNWIDTH, OnSetColumnWidth)
	ON_NOTIFY_EX(HDN_BEGINTRACKA, 0, OnHeaderBeginResize)
	ON_NOTIFY_EX(HDN_BEGINTRACKW, 0, OnHeaderBeginResize)
	ON_NOTIFY_EX(HDN_ENDDRAG, 0, OnHeaderEndDrag)
	ON_NOTIFY_EX(HDN_DIVIDERDBLCLICKA, 0, OnHeaderDividerDblClick)
	ON_NOTIFY_EX(HDN_DIVIDERDBLCLICKW, 0, OnHeaderDividerDblClick)
	ON_WM_CONTEXTMENU()	// OnContextMenu
	ON_WM_KEYDOWN()		// OnKeyDown
END_MESSAGE_MAP()


CListCtrl_Column_Picker::CListCtrl_Column_Picker()
{

}
CListCtrl_Column_Picker::~CListCtrl_Column_Picker()
{

}
int CListCtrl_Column_Picker::GetFirstVisibleColumn()
{
	int nColCount = GetHeaderCtrl()->GetItemCount();
	for(int i = 0; i < nColCount; ++i)
	{
		int nCol = GetHeaderCtrl()->OrderToIndex(i);
		if (IsColumnVisible(nCol))
		{
			return nCol;
		}
	}
	return -1;
}

BOOL CListCtrl_Column_Picker::ShowColumn(int nCol, bool bShow)
{
	SetRedraw(FALSE);

	ColumnState& columnState = GetColumnState(nCol);

	int nColCount = GetHeaderCtrl()->GetItemCount();
	int* pOrderArray = new int[nColCount];
	VERIFY( GetColumnOrderArray(pOrderArray, nColCount) );
	if (bShow)
	{
		// Restore the position of the column
		int nCurIndex = -1;
		for(int i = 0; i < nColCount ; ++i)
		{
			if (pOrderArray[i]==nCol)
				nCurIndex = i;
			else
			if (nCurIndex!=-1)
			{
				// We want to move it to the original position,
				// and after the last hidden column
				if ( (i <= columnState.m_OrgPosition)
				  || !IsColumnVisible(pOrderArray[i])
				   )
				{
					pOrderArray[nCurIndex] = pOrderArray[i];
					pOrderArray[i] = nCol;
					nCurIndex = i;
				}
			}
		}
	}
	else
	{
		// Move the column to the front of the display order list
		int nCurIndex(-1);
		for(int i = nColCount-1; i >=0 ; --i)
		{
			if (pOrderArray[i]==nCol)
			{
				// Backup the current position of the column
				columnState.m_OrgPosition = i;
				nCurIndex = i;
			}
			else
			if (nCurIndex!=-1)
			{
				pOrderArray[nCurIndex] = pOrderArray[i];
				pOrderArray[i] = nCol;
				nCurIndex = i;
			}
		}
	}

	VERIFY( SetColumnOrderArray(nColCount, pOrderArray) );
	delete [] pOrderArray;

	if (bShow)
	{
		// Restore the column width
		columnState.m_Visible = true;
		VERIFY( SetColumnWidth(nCol, columnState.m_OrgWidth) );
	}
	else
	{
		// Backup the column width
		int orgWidth = GetColumnWidth(nCol);
		VERIFY( SetColumnWidth(nCol, 0) );
		columnState.m_Visible = false;
		columnState.m_OrgWidth = orgWidth;
	}
	SetRedraw(TRUE);
	Invalidate(FALSE);
	return TRUE;
}

BOOL CListCtrl_Column_Picker::SetColumnWidthAuto(int nCol, bool includeHeader)
{
	if (nCol == -1)
	{
		for(int i = 0; i < GetHeaderCtrl()->GetItemCount() ; ++i)
		{
			SetColumnWidthAuto(i, includeHeader);
		}
		return TRUE;
	}
	else
	{
		if (includeHeader)
			return SetColumnWidth(nCol, LVSCW_AUTOSIZE_USEHEADER);
		else
			return SetColumnWidth(nCol, LVSCW_AUTOSIZE);
	}
}

CListCtrl_Column_Picker::ColumnState& CListCtrl_Column_Picker::GetColumnState(int nCol)
{
	VERIFY( nCol >=0 && nCol < m_ColumnStates.GetSize() );
	return m_ColumnStates[nCol];
}

bool CListCtrl_Column_Picker::IsColumnVisible(int nCol)
{
	return GetColumnState(nCol).m_Visible;
}

int CListCtrl_Column_Picker::GetColumnStateCount()
{
	return m_ColumnStates.GetSize();
}

void CListCtrl_Column_Picker::InsertColumnState(int nCol, bool bVisible, int nOrgWidth)
{
	VERIFY( nCol >=0 && nCol <= m_ColumnStates.GetSize() );

	ColumnState columnState;
	columnState.m_OrgWidth = nOrgWidth;
	columnState.m_Visible = bVisible;

	if (nCol == m_ColumnStates.GetSize())
	{
		// Append column picker to the end of the array
		m_ColumnStates.Add(columnState);
	}
	else
	{
		// Insert column in the middle of the array
		CSimpleArray<ColumnState> newArray;
		for(int i=0 ; i < m_ColumnStates.GetSize(); ++i)
		{
			if (i == nCol)
				newArray.Add(columnState);
			newArray.Add(m_ColumnStates[i]);
		}
		m_ColumnStates = newArray;
	}
}

void CListCtrl_Column_Picker::DeleteColumnState(int nCol)
{
	VERIFY( nCol >=0 && nCol < m_ColumnStates.GetSize() );
	m_ColumnStates.RemoveAt(nCol);
}

void CListCtrl_Column_Picker::OnContextMenu(CWnd* pWnd, CPoint point)
{
	if (point.x==-1 && point.y==-1)
	{
		// OBS! point is initialized to (-1,-1) if using SHIFT+F10 or VK_APPS
	}
	else
	{
		CPoint pt = point;
		ScreenToClient(&pt);

		CRect headerRect;
		GetHeaderCtrl()->GetClientRect(&headerRect);
		if (headerRect.PtInRect(pt))
		{
			// Show context-menu with the option to show hide columns
			CMenu menu;
			if (menu.CreatePopupMenu())
			{
				for( int i = GetColumnStateCount()-1 ; i >= 0; --i)
				{
					UINT uFlags = MF_BYPOSITION | MF_STRING;

					// Put check-box on context-menu
					if (IsColumnVisible(i))
						uFlags |= MF_CHECKED;
					else
						uFlags |= MF_UNCHECKED;

					// Retrieve column-title
					LVCOLUMN lvc = {0};
					lvc.mask = LVCF_TEXT;
					TCHAR sColText[256];
					lvc.pszText = sColText;
					lvc.cchTextMax = sizeof(sColText)-1;
					VERIFY( GetColumn(i, &lvc) );

					menu.InsertMenu(0, uFlags, i, lvc.pszText);
				}

				menu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this, 0);
			}
		}
	}
}

// Handle context-menu event for showing / hiding columns
BOOL CListCtrl_Column_Picker::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (HIWORD(wParam) == 0)
	{
		int nCol = LOWORD(wParam);
		ShowColumn(nCol, !IsColumnVisible(nCol));
	}
	return TRUE;
}

void CListCtrl_Column_Picker::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch(nChar)
	{
		case VK_ADD:	// CTRL + NumPlus (Auto size all columns)
		{
			if (GetKeyState(VK_CONTROL) < 0)
			{
				// Special handling to avoid showing "hidden" columns
				SetColumnWidthAuto(-1);
				return;
			}
		} break;
	}
	CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CListCtrl_Column_Picker::OnHeaderBeginResize(UINT, NMHDR* pNMHDR, LRESULT* pResult)
{
	// Check that column is allowed to be resized
	NMHEADER* pNMH = (NMHEADER*)pNMHDR;
	int nCol = (int)pNMH->iItem;
	if (!IsColumnVisible(nCol))
	{
		*pResult = TRUE;	// Block resize
		RedrawWindow();
		return TRUE;		// Block event
	}
	RedrawWindow();
	return FALSE;
}

LRESULT CListCtrl_Column_Picker::OnSetColumnWidth(WPARAM wParam, LPARAM lParam)
{
	// Check that column is allowed to be resized
	int nCol = (int)wParam;
	if (!IsColumnVisible(nCol))
	{
		return FALSE;
	}
	// Let the CListCtrl handle the event

	return DefWindowProc(LVM_SETCOLUMNWIDTH, wParam, lParam);
}

BOOL CListCtrl_Column_Picker::OnHeaderEndDrag(UINT, NMHDR* pNMHDR, LRESULT* pResult)
{
	NMHEADER* pNMH = (NMHEADER*)pNMHDR;
	if (pNMH->pitem->mask & HDI_ORDER)
	{
		// Correct iOrder so it is just after the last hidden column
		int nColCount = GetHeaderCtrl()->GetItemCount();
		int* pOrderArray = new int[nColCount];
		VERIFY( GetColumnOrderArray(pOrderArray, nColCount) );

		for(int i = 0; i < nColCount ; ++i)
		{
			if (IsColumnVisible(pOrderArray[i]))
			{
                pNMH->pitem->iOrder = max(pNMH->pitem->iOrder,i);
				break;
			}
		}
		delete [] pOrderArray;
	}
	RedrawWindow();
	return FALSE;
}

BOOL CListCtrl_Column_Picker::OnHeaderDividerDblClick(UINT, NMHDR* pNMHDR, LRESULT* pResult)
{
	NMHEADER* pNMH = (NMHEADER*)pNMHDR;
	SetColumnWidthAuto(pNMH->iItem);
	return TRUE;	// Don't let parent handle the event
}

LRESULT CListCtrl_Column_Picker::OnDeleteColumn(WPARAM wParam, LPARAM lParam)
{
	// Let the CListCtrl handle the event
	LRESULT lRet = DefWindowProc(LVM_DELETECOLUMN, wParam, lParam);
	if (lRet == FALSE)
		return FALSE;

	// Book keeping of columns
	DeleteColumnState((int)wParam);
	return lRet;
}

LRESULT CListCtrl_Column_Picker::OnInsertColumn(WPARAM wParam, LPARAM lParam)
{
	// Let the CListCtrl handle the event
	LRESULT lRet = DefWindowProc(LVM_INSERTCOLUMN, wParam, lParam);
	if (lRet == -1)
		return -1;

	int nCol = (int)lRet;

	// Book keeping of columns
	if (GetColumnStateCount() < GetHeaderCtrl()->GetItemCount())
		InsertColumnState((int)nCol, true);	// Insert as visible

	return lRet;
}


	LRESULT EnableWindowTheme(HWND hwnd, LPCWSTR classList, LPCWSTR subApp, LPCWSTR idlist)
	{
		HMODULE hinstDll;
		HRESULT (__stdcall *pSetWindowTheme)(HWND hwnd, LPCWSTR pszSubAppName, LPCWSTR pszSubIdList);
		HANDLE (__stdcall *pOpenThemeData)(HWND hwnd, LPCWSTR pszClassList);
		HRESULT (__stdcall *pCloseThemeData)(HANDLE hTheme);

		hinstDll = ::LoadLibrary("UxTheme.dll");
		if (hinstDll)
		{
			(FARPROC&)pOpenThemeData = ::GetProcAddress(hinstDll, TEXT("OpenThemeData"));
			(FARPROC&)pCloseThemeData = ::GetProcAddress(hinstDll, TEXT("CloseThemeData"));
			(FARPROC&)pSetWindowTheme = ::GetProcAddress(hinstDll, TEXT("SetWindowTheme"));
			::FreeLibrary(hinstDll);
			if (pSetWindowTheme && pOpenThemeData && pCloseThemeData)
			{
				HANDLE theme = pOpenThemeData(hwnd,classList);
				if (theme!=NULL)
				{
					VERIFY(pCloseThemeData(theme)==S_OK);
					return pSetWindowTheme(hwnd, subApp, idlist);
				}
			}
		}
		return S_FALSE;
	}


void CListCtrl_Column_Picker::PreSubclassWindow()
{
	CListCtrl::PreSubclassWindow();

	// Focus retangle is not painted properly without double-buffering
#if (_WIN32_WINNT >= 0x501)
	SetExtendedStyle(LVS_EX_DOUBLEBUFFER | GetExtendedStyle());
#endif
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_HEADERDRAGDROP);
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_GRIDLINES);

	// Enable Vista-look if possible
	EnableWindowTheme(GetSafeHwnd(), L"ListView", L"Explorer", NULL);
}

