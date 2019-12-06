/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include <tintaTexSpringMain.h>
#include <iostream>


#include <tintaUInt8Image.h>
#include <tintaSignalHandler.h>
#include <tintaThread.h>
//#include <tintaPixVer.h>


using namespace Tinta;
using namespace std;


#define KEY_ENTER     13
#define KEY_ARR_UPPDOWN   224
	#define KEY_SUBARR_UPP   72
	#define KEY_SUBARR_DOWN  80
#define KEY_BACKSPACE 8
#define KEY_ESCAPE    27


//#define _FREEIMAGE_TEST
//#define _TEST_
#ifdef _TEST_

#else	

class molyConsoleCmd : public tintaConsoleOutput,
    public tintaUnitsEvents {
public:
    ~molyConsoleCmd() {
    }

    void printMsg(const Tinta::String &msg, msgLevel stat) {
#if CORE_PLATFORM  == CORE_PLATFORM_WIN32       
        tintaWString print = StringUtil::createUTF16String(msg);
        if (print.length() != 0) {
            std::wcout << StringUtil::createUTF16String(msg) << std::endl;
            std::wcout.clear();
        }
        else {
            stream_out << msg << _M("\n");
            stream_out.clear();
        }
#else
        stream_out << msg << _M("\n");
        //      stream_out.clear();
        stream_out.clear();
#endif
        
    }

    void clear() {
        system("cls");
    }

    void registrate() {
        Tinta::tintaLogger::getPtr()->addConsole(this);
    }

    void remove() {
        Tinta::tintaLogger::getPtr()->removeConsole(this);
    }

    virtual void setTitle(const String &title) {
#if CORE_PLATFORM  == CORE_PLATFORM_WIN32
        SetConsoleTitle(title.c_str());
#endif
    }

    virtual void onUnitStateChanged(tintaExecutingUnit::UnitId id, tintaExecutingUnit::UnitState state) {

        updateTitle(id);
    }

    /*
    Unit has finished the job
    */
    virtual void onUnitJobsDone(tintaExecutingUnit::UnitId id, bool success) {}

    /*
    Unit was added in to the set
    */
    virtual void onUnitAdded(const tintaExecutingUnit::UnitId &unitId) {}


    /*
    Unit was removed from the set
    */
    virtual void onUnitRemoved(const tintaExecutingUnit::UnitId &unitId) {}

    virtual void onUnitNameChanged(tintaExecutingUnit::UnitId id, String name) {
        updateTitle(id);
    }

    virtual void onUnitPriorChanged(tintaExecutingUnit::UnitId id, float prior) {}
private:
    void updateTitle(tintaExecutingUnit::UnitId id) {
        const tintaExecutingUnit *unit = tintaUnitsSet::getPtr()->getUnit(id);
        if (unit) {
            StringStream msg;
            String unitState;
            if (unit->getState() == tintaExecutingUnit::enNoState)
                unitState = _M("unknown");
            else if (unit->getState() == tintaExecutingUnit::enReady)
                unitState = _M("ready");
            else if (unit->getState() == tintaExecutingUnit::enBusy)
                unitState = _M("busy");
            else if (unit->getState() == tintaExecutingUnit::enReserved)
                unitState = _M("reserved");
            msg << _M("Unit ") << unit->getId().mname << _M(" ") << ("(") << unitState << (")");

            setTitle(msg.str());
        }
    }

};





int main( int argc, char *argv[] ){

    setlocale(LC_CTYPE, "");


#if CORE_PLATFORM  == CORE_PLATFORM_WIN32
    //ShowWindow(GetConsoleWindow(), SW_HIDE);
    locale loc("rus_rus.866");
#endif

    stream_out << _M("Tinta pix library version ") <<
    #include "tintapixver";
        ;
    stream_out << "\n";

    StringVector args;
	String configName;
    
    static THREAD_CONDITION(mExit);
    TINTA_MUTEX(mExitMutex);


	for( int i  = 0; i < argc; i++  ){
		args.push_back( argv[i] );
	}

//	String configName;
	if ( args.size() > 0)
		configName = args.at( args.size() - 1 );

	tintaTexSpringMain *console ( 0 );
	molyConsoleCmd cmdOut;
    bool isServer(false);
	console = NEW_T( tintaTexSpringMain )( isServer);	

    static std::atomic<bool> bExitSignal(false);
    struct SigHandler{
        static void exitSignalHandler(int ignored)
        {            
            THREAD_NOTIFY_ONE(mExit);
            bExitSignal = true;
        }    
        
    };

    tintaSignalHandler signalHandler;
    signalHandler.setupSignalHandlers((int)SIGINT, SigHandler::exitSignalHandler);

	String command;   
	
	console->setServMode( isServer );
    bool inited = console->initialize(configName, &cmdOut);
	
    
    if ( inited ){

        while (!bExitSignal){
            TINTA_LOCK_MUTEX_NAME(mExitMutex, lock);
            THREAD_WAIT(mExit, mExitMutex, lock);       
        }
	}

	stream_out << _M("\n") << _M("Exiting...") << _M("\n");
	cmdOut.remove();
	DELETE_T( console,tintaTexSpringMain );

	return 1;
}


#endif


