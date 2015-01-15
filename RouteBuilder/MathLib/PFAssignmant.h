#pragma once
#include "DynamicAssignment.h"

class MATHLIB_API CPFAssignmant
{
	int**C;
	int n;
	double *u,*v;
	int **X;
	int CMax;
	double eps;
	int iter;
public:
	CPFAssignmant(int ** C, int n);
	~CPFAssignmant(void);
	void Print()
	{
		cout << "Result:\n";
		int sum = 0;
		for(int i=0;i< n; i++)
		{
			for(int j=0;j< n; j++)
				if(X[i][j]==1)
				{
					cout << i+1 << "->" << j+1 << "\n";
					sum  += C[i][j];
 				}
		}
		cout << "sum = " << sum << "\n"; 
	}
	double GetSum()
	{
		int sum = 0;
		for(int i=0;i< n; i++)
		{
			for(int j=0;j< n; j++)
				if(X[i][j]==1)
				{
					sum  += C[i][j];
 				}
		}
		return sum;
	}
	bool IsSumRowNull(int k)
	{
		for(int j=0;j<n;j++)
			if(X[k][j]==1)
					return false;
		return true;
	}
	bool IsSumColumnBiggerOne(int col)
	{
		short count =0; 
		for(int i=0;i<n;i++)
        {
			count +=X[i][col];
			if(count>1)
				return true;
		}
		return false;
	}
	void PushRelabel()
	{
				for(int k=0;k<n;k++)
				{
					if(IsSumRowNull(k)==true)
					{
						double min = 2*CMax;
						int ind = -1;
						for(int j=0;j<n;j++)
						{
							if(X[k][j]==0&&C[k][j]!=-1&&(min>C[k][j] - v[j]))
							{
								min = C[k][j] - v[j];
								ind = j;
							}
							iter++;
						}
						u[k] = min;
						X[k][ind] = 1;
					}
				}
				//Print();
				for(int k=0;k<n;k++)
				{
					if(IsSumColumnBiggerOne(k)==true)
					//if(GtXSumColumn(k)>1)
					{
						double min = 2*CMax;
						int ind = -1;
						for(int i=0;i<n;i++)
						{
							if(X[i][k]==1&&C[i][k]!=-1&&(min>C[i][k] - u[i]))
							{
								min = C[i][k] - u[i];
								ind = i;
							}
							X[i][k]=0;
							iter++;
						}
						v[k] = min - eps;
						X[ind][k] = 1;
					}
				}
				//Print();
	}

	void DoublePush()
	{
		for(int k=0;k<n;k++)
		{
			int j,z;
		    int ind = -1;
			double cp = 2*CMax,cp2 = 2*CMax;
			for(int s=0;s<n;s++)
			{
				if(C[k][s]!=-1&&(cp>C[k][s] - v[s]+u[k]))
				{
					cp = C[k][s] - v[s]+u[k];
					ind = s;
				}
			}
			j = ind;
			ind = -1;
			for(int s=0;s<n;s++)
			{
				if(C[k][s]!=-1&&(cp2>C[k][s] - v[s]+u[k])&&(cp<C[k][s] - v[s]+u[k]))
				{
					cp2 = C[k][s] - v[s]+u[k];
					ind = s;
				}
			}
			if(ind!=-1)
			 z = ind;
			else
			 z = j;
			X[k][j] = 1;
			u[k] = C[k][z] - v[z];
			for(int s=0;s<n;s++)
			{
				if(X[s][j]==1&&s!=k)
				{
					X[s][j] = 0;
					v[j] = C[k][j] -u[k] - eps;
				}
			}
		}
		Print();

	}
	vector<CPos> Solve(double alfa)
	{
		vector<CPos> result;
		eps = CMax;
		iter = 0;
		for(int i=0;i<n;i++)
		{
			iter++;
			u[i] = v[i] = 0;
		}
			while(eps>(1.0/(n*1.0)))	
		{
			eps = eps / alfa;
			for(int i=0;i<n;i++)
				for(int j=0;j<n;j++)
					X[i][j] = 0;
			for(int i=0;i<n;i++)
			{
				double min = CMax;
				for(int j=0;j<n;j++)
					if(C[i][j]!=-1&&(min>C[i][j] - v[j]))
					{
						iter++;
						min = C[i][j] - v[j];
					}
				u[i] = min;
			}
			while(!IsFeasible())
			{
				//cout << "*************\n";
				PushRelabel();
				//DoublePush();
			}
		}
			
			//cout << iter << "\n";
			//cout << "*******\n";
		return result;
	}



	int GtXSumRow(int row)
	{
		int count =0; 
		for(int j=0;j<n;j++)
			count +=X[row][j];
		return count;
	}
	int GtXSumColumn(int col)
	{
		int count =0; 
		for(int i=0;i<n;i++)
			count +=X[i][col];
		return count;
	}
	bool IsFeasible()
	{
			for(int i=0;i<n;i++)
			{
				if(GtXSumRow(i)!=1)
					return false;
				if(GtXSumColumn(i)!=1)
					return false;
			}
			return true;
	}

};

