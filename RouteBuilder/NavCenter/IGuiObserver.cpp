#include "StdAfx.h"
#include "IGuiObserver.h"


IGuiObserver::IGuiObserver(void)
{
}


IGuiObserver::~IGuiObserver(void)
{
}
void IGuiObserver::SetSubject(CGuiSubject* subject)
{
	this->subject = subject;
}
