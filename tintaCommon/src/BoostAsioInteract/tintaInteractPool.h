/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include <tintaConfig.h>
#ifndef TINTA_NO_INTERACT

#ifndef _TINTA_INTERACT_POOL_H_
#define _TINTA_INTERACT_POOL_H_

#include "tintaPredefine.h"
#include <BoostAsioInteract/tintaInteractFunction.h>

namespace Tinta { 

/*
    Contains threads vector with connection objects
*/
class tintaInteractPool
{

public:
    tintaInteractPool( void );
	virtual ~tintaInteractPool( void );

	void addInteract( tintaInteractFunction *func );	

private:
   typedef std::vector<THREAD_TYPE*> InteractThreadPool_t;

   InteractThreadPool_t mPool;	

	void shutdown();
};

}


#endif

#endif