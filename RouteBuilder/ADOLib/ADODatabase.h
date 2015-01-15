
#pragma once

#include "ado2.h"

namespace  ADODB
{
class ADOLIB_API CADODatabase
{
public:
	enum cadoConnectModeEnum
    {	
		connectModeUnknown = adModeUnknown,
		connectModeRead = adModeRead,
		connectModeWrite = adModeWrite,
		connectModeReadWrite = adModeReadWrite,
		connectModeShareDenyRead = adModeShareDenyRead,
		connectModeShareDenyWrite = adModeShareDenyWrite,
		connectModeShareExclusive = adModeShareExclusive,
		connectModeShareDenyNone = adModeShareDenyNone
    };

	CADODatabase();
	
	~CADODatabase();
	
	BOOL Open(LPCTSTR lpstrConnection = _T(""), LPCTSTR lpstrUserID = _T(""), LPCTSTR lpstrPassword = _T(""));
	_ConnectionPtr GetActiveConnection() ;
	BOOL  Execute(LPCTSTR lpstrExec);
	DWORD GetRecordCount(_RecordsetPtr m_pRs);
	int   GetRecordsAffected();
	long  BeginTransaction() ;
	long  CommitTransaction() ;
	long  RollbackTransaction() ;
	BOOL  IsOpen();
	void  Close();
	void  SetConnectionMode(cadoConnectModeEnum nMode);
	void SetConnectionString(LPCTSTR lpstrConnection);
	CString GetConnectionString();
	CString GetLastErrorString();
	DWORD GetLastError();
	CString GetErrorDescription() ;
	void SetConnectionTimeout(long nConnectionTimeout = 30);

protected:
	void dump_com_error(_com_error &e);

public:
	_ConnectionPtr m_pConnection;
	
protected:
	CString m_strConnection;
	CString m_strLastError;
	CString m_strErrorDescription;
	DWORD m_dwLastError;
	int m_nRecordsAffected;
	long m_nConnectionTimeout;
};

}