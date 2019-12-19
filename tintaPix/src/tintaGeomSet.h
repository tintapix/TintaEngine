/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_GEOM_SET_
#define _TINTA_GEOM_SET_

#include "tintaPredefine.h"
#include "tintaCompGeom.h"
#include "tintaObjCreator.h"
#include "tintaGeomFactory.h"

#include "tintaObjContainer.h"
#include "tintaSingleton.h"

namespace Tinta {


    //typedef tintaObjCreator< tintaGeomFactory , tintaCompGeom> molyCompObjCreator;

	template< typename TObj, typename TFactory >
	struct tintaCompObjContainer 
		: public tintaObjContainer< TObj, TFactory > {
        
			tintaCompObjContainer(){}

			virtual ~tintaCompObjContainer(){}

			// replacing property 
			bool  setPropVal( unsigned objectId, const tintaCompGeomProp &proper ){				

                t_obj_iter iter_find = std::find_if(tintaObjContainer< TObj, TFactory >::mList.begin(),
                                                                tintaObjContainer< TObj, TFactory >::mList.end(),
                    [objectId](const ObjPair &v){return v.mKey == objectId; });

                if (iter_find != tintaObjContainer< TObj, TFactory >::mList.end()) {
					return iter_find->mpobj->setPropVal( proper );				 
				}		

				return false;			 
			 }

			 tintaCompGeom *getPropPtr( unsigned objectId ){

				 //t_obj_it iter_find = tintaObjContainer< TObj, TFactory >::m_map.find( objectId );

                 t_obj_iter iter_find = std::find_if(tintaObjContainer< TObj, TFactory >::mList.begin(),
                                                                    tintaObjContainer< TObj, TFactory >::mList.end(),
                     [objectId](const ObjPair &v){return v.mKey == objectId; });

                 if (iter_find != tintaObjContainer< TObj, TFactory >::mList.end()) {
					return iter_find->mpobj;				 
				}									 
			}

			// adding without deleting 
			bool		  addPropVal( unsigned objectId, const tintaCompGeomProp &proper ){
				//t_obj_it iter_find = tintaObjContainer< TObj, TFactory >::m_map.find( objectId );

                t_obj_iter iter_find = std::find_if(tintaObjContainer< TObj, TFactory >::mList.begin(), 
                                                                tintaObjContainer< TObj, TFactory >::mList.end(),
                    [objectId](const ObjPair &v){return v.mKey == objectId; });

                if (iter_find != tintaObjContainer< TObj, TFactory >::mList.end()) {
					return iter_find->mpobj->addPropVal( proper );				 
				}	
				return false;	
			 }

	};
	
	// to create singleton
	struct _CoreExport tintaGeomSet 
		: public tintaCompObjContainer < tintaCompGeom, tintaGeomFactory >,
		  public Singleton<tintaGeomSet> {

			tintaGeomSet();
			~tintaGeomSet();			

			static tintaGeomSet* getPtr(void)	{
				return mPtr;
			}

	};

}
#endif
