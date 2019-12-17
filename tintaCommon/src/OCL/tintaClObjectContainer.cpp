/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaClObjectContainer.h"
#include "tintaMemoryAllocator.h"

namespace Tinta {


	tintaIClBase* tintaClObjectContainer::findObject(const String &programName) {

		if (mCached && mCached->getProgramName() == programName)
			return mCached;
		gpuProgIt_t iter_find = mGpuObjects.find( programName );
		
		if ( iter_find != mGpuObjects.end() ){
			mCached = iter_find->second;
			return iter_find->second;
		}
		return NULL;
	}


	void tintaClObjectContainer::clear(){
		mCached = NULL_M;
		mGpuObjects.clear();
	}

    void tintaClObjectContainer::remove(const String &programName) {
        mGpuObjects.erase(programName);       
    }

	bool tintaClObjectContainer::registerObject( tintaIClBase* obj ){
		
		assert( obj );
		
		if( !obj )
			return false;

		String name = obj->getProgramName();
				
		return addObj( obj->getProgramName(), obj );		

	}
    bool tintaClObjectContainer::addObj(const String &programName, tintaIClBase* obj ){

		assert( obj && obj->isInit() && "must be not null and initialized" );
		if( !obj || !obj->isInit() || obj->getProgramName().length() == 0 )
			return false;
		mCached = NULL_M;
		return mGpuObjects.insert( gpuObjContainer_t::value_type( programName, obj) ).second;		
	}

	tintaClObjectContainer::tintaClObjectContainer(void){
		mIterPos = mGpuObjects.begin();
	}

	tintaClObjectContainer::~tintaClObjectContainer(void) {
		for(gpuProgIt_t iter = mGpuObjects.begin(); iter != mGpuObjects.end(); iter++){			
			M_DELETE (iter->second);
		}
		mCached = NULL_M;
	}


}


