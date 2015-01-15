
#pragma once
#include "DockableLocationsPane.h"
#include "DockablePolylinesPane.h"
#include "DockableRoutesPane.h"

#include "GuiSubject.h"
#include "OutputWnd.h"

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Атрибуты
public:
	
// Операции
public:

// Переопределение
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Реализация
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // встроенные члены панели элементов управления
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonButton  *m_VisualStyleButton ;
	CMFCToolBarImages m_PanelImages;
	CMFCCaptionBar    m_wndCaptionBar;
	CDockableLocationsPane *m_dockableLocationsPane;
	CDockablePolylinesPane *m_dockablePolylinesPane;
	CDockableRoutesPane    *m_dockableRoutesPane;
	CGuiSubject m_guiSubject;
	COutputWnd        *m_wndOutput;

// Созданные функции схемы сообщений
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnViewCaptionBar();
	afx_msg void OnUpdateViewCaptionBar(CCmdUI* pCmdUI);
	afx_msg void OnOptions();
	DECLARE_MESSAGE_MAP()

	// Функция создания панелей(CLocationsWnd,...)
	bool CreateDockingWindows(void);

public:
	CGuiSubject& GetGuiSubject()
	{
		return m_guiSubject;
	}
	CDockablePolylinesPane& GetDockablePolylinesPane()
	{
		return *m_dockablePolylinesPane;
	}
	void AddInfoString(CString str)
	{
		this->m_wndOutput->AddInfoString(str);
	}
	int GetOSMDocumentCount();
public:
	afx_msg void OnButtonOsmLoadDoc();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnCheckViewlocations();
	afx_msg void OnUpdateCheckViewlocations(CCmdUI *pCmdUI);
	afx_msg void OnCheckViewpolylines();
	afx_msg void OnUpdateCheckViewpolylines(CCmdUI *pCmdUI);
	afx_msg void OnCheckOutput();
	afx_msg void OnUpdateCheckOutput(CCmdUI *pCmdUI);
	afx_msg void OnButtonOsmopendoc();
	afx_msg void OnButtonoptions();
	afx_msg void OnButtonTest();
	afx_msg void OnCheckRoutes();
	afx_msg void OnUpdateCheckRoutes(CCmdUI *pCmdUI);
	afx_msg void OnButtonXls();
	afx_msg void OnUpdateButtonXls(CCmdUI *pCmdUI);
};