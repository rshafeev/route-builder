#pragma once
#include "type_def.h"

//===================================================================================================================================================
namespace Rsmap
{
// абстрактный класс для создания rsmap файла из источника данных
class MAPLIB_API CCreateRasterMap
{
protected:
	 char* sourceFileName;				// путь к источнику данных
	 int bandCount;						// количество цветовых каналов
	 CCreateRasterMap(void);
	~CCreateRasterMap(void);
public:
	/// <summary> создает связь с ресурсом ( импортируемый файл карты/изображения </summary>
	/// <param name="FileName"> путь к файлу</param>
    /// <returns>возвращает объект для дальнейшей конвертации файла. NULL - не удалось связаться с источником данных и объект не создался</returns>
	static CCreateRasterMap* CreateObject(char* sourceFileName);	

	/// <summary> создает rsmap файл </summary>
	/// <param name="rsmapFile"> полный путь вместе с названием создаваемого rsmap-файла</param>
	/// <param name="options">параметры создаваемого файла(тип сжатия, кол-во цветов и т.д.)</param>
	/// <returns>возвращает RSMAP_OK в случае успешного завершения фукнции</returns>
	virtual int GenRsmapFile(char* rsmapFile,CRsmapFileOptions options)=NULL;

	///<summary> очистка памяти за объектом.  </summary>
	void Free();
};

//===================================================================================================================================================


//класс исключений, которые могут возникнуть при создании rsmap файла
class MAPLIB_API CCreateRasterMapExeption  : public CMapException
{
public:
enum MAPLIB_API _createRsMapExeptEnum
{
	_initError=0
};
    _createRsMapExeptEnum error;
public:
	CCreateRasterMapExeption(_createRsMapExeptEnum error_code,char* details);
	virtual int GetErrorCode();						 // возвращает код ошибки

//===================================================================================================================================================
};
}