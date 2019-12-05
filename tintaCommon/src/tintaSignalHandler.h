/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaPredefine.h"

#ifndef _TINTA_SIGNAL_HANDLER_H_
#define _TINTA_SIGNAL_HANDLER_H_


namespace Tinta {


    class tintaSignalHandler
    {
    
    protected:
        static std::atomic<bool> mbGotExitSignal;// = false;
    public:
        tintaSignalHandler();
        virtual ~tintaSignalHandler();

        static bool gotExitSignal();
        static void setExitSignal(bool signal);

        void        setupExitSignalHandler();

        // for manual setup
        //SIGABRT	Abnormal termination of the program, such as a call to abort
        //SIGFPE	An erroneous arithmetic operation, such as a divide by zero or an operation resulting in overflow.
        //SIGILL	Detection of an illegal instruction
        //SIGINT	Receipt of an interactive attention signal.
        //SIGSEGV	An invalid access to storage.
        //SIGTERM	A termination request sent to the program.
        void        setupSignalHandlers(int signalId, void(*)(int));

        static void exitSignalHandler(int ignored);
        
    };

}

#endif
