#pragma once
#include "MathLib.h"

namespace VRPTW
{
class MATHLIB_API IViewData
{

public:
	virtual void OnAddLine(int id,double x1, double y1, double x2, double y2) = NULL;
	virtual void OnAddVertex(int id,double x1, double y1, double x2, double y2) = NULL;
	virtual void OnDeleteLine(int id) = NULL;
	virtual void OnDeleteVertex(int id) = NULL;
	virtual void OnClearVertex() = NULL;
	virtual void OnClearLine() = NULL;

	IViewData(void);
	~IViewData(void);
};

}

