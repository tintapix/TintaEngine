/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_ISERIALISABLE_H_
#define _TINTA_ISERIALISABLE_H_

#include "tintaPredefine.h"
#include "tintaBufferIO.h" 

namespace Tinta
{
		
	struct tintaISerialisable	{	

		virtual size_t GetBufferSize() const = 0;

		virtual size_t packData(m_int8 *data, size_t offset) const = 0;

		virtual size_t unpackData(const m_int8 *data, size_t offset) = 0;

	};
}

#endif
