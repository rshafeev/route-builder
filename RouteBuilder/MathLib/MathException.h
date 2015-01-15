#pragma once
#include "MathLib.h"

class   CMathException
{
protected:
	string error_str;
	string details;
	int errorCode;
    CMathException* ParentException;  // если исключение вызванео в исключении, то это может быть указатель на то исключение, которое породило текущее
public:
	CMathException(int errorCode,string details = "",CMathException* ParentException = NULL); // 
	~CMathException(void);
	int GetErrorCode(); // возвращает код ошибки
	string GetErrorString(); // возвращает информацию об ошибке
	string GetDetailsText(); // возвращает доп. информацию об ошибке (чаще всего название класса и функции, где возникла ошибка.)
};

