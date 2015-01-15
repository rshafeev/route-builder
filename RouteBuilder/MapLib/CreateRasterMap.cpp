#include "StdAfx.h"
#include "CreateRasterMap.h"
#include "ImgMapFile.h"

CCreateRasterMap::CCreateRasterMap(void)
{
}


CCreateRasterMap::~CCreateRasterMap(void)
{
}
CCreateRasterMap* CCreateRasterMap::CreateObject(char* sourceFileName)
{
	CImgMapFile* mapFile = new CImgMapFile();
	mapFile->InitObject(sourceFileName);
	return (CCreateRasterMap*)mapFile;
}
void CCreateRasterMap::Free()
{
	delete this;
}


//=====================================================================================================================================

CCreateRasterMapExeption::CCreateRasterMapExeption(_createRsMapExeptEnum error_code,char* details)
{
	this->error = error_code;
	this->details = details;
	//
	switch(this->error)
	{
	case _createRsMapExeptEnum::_initError: 
		error_str = "ошибка инициализации"; 
		break;
	default:
		error_str = "неизвестная ошибка"; 
		break;
	}
}
int CCreateRasterMapExeption::GetErrorCode()
{
	return error;
}
