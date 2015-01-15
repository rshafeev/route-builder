#include "StdAfx.h"
#include "SolveVRPTWException.h"


CSolveVRPTWException::CSolveVRPTWException(_SVRPTWexc code,string  details)
	:CMathException(code,details)
{
	switch(code)
	{
		case c_iterator : this->error_str = "������ � ������ ���������."; break;
		case c_RelationNotFound : error_str = "�� ������� ����, ��� �������� � ������"; break;
		default: break;
	}
}


CSolveVRPTWException::~CSolveVRPTWException(void)
{
}
