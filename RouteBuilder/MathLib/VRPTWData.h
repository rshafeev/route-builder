#ifndef __VRPTWData_h__
#define __VRPTWData_h__

#include "MathLib.h"
namespace VRPTW
{
/*
Класс - Исходные данные
*/

class MATHLIB_API CVRPTWData
{
	
//	CVRPTWData(const CVRPTWData&);
public:
	TSettingsSolveVRPTW settings;
	vector<TQueData*> ques;
	vector<TVehicleData*> vehicles;
	vector<TDepotData*> depots;
	vector<TRelationData*> relations;

public:
	CVRPTWData();
	CVRPTWData(CVRPTWData const &copy);
	CVRPTWData(TSettingsSolveVRPTW set);
	~CVRPTWData(void);
	TSettingsSolveVRPTW& GetSettings();

};
}

#endif