// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the NAVCENTERDB_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// NAVCENTERDB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef NAVCENTERDB_EXPORTS
#define NAVCENTERDB_API __declspec(dllexport)
#else
#define NAVCENTERDB_API __declspec(dllimport)
#endif

// This class is exported from the NavCenter.DB.dll
class NAVCENTERDB_API CNavCenterDB {
public:
	CNavCenterDB(void);
	// TODO: add your methods here.
};

extern NAVCENTERDB_API int nNavCenterDB;

NAVCENTERDB_API int fnNavCenterDB(void);
