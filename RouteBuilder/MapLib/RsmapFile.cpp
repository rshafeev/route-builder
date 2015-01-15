#include "StdAfx.h"
#include "RsmapFile.h"
#include "CompressData.h"


CRsmapFileException::CRsmapFileException(_rsMapFileExeptEnum error_code,char* details)
{
	this->error = error_code;
	this->details = details;
	//
	switch(this->error)
	{
	case _fopenError: 
		error_str = "ошибка открытия файла"; 
		break;
	case _freadError:
		error_str = "ошибка чтения файла"; 
		break;
	case _unknownVersionError:
		error_str = "неизвестная версия файла. ";
	default:
		error_str = "неизвестная ошибка"; 
		break;
	}
}
int CRsmapFileException::GetErrorCode()
{
	return error;
}
//==============================================================================================================

CRsmapFile::CRsmapFile(void)
{
	Isopen = false;
}


CRsmapFile::~CRsmapFile(void)
{
	Close();
}
bool CRsmapFile::Open(char* FileName)
{
	CompressData compress;
	this->FileName = new char[strlen(FileName)+1];
	memcpy(this->FileName,FileName,strlen(FileName));
	 this->FileName[strlen(FileName)] = NULL;
// откроем файл и прочитаем главный заголовок
	file  = fopen(FileName,"rb+");
	if(file==NULL)
		throw new CRsmapFileException(CRsmapFileException::_fopenError," CRsmapFile::Open() : невозможно создать объект FILE");
	size_t readBytesCount; //кол-во прочитанных байтов с файла
	readBytesCount = fread(&headersize,1,sizeof(unsigned int),file);
	if(readBytesCount<=0||headersize<=0)
		throw new CRsmapFileException(CRsmapFileException::_freadError, " CRsmapFile::Open() : невозможно прочитать размер заголовока файла в  int readBytesCount" );
	readBytesCount = fread(&mainHeader,headersize,1,file);
	if(readBytesCount!=1)
		throw new CRsmapFileException(CRsmapFileException::_freadError, " CRsmapFile::Open() : невозможно прочитать заголовок файла в  THeader mainHeader" );
// определим тип сжатия, используемый в файле
	if(mainHeader.version==1)
	{
		switch(mainHeader.dataCompress)
		{
			case 1 : dataCompress = _deflateCompress;break;
			case 2 : dataCompress = _noCompress;break;
		}
		switch(mainHeader.graphCompress)
		{
			case 1 : imgCompress = _deflateCompress;break;
			case 2 : imgCompress = _noCompress;break;
		}
	}
	else
	{
		throw new CRsmapFileException(CRsmapFileException::_unknownVersionError, " CRsmapFile::Open() : неподдерживаемая версия файла (>1.0)" );
	}
// прочитаем слои	
	layers = new TLayer[mainHeader.layerCount];
	readBytesCount = fread(layers,mainHeader.layerStructSize,mainHeader.layerCount,file);
	if(readBytesCount!=mainHeader.layerCount)
		throw new CRsmapFileException(CRsmapFileException::_fopenError, " CRsmapFile::Open() : невозможно прочитать заголовоки слоев  в  TLayer *layers" );
// прочитаем хэш-таблицы для слоев
	HashTables = new THashElem*[mainHeader.layerCount];
	compress.SetCompressAlg(dataCompress);
	
	unsigned char *mas = new unsigned char[15360];
	for(int i=0;i<mainHeader.layerCount;i++)
	{
		int res = fseek(file,layers[i].HashTableSeek,SEEK_SET);
		unsigned int destSize = 0;
		long pos1 = ftell (file);
		if(res!=NULL)
			throw new CRsmapFileException(CRsmapFileException::_fopenError, " CRsmapFile::Open() : невозможно найти указатель на хэш-таблицу в файле" );
		compress.SetBuffers(layers[i].HashTableSize,mainHeader.hashElemStructSize*layers[i].xTileCount*layers[i].yTileCount);
		readBytesCount = fread(compress.GetSrcData(),1,layers[i].HashTableSize,file);
		long pos2 = ftell (file);
		/*if(readBytesCount!=layers[i].HashTableSize)
			throw new CRsmapFileException(CRsmapFileException::_fopenError, " CRsmapFile::Open() : невозможно прочитать хэш-таблицу" );
		*/
		compress.DeCompress(destSize);
		if(destSize!=mainHeader.hashElemStructSize *layers[i].xTileCount*layers[i].yTileCount)
		{
			throw new CRsmapFileException(CRsmapFileException::_fopenError, " CRsmapFile::Open() : невозможно провести декомпрессию хэш-таблицы" );
		}
		else
		{
			HashTables[i] = new THashElem[layers[i].xTileCount*layers[i].yTileCount];
			memcpy(HashTables[i],compress.GetDestData(),destSize);
		}
		break;
	}
	


// прочитаем палитры 
	Isopen = true;
	
	unsigned int TileSize = layers[0].ChanelCount*layers[0].wTile*layers[0].hTile;
	deCompressMainLayerObj.SetBuffers(TileSize,TileSize);
	deCompressMainLayerObj.SetCompressAlg(imgCompress);
// прочитаем привязку
	bool result;
	result = ReadLinkageData();

	return result;
}
bool CRsmapFile::ReadLinkageData()
{
	if(mainHeader.LinkageSeek> 0)
	{
		//linkageHeader;
		if(mainHeader.LinkageElemSructSize != sizeof(TLinkageElem)||
			mainHeader.LinkageSructSize != sizeof(TLinkage))
			return false;
		fseek(file,mainHeader.LinkageSeek,SEEK_SET);
		fread(&linkageHeader,mainHeader.LinkageSructSize,1,file);

		if(linkageHeader.LinkagePointsCount<=0||linkageHeader.LinkagePointsCount>10000)
			return false;
		
			linkagePoints = new CLinkagePoint[linkageHeader.LinkagePointsCount];	
			int fcount = fread(linkagePoints,sizeof(TLinkageElem),linkageHeader.LinkagePointsCount,file);
			if(fcount!=linkageHeader.LinkagePointsCount)
				return false;

		tagRECT MapRect;
		MapRect.left   = MapRect.top = 0;
		MapRect.right  = layers[0].w;
		MapRect.bottom = layers[0].h;
		linkageManager.CreateObj(MapRect);
		for(int i=0;i<linkageHeader.LinkagePointsCount;i++)
			linkageManager.AddPoint(linkagePoints[i]);
		linkageManager.GenerateKoefPriv();
		
		

	}
	else
	{
		linkageHeader.LinkagePointsCount = 0;
		linkageHeader.LinkagePointsSize = 0;
		linkagePoints = NULL;
	}
	return true;
}
void  CRsmapFile::ConvertMapToGeoCord(tagPOINT Point,double *GeoDest,double *GeoLon)
{
	if(linkageManager.IsBinding())
	linkageManager.ConvertMapToGeoCord(Point,GeoDest,GeoLon);
}
tagPOINT CRsmapFile::ConvertGeoToMapCord(double GeoDest,double GeoLon)
{
	if(linkageManager.IsBinding())
		return linkageManager.ConvertGeoToMapCord(GeoDest,GeoLon);
	return tagPOINT();
}

