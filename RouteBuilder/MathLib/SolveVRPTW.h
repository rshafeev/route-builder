#pragma once
#include "MathLib.h"
#include "SolutionData.h"
#include "Solution.h"
#include "TabuList.h"
namespace VRPTW
{
class MATHLIB_API CSolveVRPTW
{
	CVRPTWData& rowData;
	CSolutionData *solutionData;
	CSolution* currentSolution;
	void InitSolutionData();
public:
	CSolveVRPTW(CVRPTWData &data);
	~CSolveVRPTW(void);
	
	bool AddQue(TQueData que);
	bool AddVehicle(TVehicleData vehicle);
	bool AddDepot(TDepotData depot);
	bool AddRelation(TRelationData relation);	// дуга между машиной и заявкой

	void Initialize();
	CSolution**  Solve();
	CSolution**  GetCurrentSolution();
	static CVRPTWData LoadStandardProblem(string fName);
};

}

