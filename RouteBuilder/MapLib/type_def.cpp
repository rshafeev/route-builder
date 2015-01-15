#include "stdafx.h"
#include "type_def.h"
CRsmapFileOptions::CRsmapFileOptions()
{

}
CRsmapFileOptions::~CRsmapFileOptions()
{

}

CMapException::CMapException()
{

}
CMapException::~CMapException()
{

}
char* CMapException::GetErrorString()
{
	return this->error_str;
}
char* CMapException::GetdetailsText()
{
	return this->details;
}
