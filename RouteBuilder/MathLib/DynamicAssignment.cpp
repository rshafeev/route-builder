#include "StdAfx.h"
#include "DynamicAssignment.h"


CoefMatrix::CoefMatrix(const CoefMatrix  &c)
{
	 this->n = c.n;
        this->m = c.m;
		coefs = new CWeightCoef*[n];
		for(int i=0;i<n;i++)
		{
			coefs[i] = new CWeightCoef[m];
			for(int j=0;j<m;j++)
			{
				coefs[i][j] = c.coefs[i][j];
			}
		}
}
CDynamicAssignment::~CDynamicAssignment(void)
{
}


void CDynamicAssignment::Solve()
{
	CoefMatrix Fmatrix(*C);
	
	vector<CWeightCoef> solutionList;
	CoefMatrix CurrentC(*C);
	vector<CWeightCoef> tempV;
	for(int u=0;u<fm;u++)
	{
		CurrentC.Print();
    // STEP1		
		for(int j=0;j<fm-fn+1;j++)
		{
			int minInd =  CurrentC.MinStr(0,0,j);
			Fmatrix(0,j) =	 CurrentC(0,minInd);
		}
			
		vector<CWeightCoef> minList;
		for(int i=1;i<fn;i++)
	    {
			for(int j=0;j<fm-fn+1;j++)
			{
			    minList.clear();
				for(int k=0;k<=j;k++)
				{
					minList.push_back(CurrentC(i,i+k)+Fmatrix(i-1,k));
				}
			  int minInd = CWeightCoef::Min(minList);
			  Fmatrix(i,j) = minList[minInd];
			}
		}
		solutionList.push_back(Fmatrix(fn-1,fm-fn));
    // STEP2
		tempV.clear();
		for(int i=0;i<fm;i++)
			tempV.push_back(CurrentC(0,i));
		for(int i=0;i<fn-1;i++)
		  for(int j=0;j<fm;j++)
		  {
			  CurrentC(i,j) = CurrentC(i+1,j);
			  CurrentC(i,j).pos[0].X = i;
			  CurrentC(i,j).pos[0].Y = j; 

		  }
		for(int i=0;i<fm;i++)
			CurrentC(fn-1,i) = tempV[i];


/*		
		for(int i=0;i<fn;i++)
			tempV.push_back(CurrentC(i,0));
for(int i=0;i<fn;i++)
			CurrentC(i,fm-1) = tempV[i];
*/
	}
}