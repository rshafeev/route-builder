#pragma once
#include "navcenterdoc.h"
#include "..\NavCenter.DB\IDBService.h"

class COSMDoc :	public CNavCenterDoc
{
// создать только из сериализации
	IDBService *dbService;	
public: 
	COSMDoc();
	DECLARE_DYNCREATE(COSMDoc)
	CString fName;
	IDBService* GetDBService(){	return dbService;}

	CString GetFileName();
// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()

// Реализация
public:
	virtual	~COSMDoc(void);
	// Переопределение

public:
	virtual void Serialize(CArchive& ar);
	
	#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
	#endif


		virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
		virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
};

