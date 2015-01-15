#include "StdAfx.h"
#include "OSMDoc.h"
#include <propkey.h>
#include "NavCenter.h"
#include "NavCenterView.h"
#include "MainFrm.h"
#include <io.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(COSMDoc, CNavCenterDoc)

BEGIN_MESSAGE_MAP(COSMDoc, CNavCenterDoc)
END_MESSAGE_MAP()

COSMDoc::COSMDoc(void)
{
	dbService = NULL;
	theApp.CloseAllDocuments(true);
}


COSMDoc::~COSMDoc(void)
{
	if(dbService!=NULL)
		dbService->Close();
}
CString COSMDoc::GetFileName()
{
	return this->fName;
}

// сериализация COSMDoc
void COSMDoc::Serialize(CArchive& ar)
{
	CString dbName = ar.GetFile()->GetFileName();
		
	if (ar.IsStoring())
	{
		// TODO: добавьте код сохранения
	}
	else
	{
		// TODO: добавьте код загрузки
	}
}

#ifdef _DEBUG
void COSMDoc::AssertValid() const
{
	CNavCenterDoc::AssertValid();
}

void COSMDoc::Dump(CDumpContext& dc) const
{
	CNavCenterDoc::Dump(dc);
}
#endif //_DEBUG



BOOL COSMDoc::CanCloseFrame(CFrameWnd* pFrame)
{
	// TODO: Add your specialized code here and/or call the base class
	/*if(AfxMessageBox(_T("Do you want to save current document"), MB_YESNO|MB_ICONSTOP)==MB_OK)
	{
		SetModifiedFlag(true);
	//	OnSaveDocument
	}
	else
	{
		SetModifiedFlag(false);
	}*/
	return CNavCenterDoc::CanCloseFrame(pFrame);
}

bool IsFileExist(LPCTSTR strFileName)
{
	if (access (strFileName,     0)  == 0)
	 return true;
	return false;
}
BOOL COSMDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	//((CMainFrame*)theApp.m_pMainWnd)->AddInfoString("COSMDoc::OnOpenDocument(LPCTSTR lpszPathName)");
	if(!IsFileExist(lpszPathName))
	{
		theApp.m_pMainWnd->MessageBox("File does not exist!","Error");
		return FALSE;
	}
	dbService = IDBService::CreateObject();
	fName = lpszPathName;
	//((CMainFrame*)theApp.m_pMainWnd)->AddInfoString("COSMDoc::OnOpenDocument(LPCTSTR lpszPathName): Create dbService");
	if(dbService==NULL||dbService->Connect(lpszPathName)==FALSE)
	{	
		theApp.m_pMainWnd->MessageBox("Do not connect to DB","Error");
		return FALSE;
	}
	
   POSITION pos = GetFirstViewPosition();
  
   while (pos != NULL)
   {
	   CNavCenterView* pView =(CNavCenterView*) GetNextView(pos);
	   if(pView!=NULL)
		   pView->Init(dynamic_cast<CMainFrame*>(theApp.m_pMainWnd)->GetGuiSubject());

	//   ((CMainFrame*)theApp.m_pMainWnd)->AddInfoString("COSMDoc::OnOpenDocument(LPCTSTR lpszPathName): Create view!");
   }   
	return TRUE;
}
