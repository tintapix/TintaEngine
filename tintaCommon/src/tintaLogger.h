/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */


#ifndef __TINTA_LOGGER_H__
#define __TINTA_LOGGER_H__

#include "tintaConsoleOutput.h"
#include "tintaObserved.h"
#include "tintaPredefine.h"
#include "tintaSingleton.h"

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
        
        ~tintaLogger();        

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

        void clearQueue();
		
		 

		static tintaLogger* getPtr(void);

		tintaObserved< Tinta::tintaConsoleOutput > mObserved;		

    };	
}

#endif
