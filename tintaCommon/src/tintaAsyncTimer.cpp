/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov
tintapix.com
tintapix@gmail.com  */

#include "tintaAsyncTimer.h"

namespace Tinta{


    tintaAsyncTimer::tintaAsyncTimer(const Interval_t &interval,
        const TimeoutFunc &timeout) :
        mInterval(interval),
        mTimeout(timeout),
        mRunning(false){
    }

    tintaAsyncTimer::tintaAsyncTimer(const TimeoutFunc &timeout):
        mInterval( 0 ),
        mTimeout(timeout),
        mRunning(false){
    }

    tintaAsyncTimer::~tintaAsyncTimer(){
        stop();        
    }

    void tintaAsyncTimer::start()
    {
        if (mInterval > 0) {
            mRunning = true;

            mTimerThread = THREAD_TYPE([=]()
            {
                while (mRunning) {
                    THREAD_SLEEP(mInterval);
                        mTimeout();
                }
            });
        }
    }

    void tintaAsyncTimer::stop()
    {
        mRunning = false;

        if (mTimerThread.joinable())
            mTimerThread.join();

    }

    void tintaAsyncTimer::reset(const Interval_t &interval, bool bStart ){

        mRunning = false;
        stop();
        mInterval = interval;
        if ( bStart )
            start();
    }

    tintaAsyncTimer::tintaAsyncTimer()
        :mTimeout(TimeoutFunc()){}
}
