// Этот исходный код MFC Samples демонстрирует функционирование пользовательского интерфейса Fluent на основе MFC в Microsoft Office
// (Fluent UI) и предоставляется исключительно как справочный материал в качестве дополнения к
// справочнику по пакету Microsoft Foundation Classes и связанной электронной документации,
// включенной в программное обеспечение библиотеки MFC C++. 
// Условия лицензионного соглашения на копирование, использование или распространение Fluent UI доступны отдельно. 
// Для получения дополнительных сведений о нашей лицензионной программе Fluent UI посетите веб-узел
// http://msdn.microsoft.com/officeui.
//
// (C) Корпорация Майкрософт (Microsoft Corp.)
// Все права защищены.

// MainFrm.cpp : реализация класса CMainFrame
//

#include "stdafx.h"
#include "NavCenter.h"

#include "MainFrm.h"


#include "OSMDoc.h"
#include "OSMView.h"
#include "ChildFrm.h"
#include "DlgOptions.h"
#include "..\InetLib\RouteCloudeMade.h"
#include "DlgExportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_COMMAND(ID_VIEW_CAPTION_BAR, &CMainFrame::OnViewCaptionBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CAPTION_BAR, &CMainFrame::OnUpdateViewCaptionBar)
	ON_COMMAND(ID_TOOLS_OPTIONS, &CMainFrame::OnOptions)
	ON_COMMAND(ID_BUTTON_OSMLOADDOC, &CMainFrame::OnButtonOsmLoadDoc)
	ON_COMMAND(ID_FILE_NEW, &CMainFrame::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CMainFrame::OnFileOpen)
	
	ON_COMMAND(ID_CHECK_VIEWLOCATIONS, &CMainFrame::OnCheckViewlocations)
	ON_UPDATE_COMMAND_UI(ID_CHECK_VIEWLOCATIONS, &CMainFrame::OnUpdateCheckViewlocations)
	ON_COMMAND(ID_CHECK_VIEWPOLYLINES, &CMainFrame::OnCheckViewpolylines)
	ON_UPDATE_COMMAND_UI(ID_CHECK_VIEWPOLYLINES, &CMainFrame::OnUpdateCheckViewpolylines)
	ON_COMMAND(ID_CHECK_OUTPUT, &CMainFrame::OnCheckOutput)
	ON_UPDATE_COMMAND_UI(ID_CHECK_OUTPUT, &CMainFrame::OnUpdateCheckOutput)
	ON_COMMAND(ID_BUTTON_OSMOPENDOC, &CMainFrame::OnButtonOsmopendoc)
	ON_COMMAND(ID_BUTTONOPTIONS, &CMainFrame::OnButtonoptions)
	ON_COMMAND(ID_BUTTON3, &CMainFrame::OnButtonTest)
	ON_COMMAND(ID_CHECK_ROUTES, &CMainFrame::OnCheckRoutes)
	ON_UPDATE_COMMAND_UI(ID_CHECK_ROUTES, &CMainFrame::OnUpdateCheckRoutes)
	ON_COMMAND(ID_BUTTON_XLS, &CMainFrame::OnButtonXls)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_XLS, &CMainFrame::OnUpdateButtonXls)
END_MESSAGE_MAP()

// создание/уничтожение CMainFrame

CMainFrame::CMainFrame()
{
	// TODO: добавьте код инициализации члена
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);
	//GetFileVersionInfoW(
	
	//MoveFileEx("C:\\Program Files\\Common Files\\System\\ado\\msado15.dll"	,"C:\\Program Files\\Common Files\\System\\ado\\msado15_new.dll",MOVEFILE_DELAY_UNTIL_REBOOT);
	//MoveFileEx("C:\\Program Files\\Common Files\\System\\adoLast\\msado15.dll"	,"C:\\Program Files\\Common Files\\System\\ado\\msado15.dll",MOVEFILE_DELAY_UNTIL_REBOOT);

	//
	//MoveFileEx
	COSMCtrlEx::GetDistance(CDBPolyLine::GeomToMas("(30;56),(10;20)"));
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	/*if(!DeleteFile("C:\\Program Files\\Common Files\\System\\ado\\msado15.dll"))
		return -1;*/
	BOOL bNameValid;
	// установите наглядный диспетчер и стиль на основе постоянного значения
	m_VisualStyleButton =      new CMFCRibbonButton(-1, _T("Style"), -1, -1);
    m_VisualStyleButton->SetMenu(IDR_THEME_MENU, FALSE, TRUE);
    m_wndRibbonBar.AddToTabs(m_VisualStyleButton);
	OnApplicationLook(theApp.m_nAppLook);

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // другие доступные стили...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // установите значение FALSE, чтобы расположить кнопку \"Закрыть\" в правой части области вкладки
	mdiTabParams.m_bTabIcons = FALSE;    // установите значение TRUE, чтобы включить значки документов на вкладках MDI
	mdiTabParams.m_bAutoColor = TRUE;    // установите значение FALSE, чтобы отключить автоматическое выделение цветом вкладок MDI
	mdiTabParams.m_bDocumentMenu = TRUE; // включить меню документа на правой границе области вкладки
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	// включить режим работы закрепляемых окон стилей Visual Studio 2005
	CDockingManager::SetDockingMode(DT_SMART);
	// включить режим работы автоматического скрытия закрепляемых окон стилей Visual Studio 2005
	EnableAutoHidePanes(CBRS_ALIGN_ANY);


	// Включить диалоговое окно расширенного управления окнами
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	
	// Enable toolbar and docking window menu replacement
	//EnablePaneMenu(TRUE, ID_WINDOW_MANAGER, _T("dfgdfg"), ID_VIEW_TOOLBAR);

	// Переключите порядок имени документа и имени приложения в заголовке окна. Это
	// повышает удобство использования панели задач, так как на эскизе отображается имя документа.
	ModifyStyle(0, FWS_PREFIXTITLE);
		// enable menu personalization (most-recently used commands)
	// TODO: define your own basic commands, ensuring that each pulldown menu has at least one basic command.
	CreateDockingWindows();
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

	return TRUE;
}


