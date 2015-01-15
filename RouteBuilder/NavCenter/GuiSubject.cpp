#include "StdAfx.h"
#include "GuiSubject.h"
#include "IGuiObserver.h"

CGuiSubject::CGuiSubject(void)
{
	m_observers = new list<IGuiObserver*>;
}


CGuiSubject::~CGuiSubject(void)
{
}
void CGuiSubject::ChangePolylineToRoute(int &routeID, int &oldPolylineID,int &newPolylineID)
{
	for(list<IGuiObserver*>::iterator i = m_observers->begin(); i!=m_observers->end();i++)
	{
		(*i)->OnChangePolylineToRoute(routeID,oldPolylineID,newPolylineID);
	}
}


bool  CGuiSubject::IsAttached(IGuiObserver* obs )
{
	
	for(list<IGuiObserver*>::iterator i = m_observers->begin(); i!=m_observers->end();i++)
	{
		if((*i) == obs)
			return true;
	}
	return false;
}
void CGuiSubject::Attach(IGuiObserver* obs)
{
	if(IsAttached(obs)==false)
	this->m_observers->push_back(obs);
}
void CGuiSubject::Detach(IGuiObserver* obs)
{
	for(list<IGuiObserver*>::iterator i = m_observers->begin(); i!=m_observers->end();i++)
	{
		if((*i) == obs)
		{
			m_observers->erase(i);
			break;
		}
	}
	//this->m_observers->remove(obs);
}
void CGuiSubject::AddMarker(CDBLocation& loc)
{
	for(list<IGuiObserver*>::iterator i = m_observers->begin(); i!=m_observers->end();i++)
	{
		(*i)->OnAddMarker(loc);
	}
}
void CGuiSubject::DeleteMarker(int &locID, CString &Name)
{
	for(list<IGuiObserver*>::iterator i = m_observers->begin(); i!=m_observers->end();i++)
	{
		(*i)->OnDeleteMarker(locID,Name);
	}
}
void CGuiSubject::SelectMarker(int locID)
{
	for(list<IGuiObserver*>::iterator i = m_observers->begin(); i!=m_observers->end();i++)
	{
		(*i)->OnSelectMarker(locID);
	}
}
void CGuiSubject::RefreshData(COSMCtrlEx &ctrl,bool forcibly)
{
	for(list<IGuiObserver*>::iterator i = m_observers->begin(); i!=m_observers->end();i++)
	{
		(*i)->OnRefreshData(ctrl,forcibly);
	}
}

void CGuiSubject::FreeAllData()
{
	for(list<IGuiObserver*>::iterator i = m_observers->begin(); i!=m_observers->end();i++)
	{
		(*i)->OnFreeAllData();
	}
}
void CGuiSubject::UnselectedPolylines(bool unselectedEx)
{
	for(list<IGuiObserver*>::iterator i = m_observers->begin(); i!=m_observers->end();i++)
	{
		(*i)->OnUnselectedPolylines(unselectedEx);
	}
}


void CGuiSubject::OpenManualAddPolyline(TAddPolylineProps props)
{
	for(list<IGuiObserver*>::iterator i = m_observers->begin(); i!=m_observers->end();i++)
	{
		(*i)->OnOpenManualAddPolyline(props);
	}
}
void CGuiSubject::CloseManualAddPolyline()
{
	for(list<IGuiObserver*>::iterator i = m_observers->begin(); i!=m_observers->end();i++)
	{
		(*i)->OnCloseManualAddPolyline();
	}
}
void CGuiSubject::AddPolyline(CDBPolyLine& line)
{
	for(list<IGuiObserver*>::iterator i = m_observers->begin(); i!=m_observers->end();i++)
	{
		(*i)->OnAddPolyline(line);
	}
}
void CGuiSubject::DeletePolyline(int lineID)
{
	for(list<IGuiObserver*>::iterator i = m_observers->begin(); i!=m_observers->end();i++)
	{
		(*i)->OnDeletePolyline(lineID);
	}
}
void CGuiSubject::SelectPolyline(int lineID)
{
	for(list<IGuiObserver*>::iterator i = m_observers->begin(); i!=m_observers->end();i++)
	{
		(*i)->OnSelectPolyline(lineID);
	}
}

void CGuiSubject::AddRoute(CDBRoute& route)
{
	for(list<IGuiObserver*>::iterator i = m_observers->begin(); i!=m_observers->end();i++)
	{
		(*i)->OnAddRoute(route);
	}
}
void CGuiSubject::DeleteRoute(int &routeID)
{
	for(list<IGuiObserver*>::iterator i = m_observers->begin(); i!=m_observers->end();i++)
	{
		(*i)->OnDeleteRoute(routeID);
	}
}
void  CGuiSubject::SelectRoute(int &routeID)
{
	for(list<IGuiObserver*>::iterator i = m_observers->begin(); i!=m_observers->end();i++)
	{
		(*i)->OnSelectRoute(routeID);
	}
}

