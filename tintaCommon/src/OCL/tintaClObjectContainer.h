/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_CL_OBJECT_CONTAINER_H_
#define _TINTA_CL_OBJECT_CONTAINER_H_
 

#include "tintaPredefine.h"
#include "tintaClBaseImpl.h"

namespace Tinta
{

	class _CoreExport tintaClObjectContainer 
	{

	public:
		tintaClObjectContainer(void);
		~tintaClObjectContainer(void);

		tintaIClBase* findObject(const String &programName);		

		void clear();	

        void remove(const String &programName);

		bool registerObject( tintaIClBase* obj );				
					
	private:
			
		bool addObj( const String &programName, tintaIClBase* obj );

		typedef std::map<String, tintaIClBase*> gpuObjContainer_t;

		typedef std::map<String, tintaIClBase*>::iterator gpuProgIt_t;

		gpuObjContainer_t mGpuObjects;

		gpuProgIt_t mIterPos;		

	};

}


#endif