// диагностика CMainFrame

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// обработчики сообщений CMainFrame

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;
	//IDR_THEME_MENU

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
	}
	
	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

void CMainFrame::OnViewCaptionBar()
{
	m_wndCaptionBar.ShowWindow(m_wndCaptionBar.IsVisible() ? SW_HIDE : SW_SHOW);
	RecalcLayout(FALSE);
}

void CMainFrame::OnUpdateViewCaptionBar(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndCaptionBar.IsVisible());
}

void CMainFrame::OnOptions()
{
	CMFCRibbonCustomizeDialog *pOptionsDlg = new CMFCRibbonCustomizeDialog(this, &m_wndRibbonBar);
	ASSERT(pOptionsDlg != NULL);

	pOptionsDlg->DoModal();
	delete pOptionsDlg;
}


// Функция создания панелей(CAbonentsWnd,...)
bool CMainFrame::CreateDockingWindows(void)
{
// Locations
	m_dockableLocationsPane = new CDockableLocationsPane(GetGuiSubject());
	DWORD dwStyle =WS_CHILD  | 	WS_CLIPSIBLINGS | WS_CLIPCHILDREN  | CBRS_LEFT|CBRS_FLOAT_MULTI;
				
	if (!m_dockableLocationsPane->Create(_T("Locations"), this, CSize(200, 200), TRUE, ID_VIEWLOCATIONWND,dwStyle ))
	{
		TRACE0("Не удалось создать окно \"Свойства\"\n");
		return FALSE; // не удалось создать
	}
	m_dockableLocationsPane->EnableDocking(CBRS_LEFT|CBRS_RIGHT);//CBRS_LEFT|CBRS_RIGHT
	DockPane(m_dockableLocationsPane);

// Polylines
	m_dockablePolylinesPane = new CDockablePolylinesPane(GetGuiSubject());
	if (!m_dockablePolylinesPane->Create(_T("Polylines"), this, CSize(200, 200), TRUE, ID_VIEWPOLYLINESWND,dwStyle ))//ID_VIEWPOLYLINESWND
	{
		TRACE0("Не удалось создать окно \"Свойства\"\n");
		return FALSE; // не удалось создать
	}
	m_dockablePolylinesPane->EnableDocking(CBRS_LEFT|CBRS_RIGHT);

	DockPane(m_dockablePolylinesPane);
// Routes 
	m_dockableRoutesPane = new CDockableRoutesPane(GetGuiSubject());
	if (!m_dockableRoutesPane->Create(_T("Routes"), this, CSize(200, 200), TRUE, ID_VIEWROUTESWND,dwStyle ))//ID_VIEWPOLYLINESWND
	{
		TRACE0("Не удалось создать окно \"Свойства\"\n");
		return FALSE; // не удалось создать
	}
	m_dockableRoutesPane->EnableDocking(CBRS_LEFT|CBRS_RIGHT);
	DockPane(m_dockableRoutesPane);

// Output
	// Create output window
	m_wndOutput = new COutputWnd();
	if (!m_wndOutput->Create("Output", this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Output window\n");
		return FALSE; // failed to create
	}
	m_wndOutput->EnableDocking(CBRS_ALIGN_ANY);
	DockPane(m_wndOutput);
	return true;
}

int GetDocCount(CDocTemplate* ptemplate)
{
	POSITION pos = ptemplate->GetFirstDocPosition();
	if(pos==0)		return 0 ;

	int count = 0;
	while(ptemplate->GetNextDoc(pos)!=NULL)
	{
		count++;
		if(pos==0) break;
	}
	return count;
}
int CMainFrame::GetOSMDocumentCount()
{
	int count = 0;
	POSITION pos = theApp.GetFirstDocTemplatePosition();
	if(pos==0)		return 0;
	CDocTemplate* ptemplate = theApp.GetNextDocTemplate(pos);
	if(ptemplate!=NULL)
	{
		POSITION docPos = ptemplate->GetFirstDocPosition();
		while(docPos!=NULL)
		{
			ptemplate->GetNextDoc(docPos);
			count++;
		}
	}
	return count;
}
void CMainFrame::OnButtonOsmLoadDoc()
{
	OnFileNew();
}


void CMainFrame::OnFileNew()
{
	//theApp.CloseAllDocuments(true);
	POSITION pos = theApp.GetFirstDocTemplatePosition();
	if(pos==0)		return;

	CDocTemplate* ptemplate = theApp.GetNextDocTemplate(pos);
	if(ptemplate!=NULL)
	{
		CFileDialog * dlg = new CFileDialog(false,_T("*.accdb"),0,0,
			_T("Data base (*.accdb)|*.accdb"),
			0,0,1);
		
		//TRUE,NULL,"*.cpp");
		if(dlg->DoModal()==IDOK)
		{
			CString originalDBPath("sys\\geoDB.nav");
			CopyFile(originalDBPath,dlg->GetPathName(),true);
			CDocument* doc = ptemplate->OpenDocumentFile(dlg->GetPathName());
		
		}
	}

}


void CMainFrame::OnFileOpen()
{
	//theApp.CloseAllDocuments(true);
	POSITION pos = theApp.GetFirstDocTemplatePosition();
	if(pos==0)		return;
	CDocTemplate* ptemplate = theApp.GetNextDocTemplate(pos);
	
	if(ptemplate!=NULL)
	{
		CFileDialog * dlg = new CFileDialog(true,_T("*.accdb"),0,0,
			_T("Data base (*.accdb)|*.accdb"),
			0,0,1);
		
		if(dlg->DoModal()==IDOK)
		{
			CDocument* doc = ptemplate->OpenDocumentFile(dlg->GetPathName(),TRUE);
		}
	}
}





void CMainFrame::OnCheckViewlocations()
{
	if(m_dockableLocationsPane->IsVisible())
		m_dockableLocationsPane->ShowPane(false,false,false);
	else
		m_dockableLocationsPane->ShowPane(true,false,false);
}


void CMainFrame::OnUpdateCheckViewlocations(CCmdUI *pCmdUI)
{
	if(m_dockableLocationsPane->IsVisible())
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
	
}


void CMainFrame::OnCheckViewpolylines()
{
	if(m_dockablePolylinesPane->IsVisible())
		m_dockablePolylinesPane->ShowPane(false,false,false);
	else
		m_dockablePolylinesPane->ShowPane(true,false,false);
}


void CMainFrame::OnUpdateCheckViewpolylines(CCmdUI *pCmdUI)
{
	if(m_dockablePolylinesPane->IsVisible())
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}


void CMainFrame::OnCheckOutput()
{
	if(m_wndOutput->IsVisible())
		m_wndOutput->ShowPane(false,false,false);
	else
		m_wndOutput->ShowPane(true,false,false);
}


void CMainFrame::OnUpdateCheckOutput(CCmdUI *pCmdUI)
{
		if(m_wndOutput->IsVisible())
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}


void CMainFrame::OnButtonOsmopendoc()
{
	this->OnFileOpen();
}


void CMainFrame::OnButtonoptions()
{
	CDlgOptions options(this);
	options.DoModal();

}


void CMainFrame::OnButtonTest()
{
	/*CWaitWindow *w = new CWaitWindow(this);
	w->Open();
	EnableWindow(FALSE);
	Sleep(4000);
	EnableWindow(TRUE);
	w->Close();
	*/
}


void CMainFrame::OnCheckRoutes()
{
	if(m_dockableRoutesPane->IsVisible())
		m_dockableRoutesPane->ShowPane(false,false,false);
	else
		m_dockableRoutesPane->ShowPane(true,false,false);
}


void CMainFrame::OnUpdateCheckRoutes(CCmdUI *pCmdUI)
{
	if(m_dockableRoutesPane->IsVisible())
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}


void CMainFrame::OnButtonXls()
{
	CDlgExportDlg dlg(this);
	dlg.DoModal();
}

void CMainFrame::OnUpdateButtonXls(CCmdUI *pCmdUI)
{
		CMDIFrameWnd *pFrame =   (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
		CMDIChildWnd *pChild =   (CMDIChildWnd *) pFrame->GetActiveFrame();
		if(pChild==NULL)
		{
			pCmdUI->Enable(false);
			return;
		}
		COSMDoc* doc = dynamic_cast<COSMDoc*>(pChild->GetActiveDocument());
		if(doc==NULL)
		{
		{
			pCmdUI->Enable(false);
			return;
		}
		}
		pCmdUI->Enable(true);
}
