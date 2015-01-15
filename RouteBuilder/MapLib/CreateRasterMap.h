#pragma once
#include "type_def.h"

//===================================================================================================================================================
namespace Rsmap
{
// ����������� ����� ��� �������� rsmap ����� �� ��������� ������
class MAPLIB_API CCreateRasterMap
{
protected:
	 char* sourceFileName;				// ���� � ��������� ������
	 int bandCount;						// ���������� �������� �������
	 CCreateRasterMap(void);
	~CCreateRasterMap(void);
public:
	/// <summary> ������� ����� � �������� ( ������������� ���� �����/����������� </summary>
	/// <param name="FileName"> ���� � �����</param>
    /// <returns>���������� ������ ��� ���������� ����������� �����. NULL - �� ������� ��������� � ���������� ������ � ������ �� ��������</returns>
	static CCreateRasterMap* CreateObject(char* sourceFileName);	

	/// <summary> ������� rsmap ���� </summary>
	/// <param name="rsmapFile"> ������ ���� ������ � ��������� ������������ rsmap-�����</param>
	/// <param name="options">��������� ������������ �����(��� ������, ���-�� ������ � �.�.)</param>
	/// <returns>���������� RSMAP_OK � ������ ��������� ���������� �������</returns>
	virtual int GenRsmapFile(char* rsmapFile,CRsmapFileOptions options)=NULL;

	///<summary> ������� ������ �� ��������.  </summary>
	void Free();
};

//===================================================================================================================================================


//����� ����������, ������� ����� ���������� ��� �������� rsmap �����
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
	virtual int GetErrorCode();						 // ���������� ��� ������

//===================================================================================================================================================
};
}