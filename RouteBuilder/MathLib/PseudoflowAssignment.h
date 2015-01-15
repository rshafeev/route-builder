#pragma once
#include "Graph.h"
#include <iostream>
class MATHLIB_API CPseudoflowAssignment
{
	CBipartiteGraph G;

	vector<CArc*> Ef;
	int alfa;
	double eps;
public:
	CPseudoflowAssignment(void);
	CPseudoflowAssignment(CBipartiteGraph G)
	{
		this->G = G;
	}

	void Print(bool isFlow=false)
	{
       
		for(unsigned int i=0;i< G.X.size();i++)
		{
			for(unsigned int j=0;j < G.X[i]->outputArcs.size();j++)
			{
				if(isFlow==false||(G.X[i]->outputArcs[j]->Flow==1))
				{
					cout << G.X[i]->ID << ", "<<G.X[i]->outputArcs[j]->w->ID << "\n";
				}
			}

		}
	}
	void Solve()
	{
		alfa = 10;
		eps = G.Cmax;
		int iter =0; 
		while(eps>=1/(G.GetN()*1.0))
		{
			iter++;
			eps = eps / alfa;
			for(unsigned int i=0;i<G.arcs.size();i++)
				G.arcs[i]->Flow  = 0;
			for(unsigned int i=0;i<G.X.size();i++)
			{
				double min  = G.X[i]->outputArcs[0]->Cost - G.X[i]->outputArcs[0]->w->p;
				for(unsigned int j=1;j < G.X[i]->outputArcs.size();j++)
				{
					if(min>G.X[i]->outputArcs[j]->Cost - G.X[i]->outputArcs[j]->w->p)
						min = G.X[i]->outputArcs[j]->Cost - G.X[i]->outputArcs[j]->w->p;
				}
				G.X[i]->p  = min;
			}
			int ind = 0;
			bool f = true;
			while(f)
			{
				ind++;
				if(ind>10)
				{
					ind = ind;
				}
				for(unsigned int i=0;i<G.X.size();i++)
				{
					int count = 0;
				    if(G.IsFlow()==true)
				    {
					  f = false;
					  break;
				    }
					for(unsigned int j=0;j< G.X[i]->outputArcs.size();j++)
					{
						count += G.X[i]->outputArcs[j]->Flow;
					}
					if(cout==0)
						DoublePush(G.X[i],eps);
				}
				/*for(int i=0;i<G.Y.size();i++)
				{
					int count = 0;
				  if(G.IsFlow()==true)
				  {
					  f = false;
					  break;
				  }
				  for(int j=0;j< G.Y[i]->outputArcs.size();j++)
					{
						count += G.Y[i]->inputArcs[j]->Flow;
					}
				 	if(count>1)
				  	  DoublePush(G.Y[i],eps);
				}*/

			}
            //break;
			//cout <<"********\n";
		}
              cout << "*****************************\n";
              cout <<"IsFlow: " << G.IsFlow() << "\n";
			  G.PrintVertex();
              G.PrintArcs();
	  cout << "*************RESULT****************\n";
      Print(true);
	}
	
	void DoublePush(CVertex* v,double eps)
	{
		CVertex *w,*z;
		CArc* vw,*vz;
		int ind=0;
		
		// Step 1. find smallest and second-smallest reduced costs
		double cp=v->outputArcs[0]->GetCp();
		double secondCp=v->outputArcs[0]->GetCp();
		
		w = v->outputArcs[0]->w;
		z = v->outputArcs[0]->w;
		vw = vz = v->outputArcs[0];
		for(unsigned int i=0;i<v->outputArcs.size();i++ )
		{
			if(cp>v->outputArcs[i]->GetCp())
			{
				w  = v->outputArcs[i]->w;
				cp = v->outputArcs[i]->GetCp();
				vw = v->outputArcs[i];
			}
			if(secondCp<v->outputArcs[i]->GetCp())
			{
				z        = v->outputArcs[i]->w;
				vz       = v->outputArcs[i];
				secondCp = v->outputArcs[i]->GetCp();
			}
		}
		for(unsigned int i=0;i<v->outputArcs.size();i++ )
		{
			if(secondCp>v->outputArcs[i]->GetCp()&&v->outputArcs[i]->GetCp()>cp)
			{
				secondCp = v->outputArcs[i]->GetCp();
				z        = v->outputArcs[i]->w;
				vz       = v->outputArcs[i];
			}
		}
		// Step2
		vw->Flow = 1;
		//Push(v,w);
		v->p = vz->GetCpEx();
		for(unsigned int i=0;i<G.X.size();i++)
			if(G.Arc(G.X[i],w)->Flow==1)
			{
				G.Arc(G.X[i],w)->Flow = 0;
				w->p =  vw->Cost -v->p - eps;		
			}
	}
	void Push(CVertex* v1, CVertex* v2)
	{
		if(v1->GetEf()!=0)
		{
			CArc* arc=NULL;
			for(unsigned int i=0;i<v1->outputArcs.size();i++ )
			{
				if(v1->outputArcs[i]->w==v2)
				{
					v1->outputArcs[i]->Flow=1;
					break;
				}
			}
			for(unsigned int i=0;i<v2->outputArcs.size();i++ )
			{
				if(v2->outputArcs[i]->w==v1)
				{
					v2->outputArcs[i]->Flow=0;
					break;
				}
			}
			/*if(arc)
			{
				arc->Flow = 1;
			}*/
		}
	}
	void Relabel(CVertex* v, double eps)
	{
		double max = -100*G.Cmax;
		for(unsigned int j=0;j < G.arcs.size();j++)
		{
			if(G.arcs[j]->Flow==0 && G.arcs[j]->v->d == 1)
				if(max< G.arcs[j]->w->p - G.arcs[j]->Cost)
				max = G.arcs[j]->w->p - G.arcs[j]->Cost;
			else
			if(G.arcs[j]->Flow==1 && G.arcs[j]->v->d == -1)
			   if(max< G.arcs[j]->w->p - G.arcs[j]->Cost)
				   max = G.arcs[j]->w->p - G.arcs[j]->Cost;
		}
		if(v->d == 1)
		v->p = max;
		else
			v->p = max - eps;
	//	v-> p = 
	}




	~CPseudoflowAssignment(void);
};

