#pragma once
#include "MathLib.h"
#include "VRPTWData.h"
#include "Vehicle.h"
#include "Depot.h"
#include "Que.h"
namespace VRPTW
{
class MATHLIB_API CSolutionData
{
	CVRPTWData& data;
	vector<CVehicle*> vehicles;
	vector<CDepot*> depots;
	vector<CQue*> ques;
	vector<IVertex*> AllVertexes;
	vector<CRelation*> relations;
public:
    bool IsQuesEmpty(); // есть ли необслуженные заявки?
	CSolutionData(CVRPTWData& rawdata);
	~CSolutionData(void);
	IVertex* GetVertex(int id, _VertexType type); // NULL - не найдено	
	CVRPTWData& GetData();

	vector<CVehicle*>& GetVehicles();
	vector<CDepot*>& GetDepots();
	vector<CQue*>& GetQues();
	vector<IVertex*>& GetAllVertexes();
	vector<CRelation*>& GetRelations();

	CRelation* GetRelation(IVertex* vBegin, IVertex* vEnd);
};
}

