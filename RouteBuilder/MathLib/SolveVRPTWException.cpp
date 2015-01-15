#include "StdAfx.h"
#include "SolveVRPTWException.h"


CSolveVRPTWException::CSolveVRPTWException(_SVRPTWexc code,string  details)
	:CMathException(code,details)
{
	switch(code)
	{
		case c_iterator : this->error_str = "Ошибка в работе итератора."; break;
		case c_RelationNotFound : error_str = "Не найдена дуга, что повлекло к ошибке"; break;
		default: break;
	}
}


CSolveVRPTWException::~CSolveVRPTWException(void)
{
}
