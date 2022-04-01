/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */


#ifndef __TINTA_LOGGER_H___
#define __TINTA_LOGGER_H___

#include "tintaConsoleOutput.h"
#include "tintaObserved.h"
#include "tintaPredefine.h"
#include "tintaSingleton.h"
#include "tintaMemoryAllocator.h"

namespace Tinta {
	
    

    
	class _CoreExport tintaLogger 
        : public Singleton<tintaLogger>,							  
		  public AllocatedObjectLog
    {
    private:
        std::ofstream	mFileStream;
        String			mLogName;
        bool            mTime = true;
    public:
		class Stream;
        AUTO_MUTEX;

		tintaLogger( );

        tintaLogger( const String& name );	
        
        virtual ~tintaLogger();        

		void    createLog(const String& name, bool printTime = true);

        String  getName() const;
        
        void    logMsg( const String& message, msgLevel lml = msg_info, bool toConsole = true );

        /*
			from - for logging clients messages	
		*/
		void    logMsg( const String& from, const String& message, msgLevel lml = msg_info, bool toConsole = true );
			
		
		//Stream stream( msgLevel lml = msg_warning, bool maskDebug = true );       

		void    removeConsole( Tinta::tintaConsoleOutput *console );

		void    addConsole( Tinta::tintaConsoleOutput *console );

        void    showTime(bool bShowTime);

        void clearQueue();
		
		 

        static tintaLogger* getPtr(void){
            return  mPtr;
        }

		tintaObserved< Tinta::tintaConsoleOutput > mObserved;		

    };	
}

#endif
