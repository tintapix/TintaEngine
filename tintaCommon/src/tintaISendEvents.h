/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_ISEND_EVENTS_H_
#define _TINTA_ISEND_EVENTS_H_


#include "tintaPredefine.h"

namespace Tinta {

	struct tintaISendEvents{			
		virtual void onAfterSend(size_t sendId) = 0;

	};	
	

}

#endif