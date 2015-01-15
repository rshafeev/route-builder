#pragma once
#include "Solution.h"
namespace VRPTW
{
class CAlgSolomon
{
	int Solve(CSolution& solution );
public:
	CAlgSolomon(void);
	~CAlgSolomon(void);
	CSolution* Solve(CSolutionData& solutionData);
};
}

