#pragma once
#include <vector>
#include <iostream>
using namespace std;

#ifdef MATHLIB_EXPORTS
#define MATHLIB_API __declspec(dllexport)
#else
#define MATHLIB_API __declspec(dllimport)
#endif

class CArc;
class MATHLIB_API CVertex
{
public:
	CVertex()
{
muVert = NULL;
}
	int ID;
	double p;
	int d;
    
	vector<CArc*> inputArcs;
	vector<CArc*> outputArcs;
 	CVertex* muVert;
    int GetEf();
};
class MATHLIB_API CArc
{
public:
	CVertex*v,*w;
	int Cost;
	int Flow;

	double GetCp()
	{
		return Cost + v->p - w->p;
	}
	double GetCpEx()
	{
		return Cost  - w->p;
	}


};
class MATHLIB_API CBipartiteGraph
{
public:
	vector<CVertex*> X,Y;
	vector<CArc*> arcs;
	int Cmax;


	void AddEdge(int ID1,int ID2,int cost);

	bool IsFlow()
	{
		for(unsigned int i=0;i<X.size();i++)
			if(X[i]->GetEf()!=0)
				return false;
		for(unsigned int i=0;i<Y.size();i++)
			if(Y[i]->GetEf()!=0)
				return false;
		return true;
	}
	int GetN()
	{
		return X.size() + Y.size();
	}
	CArc* Arc(CVertex* v ,CVertex* w)
	{
		for(unsigned int i=0;i< v->outputArcs.size();i++)
		{
			if(v->outputArcs[i]->w == w)
				return v->outputArcs[i];
		}
		return NULL;
	}
void PrintVertex()
{
cout << "Vertex X: \n";
	for(unsigned int i=0;i<X.size();i++)
	{
		
		cout << X[i]->ID << ": d = " << X[i]->d << ", p = " << X[i]->p << ", ef = " <<X[i]->GetEf()<< ";\n";
	}
cout << "Vertex Y: \n";
	for(unsigned int i=0;i<Y.size();i++)
	{
		cout << Y[i]->ID << ": d = " << Y[i]->d << ", p = " << Y[i]->p << ", ef = " <<Y[i]->GetEf()<< ";\n";
	}
}
void PrintArcs()
{
  cout << "Arcs:\n";
	for(unsigned int i=0;i<arcs.size();i++)
	{
		cout <<"(" <<arcs[i]->v->ID <<"," << arcs[i]->w->ID <<") : f="<< arcs[i]->Flow << "; Cp= "<< arcs[i]->GetCp()<<";\n";
	}
}
	CBipartiteGraph(void);
	~CBipartiteGraph(void);
};

