#pragma once
#include "ado2.h"
#include "ADODatabase.h"
#include "ADOCommand.h"
#include "ADOParameter.h"
namespace ADODB
{

class ADOLIB_API CADORecordset
{
public:
BOOL Clone(CADORecordset& pRs);
	
	enum cadoOpenEnum
	{
		openUnknown = 0,
		openQuery = 1,
		openTable = 2,
		openStoredProc = 3
	};

	enum cadoEditEnum
	{
		dbEditNone = 0,
		dbEditNew = 1,
		dbEdit = 2
	};
	
	enum cadoPositionEnum
	{
		positionUnknown = -1,
		positionBOF = -2,
		positionEOF = -3
	};
	
	enum cadoSearchEnum
	{	
		searchForward = 1,
		searchBackward = -1
	};

	enum cadoDataType
	{
		typeEmpty = ADODB::adEmpty,
		typeTinyInt = ADODB::adTinyInt,
		typeSmallInt = ADODB::adSmallInt,
		typeInteger = ADODB::adInteger,
		typeBigInt = ADODB::adBigInt,
		typeUnsignedTinyInt = ADODB::adUnsignedTinyInt,
		typeUnsignedSmallInt = ADODB::adUnsignedSmallInt,
		typeUnsignedInt = ADODB::adUnsignedInt,
		typeUnsignedBigInt = ADODB::adUnsignedBigInt,
		typeSingle = ADODB::adSingle,
		typeDouble = ADODB::adDouble,
		typeCurrency = ADODB::adCurrency,
		typeDecimal = ADODB::adDecimal,
		typeNumeric = ADODB::adNumeric,
		typeBoolean = ADODB::adBoolean,
		typeError = ADODB::adError,
		typeUserDefined = ADODB::adUserDefined,
		typeVariant = ADODB::adVariant,
		typeIDispatch = ADODB::adIDispatch,
		typeIUnknown = ADODB::adIUnknown,
		typeGUID = ADODB::adGUID,
		typeDate = ADODB::adDate,
		typeDBDate = ADODB::adDBDate,
		typeDBTime = ADODB::adDBTime,
		typeDBTimeStamp = ADODB::adDBTimeStamp,
		typeBSTR = ADODB::adBSTR,
		typeChar = ADODB::adChar,
		typeVarChar = ADODB::adVarChar,
		typeLongVarChar = ADODB::adLongVarChar,
		typeWChar = ADODB::adWChar,
		typeVarWChar = ADODB::adVarWChar,
		typeLongVarWChar = ADODB::adLongVarWChar,
		typeBinary = ADODB::adBinary,
		typeVarBinary = ADODB::adVarBinary,
		typeLongVarBinary = ADODB::adLongVarBinary,
		typeChapter = ADODB::adChapter,
		typeFileTime = ADODB::adFileTime,
		typePropVariant = ADODB::adPropVariant,
		typeVarNumeric = ADODB::adVarNumeric,
		typeArray = ADODB::adVariant
	};
	
