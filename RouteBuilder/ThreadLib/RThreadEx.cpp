#include "StdAfx.h"
#include "RThreadEx.h"


RThreadEx::RThreadEx(void)
{
	thread = NULL;
	IsFinish = false;
	IsRunStart = false;
	terminate = false;
}

RThreadEx::~RThreadEx(void)
{
	if (thread)
		delete thread;
	thread = 0;
}
bool RThreadEx::IsTerminate()
{
	return terminate;
}

unsigned WINAPI RThreadEx::ThreadProc(LPVOID threadInfo)
{
	RThreadEx* obj = (RThreadEx*)threadInfo;
	if(obj==NULL)
		return 0;
	return obj->Run();
}

int RThreadEx::InitializeThread(bool IsStart)
{
	thread = new RThread(RThreadEx::ThreadProc, (void*)this);
	if(thread&&IsStart == true)
	{
		this->Start();
		return 1;
	}
	else
	if(thread)
		return 1;
	return 0;
}
int RThreadEx::Run()
{
	IsRunStart = true;

	IsFinish = true;
	return 1;
}
void RThreadEx::Terminate()
{
	terminate = true;
}
int RThreadEx::waitForThreadToFinish(int timeSec)
{
  if(thread)
		return thread->waitForThreadToFinish((DWORD)timeSec);
	return 0;
}
int RThreadEx::Start()
{
	if(thread==NULL) 
		return 0;
	thread->execute();
	return 1;
}
int RThreadEx::Stop()
{
	if(thread==NULL) 
		return 0;
	this->Terminate();
	this->waitForThreadToFinish(INFINITE);
	return 1;
}
void RThreadEx::Resume()
{
	if(thread==NULL) return;
	thread->resume();
}
void RThreadEx::Suspend()
{
	if(thread==NULL) return;
	thread->suspend();
}
int  RThreadEx::getThreadPriority()
{
	if(thread==NULL) return 0;
	return thread->getThreadPriority();
}
bool RThreadEx::setThreadPriority(int priority)
{
	if(thread==NULL) return false;
	thread->setThreadPriority(priority);
	return true;
}

RThread* RThreadEx::GetRThreadHandle()
{
	if(this==NULL)
		return NULL;
	return this->thread;
}
