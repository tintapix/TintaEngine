/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef TINTA_ITIMER_CALLBACK_H
#define TINTA_ITIMER_CALLBACK_H

namespace Graphica {

 struct  tintaITimerCallback
{
    virtual void onReset() = 0;

    virtual void onProcess(m_uint32 mlsElapsed) = 0;

};
 
}


#endif