	enum cadoSchemaType 
	{
		schemaSpecific = adSchemaProviderSpecific,	
		schemaAsserts = adSchemaAsserts,
		schemaCatalog = adSchemaCatalogs,
		schemaCharacterSet = adSchemaCharacterSets,
		schemaCollections = adSchemaCollations,
		schemaColumns = adSchemaColumns,
		schemaConstraints = adSchemaCheckConstraints,
		schemaConstraintColumnUsage = adSchemaConstraintColumnUsage,
		schemaConstraintTableUsage  = adSchemaConstraintTableUsage,
		shemaKeyColumnUsage = adSchemaKeyColumnUsage,
		schemaTableConstraints = adSchemaTableConstraints,
		schemaColumnsDomainUsage = adSchemaColumnsDomainUsage,
		schemaIndexes = adSchemaIndexes,
		schemaColumnPrivileges = adSchemaColumnPrivileges,
		schemaTablePrivileges = adSchemaTablePrivileges,
		schemaUsagePrivileges = adSchemaUsagePrivileges,
		schemaProcedures = adSchemaProcedures,
		schemaTables = adSchemaTables,
		schemaProviderTypes = adSchemaProviderTypes,
		schemaViews = adSchemaViews,
		schemaViewTableUsage = adSchemaViewTableUsage,
		schemaProcedureParameters = adSchemaProcedureParameters,
		schemaForeignKeys = adSchemaForeignKeys,
		schemaPrimaryKeys = adSchemaPrimaryKeys,
		schemaProcedureColumns = adSchemaProcedureColumns,
		schemaDBInfoKeywords = adSchemaDBInfoKeywords,
		schemaDBInfoLiterals = adSchemaDBInfoLiterals,
		schemaCubes = adSchemaCubes,
		schemaDimensions = adSchemaDimensions,
		schemaHierarchies  = adSchemaHierarchies, 
		schemaLevels = adSchemaLevels,
		schemaMeasures = adSchemaMeasures,
		schemaProperties = adSchemaProperties,
		schemaMembers = adSchemaMembers,
	}; 

	
	BOOL SetFieldValue(int nIndex, int nValue);
	BOOL SetFieldValue(LPCTSTR lpFieldName, int nValue);
	BOOL SetFieldValue(int nIndex, long lValue);
	BOOL SetFieldValue(LPCTSTR lpFieldName, long lValue);
	BOOL SetFieldValue(int nIndex, unsigned long lValue);
	BOOL SetFieldValue(LPCTSTR lpFieldName, unsigned long lValue);
	BOOL SetFieldValue(int nIndex, double dblValue);
	BOOL SetFieldValue(LPCTSTR lpFieldName, double dblValue);
	BOOL SetFieldValue(int nIndex, CString strValue);
	BOOL SetFieldValue(LPCTSTR lpFieldName, CString strValue);
	BOOL SetFieldValue(int nIndex, COleDateTime time);
	BOOL SetFieldValue(LPCTSTR lpFieldName, COleDateTime time);
	BOOL SetFieldValue(int nIndex, bool bValue);
	BOOL SetFieldValue(LPCTSTR lpFieldName, bool bValue);
	BOOL SetFieldValue(int nIndex, COleCurrency cyValue);
	BOOL SetFieldValue(LPCTSTR lpFieldName, COleCurrency cyValue);
	BOOL SetFieldValue(int nIndex, _variant_t vtValue);
	BOOL SetFieldValue(LPCTSTR lpFieldName, _variant_t vtValue);

	BOOL SetFieldEmpty(int nIndex);
	BOOL SetFieldEmpty(LPCTSTR lpFieldName);

	void CancelUpdate();
	BOOL Update();
	void Edit();
	BOOL AddNew();
	BOOL AddNew(CADORecordBinding &pAdoRecordBinding);

	BOOL Find(LPCTSTR lpFind, int nSearchDirection = CADORecordset::searchForward);
	BOOL FindFirst(LPCTSTR lpFind);
	BOOL FindNext();
public:
	CADORecordset(void);
	~CADORecordset(void);
	CADORecordset(CADODatabase* pAdoDatabase);
	CString GetQuery();
	void SetQuery(LPCSTR strQuery);
	BOOL RecordBinding(CADORecordBinding &pAdoRecordBinding);
	DWORD GetRecordCount();
	BOOL IsOpen();
	void Close();
	BOOL Open(_ConnectionPtr mpdb, LPCTSTR lpstrExec = _T(""), int nOption = CADORecordset::openUnknown);
	BOOL Open(LPCTSTR lpstrExec = _T(""), int nOption = CADORecordset::openUnknown);
	BOOL OpenSchema(int nSchema, LPCTSTR SchemaID = _T(""));
	long GetFieldCount();
	BOOL GetFieldValue(LPCTSTR lpFieldName, int& nValue);
	BOOL GetFieldValue(int nIndex, int& nValue);
	BOOL GetFieldValue(LPCTSTR lpFieldName, long& lValue);
	BOOL GetFieldValue(int nIndex, long& lValue);
	BOOL GetFieldValue(LPCTSTR lpFieldName, unsigned long& ulValue);
	BOOL GetFieldValue(int nIndex, unsigned long& ulValue);
	BOOL GetFieldValue(LPCTSTR lpFieldName, double& dbValue);
	BOOL GetFieldValue(int nIndex, double& dbValue);
	BOOL GetFieldValue(LPCTSTR lpFieldName, CString& strValue, CString strDateFormat = _T(""));
	BOOL GetFieldValue(int nIndex, CString& strValue, CString strDateFormat = _T(""));
	BOOL GetFieldValue(LPCTSTR lpFieldName, COleDateTime& time);
	BOOL GetFieldValue(int nIndex, COleDateTime& time);
	BOOL GetFieldValue(int nIndex, bool& bValue);
	BOOL GetFieldValue(LPCTSTR lpFieldName, bool& bValue);
	BOOL GetFieldValue(int nIndex, COleCurrency& cyValue);
	BOOL GetFieldValue(LPCTSTR lpFieldName, COleCurrency& cyValue);
	BOOL GetFieldValue(int nIndex, _variant_t& vtValue);
	BOOL GetFieldValue(LPCTSTR lpFieldName, _variant_t& vtValue);
	
