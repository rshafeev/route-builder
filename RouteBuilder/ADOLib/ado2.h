
#if !defined(AFX_ADO2_H_INCLUDED_)
#define AFX_ADO2_H_INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#ifdef ADOLIB_EXPORTS
#define ADOLIB_API __declspec(dllexport)
#else
#define ADOLIB_API __declspec(dllimport)
#endif

//#pragma warning (disable: 4146)
// CG : In order to use this code against a different version of ADO, the appropriate
// ADO library needs to be used in the #import statement
#pragma message ("Make sure you go to Tools.Options.Directories.Library files and add the paths to msado15.dll and msjro.dll will usually be in C:\\Program Files\\Common Files\\System\\ado")

#import "C:\\Program Files\\Common Files\\System\\ado\\msado15.dll" rename("EOF", "EndOfFile")
//#import "C:\\Program Files\\Common Files\\System\\ado\\MSJRO.DLL" no_namespace rename("ReplicaTypeEnum", "_ReplicaTypeEnum") 
//#pragma warning (default: 4146)
#include <icrsint.h>

namespace ADODB
{

struct CADOFieldInfo
{
	char m_strName[30]; 
	short m_nType;
	long m_lSize; 
	long m_lDefinedSize;
	long m_lAttributes;
	short m_nOrdinalPosition;
	BOOL m_bRequired;   
	BOOL m_bAllowZeroLength; 
	long m_lCollatingOrder;  
};

CString IntToStr(int nVal);
CString LongToStr(long lVal);
CString ULongToStr(unsigned long ulVal);
CString DblToStr(double dblVal, int ndigits = 20);
CString DblToStr(float fltVal);

}
#endif //AFX_ADO2_H_INCLUDED_
