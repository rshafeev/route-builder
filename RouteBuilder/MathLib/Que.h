#pragma once
#include "IVertex.h"
#include "MathLib.h"

namespace VRPTW
{
class CSolutionData;
class MATHLIB_API CQue : public IVertex
{
	TQueData& data;
public:
	const int tA;
	const int dtA;
	const int tAB; 
	void SetReal_tA(int time);


	CQue(TQueData& rawdata,CSolutionData& sData);
	~CQue(void);
	TQueData& GetData();
};

}

