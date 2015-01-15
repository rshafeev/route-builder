#pragma once
#ifdef MAPLIB_EXPORTS
#define MAPLIB_API __declspec(dllexport)
#else
#define MAPLIB_API __declspec(dllimport)
#endif
namespace Rsmap
{
struct MAPLIB_API THeader					// ��������� �����
{
	unsigned char	version;				// ������ �����
	unsigned int	mapW;					// ������ �����
	unsigned int	mapH;					// ������ �����
	unsigned char	layerCount;				// ���������� ����� � �����
	unsigned char	graphCompress;			// �������� ������ ����������� ������
	unsigned char	dataCompress;			// �������� ������ ���-�������
	unsigned int	layerStructSize;		// ������ ��������� ���� Tlayer � ������
	unsigned int    hashElemStructSize;     // ������ ��������� THashElem
    unsigned int    LinkageSructSize;       // ������ ��������� TLinkage
    unsigned int    LinkageElemSructSize;   // ������ ��������� TLinkageElem
	unsigned int	LinkageSeek;   			// ��������� ���-�������� � ����� TLinkage(��������� (�����������) �� ���������!!!, ��� ������, ���  ��������� �� ��������)
};
struct MAPLIB_API TLayer					// ��������� ���� 
{
	unsigned int	w;						// ������ ����
	unsigned int	h;						// ������ ����
	unsigned int	wTile;					// ������ ����� ����
	unsigned int	hTile;					// ������ ����� ����
	unsigned int	xTileCount;				// ���������� ������ ���� �� ������	
	unsigned int	yTileCount;				// ���������� ������ ���� �� ������
	unsigned int	ImgbitCount;			// ������� �����(8 ��� 24 ����)
	unsigned int	PaletteSeek;			// ��������� ������� � �����
	unsigned int	PaletteColorCount;		// ���������� ������ �������
	unsigned int	PaletteSize;			// ������ ������� � ������
	unsigned int	HashTableSeek;			// ��������� ���-������� � �����
	unsigned int	HashTableSize;			// ������ ���-������� � ������
	unsigned char   ChanelCount;            // ���������� �������� �������
};
struct MAPLIB_API THashElem
{
	unsigned int pos;						// ������� ����� � �����
	unsigned int size;						// ������ �����
};
struct MAPLIB_API TLinkage
{
	unsigned int    LinkagePointsSize;      // ������ ������� ���-�������� 
	unsigned int    LinkagePointsCount;     // ���������� ����� ���-��������
};
struct MAPLIB_API TLinkageElem
{
	TLinkageElem(double X,double Y,double Long,double Lat);
	TLinkageElem();
	~TLinkageElem();
	
	double X,Y;								// ���������� �� �����
	double Long, Lat;						// ������� , ������
};
const unsigned int  RSMAP_TITLEW = 64;
const unsigned int  RSMAP_TITLEH = 64;
const unsigned int  RSMAP_MINIMAPW = 800;
const unsigned int  RSMAP_MINIMAPH = 600;

}