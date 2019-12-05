/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */



#ifndef __TINTA_EXCEPTION_H_
#define __TINTA_EXCEPTION_H_


#include "tintaPredefine.h"
#include <exception>




namespace Tinta {

	class tintaException
        : public std::exception
	{
	
    protected:
        long           mLine;       
        String         mDescription;
        String         mSource;
        String         mFile;
		mutable String mFullDesc;
        void print();
    public:     

        tintaException( const String& descr, const char* src, const char* file, long line );
        tintaException( const String& descr, const char* src ); 

        tintaException(const tintaException& rhs);		
		~tintaException() throw() {}
        
        void operator = (const tintaException& rhs);        
		virtual const String& getDescr(void) const;  
        
        virtual const String &getSrc() const { return mSource; }        
        virtual const String &getSrcFile() const { return mFile; }        
        virtual long getLine() const { return mLine; }			
		const char* what() const throw();        
    };		

#ifndef EXCEPTION
#define EXCEPTION(desc) throw Tinta::tintaException( desc, __FUNCTION__, __FILE__, __LINE__ );
#endif

#ifdef EXCEPT_ASSERT
#   if DEBUG_MODE
#       define CoreAssert( a, b ) assert( (a) && (b) )
#	else
#		define CoreAssert( a, b ) if( !(a) ) EXCEPTION( (b) )
#	endif
#else
#   define CoreAssert( a, b ) assert( (a) && (b) )
#endif

} 

#endif
