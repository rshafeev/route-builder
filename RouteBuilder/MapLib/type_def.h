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
	_deflateCompress    = 1,					// deflate-сжатие 
	_noCompress         = 2					// без сжатия
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
	virtual int GetErrorCode()=NULL;		 // возвращает код ошибки
	char* GetErrorString();					 // возвращает информацию об ошибке
	char* GetdetailsText();					 // возвращает доп. информацию об ошибке (чаще всего название класса и функции, где возникла ошибка.)
};

}