/*  Copyright (C) 2011 - 2018 Evdokimov Mikhail 
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_IMAGE_FACTORY_H_
#define _TINTA_IMAGE_FACTORY_H_

#include "tintaPredefine.h"
#include "tintaUInt8Image.h"
#include "tintaFactoryObj.h"

namespace Tinta {

	class _CoreExport tintaImageFactory
		: public tintaFactoryObj<tintaUInt8Image>
		, public AllocatedImageFactory	{ 
public:
		virtual ~tintaImageFactory(){}
	};


}

#endif
