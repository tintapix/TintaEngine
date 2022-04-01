
#ifndef TINTA_RESOURCE_H
#define TINTA_RESOURCE_H

#include "tintaAttributes.h"

namespace Graphica
{

     template< class T >
     class  tintaResource
     {
     public:
        tintaResource() 
            : mId( NULL_M ){ }
 
        ~tintaResource(){
            if (mId)
                M_DELETE mId;
            mId = NULL_M;
        }

        T   *GetId() const {
            return mId;
        }
    
        void setName(const String &name){
            mName = name;
        }

        const String &getName() const{
            return mName;
        }

        T *createID() {
            if ( !mId )
                  mId = M_NEW T();
            return mId;
        }

        void releaseID() {
            if (mId)
                M_DELETE mId;
            mId = NULL_M;
        }
    protected:    
        T*      mId;        
	    String  mName; 
    };
         
}

#endif
