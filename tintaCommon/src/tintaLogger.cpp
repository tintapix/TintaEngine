/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaLogger.h"


namespace Tinta
{

    
    tintaLogger::tintaLogger( const String& name )
       :mLogName(name)
    {
        mFileStream.open(name.c_str());		
    }
	tintaLogger::tintaLogger(  ){		
	}	
    
    tintaLogger::~tintaLogger(){		
        TINTA_LOCK_RECUR_MUTEX_AUTO;
        if(mFileStream.is_open() )
            mFileStream.close();
		
    }   

	void tintaLogger::createLog( const String& name, bool printTime)
	{
		
#if CORE_PLATFORM == CORE_PLATFORM_WIN32
        mFileStream.open(name.c_str());
        mFileStream << UTF8BOM.c_str();
        //mFileStream.write( UTF8BOM.c_str(), sBOM);
#else

        mStream.open( name.c_str() );

#endif		
		mLogName = name;	
        mTime = printTime;
	}	


    String tintaLogger::getName() const {
        return mLogName;
    }

	void tintaLogger::logMsg( const String& from, const String& message, msgLevel lml, bool toConsole ) {

        TINTA_LOCK_RECUR_MUTEX_AUTO;

		StringUtil::StrStreamType msg;
		msg<<" ";
		msg<<from;
		msg<<" ";
		logMsg( msg.str(),lml, toConsole );
	
	}


	
	void tintaLogger::addConsole( Tinta::tintaConsoleOutput *console ){

        TINTA_LOCK_RECUR_MUTEX_AUTO;
		//bool rez = mConsoles.insert(console).second;
		mObserved.addCallback(console);

		//m_console = console;
	}

	void tintaLogger::removeConsole( Tinta::tintaConsoleOutput *console ) {
        TINTA_LOCK_RECUR_MUTEX_AUTO;
		mObserved.removeCallback( console );
	}

    void tintaLogger::logMsg( const String& message, msgLevel l, bool toConsole )   {		 
        using namespace std::chrono;
        TINTA_LOCK_RECUR_MUTEX_AUTO;

        if ( l >= LOG_LEVEL ){

			StringUtil::StrStreamType msg;			
            if ( mTime ) {
                msg << StringUtil::getTime();
            }
			msg<<message;			

 			if( toConsole ){							
				Tinta::tintaConsoleOutput *c = mObserved.getFirst();
				for( ;c;c =mObserved.getNext() ){ c->printMsg( msg.str(), l ); }											
			}
           
			if( mLogName.length() == 0 ){
				assert( !"log file name is not selected" );
				return;
			}				
            mFileStream << msg.str() << std::endl;
            mFileStream.flush();
        }
    }

    void tintaLogger::clearQueue() {
        Tinta::tintaConsoleOutput *c = mObserved.getFirst();
        for (; c; c = mObserved.getNext()){ c->clear(); }
    }

	template<> tintaLogger* Singleton<tintaLogger>::mPtr = 0;

	tintaLogger* tintaLogger::getPtr(void){		
		return mPtr;
	}	
}
