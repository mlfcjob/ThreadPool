#include "Thread.h"
#include <iostream>


void CTask::SetData(void *data)
{
    mPtrData = data;
}

vector<CTask*> CThreadPool::mVecTaskList;     //task list
bool CThreadPool::shutdown = false;

pthread_mutex_t  CThreadPool::mPthreadMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t   CThreadPool::mPthreadCond  = PTHREAD_COND_INITIALIZER;


/* constructor for threadpool manage class */
CThreadPool::CThreadPool(int threadNum)
{
    this->mIThreadNum = threadNum;
	cout<<"I will create " << threadNum <<" threads"<<endl;
	Create();
}


/* callback of thread */
void *CThreadPool::ThreadFunc(void *threadData)
{
    pthread_t tid = pthread_self();
	while(1) {
	    pthread_mutex_lock(&mPthreadMutex);
		while(mVecTaskList.size() == 0 && !shutdown) {
		    pthread_cond_wait(&mPthreadCond, &mPthreadMutex);
		}

		if (shutdown) {
		    pthread_mutex_unlock(&mPthreadMutex);
			printf("thread %lu will exit.\n", pthread_self());
			pthread_exit(NULL);
		}

		printf("tid %lu run \n", tid);
		vector<CTask*>::iterator iter = mVecTaskList.begin();

		/* pull a task and deal it */
		CTask *task = *iter;
		if (iter != mVecTaskList.end()) {
		    task = *iter;
			mVecTaskList.erase(iter);
		}

		pthread_mutex_unlock(&mPthreadMutex);
		task->Run();
		printf("tid:%lu, idle \n", tid);
	}

	return (void*)0;
}


/* push to queue and send thread sync signal */
int CThreadPool::AddTask(CTask *task)
{
    pthread_mutex_lock(&mPthreadMutex);
	this->mVecTaskList.push_back(task);
	pthread_mutex_unlock(&mPthreadMutex);
	pthread_cond_signal(&mPthreadCond);
	return 0;
}

/* create thread */
int CThreadPool::Create()
{
    pthread_id = (pthread_t*)malloc(sizeof(pthread_t) * mIThreadNum);
	for(int i = 0; i < mIThreadNum; i++) {
	   pthread_create(&pthread_id[i], NULL, ThreadFunc, NULL);
	}

	return 0;
}

/* stop all thread */
int CThreadPool::StopAll()
{
    if (shutdown) {
	    return -1;
	}

	printf("Now i will end all threads!!\n");
	/* wake all wating threads */
	shutdown = true;
	pthread_cond_broadcast(&mPthreadCond);

	/* block wait for thread quiting */
	for (int i = 0; i < mIThreadNum; i++) {
	    pthread_join(pthread_id[i], NULL);
	}

	free(pthread_id);
	pthread_id = NULL;

	/* destroy mutex and cond */
	pthread_mutex_destroy(&mPthreadMutex);
	pthread_cond_destroy(&mPthreadCond);

	return 0;
}

/* get task mount in current queue */
int CThreadPool::getTaskSize()
{
    return mVecTaskList.size();
}





