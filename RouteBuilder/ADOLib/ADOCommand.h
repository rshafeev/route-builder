#pragma once



#include "ado2.h"
#include "ADOParameter.h"

namespace ADODB
{

class ADOLIB_API CADOCommand
{
public:
	enum cadoCommandType
	{
		typeCmdText = adCmdText,
		typeCmdTable = adCmdTable,
		typeCmdTableDirect = adCmdTableDirect,
		typeCmdStoredProc = adCmdStoredProc,
		typeCmdUnknown = adCmdUnknown,
		typeCmdFile = adCmdFile
	};
	
	CADOCommand(CADODatabase* pAdoDatabase, CString strCommandText = _T(""), int nCommandType = typeCmdStoredProc);
		
	~CADOCommand();

	void SetTimeout(long nTimeOut);
	void SetText(CString strCommandText);
	void SetType(int nCommandType);
	int GetType();
	BOOL AddParameter(CADOParameter* pAdoParameter);
	BOOL AddParameter(CString strName, int nType, int nDirection, long lSize, int nValue);
	BOOL AddParameter(CString strName, int nType, int nDirection, long lSize, long lValue);
	BOOL AddParameter(CString strName, int nType, int nDirection, long lSize, double dblValue, int nPrecision = 0, int nScale = 0);
	BOOL AddParameter(CString strName, int nType, int nDirection, long lSize, CString strValue);
	BOOL AddParameter(CString strName, int nType, int nDirection, long lSize, COleDateTime time);
	BOOL AddParameter(CString strName, int nType, int nDirection, long lSize, _variant_t vtValue, int nPrecision = 0, int nScale = 0);
	CString GetText();
	BOOL Execute(int nCommandType = typeCmdStoredProc);
	int GetRecordsAffected();
	_CommandPtr GetCommand();

protected:
	void dump_com_error(_com_error &e);

protected:
	_CommandPtr m_pCommand;
	int m_nCommandType;
	int m_nRecordsAffected;
	CString m_strCommandText;
	CString m_strLastError;
	DWORD m_dwLastError;
};

}