#pragma once
#include "type_def.h"
#include "filetype_def.h"
#include "CompressData.h"
#include "..\\MathLib\\Linkage.h"
#include <vector>
using namespace std;
namespace Rsmap
{
// класс исключений, которые могут возникнуть при открытии rsmap файла(класс CRsmapFile)
class MAPLIB_API CRsmapFileException : public CMapException
{
public:
enum MAPLIB_API _rsMapFileExeptEnum 
{
	_fopenError			 = 0,
	_freadError		 	 = 1,
	_unknownVersionError = 2
};
	_rsMapFileExeptEnum error;
public:
	CRsmapFileException(_rsMapFileExeptEnum error_code,char* details);
	virtual int GetErrorCode();
};
// класс для чтения rsmap файла
class MAPLIB_API CRsmapFile
{

	char* FileName;										// путь к файлу: строка получена при вызове функции  Open()
//считанные данные с файла при вызове функции Open()
	unsigned int headersize;							// размер главной структуры (хранится в начале файла)
	THeader mainHeader;									// основная информация о карте					  
	TLayer *layers;										// информация о слоях файла
	FILE* file;											// объект для "общения" с файлом
	THashElem** HashTables;								// массив хэш-таблиц
	RGBQUAD **palettes;									// массив палитр для слоев palettes[i]  - палитра для i-го слоя
	_CompressTypeEnum dataCompress,imgCompress;			// типы сжатия данных файлов

	TLinkage linkageHeader;
	CLinkagePoint*  linkagePoints;                           // массив точек гео-привязки
// вычисленные данные в функции  Open()
	bool Isopen;										// открыт ли файл?
	CompressData deCompressMainLayerObj;				//  объект для декомпрессии растровых данных главного слоя(layers[0])(исп. в функции считывания блоков, например: GetTile() )
    CLinkage linkageManager;                            // класс для привязки карты
// функции чтения данных
	bool ReadLinkageData();								// считывает с файла данные о геопривязке
public:
	CRsmapFile(void);
	~CRsmapFile(void);

	/// <summary> осуществляет открытие карты и считывание данных о структуры карты, его индексации и хэш-данные</summary>
	/// <param name="FileName"> путь к файлу</param>
    /// <returns>true - открытие осуществлено успешно, все структуры данных заполнены, объект file создан; false - открытие файла потерпело неудачу</returns>
	bool   Open(char* FileName);								
	
	/// <summary> возвращает массив растровых данных запрошенного блока карты </summary>
	/// <param name="layer">слой</param>
	/// <param name="x"> расположение блока по ширине</param>
	/// <param name="y"> расположение блока по высоте</param>
	/// <param name="data">массив, куда будут записаны растровые данные (массив уже должен быть размещен в дин. памяти)</param>
    /// <returns>true - считывание блока прошло успешно; false - не удалось считать блок </returns>
	bool   GetTile(int layer,int x, int y,unsigned char** data);

	bool   RewriteLinkage(CLinkagePoint* points,int count);       
	void      ConvertMapToGeoCord(tagPOINT Point,double *GeoDest,double *GeoLon);   
    tagPOINT  ConvertGeoToMapCord(double GeoDest,double GeoLon);    

	/// <summary> закрывает файл, очищает динамическую память, выделенную для работы с картой</summary>
    /// <returns>void</returns>
	void   Close();



	bool IsOpen();
	TLayer GetLayerInfo(int layer);
};







}