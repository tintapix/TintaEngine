/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_VAL_BOX_FACTORY_H_
#define _TINTA_VAL_BOX_FACTORY_H_

#include <tintaFactoryObj.h>
#include "tintaMemoryAllocator.h"

namespace Tinta {

	class tintaArrayBox;

	class _CoreExport tintaArrayBoxObjFactory
		: public tintaFactoryObj<tintaArrayBox> 
		, public AllocatedValMapFactory
	{ 
public:
		virtual ~tintaArrayBoxObjFactory(){}
	};


}

#endif