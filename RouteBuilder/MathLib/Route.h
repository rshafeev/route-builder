
#ifndef __route_h__
#define __route_h__

//#pragma once
#include "MathLib.h"
#include "Relation.h"
#include "Vehicle.h"
#include <list>
namespace VRPTW
{

class MATHLIB_API CRoute
{
CVehicle& vehicle;
list<CRelation*> relations;
public:
	int freightSize; // необходимый груз для выполнения маршрута

	CRoute( CRoute const &);
	CRoute(CVehicle& v);
	~CRoute(void);
	double GetCost();
	bool IsValid(); 

	list<CRelation*>& GetAllRelations();

	CVehicle& GetVehicle();
	bool InsertAfter(list<CRelation*>::iterator ind, CRelation* value);
	bool DeleteAfter(list<CRelation*>::iterator ind, int count = 1);
	bool InsertAfter(CRelation* after, CRelation* value);
	bool DeleteAfter(CRelation* after, int count = 1);
	bool InsertRelationToEnd(CRelation* relation);
	
	bool Replace(CRelation* Insert_r1,CRelation* Insert_r2,CRelation* Delete_r); // вместо (..->Delete_r->..) вставить  (...->Insert_r1 -> Insert_r2->..)
	bool Replace(CRelation* Insert_r1,CRelation* Insert_r2,list<CRelation*>::iterator Delete_r);
};

}

#endif