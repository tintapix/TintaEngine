/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */


#ifndef _TINTA_OBJ_CONTAINER_H_
#define _TINTA_OBJ_CONTAINER_H_

#include "tintaObjCreator.h"

namespace Tinta {
	    


    /*
        creates and stores objectes created, using factory
    */
    template< typename TObj, typename TFactory >
    class  tintaObjContainer {
    protected:
        typedef struct ObjPair {
            ObjPair(TObj *obj, const StringBasic &name, m_uint32 key)
                : mpobj(obj)
                , mname(name)
                , mKey(key)
            {}

            ObjPair()
            {}

            TObj		   *mpobj = NULL_M;
            StringBasic mname;
            m_uint32        mKey = ZERO_ID;

        } ObjPair_t;

        typedef typename  std::list< ObjPair > t_obj_container;
        typedef  typename std::list< ObjPair >::iterator t_obj_iter;
        typedef  typename std::list< ObjPair >::const_iterator t_obj_citer;
        typedef typename Tinta::tintaObjContainer<TObj, TFactory>::t_obj_container::value_type cont_val_t;

        t_obj_container       mList;

        // keeping index increment
        std::atomic<m_uint32> mIndex;

        // selected object in current moment
        mutable m_uint32      mSelected;

        //to cache last search
        mutable TObj	*mCachedval;

        tintaObjCreator< TFactory, TObj > mCreator;

        mutable t_obj_citer mCIter;

        MUTEX_RECURSIVE(mChannelMapMutex)

    public:

        tintaObjContainer() :
              mIndex(ZERO_ID)
            , mSelected(ZERO_ID)
            , mCachedval(NULL_M)
            , mCIter(mList.begin())
        {}
        
        m_uint32 addObj(const StringBasic  &objName){
            
            TINTA_LOCK_RECUR_MUTEX_AUTO;
                          

            TObj* obj = mCreator.createObj(objName);

            //CoreAssert( obj, "TObj* obj = mCreator.createObj( objName );" );
            if ( obj ){

                incrIndex();
                if (mIndex == ZERO_ID) // next round after max_uint32
                    incrIndex();
                mList.push_back(cont_val_t(ObjPair(obj, objName, mIndex)));
                mCachedval = obj;
                mSelected = mIndex;
                updateIter();
                return mIndex;
            }

            return ZERO_ID;
        }

        

        size_t getSize() const {
            return  mList.size();
        }


        void clear(){

            TINTA_LOCK_RECUR_MUTEX_AUTO;


                t_obj_iter it_del = mList.begin();
            for (; it_del != mList.end(); it_del++){
                mCreator.releaseObj(it_del->mname, it_del->mpobj);
            }
            mList.clear();
            mSelected = ZERO_ID;
            mIndex = ZERO_ID;
            mCachedval = NULL_M;
            updateIter();

        }

        bool registerFactory(TFactory *objFact) {
            CoreAssert(objFact, "objFact");
            return mCreator.registerFactory(objFact);
        }


        // selects object by id value
        bool selectObj( m_uint32 objectId ){

            TINTA_LOCK_RECUR_MUTEX_AUTO;

            if ( objectId == mSelected )
                return true;

            t_obj_iter iter_find = std::find_if( mList.begin(), mList.end(),
                [objectId](const ObjPair &v){return v.mKey == objectId; } );

            if ( objectId != ZERO_ID && iter_find != mList.end() ) {
                mSelected = objectId;
                mCachedval = iter_find->mpobj;
                
                return true;
            }           

            return false;
        }


        bool delObj( m_uint32 index ){

            TINTA_LOCK_RECUR_MUTEX_AUTO;
                             
            t_obj_iter iter_find = std::find_if(mList.begin(), mList.end(),
                [index](const ObjPair &v){return v.mKey == index; });

            if ( iter_find == mList.end() )
                return false;

            mCreator.releaseObj( iter_find->mname, iter_find->mpobj );

            mList.erase( iter_find );

            if (mSelected == index){
                mSelected = ZERO_ID;
                mCachedval = NULL_M;
            }
            updateIter();

            return true;
           
        }       


        // gets first object or NULL_M if thre is no objects in container
        const TObj* getFirstObj( m_uint32 &index ) const {
            TINTA_LOCK_RECUR_MUTEX_AUTO;

            updateIter();
            if (mCIter == mList.end()){
                return NULL_M;
            }
            index = mCIter->mKey;
            return mCIter->mpobj;
        }

        // gets first object or NULL_M if thre is no objects in container
        const TObj* getNextObj(m_uint32 &index) const {
            TINTA_LOCK_RECUR_MUTEX_AUTO;
                
            //t_obj_citer iter_find = std::find_if(mList.begin(), mList.end(),
            //    [curIndex](const ObjPair &v){return v.mKey == curIndex; });
            if (mCIter != mList.end() && ++mCIter != mList.end()){

                index = mCIter->mKey;
                return mCIter->mpobj;
            }
            return NULL_M;

        }

        

        TObj* getObjEx() {
            TINTA_LOCK_RECUR_MUTEX_AUTO;
            if (mCachedval)
                return mCachedval;
            return NULL_M;
        }
            

        const TObj* getObj() const { 
            TINTA_LOCK_RECUR_MUTEX_AUTO;
            if (mCachedval)
                return mCachedval;
            return NULL_M;
        }
              
        // selecting only via explicit id
        const TObj* getObj(m_uint32 objectId) const {
            TINTA_LOCK_RECUR_MUTEX_AUTO;


                if (objectId == mSelected && mCachedval)
                    return mCachedval;

            t_obj_citer iter_find = std::find_if(mList.cbegin(), mList.cend(),
                [objectId](const ObjPair &v){ return v.mKey == objectId; });
            if (iter_find == mList.end())
                return NULL_M;
            mSelected = objectId;
            mCachedval = iter_find->mpobj;

            return mCachedval;
        }

        // selecting only via explicit id
        TObj* getObjEx(m_uint32 objectId){

            TINTA_LOCK_RECUR_MUTEX_AUTO;
                    

            if (objectId == mSelected && mCachedval)
                return mCachedval;

            t_obj_iter iter_find = std::find_if(mList.begin(), mList.end(),
                [objectId](const ObjPair &v){ return v.mKey == objectId; });
            if (iter_find == mList.end())
                return NULL_M;
            mSelected = objectId;
            mCachedval = iter_find->mpobj;
            
            return mCachedval;
        }

        // selecting only via explicit id
        StringBasic getObjName(m_uint32 objectId) const {
            TINTA_LOCK_RECUR_MUTEX_AUTO;
                t_obj_citer iter_find = std::find_if(mList.begin(), mList.end(),
                [objectId](const ObjPair &v){ return v.mKey == objectId; });
            if (iter_find == mList.end())
                return "";

            return iter_find->mname;
        }

        // selects object by id value
        void	 discardSel() {
            mSelected = ZERO_ID;
            mCachedval = NULL_M;
        }

        unsigned getIndexSel() const {
            TINTA_LOCK_RECUR_MUTEX_AUTO;
                return mSelected;
        }


    protected:
        // no allowing owerflow int max value
        void incrIndex(){            
            ++mIndex;            
        }

        void updateIter() const {
            mCIter = mList.begin();
        }

        AUTO_MUTEX;
    };


} 



#endif
