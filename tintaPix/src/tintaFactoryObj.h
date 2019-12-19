/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_FACTORY_OBJECT_H_
#define _TINTA_FACTORY_OBJECT_H_

#include "tintaPredefine.h"

namespace Tinta {

	template< typename T > class tintaFactoryObj	
    {
    public:
        virtual ~tintaFactoryObj() {}    
        virtual const String& getName() const = 0;    
        virtual T*			  createInstance( ) const = 0;               
    };
	
} 

#endif
