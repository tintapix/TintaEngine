/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */
#include <tintaConfig.h>

#include "tintaInteractFunction.h"

#ifndef TINTA_NO_INTERACT

namespace Tinta{


  void tintaInteractFunction::operator()() {
    if ( mPInteracted )
        mPInteracted->startInteract();
  }

}


#endif