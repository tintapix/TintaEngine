/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_ASYNC_TIMER_H_
#define _TINTA_ASYNC_TIMER_H_

#include <tintaPredefine.h>

namespace Tinta{

    /*
    Functor example:
        tintaAsyncTimer::TimeoutFunc  timeout = []()
        {
        cout << "Hello!" << endl;
        };
        Example:
            tintaAsyncTimer mExecuteTimer( 1000, std::bind(&className::timaerFunction, this) );
    */
    class _CoreExport tintaAsyncTimer
    {

    public:        
        typedef m_ulong32 Interval_t;
        typedef std::function<void(void)> TimeoutFunc;

        tintaAsyncTimer(const Interval_t &interval,
            const TimeoutFunc &timeout);
        tintaAsyncTimer(const TimeoutFunc &timeout);

        ~tintaAsyncTimer();

        void start();

        void stop();

        // stops and restart times with ne wperiod
        void reset(const Interval_t &interval, bool bStart = false );

    private:

        tintaAsyncTimer();

        THREAD_TYPE      mTimerThread;
        Interval_t  mInterval;

        std::atomic_bool        mRunning = false;
        TimeoutFunc mTimeout;
    };

}
#endif