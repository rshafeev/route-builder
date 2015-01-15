#pragma once
#include "type_def.h"

//
namespace Rsmap
{
class CompressData
{
	_CompressTypeEnum type;

	bool IsImportDestBuf;   //находится ли буфер destBuf во вне объекта(выделен в ДП не в объекте CompressData) : задается в функции ImoportDestBuf()
	bool (*CompressPt)(unsigned char** srcData,unsigned int srcSize,
								 unsigned char** destData,unsigned int &DestBufSize);
	bool (*DeCompressPt)(unsigned char** srcData,unsigned int srcSize,
								 unsigned char** destData,unsigned int &DestBufSize);
	static bool CompressDeflate(unsigned char** srcData,unsigned int srcSize,
										  unsigned char** destData,unsigned int &DestBufSize);
	static bool DeCompressDeflate(unsigned char** srcData,unsigned int srcSize,
											unsigned char** destData,unsigned int &DestBufSize);
	static bool NoCompress(unsigned char** srcData,unsigned int srcSize,
									 unsigned char** destData,unsigned int &DestBufSize);

public:
	unsigned char** srcBuf;
	unsigned char** destBuf;

	unsigned int srcSize;
	unsigned int MaxDestSize;


	void SetCompressAlg(_CompressTypeEnum type);
	void SetBuffers(int srcSize,int MaxDestSize);
	void ImoportDestBuf(unsigned char** buf,int destBufSize,bool ClearLastImportDestBuf);
	unsigned char*	  GetSrcData();
	unsigned char*	  GetDestData();

	_CompressTypeEnum GetCompressAlg();
	
	unsigned char* Compress(unsigned int &destSize);
	unsigned char* DeCompress(unsigned int &destSize);
	unsigned char* DeCompress(unsigned int srcSize,unsigned int &destSize);
	void ClearBuffers();

	CompressData(void);
	~CompressData(void);
};

}

