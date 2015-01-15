#pragma once
#include "CreateRasterMap.h"
#include "filetype_def.h"
#include "Gdal\gdal_priv.h"
namespace Rsmap
{
class CImgMapFile  : CCreateRasterMap
{
private:
	GDALDataset  *poDataset;
    GDALRasterBand  **poBand;
	void GetTile(unsigned int &indX,unsigned int &indY,unsigned int &title_w,unsigned int &title_h,unsigned char* data);
	
	bool   GenMiniMap(int &w, int &h,unsigned char** data);
public:
	CImgMapFile();
	int InitObject(char* sourceFileName);
	virtual int GenRsmapFile(char* rsmapFile,CRsmapFileOptions options);
	~CImgMapFile(void);
};
}