bool  CRsmapFile::RewriteLinkage(CLinkagePoint* points,int count)
{
	if(!IsOpen()) 
		return false;
	//if(mainHeader.LinkageSeek==0)
	{
		int headerSize = sizeof(mainHeader);
		if(this->headersize!=headerSize) return false;
		// привязка в файле еще не выполнена, поэтому будем записывать в конец файла
		fseek(file,0,SEEK_END);
		TLinkage linkageHeader;
		int pos = ftell(file);
		mainHeader.LinkageElemSructSize = sizeof(TLinkageElem);
		mainHeader.LinkageSructSize  = sizeof(TLinkage);
		mainHeader.LinkageSeek = pos;


		linkageHeader.LinkagePointsCount = count;
		linkageHeader.LinkagePointsSize = sizeof(TLinkage)*count;
		int wrcount;
		
		wrcount = fwrite(&linkageHeader,sizeof(TLinkage),1,file);
		if(wrcount!=1)      return false;
		
		wrcount = fwrite(points,sizeof(TLinkageElem),count,file);
		if(wrcount!=count) 	return false;

		fseek(file,0,SEEK_SET);
		
		wrcount = fwrite(&headerSize,sizeof(headerSize),1,file);
		if(wrcount!=1) return false;
		wrcount = fwrite(&mainHeader,sizeof(THeader),1,file);
		if(wrcount!=1) return false;

	}
	//else
	{
		// 
	}
	return true;
}
bool  CRsmapFile::GetTile(int layer,int x, int y,unsigned char** data)
{
	
	if(layer>=mainHeader.layerCount)
	 return false;
		int	ind = x* layers[layer].yTileCount+y;
	if(ind>= layers[layer].xTileCount*layers[layer].yTileCount)
		return false;
	int pos  = HashTables[layer][ind].pos;

	unsigned int TileSize = layers[layer].ChanelCount*layers[layer].wTile*layers[layer].hTile;
	if(layer==0)
	{
		//deCompressMainLayerObj.destBuf = data;
		deCompressMainLayerObj.ImoportDestBuf(data,TileSize,false);
		deCompressMainLayerObj.srcSize = HashTables[layer][ind].size;
	}
	else
		deCompressMainLayerObj.SetBuffers(TileSize,TileSize);
	if(file!=NULL)
	{
		fseek(file,pos,SEEK_SET);//!!!!!!!!!
		int read_count = fread(deCompressMainLayerObj.GetSrcData(),HashTables[layer][ind].size,1,file);
		if(read_count<1) return false;
		deCompressMainLayerObj.DeCompress(HashTables[layer][ind].size,TileSize);
	}
	//*data = deCompressMainLayerObj.GetDestData();
	return true;
}

void CRsmapFile::Close()
{
	if(Isopen==true)
		fclose(file);
	Isopen = false;
}


bool CRsmapFile::IsOpen()
{
	return Isopen;
}

TLayer CRsmapFile::GetLayerInfo(int layer)
{
	return layers[layer];
}


