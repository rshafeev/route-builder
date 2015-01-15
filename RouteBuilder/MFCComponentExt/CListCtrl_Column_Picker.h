#pragma once

#ifdef MFCCOMPONENTEXT_EXPORTS
#define MFCCOMPONENTEXT_API __declspec(dllexport)
#else
#define MFCCOMPONENTEXT_API __declspec(dllimport)
#endif

class AFX_EXT_CLASS CListCtrl_Column_Picker : public CListCtrl
{
	struct AFX_EXT_CLASS ColumnState
	{
		ColumnState();
		bool m_Visible;
		int  m_OrgWidth;	// Width it had before being hidden
		int  m_OrgPosition;	// Position it had before being hidden
	};
	CSimpleArray<ColumnState>	m_ColumnStates;
	int GetColumnStateCount();

	ColumnState& GetColumnState(int nCol);

	DECLARE_MESSAGE_MAP()

	virtual afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual afx_msg LRESULT OnDeleteColumn(WPARAM wParam, LPARAM lParam);
	virtual afx_msg LRESULT OnInsertColumn(WPARAM wParam, LPARAM lParam);
	virtual afx_msg LRESULT OnSetColumnWidth(WPARAM wParam, LPARAM lParam);
	virtual afx_msg BOOL OnHeaderBeginResize(UINT id, NMHDR* pNmhdr, LRESULT* pResult);
	virtual afx_msg BOOL OnHeaderEndDrag(UINT id, NMHDR* pNmhdr, LRESULT* pResult);
	virtual afx_msg BOOL OnHeaderDividerDblClick(UINT, NMHDR* pNMHDR, LRESULT* pResult);
	virtual afx_msg void OnContextMenu(CWnd*, CPoint point);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	virtual void PreSubclassWindow();
	void InsertColumnState(int nCol, bool bVisible, int nOrgWidth = 0);
	void DeleteColumnState(int nCol);
public:
	CListCtrl_Column_Picker();
	~CListCtrl_Column_Picker();
	bool IsColumnVisible(int nCol);
	int GetFirstVisibleColumn();
	BOOL ShowColumn(int nCol, bool bShow);
	BOOL SetColumnWidthAuto(int nCol = -1, bool includeHeader = false);

};