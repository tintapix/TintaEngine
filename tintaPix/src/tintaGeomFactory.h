/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_GEOM_FACTORY_H_
#define _TINTA_GEOM_FACTORY_H_

#include <tintaFactoryObj.h>
#include "tintaCompGeom.h"


namespace Tinta {

	class tintaGeomFactory
		: public tintaFactoryObj<tintaCompGeom> 
		, public AllocatedObjectCompObjFactory
	{ 
public:
		virtual ~tintaGeomFactory(){}
	};


}

#endif