	BOOL IsFieldNull(LPCTSTR lpFieldName);
	BOOL IsFieldNull(int nIndex);
	BOOL IsFieldEmpty(LPCTSTR lpFieldName);
	BOOL IsFieldEmpty(int nIndex);	
	BOOL IsEof();
	BOOL IsEOF();
	BOOL IsBof();
	BOOL IsBOF();
	void MoveFirst() ;
	void MoveNext() ;
	void MovePrevious() ;
	void MoveLast() ;
	long GetAbsolutePage();
	void SetAbsolutePage(int nPage);
	long GetPageCount();
	long GetPageSize();
	void SetPageSize(int nSize);
	long GetAbsolutePosition();
	void SetAbsolutePosition(int nPosition);
	BOOL GetFieldInfo(LPCTSTR lpFieldName, CADOFieldInfo* fldInfo);
	BOOL GetFieldInfo(int nIndex, CADOFieldInfo* fldInfo);
	BOOL AppendChunk(LPCTSTR lpFieldName, LPVOID lpData, UINT nBytes);
	BOOL AppendChunk(int nIndex, LPVOID lpData, UINT nBytes);

	BOOL GetChunk(LPCTSTR lpFieldName, CString& strValue);
	BOOL GetChunk(int nIndex, CString& strValue);
	
	BOOL GetChunk(LPCTSTR lpFieldName, LPVOID pData);
	BOOL GetChunk(int nIndex, LPVOID pData);

	CString GetString(LPCTSTR lpCols, LPCTSTR lpRows, LPCTSTR lpNull, long numRows = 0);
	CString GetLastErrorString();
	DWORD GetLastError();
	void GetBookmark();
	BOOL SetBookmark();
	BOOL Delete();
	BOOL IsConnectionOpen();
	_RecordsetPtr GetRecordset();
	_ConnectionPtr GetActiveConnection() ;

	BOOL SetFilter(LPCTSTR strFilter);
	BOOL SetSort(LPCTSTR lpstrCriteria);
	BOOL SaveAsXML(LPCTSTR lpstrXMLFile);
	BOOL OpenXML(LPCTSTR lpstrXMLFile);
	BOOL Execute(CADOCommand* pCommand);
	BOOL Requery();

public:
	_RecordsetPtr m_pRecordset;
	_CommandPtr m_pCmd;
	
protected:
	_ConnectionPtr m_pConnection;
	int m_nSearchDirection;
	CString m_strFind;
	_variant_t m_varBookFind;
	_variant_t m_varBookmark;
	int m_nEditStatus;
	CString m_strLastError;
	DWORD m_dwLastError;
	void dump_com_error(_com_error &e);
	IADORecordBinding *m_pRecBinding;
	CString m_strQuery;

protected:
	BOOL PutFieldValue(LPCTSTR lpFieldName, _variant_t vtFld);
	BOOL PutFieldValue(_variant_t vtIndex, _variant_t vtFld);
	BOOL GetFieldInfo(FieldPtr pField, CADOFieldInfo* fldInfo);
	BOOL GetChunk(FieldPtr pField, CString& strValue);
	BOOL GetChunk(FieldPtr pField, LPVOID lpData);
	BOOL AppendChunk(FieldPtr pField, LPVOID lpData, UINT nBytes);


};
}

