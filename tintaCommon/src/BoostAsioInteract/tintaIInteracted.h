/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */


#ifndef _TINTA_IINTERACTED_H_
#define _TINTA_IINTERACTED_H_

#include <tintaConfig.h>
#ifndef TINTA_NO_INTERACT

namespace Tinta {

    struct tintaIInteracted {
        virtual void startInteract() = 0;

    };

}

#endif

#endif