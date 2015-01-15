#pragma once

#ifdef MATHLIB_EXPORTS
#define MATHLIB_API __declspec(dllexport)
#else
#define MATHLIB_API __declspec(dllimport)
#endif
#include <iostream>
#include  <vector>
using namespace std;

class CPos
{
public:
 int X,Y;
 CPos(int x , int y)
 {
	 X = x;
	 Y = y;
 }

};
class MATHLIB_API CWeightCoef
{
   

public:
	 vector<CPos> pos;
	CWeightCoef()
	{

	}
	CWeightCoef(int value, int i, int j)
	{
		this->value = value;
		this->pos.push_back(CPos(i,j));
	}
	int value;
	void operator=(int value)
	{
		this->value = value;
		
	}
    static CWeightCoef Min(CWeightCoef w1,CWeightCoef w2)
	{
	  if(w1.value < w2.value) return w1;
		return w2;
	}


	static int Min(vector<CWeightCoef> minList)
	{
		int MinValue = minList[0].value;
	    int minInd = 0;
		for(unsigned int i=0;i<minList.size();i++)
		{
			if(MinValue>minList[i].value)
			{
				MinValue=minList[i].value;
				minInd = i;
			}
		}
		return minInd;
	}
	bool operator==(CWeightCoef &w)
	{
		if(w.value == this->value)
	      return true;
	   return false;
	}
	CWeightCoef operator+(CWeightCoef& add)
	{
		CWeightCoef coef;
		coef.pos = this->pos;

		for(unsigned int i=0;i<add.pos.size();i++)
		  coef.pos.push_back(add.pos[i]);
		coef.value = this->value + add.value;
		return coef;
	}

};
class MATHLIB_API CoefMatrix
{
	CWeightCoef** coefs;
	int n,m;
public:
	CoefMatrix(int n,int m)
	{
		coefs = new CWeightCoef*[n];
		for(int i=0;i<n;i++)
		{
			coefs[i] = new CWeightCoef[m];
			for(int j=0;j<m;j++)
			{
				this->coefs[i][j].pos.push_back(CPos(i,j));
			}
		}
        this->n = n;
        this->m = m;
	}
	CWeightCoef& operator()(int i,int j)
	{
		return this->coefs[i][j];
	}
	CoefMatrix(const CoefMatrix  &);
int GetN(){return n;}
int GetM(){return m;}
   
int MinStr(int i, int j1 = 0, int j2 = -1)
{
  if(j2==-1) j2  = n;
CWeightCoef Min = coefs[i][j1];
int minIndex=j1;
  for(int j=j1+1;j<=j2;j++)
{
	if(!(CWeightCoef::Min(Min,coefs[i][j])==Min))
	{
		Min=coefs[i][j];
		minIndex = j;
	}
	

}
  return minIndex;
}

void Print()
{
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<m;j++)
			cout << coefs[i][j].value << " " ;
		cout <<"\n";
	}
	cout << "*********\n";
}
};

class MATHLIB_API CDynamicAssignment
{
	CoefMatrix *C;
	vector<CPos> **F;
int fn,fm;
public:
	
	CDynamicAssignment(CoefMatrix *C)
    {
      this->C = C;
      fn = C->GetN();
      fm= C->GetM();
      F = new vector<CPos> *[fn];
	  for(int i=0;i<fn;i++)
	  {
			F[i] = new vector<CPos>[fm];
	  }
    }
 	
    void InitInstance();
	
    void Solve();
	~CDynamicAssignment(void);
};

