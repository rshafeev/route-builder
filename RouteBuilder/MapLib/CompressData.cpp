#include "StdAfx.h"
#include "CompressData.h"
#include "Gdal\frmts\zlib\zlib.h"

#define RESERVE_BUF 4096
CompressData::CompressData(void)
{
	type = _noCompress;

	srcBuf = new unsigned char*;
	destBuf = new unsigned char*;
	*srcBuf = NULL;
	*destBuf = NULL;

	CompressPt = NoCompress;
	DeCompressPt = NoCompress;

}


CompressData::~CompressData(void)
{

}

void CompressData::ImoportDestBuf(unsigned char** buf,int destBufSize,bool ClearLastImportDestBuf)
{
	if(this->type!=_noCompress)
	{
		if(*destBuf!=NULL&&IsImportDestBuf==false) delete[] *destBuf;
	}
	else
	if(*destBuf!=NULL&&IsImportDestBuf==true&&ClearLastImportDestBuf==true)
	{
		delete[] *destBuf;
	}
	destBuf = buf;
	this->MaxDestSize = destBufSize;
}
void CompressData::SetCompressAlg(_CompressTypeEnum type)
{
	
	if(type==this->type) return;


	if(this->type!=_noCompress)
	{
		if(*destBuf!=NULL) delete[] *destBuf;
		*destBuf = new unsigned char[MaxDestSize+RESERVE_BUF];
	}
	if(type == _deflateCompress)
	{
		CompressPt = CompressDeflate;
		DeCompressPt = DeCompressDeflate;
	}
	else
	{
		CompressPt = NoCompress;
		DeCompressPt = NoCompress;
	}
	this->type = type;
}
_CompressTypeEnum CompressData::GetCompressAlg()
{
	return this->type;
}
unsigned char* CompressData::Compress(unsigned int &destSize)
{
	unsigned int destCount = this->MaxDestSize+RESERVE_BUF;
	CompressPt(srcBuf,srcSize,destBuf,destCount);
	destSize = destCount;
	return *destBuf;
}
unsigned char* CompressData::DeCompress(unsigned int &destSize)
{
	unsigned int destCount = this->MaxDestSize+RESERVE_BUF;
	DeCompressPt(srcBuf,srcSize,destBuf,destCount);
	destSize = destCount;
	return *destBuf;
}
unsigned char* CompressData::DeCompress(unsigned int srcSize,unsigned int &destSize)
{
	if(srcSize>this->srcSize) srcSize>this->srcSize;
	unsigned int destCount = this->MaxDestSize+RESERVE_BUF;
	DeCompressPt(srcBuf,srcSize,destBuf,destCount);
	destSize = destCount;
	return *destBuf;	
}
void CompressData::SetBuffers(int srcSize,int MaxDestSize)
{
	ClearBuffers();
	*srcBuf      = new unsigned char[srcSize];
	if(type!=_noCompress)
		*destBuf = new unsigned char[MaxDestSize+RESERVE_BUF];
	else
		*destBuf = *srcBuf;
	this->srcSize = srcSize;
	this->MaxDestSize = MaxDestSize;
}
void CompressData::ClearBuffers()
{
	/*if(*srcBuf!=NULL)
	{
		delete[] *srcBuf;
		*srcBuf = NULL;
	}
	if((*destBuf!=NULL)&&type!=_CompressTypeEnum::noCompress)
	{
		delete[] *destBuf;
		*destBuf = NULL;
	}*/
}
unsigned char*	  CompressData::GetSrcData()
{
	return *srcBuf;
}
unsigned char*	  CompressData::GetDestData()
{
	return *destBuf;
}
//=====
bool CompressData::NoCompress(unsigned char** srcData,unsigned int srcSize,unsigned char** destData,unsigned int &DestBufSize)
{
	DestBufSize = srcSize;
	*destData = *srcData;
	return true;
}
bool  CompressData::CompressDeflate(unsigned char** srcData,unsigned int srcSize,unsigned char** destData,unsigned int &DestBufSize)
{
	ULONG destLen = DestBufSize;
	int result = compress2(*destData,&destLen,*srcData,srcSize,2);
	DestBufSize = destLen;
	/*z_stream strm;
	int ret, flush =Z_FINISH ; // Z_NO_FLUSH ;
	int level = 1;
	strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

	strm.avail_in  = srcSize;
	strm.next_in   = *srcData;
	strm.avail_out = DestBufSize;
    strm.next_out  = *destData;

	ret = deflateInit(&strm, level);
	ret = deflate(&strm, flush);   
	DestBufSize = strm.total_out;
	(void)deflateEnd(&strm);*/
	return true;
}
bool CompressData::DeCompressDeflate(unsigned char** srcData,unsigned int srcSize,unsigned char** destData,unsigned int &DestBufSize)
{
	ULONG destLen;
	destLen = DestBufSize;

	uncompress(*destData,&destLen,*srcData,srcSize);
	DestBufSize = destLen;
	
	/*z_stream strm;
	int ret, flush =Z_NO_FLUSH; // Z_NO_FLUSH ;
	strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

	strm.avail_in  = srcSize;
	strm.next_in   = *srcData;
	strm.avail_out = DestBufSize;
    strm.next_out  = *destData;

	ret = inflateInit(&strm);
	ret = inflate(&strm, flush);
	DestBufSize = strm.total_out;
	(void)deflateEnd(&strm);*/
	return true;
}
