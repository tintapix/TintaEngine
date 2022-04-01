/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaClObjectContainer.h"
#include "tintaMemoryAllocator.h"

namespace Tinta {


	tintaIClBase* tintaClObjectContainer::findObject(const String &programName) {

		
		gpuProgIt_t iter_find = mGpuObjects.find( programName );
		
		if ( iter_find != mGpuObjects.end() ){			
			return iter_find->second;
		}
		return NULL;
	}


	void tintaClObjectContainer::clear(){		
		mGpuObjects.clear();
	}

    void tintaClObjectContainer::remove(const String &programName) {
        mGpuObjects.erase(programName);       
    }

	bool tintaClObjectContainer::registerObject( tintaIClBase* obj ){
		
        CoreAssert(obj, "obj == NULL");
		
		if( !obj )
			return false;

		String name = obj->getProgramName();
				
		return addObj( obj->getProgramName(), obj );		

	}
    bool tintaClObjectContainer::addObj(const String &programName, tintaIClBase* obj ){

        CoreAssert( obj && obj->isInit() , "must be not null and initialized" );
		if( !obj || !obj->isInit() || obj->getProgramName().length() == 0 )
			return false;
		
		return mGpuObjects.insert( gpuObjContainer_t::value_type( programName, obj) ).second;		
	}

	tintaClObjectContainer::tintaClObjectContainer(void){
		mIterPos = mGpuObjects.begin();
	}

	tintaClObjectContainer::~tintaClObjectContainer(void) {
		for(gpuProgIt_t iter = mGpuObjects.begin(); iter != mGpuObjects.end(); iter++){			
			M_DELETE (iter->second);
		}		
        mGpuObjects.clear();
	}


}


