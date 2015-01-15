#pragma once
#include "navcenterdoc.h"
#include "..\NavCenter.DB\IDBService.h"

class COSMDoc :	public CNavCenterDoc
{
// ������� ������ �� ������������
	IDBService *dbService;	
public: 
	COSMDoc();
	DECLARE_DYNCREATE(COSMDoc)
	CString fName;
	IDBService* GetDBService(){	return dbService;}

	CString GetFileName();
// ��������� ������� ����� ���������
protected:
	DECLARE_MESSAGE_MAP()

// ����������
public:
	virtual	~COSMDoc(void);
	// ���������������

public:
	virtual void Serialize(CArchive& ar);
	
	#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
	#endif


		virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
		virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
};

