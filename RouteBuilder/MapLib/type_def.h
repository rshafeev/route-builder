#pragma once
#ifdef MAPLIB_EXPORTS
#define MAPLIB_API __declspec(dllexport)
#else
#define MAPLIB_API __declspec(dllimport)
#endif
namespace Rsmap
{
enum MAPLIB_API _resultEnum
{
	_result_ok    = 1,
	_result_error = 2
};
enum MAPLIB_API _CompressTypeEnum
{
	_deflateCompress    = 1,					// deflate-������ 
	_noCompress         = 2					// ��� ������
};

class MAPLIB_API CRsmapFileOptions
{
private:
public:
	_CompressTypeEnum imageCompress;
	_CompressTypeEnum dataCompress;
	int PaletteBitCount;
	int PaletteColorCount;
	CRsmapFileOptions();
	~CRsmapFileOptions();
};

class MAPLIB_API CMapException
{
protected:
	char* error_str;
	char* details;
public:
	CMapException();
	~CMapException();
	virtual int GetErrorCode()=NULL;		 // ���������� ��� ������
	char* GetErrorString();					 // ���������� ���������� �� ������
	char* GetdetailsText();					 // ���������� ���. ���������� �� ������ (���� ����� �������� ������ � �������, ��� �������� ������.)
};

}