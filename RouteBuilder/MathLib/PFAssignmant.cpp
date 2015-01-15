#include "StdAfx.h"
#include "PFAssignmant.h"


CPFAssignmant::CPFAssignmant(int ** C, int n)
{
	this->C = C;
	this->n =n;
	X = new int*[n];
	u = new double[n];
	v = new double[n];
	for(int i=0;i< n; i++)
		X[i] = new int[n];

	CMax = 0;
	for(int i=0;i< n; i++)
		for(int j=0;j< n; j++)
			if(CMax<C[i][j])
				CMax = C[i][j];
	
}


CPFAssignmant::~CPFAssignmant(void)
{
	for(int i=0;i< n; i++)
	{
		delete X[i];
		X[i] = NULL;
	}
	delete[] u;
	delete[] v;
	delete X;
	X = NULL;
	C = NULL;
}
