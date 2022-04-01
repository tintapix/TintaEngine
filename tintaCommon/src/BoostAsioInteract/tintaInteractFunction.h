

/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include <tintaConfig.h>
#ifndef TINTA_NO_INTERACT

#ifndef _TINTA_INTERACT_FUNCTION_H_
#define _TINTA_INTERACT_FUNCTION_H_


#include "tintaIInteracted.h"
#include "tintaPredefine.h"

namespace Tinta {

struct tintaInteractFunction
{
    tintaIInteracted *mPInteracted = NULL_M;
    tintaInteractFunction() = delete;
    tintaInteractFunction(tintaIInteracted *intr)
        :mPInteracted(intr) {
    }

    void operator()();
};


}


#endif


#endif