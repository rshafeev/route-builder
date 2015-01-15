#pragma once
#include "MathLib.h"

class   CMathException
{
protected:
	string error_str;
	string details;
	int errorCode;
    CMathException* ParentException;  // ���� ���������� �������� � ����������, �� ��� ����� ���� ��������� �� �� ����������, ������� �������� �������
public:
	CMathException(int errorCode,string details = "",CMathException* ParentException = NULL); // 
	~CMathException(void);
	int GetErrorCode(); // ���������� ��� ������
	string GetErrorString(); // ���������� ���������� �� ������
	string GetDetailsText(); // ���������� ���. ���������� �� ������ (���� ����� �������� ������ � �������, ��� �������� ������.)
};

