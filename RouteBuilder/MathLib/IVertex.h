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
	int real_tA; // ����� ������� ��� (������ �������� �����������, ��� ������)
	int real_tB; // ����� ������������ ��� (������ ���������, ��� �������������)	
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
	CRelation* GetRelationByEndVertex(IVertex* endVertex,int bizyType); // ������� ���� � ������(this, endVertex), bizyType =(0 - only free, 1 - only bizy, 2 - all )
	CRelation* GetRelationByBeginVertex(IVertex* beginVertex,int bizyType); // ������� ���� � ������(beginVertex, this)

	CSolutionData& GetSolutionData();
};

}

