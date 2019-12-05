/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_CONNECTION_EVENTS_H_
#define _TINTA_CONNECTION_EVENTS_H_


#include "tintaPredefine.h"

namespace Tinta {

	struct tintaConnectionEvents	
	{	
		enum mask {
			enUnplagged = 0x01,
			enError = 0x02,
			enConnect = 0x04,
		};	

        virtual void     onUnplagged(m_ulong32 id) = 0;

        virtual  void    onErrorOccured(m_ulong32 id, long error) = 0;

		virtual m_uint32 getObservMask()const = 0;

        virtual void     onConnected(m_ulong32 id) = 0;

	};	
	

}

#endif