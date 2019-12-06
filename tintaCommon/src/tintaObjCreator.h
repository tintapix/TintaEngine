#ifndef _TINTA_OBJ_CREATOR_H_
#define _TINTA_OBJ_CREATOR_H_

namespace Tinta {
	
	
	/*
		class container for objects fabrics
	*/
	class tintaCompGeom;
    template< typename TFactory, typename TObj >
    class tintaObjCreator {
    public:
        typedef std::map< StringBasic, TFactory* > ObjFactories;
        typedef typename std::map< StringBasic, TFactory* >::iterator ObjFactories_it;
        typedef typename std::map< StringBasic, TFactory* >::const_iterator ObjFactories_cit;
        typedef typename std::map<StringBasic, TFactory*>::value_type ObjFactories_val;

        tintaObjCreator(void){}

        virtual ~tintaObjCreator(void){}

        bool registerFactory(TFactory *objFact){

            if (!objFact)
                return false;

            ObjFactories_it it = mObjectsFact.find(objFact->getName());
            //CoreAssert( it == mObjectsFact.end(),"it == mObjectsFact.end()" );

            if (it != mObjectsFact.end())
                return false;
            return mObjectsFact.insert(ObjFactories_val(objFact->getName(), objFact)).second;

        }

        TObj* createObj(const StringBasic &name){
            ObjFactories_it it = mObjectsFact.find(name);
            //CoreAssert( it != mObjectsFact.end(),"it != mObjectsFact.end()" );
            if (it == mObjectsFact.end())
                return 0;

            return it->second->createInstance();
        }

        void releaseObj(const StringBasic &name, TObj* pobj){
            ObjFactories_it it = mObjectsFact.find(name);

            //CoreAssert( it != mObjectsFact.end(),"it != mObjectsFact.end()" );

            if (it == mObjectsFact.end())
                return;
            it->second->releaseInstance(pobj);
        }

    protected:
        ObjFactories mObjectsFact;
    };
	
}
#endif
