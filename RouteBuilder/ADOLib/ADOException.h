
/////////////////////////////////////////////////////////////////////
//
//		CADOException Class
//
#pragma once
#include "ado2.h"
class ADOLIB_API CADOException
{
public:
	CADOException() ;

	CADOException(long lErrorCode);

	CADOException(long lErrorCode, const CString& strError);

	CADOException(const CString& strError) ;

	CADOException(long lErrorCode, const char* szError) ;

	CADOException(const char* szError);

	virtual ~CADOException();
	CString GetErrorMessage() const;
	void SetErrorMessage(LPCSTR lpstrError = _T(""));
	long GetError();
	void SetError(long lErrorCode = 0);

protected:
	CString m_strError;
	long m_lErrorCode;
};
