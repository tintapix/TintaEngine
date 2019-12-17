#ifndef _TINTA_FACTORY_OBJ__H_
#define _TINTA_FACTORY_OBJ__H_

#include "tintaPredefine.h"

namespace Tinta {
    

    /*
        Base class for factory objects
    */
	template< typename T > 
    class tintaFactoryObj:
        public	tintaINamed
    {
    public:
        virtual ~tintaFactoryObj() {}            
        virtual T*			  createInstance( ) const       = 0;        
        virtual void		  releaseInstance( T* )         = 0;    
    };
	
} 

#endif
