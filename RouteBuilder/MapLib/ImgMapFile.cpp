#include "StdAfx.h"
#include "ImgMapFile.h"
#include "CompressData.h"

CImgMapFile::CImgMapFile()
{

}

CImgMapFile::~CImgMapFile(void)
{
	for(int i=0;i<bandCount;i++)
		poBand[i]->FlushCache();
	poDataset->FlushCache();
	GDALClose(poDataset);
}
int CImgMapFile::InitObject(char* sourceFileName)
{

	GDALAllRegister();
	GDALRegister_BMP();
	poDataset = (GDALDataset *) GDALOpen( sourceFileName, GA_ReadOnly );
    if( poDataset == NULL )
		throw new CCreateRasterMapExeption(CCreateRasterMapExeption::_initError ,
										   "CImgMapFile::InitObject() : невозможно создать объект GDALDataset"
										   );
	bandCount = poDataset->GetRasterCount();
	bandCount = min(bandCount,3);
	poBand = new GDALRasterBand*[bandCount];
	for(int i=0;i<bandCount;i++)
		poBand[i] = poDataset->GetRasterBand(i+1);
	return _result_ok;
}
void CImgMapFile::GetTile(unsigned int &indX,unsigned int &indY,unsigned int &title_w,unsigned int &title_h,unsigned char* data)
{
	for(int x=0; x< bandCount; x++)
	{
		poBand[x]->RasterIO(GF_Read,indX*title_w,indY*title_h,title_w,title_h,data+(bandCount-1-x),title_w,title_h,GDT_Byte,bandCount,0);
	}
}
int CImgMapFile::GenRsmapFile(char* rsmapFile,CRsmapFileOptions options)
{
	FILE* f;
	unsigned int headersize = sizeof(THeader);
	THeader header;
	TLayer mainLayer,miniLayer;
	CompressData compress;
	f = fopen(rsmapFile,"wb+");
	if(f==NULL)
		throw new CCreateRasterMapExeption(CCreateRasterMapExeption::_initError, "CImgMapFile::GenRsmapFile() : невозможно создать объект FILE" );
	// заполним заголовок
	header.version				= 1;
	header.mapW					= poDataset->GetRasterXSize();
	header.mapH					= poDataset->GetRasterYSize();
	header.layerCount			= 2;
	header.dataCompress			= options.dataCompress;
	header.graphCompress		= options.imageCompress;
	header.layerStructSize		= sizeof(TLayer);
	header.hashElemStructSize   = sizeof(THashElem);
	header.LinkageElemSructSize = sizeof(TLinkageElem);
	header.LinkageSructSize     = sizeof(TLinkage);
	header.LinkageSeek          = 0;

	fwrite(&headersize,1,sizeof(unsigned int),f);									// записали размер заголовка
	fwrite(&header,1,sizeof(THeader),f);											// записали заголовок
	
	mainLayer.w					= header.mapW;
	mainLayer.h				    = header.mapH;
	mainLayer.wTile				= RSMAP_TITLEW;
	mainLayer.hTile				= RSMAP_TITLEH;
	mainLayer.xTileCount		= header.mapW/RSMAP_TITLEW + (header.mapW%RSMAP_TITLEW==0? 0 : 0);
	mainLayer.yTileCount		= header.mapH/RSMAP_TITLEH + (header.mapH%RSMAP_TITLEH==0? 0 : 0);
	mainLayer.PaletteColorCount = options.PaletteColorCount;
	mainLayer.ImgbitCount		= options.PaletteBitCount;
	mainLayer.ChanelCount       = options.PaletteBitCount/8;

	fwrite(&mainLayer,1,sizeof(TLayer),f);									// записали заголовок слоя карты
	fwrite(&miniLayer,1,sizeof(TLayer),f);									// записали заголовок слоя миникарты

	// запишем  блоки основного слоя растровых данных
	compress.SetCompressAlg(options.imageCompress);
	compress.SetBuffers(mainLayer.wTile*mainLayer.hTile*min(3,bandCount),mainLayer.wTile*mainLayer.hTile*min(3,bandCount));
	unsigned int destSize = mainLayer.wTile*mainLayer.hTile*min(3,bandCount);
	unsigned char* destData;
	long pos;
	THashElem *hashTable = new THashElem[mainLayer.xTileCount*mainLayer.yTileCount];
	for(unsigned int j=0; j < mainLayer.yTileCount;j++)
		for(unsigned int i=0;i < mainLayer.xTileCount;i++)
		{
			GetTile(i,j,mainLayer.wTile,mainLayer.hTile,compress.GetSrcData());	// считали данные
			destData = compress.Compress(destSize);								// провели компрессию данных
			pos = ftell(f);
			fwrite(destData,sizeof(unsigned char),destSize,f);					// записали в файл
			hashTable[i*mainLayer.yTileCount+j].size = destSize;
			hashTable[i*mainLayer.yTileCount+j].pos = pos;

		}
	// запишем блок миникарты
	miniLayer.w					= min(RSMAP_MINIMAPW,header.mapW);
	miniLayer.h				    = min(RSMAP_MINIMAPW,header.mapH);
	miniLayer.wTile				= min(RSMAP_MINIMAPW,header.mapW);
	miniLayer.hTile				= min(RSMAP_MINIMAPW,header.mapH);
	miniLayer.xTileCount		= header.mapW/RSMAP_TITLEW + (header.mapW%RSMAP_TITLEW==0? 0 : 0);
	miniLayer.yTileCount		= header.mapH/RSMAP_TITLEH + (header.mapH%RSMAP_TITLEH==0? 0 : 0);
	miniLayer.PaletteColorCount = options.PaletteColorCount;
	miniLayer.ImgbitCount		= options.PaletteBitCount;
	miniLayer.ChanelCount       = options.PaletteBitCount/8;
	
	//...
	// запишем хэш-таблицу главного слоя 
	unsigned int hashSize;
	compress.SetCompressAlg(options.dataCompress);
	compress.SetBuffers(mainLayer.xTileCount*mainLayer.yTileCount*sizeof(THashElem),mainLayer.xTileCount*mainLayer.yTileCount*sizeof(THashElem));
	memcpy((void*)compress.GetSrcData(),(void*)hashTable,mainLayer.xTileCount*mainLayer.yTileCount*sizeof(THashElem));
	compress.Compress(hashSize);
	pos = ftell (f);
	int res = fwrite(compress.GetDestData(),1,hashSize,f);		
	
	mainLayer.HashTableSeek = pos;
	mainLayer.HashTableSize = hashSize;
	pos = ftell (f);
	ftell (f);


	// обновим заголовки
	fseek(f,0,SEEK_SET);
	fwrite(&headersize,1,sizeof(unsigned int),f);						    // записали размер заголовка
	fwrite(&header,1,sizeof(THeader),f);									// записали заголовок
	fwrite(&mainLayer,1,sizeof(TLayer),f);									// записали заголовок слоя карты
	fwrite(&miniLayer,1,sizeof(TLayer),f);									// записали заголовок слоя миникарты

	fclose(f);
	compress.ClearBuffers();
	
	return _result_ok;
}
bool   CImgMapFile::GenMiniMap(int &w, int &h,unsigned char** data)
{
	return true;
}
