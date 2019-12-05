/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaSignalHandler.h"
#include "tintaException.h"


namespace Tinta {


    tintaSignalHandler::tintaSignalHandler()
    {
    }

    tintaSignalHandler::~tintaSignalHandler()
    {
    }

    bool tintaSignalHandler::gotExitSignal()
    {
        return mbGotExitSignal;
    }

    void tintaSignalHandler::setExitSignal(bool signal)
    {
        mbGotExitSignal = signal;
    }
    void tintaSignalHandler::exitSignalHandler(int ignored)
    {
        mbGotExitSignal = true;
    }

    void tintaSignalHandler::setupExitSignalHandler()
    {
        /*
        if ( signal((int)SIGINT, tintaSignalHandler::exitSignalHandler) == SIG_ERR )     {           

            C_EXCEPTION(_M("Signal handle error"));
        }
        */

        setupSignalHandlers( (int)SIGINT, tintaSignalHandler::exitSignalHandler );
    }

    void tintaSignalHandler::setupSignalHandlers( int signalId, void(*hndl)(int) ) {
        if (signal(signalId, hndl) == SIG_ERR)     {

           EXCEPTION(_M("Signal handle error"));
        }
    }

    std::atomic<bool> tintaSignalHandler::mbGotExitSignal(false);
}
