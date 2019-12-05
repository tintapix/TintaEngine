/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_DISPATCHER_H_
#define _TINTA_DISPATCHER_H_

#include <tintaPredefine.h>

namespace Tinta {
        

    /*
        template class for events handling registered by id
    */
	template
	<
		class CDispatcherObject,
		typename TIdentifierType,
		typename TFunctionType = bool (CDispatcherObject::*)()
	>
	class tintaDispatcher
	{
	    		
		struct TData {
	        			
			TData( CDispatcherObject *Object, const TFunctionType Function )
				:mObj ( Object )
				,mFunc ( Function )
			{			
            }
            CDispatcherObject *mObj;			
			TFunctionType mFunc;			
		};
	
	public:

		
		bool Register (const TIdentifierType& Id, CDispatcherObject *Object, const TFunctionType FunctionType )	{
			TData Data( Object, FunctionType );
			return mFuncPtr.insert( TAssocMap_val( Id, Data )).second;
		}
		
		bool IsRegistered( const TIdentifierType& Id ) const{
			return mFuncPtr.find( Id )!=mFuncPtr.end();
		}		
		bool Unregister( const TIdentifierType & Id ){
			return mFuncPtr.erase( Id ) == 1;
		}

		
		bool UnregisterAll(){
			mFuncPtr.clear();
			return true;
		}	
		
		bool Dispatchering ( const TIdentifierType & Id ){
	    
			TMap_cit it = mFuncPtr.find( Id );
			if ( it != mFuncPtr.end() ){
				

				if ( !it->second.mObj )
					 return false;
	            
				return ( it->second.mObj->*it->second.mFunc)();
			}    
			
			return false;
		}

	
		template< class CArg >
		bool Dispatchering ( const TIdentifierType & Id, CArg & Arg ){

			TMap_cit it = mFuncPtr.find( Id );
			if ( it != mFuncPtr.end() ){				

				if ( !it->second.mObj )
					return false;

				return ( it->second.mObj->*it->second.mFunc)( Arg );
			}			
			return false;
		}    

	private:

		typedef std::map< const TIdentifierType, TData > TAssocMap;
		typedef typename std::map< const TIdentifierType, TData >::value_type TAssocMap_val;
		TAssocMap mFuncPtr;
		typedef typename TAssocMap::const_iterator TMap_cit;


	
	};

};

#endif 
