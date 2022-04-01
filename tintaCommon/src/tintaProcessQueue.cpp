/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaProcessQueue.h"

namespace Tinta
{

	
	tintaProcessQueue::tintaProcessQueue(const String& name)
	: DefaultThreadPoolBase(name)
	{}
	
	tintaProcessQueue::~tintaProcessQueue(){
		shutdown();
	}
	
	void tintaProcessQueue::startup(bool forceRestart){
		if ( mIsRunning ) {
			if ( forceRestart )
				shutdown();
			else
				return;
		}
		mShuttingDown = false;
		mWorkerFunc = NEW_T( WorkerFunc(this) );

 		for (size_t i = 0; i < mWorkerThreadCount; ++i){				
 			THREAD_CREATE( t, *mWorkerFunc );
 			mWorkers.push_back( t );
 		}			

		mIsRunning = true;
	}
	
	void tintaProcessQueue::notifyThreadRegistered(){
        TINTA_LOCK_MUTEX(mInitMutex);
		THREAD_NOTIFY_ALL(mInitSync);
	}
	
	void tintaProcessQueue::shutdown(){
		if( !mIsRunning )
			return;		
		mShuttingDown = true;		

        THREAD_NOTIFY_ALL(mRequestCondition);
		for ( WorkerThreadList::iterator i = mWorkers.begin(); i != mWorkers.end(); ++i ){				
			(*i)->join();
			THREAD_DESTROY(*i);
		}
		mWorkers.clear();	

		if ( mWorkerFunc )
		{
			DELETE_T(mWorkerFunc, WorkerFunc);
			mWorkerFunc = NULL_M;
		}
		mIsRunning = false;
	}
	
	void tintaProcessQueue::notifyWorkers() {		
        THREAD_NOTIFY_ONE(mRequestCondition);
	}

	
	void tintaProcessQueue::waitForNextRequest(){	
		
        TINTA_LOCK_MUTEX_NAME(mRequestMutex, lock);
		if ( mRequestQueue.empty() ){			
            THREAD_WAIT(mRequestCondition, mRequestMutex, lock);
		}
	}
	
    void tintaProcessQueue::run(){
		while( !mShuttingDown ){
			waitForNextRequest();
			processNext();
		}	
	
	}
}