#ifndef __THREAD_POOL_H
#define __THREAD_POOL_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

/* The class of thread execs */
class CTask {
protected:
    string mStrTaskName;    // name of task
	void   *mPtrData;       // data to exec task with

public:
    CTask(){}
	CTask(string taskName) {
	    mStrTaskName = taskName;
		mPtrData     = NULL;
	}

	virtual int Run() = 0;
	void SetData(void *data);   // set task data
public:
	virtual ~CTask(){}
};

/* the manage class of thread pool */
class CThreadPool {
private:
    static vector<CTask*> mVecTaskList;    /*task list*/
	static bool shutdown;                  /* flag of thread exit */
	int    mIThreadNum;                    /* the running tread count in the pool */
	pthread_t  *pthread_id;

	static pthread_mutex_t  mPthreadMutex;
	static pthread_cond_t   mPthreadCond;

protected:
	static void* ThreadFunc(void *threadData);   /* the callback of new thread */
	static int MoveToIdle(pthread_t tid);        /* move to idle */
	static int MoveToBusy(pthread_t tid);        /* move to busy */

	int Create();                  /* create thread in thread pool */
public:
	CThreadPool(int threadNum=10);
	int AddTask(CTask *task);     /* add task to task queue */
	int StopAll();                /* quit the thread in the pool */
	int getTaskSize();            /* get current task count in the thread pool */
};







#endif //__THREAD_POOL_H
