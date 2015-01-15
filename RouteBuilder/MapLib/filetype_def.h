#pragma once
#ifdef MAPLIB_EXPORTS
#define MAPLIB_API __declspec(dllexport)
#else
#define MAPLIB_API __declspec(dllimport)
#endif
namespace Rsmap
{
struct MAPLIB_API THeader					// заголовок файла
{
	unsigned char	version;				// версия файла
	unsigned int	mapW;					// ширина карты
	unsigned int	mapH;					// высота карты
	unsigned char	layerCount;				// количество слоев в карте
	unsigned char	graphCompress;			// алгоритм сжатия графических данных
	unsigned char	dataCompress;			// алгоритм сжатия хэш-таблицы
	unsigned int	layerStructSize;		// размер структуры слоя Tlayer в байтах
	unsigned int    hashElemStructSize;     // размер структуры THashElem
    unsigned int    LinkageSructSize;       // размер структуры TLinkage
    unsigned int    LinkageElemSructSize;   // размер структуры TLinkageElem
	unsigned int	LinkageSeek;   			// положение гео-привязки в файле TLinkage(структура (геопривязка) не сжимается!!!, нет смысла, она  небольшая по размерам)
};
struct MAPLIB_API TLayer					// структура слоя 
{
	unsigned int	w;						// ширина слоя
	unsigned int	h;						// высота слоя
	unsigned int	wTile;					// ширина блока слоя
	unsigned int	hTile;					// высота блока слоя
	unsigned int	xTileCount;				// количество блоков слоя по ширине	
	unsigned int	yTileCount;				// количество блоков слоя по высоте
	unsigned int	ImgbitCount;			// глубина цвета(8 или 24 бита)
	unsigned int	PaletteSeek;			// положение палитры в файле
	unsigned int	PaletteColorCount;		// количество цветов палитры
	unsigned int	PaletteSize;			// размер палитры в байтах
	unsigned int	HashTableSeek;			// положение хэш-таблицы в файле
	unsigned int	HashTableSize;			// размер хэш-таблицы в байтах
	unsigned char   ChanelCount;            // количество цветовых каналов
};
struct MAPLIB_API THashElem
{
	unsigned int pos;						// позиция блока в файле
	unsigned int size;						// размер блока
};
struct MAPLIB_API TLinkage
{
	unsigned int    LinkagePointsSize;      // размер массива гео-привязки 
	unsigned int    LinkagePointsCount;     // количество точек гео-привязки
};
struct MAPLIB_API TLinkageElem
{
	TLinkageElem(double X,double Y,double Long,double Lat);
	TLinkageElem();
	~TLinkageElem();
	
	double X,Y;								// координаты на карте
	double Long, Lat;						// долгота , широта
};
const unsigned int  RSMAP_TITLEW = 64;
const unsigned int  RSMAP_TITLEH = 64;
const unsigned int  RSMAP_MINIMAPW = 800;
const unsigned int  RSMAP_MINIMAPH = 600;

}