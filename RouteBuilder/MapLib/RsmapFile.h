#pragma once
#include "type_def.h"
#include "filetype_def.h"
#include "CompressData.h"
#include "..\\MathLib\\Linkage.h"
#include <vector>
using namespace std;
namespace Rsmap
{
// ����� ����������, ������� ����� ���������� ��� �������� rsmap �����(����� CRsmapFile)
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
// ����� ��� ������ rsmap �����
class MAPLIB_API CRsmapFile
{

	char* FileName;										// ���� � �����: ������ �������� ��� ������ �������  Open()
//��������� ������ � ����� ��� ������ ������� Open()
	unsigned int headersize;							// ������ ������� ��������� (�������� � ������ �����)
	THeader mainHeader;									// �������� ���������� � �����					  
	TLayer *layers;										// ���������� � ����� �����
	FILE* file;											// ������ ��� "�������" � ������
	THashElem** HashTables;								// ������ ���-������
	RGBQUAD **palettes;									// ������ ������ ��� ����� palettes[i]  - ������� ��� i-�� ����
	_CompressTypeEnum dataCompress,imgCompress;			// ���� ������ ������ ������

	TLinkage linkageHeader;
	CLinkagePoint*  linkagePoints;                           // ������ ����� ���-��������
// ����������� ������ � �������  Open()
	bool Isopen;										// ������ �� ����?
	CompressData deCompressMainLayerObj;				//  ������ ��� ������������ ��������� ������ �������� ����(layers[0])(���. � ������� ���������� ������, ��������: GetTile() )
    CLinkage linkageManager;                            // ����� ��� �������� �����
// ������� ������ ������
	bool ReadLinkageData();								// ��������� � ����� ������ � �����������
public:
	CRsmapFile(void);
	~CRsmapFile(void);

	/// <summary> ������������ �������� ����� � ���������� ������ � ��������� �����, ��� ���������� � ���-������</summary>
	/// <param name="FileName"> ���� � �����</param>
    /// <returns>true - �������� ������������ �������, ��� ��������� ������ ���������, ������ file ������; false - �������� ����� ��������� �������</returns>
	bool   Open(char* FileName);								
	
	/// <summary> ���������� ������ ��������� ������ ������������ ����� ����� </summary>
	/// <param name="layer">����</param>
	/// <param name="x"> ������������ ����� �� ������</param>
	/// <param name="y"> ������������ ����� �� ������</param>
	/// <param name="data">������, ���� ����� �������� ��������� ������ (������ ��� ������ ���� �������� � ���. ������)</param>
    /// <returns>true - ���������� ����� ������ �������; false - �� ������� ������� ���� </returns>
	bool   GetTile(int layer,int x, int y,unsigned char** data);

	bool   RewriteLinkage(CLinkagePoint* points,int count);       
	void      ConvertMapToGeoCord(tagPOINT Point,double *GeoDest,double *GeoLon);   
    tagPOINT  ConvertGeoToMapCord(double GeoDest,double GeoLon);    

	/// <summary> ��������� ����, ������� ������������ ������, ���������� ��� ������ � ������</summary>
    /// <returns>void</returns>
	void   Close();



	bool IsOpen();
	TLayer GetLayerInfo(int layer);
};







}