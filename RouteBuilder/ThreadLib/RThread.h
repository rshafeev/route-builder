#ifndef SSMCTHREAD_H
#define SSMCTHREAD_H


typedef unsigned (__stdcall *SSMCTHREADFUNC) (LPVOID);

// класс управления потока
class  RThread
{

public:

    RThread(
		SSMCTHREADFUNC pThreadFunc,				// address of thread callback
		LPVOID pThreadFuncParameter=NULL,		// address of callback's parameter,
		DWORD  dwExeFlags=0,					// creation flag
		DWORD  dwSize=0,						// stack size
		BOOL   inheritable=FALSE);				// inheritable

    virtual ~RThread();

public:

	void execute();
    void resume();
	void suspend();
	int waitForThreadToFinish(DWORD msec = INFINITE);

    int  getThreadPriority();
	void setThreadPriority(int);

    void setProcessorMask(DWORD);

	LPVOID getThreadArgument() { return threadArgument; }
    DWORD getExitCode();
	DWORD getThreadId();

#ifdef SSMC_INCLUDE_NT
	// these four are available on NT only
    void disablePriorityBoost(BOOL);
	BOOL isPriorityBoostDisabled();
	void setIdealProcessor(DWORD);
	void  getThreadStatistics(LPFILETIME,LPFILETIME,LPFILETIME,LPFILETIME);
#endif	// SSMC_INCLUDE_NT

private:
    LPSECURITY_ATTRIBUTES	sa;
	SSMCTHREADFUNC	threadFunction;
    HANDLE			thread;
    unsigned		threadId;
	DWORD			stackSize;
	DWORD			executeFlag;
	LPVOID			threadArgument;
    DWORD			exitCode;

private:

    static void getErrorDetail(int&, LPCTSTR);
};

#endif //SSMCTHREAD_H
