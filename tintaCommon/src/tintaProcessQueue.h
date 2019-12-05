/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_PROCESS_QUEUE_H_
#define _TINTA_PROCESS_QUEUE_H_

#include "tintaThreadPool.h"

namespace Tinta
{
    /*
        Multithread process queue
    */
    class _CoreExport tintaProcessQueue
		: public DefaultThreadPoolBase
	{
	public:                     
					 tintaProcessQueue(const String& name = _M(""));
		virtual	    ~tintaProcessQueue(); 		
        virtual void run();
		virtual void shutdown();		
		virtual void startup(bool forceRestart = true);

	protected:
		
		virtual void waitForNextRequest();
		
		virtual void notifyThreadRegistered();

		virtual void notifyWorkers();

		
        MUTEX_T(mInitMutex);

        THREAD_CONDITION(mInitSync);

        THREAD_CONDITION(mRequestCondition);

		typedef vector<THREAD_TYPE*> WorkerThreadList;
		WorkerThreadList mWorkers;

	};
}
#endif