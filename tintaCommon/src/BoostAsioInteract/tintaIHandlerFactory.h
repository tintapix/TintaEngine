/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */


#include <tintaConfig.h>
#ifndef TINTA_NO_INTERACT

#ifndef _TINTA_IHANDLER_FACTORY_H_
#define _TINTA_IHANDLER_FACTORY_H_


#include "tintaInteractHandler.h"

namespace Tinta {

	struct  tintaIHandlerFactory
	{	
        virtual tintaInteractHandler * createHandler(m_ulong32 id) = 0;

        virtual void removeHandler(m_ulong32 id) = 0;
	};


}

#endif

#endif