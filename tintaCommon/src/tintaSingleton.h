/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_SINGLETON_H_
#define _TINTA_SINGLETON_H_

#include "tintaPredefine.h"

namespace Tinta {

    template <typename T> class Singleton   {
	private:		
		Singleton(const Singleton<T> &);	
		Singleton& operator=(const Singleton<T> &);    
	protected:
        static T* mPtr;
    public:
        Singleton( void ) {
            assert(!mPtr);
	    mPtr = static_cast< T* >( this );
        }
        ~Singleton( void ){  			
			mPtr = NULL_M;  
		}        
        static T* getPtr( void ){			
			return mPtr; 
		}
    };
	

}

#endif
