/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include <tintaConfig.h>
#ifndef TINTA_NO_INTERACT

#include "tintaInteractPool.h"

namespace Tinta { 

tintaInteractPool::tintaInteractPool(void)
{
}

tintaInteractPool::~tintaInteractPool(void)
{
	shutdown();
}

void tintaInteractPool::shutdown(){


		for (auto i = mPool.begin(); i != mPool.end(); ++i){
			// trying make it correct
            if( (*i)->joinable() )
			    (*i)->join();
			THREAD_DESTROY(*i);
		}
		mPool.clear();
		//mIsRunning = false;
}




void tintaInteractPool::addInteract( tintaInteractFunction *func  )
{	
    if (!func)
        return;

	THREAD_CREATE( t, *func );
	t->detach();

	mPool.push_back(t);
	
}


}


#endif