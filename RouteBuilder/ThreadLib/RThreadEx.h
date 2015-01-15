#pragma once
#include "rthread.h"


// усовершенствованный класс управления потока RThread
// объект данного класса включает в себя тело потока{ функция Run() }
// перегружены функции класса RThread

class  RThreadEx 
{
private:
	static unsigned WINAPI ThreadProc(LPVOID threadInfo);
protected:
	RThread *thread;                           // объект - поток
	bool IsFinish;                             // true: поток окончил свое выполнение 
	bool IsRunStart;                           // true: поток был запущен
    bool terminate;							   // руководит остановкой потока
public:
	RThreadEx(void);
	virtual ~RThreadEx(void);
	virtual int Run();							// тело потока
	virtual int InitializeThread(bool IsStart); // инициализация потока
	virtual int Start();						// запуск потока
	virtual int Stop();							// полная остановка потока 
	
	virtual void Terminate();					// остановить выполнение функции Run()
	int waitForThreadToFinish(int timeSec);     // ожидание остановки и выгружения потока
	void Resume();								// возобнавление работы потока(после вызова функции Suspend())
	void Suspend();								// приостановить поток
	int  getThreadPriority();                   // возврат значение приоритета выделения процессорного времени
	bool setThreadPriority(int priority);       // задать приоритет распределения процессорного времени
	bool IsTerminate();
	RThread* GetRThreadHandle();                // вернуть указатель на объект потока RThread
	
};

