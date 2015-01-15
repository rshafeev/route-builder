#include "stdafx.h"
#include "MathException.h"

CMathException::CMathException(int errorCode,string details,CMathException* ParentException)
{
	this->ParentException = ParentException;
	this->errorCode = errorCode;
	this->details = details;

}

CMathException::~CMathException(void)
{
}
int CMathException::GetErrorCode()
{
	return errorCode;
}
string CMathException::GetErrorString()
{
	return this->error_str;
}
string CMathException::GetDetailsText()
{
	return this->details;
}