#pragma once
#include "rthread.h"


// ������������������� ����� ���������� ������ RThread
// ������ ������� ������ �������� � ���� ���� ������{ ������� Run() }
// ����������� ������� ������ RThread

class  RThreadEx 
{
private:
	static unsigned WINAPI ThreadProc(LPVOID threadInfo);
protected:
	RThread *thread;                           // ������ - �����
	bool IsFinish;                             // true: ����� ������� ���� ���������� 
	bool IsRunStart;                           // true: ����� ��� �������
    bool terminate;							   // ��������� ���������� ������
public:
	RThreadEx(void);
	virtual ~RThreadEx(void);
	virtual int Run();							// ���� ������
	virtual int InitializeThread(bool IsStart); // ������������� ������
	virtual int Start();						// ������ ������
	virtual int Stop();							// ������ ��������� ������ 
	
	virtual void Terminate();					// ���������� ���������� ������� Run()
	int waitForThreadToFinish(int timeSec);     // �������� ��������� � ���������� ������
	void Resume();								// ������������� ������ ������(����� ������ ������� Suspend())
	void Suspend();								// ������������� �����
	int  getThreadPriority();                   // ������� �������� ���������� ��������� ������������� �������
	bool setThreadPriority(int priority);       // ������ ��������� ������������� ������������� �������
	bool IsTerminate();
	RThread* GetRThreadHandle();                // ������� ��������� �� ������ ������ RThread
	
};

