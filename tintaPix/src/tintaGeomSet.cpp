/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaPrecompile.h"
#include "tintaGeomSet.h"

namespace Tinta {


	tintaGeomSet::tintaGeomSet( ){
	}

	tintaGeomSet::~tintaGeomSet( ){
	}

	// for export
	template<> Tinta::tintaGeomSet* Tinta::Singleton<tintaGeomSet>::mPtr = 0;

}