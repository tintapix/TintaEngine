/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_ISERIALISABLE_SET_H_
#define _TINTA_ISERIALISABLE_SET_H_

#include "tintaPredefine.h"
#include "tintaBufferIO.h" 

namespace Tinta
{
		
	struct tintaISerialisableSet	{	

        virtual StringBasic GetObjectName(m_uint32 key) const = 0;

        virtual size_t GetBufferSize(m_uint32 key) const = 0;

        virtual size_t packData(m_uint32 key, m_int8 *data, size_t offset) const = 0;

        virtual size_t unpackData(const StringBasic &type, const m_int8 *data, size_t offset, m_uint32 &rezId) = 0;

	};
}

#endif
