#ifndef __solution_h__
#define __solution_h__

#include "SolutionData.h"
#include "Route.h"
namespace VRPTW
{
class MATHLIB_API CSolution
{
	CSolutionData& data;
	vector<CRoute*> routes;
public:
	CSolution(CSolution const &copy);
	CSolution(CSolutionData& s);
	~CSolution(void);
	int SolveSolomon();
	double GetWholeCost();
	CSolutionData& GetSolutionData();
	vector<CRoute*>& GetRoutes();
	bool IsValidRoutes();

};
}

#endif