#pragma once
#include <list>
using namespace std;

class CWebGrabData 
{

public:
	class CWebGrabDataElem
	{
	  public:
		unsigned char* mas;
		int size;
		CWebGrabDataElem()
		{

		}
		CWebGrabDataElem(unsigned char* mas, int size, bool isMemCpy=true)
		{
			this->size = size;
			if(isMemCpy==false)
				this->mas = mas;
			else
			{
				this->mas = new unsigned char[size];
				memcpy(this->mas,mas,size);
			}
		}
	};
private:
	list<CWebGrabDataElem> mas;
public:
	void AddData(CWebGrabDataElem elem)
	{
		mas.push_back(elem);
	}
	list<CWebGrabDataElem> GetAllData()
	{
		return this->mas;
	}
	CWebGrabDataElem& Combine()
	{
        list<CWebGrabData::CWebGrabDataElem> data = GetAllData();
		int count = 0;
		for(list<CWebGrabData::CWebGrabDataElem>::iterator it =data.begin();	it!=data.end(); it++)
		{
			count+=(*it).size;
		}
        CWebGrabDataElem* el = new CWebGrabDataElem();
		el->size = count;
		el->mas = new unsigned char[count];
		int start = 0;
		for(list<CWebGrabData::CWebGrabDataElem>::iterator it =data.begin();	it!=data.end(); it++)
		{
			memcpy(el->mas+start,it->mas,it->size);
			start+=it->size;
		}
        
        return *el;
	}
};