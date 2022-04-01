/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_APP_H_
#define _TINTA_APP_H_


#include <tintaPredefine.h>
#include "tintaITimerCallback.h"
#include <tintaTimer.h>
#include <tintaObserved.h>
#include <tintaControls.h>

namespace Graphica
{


    class tintaDrawable;
    class _CoreExport tintaApp
	    : public  AllocatedObjectApplication
    {

    public:	
	
	    static void Initialize ();	
    
        virtual ~tintaApp();
    
   
        typedef int (*EntryPoint)(int, char**);

        static EntryPoint Run;  

        virtual int Main(int iQuantity, char** apcArgument) = 0;

        m_uint32 addOnTimerBuff(m_uint32 elapsed, const StringBasic &execute);

        bool delTimer(m_uint32 pos);

        bool setTimer(m_uint32 pos, const String &script);

        bool setTimer(m_uint32 pos, m_uint32 elapsed);

        void delTimers();

        size_t timers() const;

        void setManual(int timerPeriod);       

        virtual void addCallback(tintaITimerCallback * v);

        virtual tintaControls* GetKeyStates();

        virtual bool setCursor( const String &path) = 0;  

        m_uint32 getTime() const;
    
    protected:

        tintaApp();

        typedef std::list<tintaTimerScript> onTimer_t;

        onTimer_t mTimerScripts;

        m_uint32  mTimersKey = 0;       

        m_uint32 mTimerPeriod = 0;

        tintaTimer            *mTimer;

        String mRecPrefix;

        String mShotDirectory;

        tintaObserved< tintaITimerCallback > mTimerObserved;

        tintaControls mKeyKeeper;

        // msec for main timer loop
        m_uint32 mElapsed = 0;

        //double mElapsed;
	    //Tinta::tintaTimer m_frame_timer;
    };

}

#endif
