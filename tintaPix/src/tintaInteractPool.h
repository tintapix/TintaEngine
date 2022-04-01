// Copyright (C) 2011 - 2017 Evdokimov Mikhail 
// tintapalma.com
// tintapalma@gmail.com

#ifndef TINTA_NO_INTERACT

#ifndef _TINTA_INTERACT_POOL_H_
#define _TINTA_INTERACT_POOL_H_

#include "tintaPredefine.h"
#include <BoostAsioInteract/tintaInteractFunction.h>

namespace Tinta { 

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