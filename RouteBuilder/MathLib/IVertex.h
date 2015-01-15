#pragma once
#include "MathLib.h"
namespace VRPTW
{
class  CRelation;
class  CSolutionData;
class  MATHLIB_API IVertex  
{
private:
	void SetType(_VertexType type);
protected:

	CSolutionData &solutionData;
	_VertexType type;
    int input,output;
	int real_tA; // время прихода АТС (заявка начинает выполняться, АТС занято)
	int real_tB; // время освобождения АТС (заявка выполнена, АТС освобиждается)	
public:
	vector<CRelation*> allInputRelations;
	vector<CRelation*> allOutputRelations;
	IVertex(_VertexType _type,CSolutionData &sData);
	virtual ~IVertex(void);
	void Initialize(vector<CRelation*> allInputRelations,vector<CRelation*> allOutputRelations);
	vector<CRelation*>& GetAllInputRelations();
	vector<CRelation*>& GetAllOutputRelations();
	_VertexType GetType();
	void PushInput();
	void PopInput();
	void PushOutput();
	void PopOutput();

	int GetInput();
	int GetOutput();
	int GetReal_tA();
	int GetReal_tB();
	CRelation* GetRelationByEndVertex(IVertex* endVertex,int bizyType); // вернуть дугу с узлами(this, endVertex), bizyType =(0 - only free, 1 - only bizy, 2 - all )
	CRelation* GetRelationByBeginVertex(IVertex* beginVertex,int bizyType); // вернуть дугу с узлами(beginVertex, this)

	CSolutionData& GetSolutionData();
};

}

