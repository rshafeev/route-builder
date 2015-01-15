#include "StdAfx.h"
#include "Graph.h"


CBipartiteGraph::CBipartiteGraph(void)
{
	Cmax = 0;
}


CBipartiteGraph::~CBipartiteGraph(void)
{
}
void CBipartiteGraph::AddEdge(int ID1,int ID2,int cost)
{
	bool IsX=false,IsY=false;
	int n = X.size();
	int m = Y.size();
	CVertex *vx,*vy;
	for(int i=0;i<n;i++)
	{
		if(X[i]->ID==ID2)
			return;
		if(X[i]->ID==ID1)
		{
			vx = X[i];
			IsX = true;
		}
	}
	for(int i=0;i<m;i++)
	{
		if(Y[i]->ID==ID2)
		{
			IsY = true;
			vy = Y[i];
			break;
		}
	}
	if(IsX==false)
	{
		CVertex* vertex1 = new CVertex;
		vertex1->ID = ID1;
		vertex1->d = 1;
		vertex1->p = 0;
		X.push_back(vertex1);
		vx = vertex1;
	}
	if(IsY==false)
	{
		CVertex *vertex2 = new CVertex;
		vertex2->ID = ID2;
		vertex2->d = -1;
		vertex2->p = 0;
		Y.push_back(vertex2);
		vy = vertex2;
	}
	/*if(IsX==true&&IsY==true)
		return;*/
	CArc *arc1 = new CArc;
	CArc *arc2 = new CArc;

	arc1->Cost = (cost);
	arc1->v = vx;
    arc1->w = vy;
	arc1->Flow = 0;

	arc2->Cost = -(cost);
	arc2->v = vy;
    arc2->w = vx;
	arc2->Flow = 0;

	arcs.push_back(arc1);
	arcs.push_back(arc2);

	
	vx->outputArcs.push_back(arc1);
	vx->inputArcs.push_back(arc2);

	vy->outputArcs.push_back(arc2);
	vy->inputArcs.push_back(arc1);

	if(Cmax<cost)
		Cmax = cost;

}


int CVertex::GetEf()
{
		int f1=0;
		int f2=0;
		for(unsigned int i=0;i<inputArcs.size();i++)
			f1+=inputArcs[i]->Flow;
		for(unsigned int i=0;i<outputArcs.size();i++)
			f2+=outputArcs[i]->Flow;
		return (d + f1 - f2);
}


