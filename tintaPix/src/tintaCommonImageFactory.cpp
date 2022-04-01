/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaPrecompile.h"
#include "tintaCommonImageFactory.h"

namespace Tinta
{


	const StringBasic& tintaCommonImageFactory::getName(void) const {
        static StringBasic name = tintaUInt8Image::str_common;
		return name;
	}
}