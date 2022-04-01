/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_OBSERVED_H_ 
#define _TINTA_OBSERVED_H_

#include "tintaPredefine.h"
#include "tintaThread.h"
#include "tintaException.h"


namespace Tinta {

	

	struct IObservMaskable {
		virtual m_uint32 getObservMask() = 0;
	}; 


    /*
        Implements pattern observer / ovserved
    */
	template<typename T>
	struct tintaObservedBase {
public:
		typedef std::list< T* > callbacks_t;
		typedef typename callbacks_t::iterator callbacks_iter_t;

	protected:		
		tintaObservedBase(){mCurPos = m_vecCallbacks.begin();}

		mutable callbacks_t m_vecCallbacks;			

		// for iterations
		//mutable unsigned int m_uCurrentCallbackID;
		callbacks_iter_t mCurPos;

		T* movetoNext() {		

			if( mCurPos == m_vecCallbacks.end() )
				return NULL;
			mCurPos++;
			if( mCurPos == m_vecCallbacks.end() )
				return NULL;

			return *mCurPos;

		}	
	public:
		size_t addCallback( T * v){
			
            CoreAssert(v, "v==NULL");

			m_vecCallbacks.push_back( v );		

			return m_vecCallbacks.size() - 1;
		}

		T* removeCallback( T * v ) {

			callbacks_iter_t itFind = std::find(m_vecCallbacks.begin(), m_vecCallbacks.end(), v );
            CoreAssert(itFind != m_vecCallbacks.end(), "itFind == m_vecCallbacks.end()");

			T * ret = NULL;

			if( itFind == m_vecCallbacks.end() )
				return ret;

			if( mCurPos == itFind )
				ret = movetoNext();


			if(itFind != m_vecCallbacks.end() )
				m_vecCallbacks.erase( itFind );

			return ret;		
		}
	};


	template<typename T>
	struct tintaObserved 
		: public tintaObservedBase<T> {

			typedef std::list< T* > callbacks_t;
			typedef typename callbacks_t::iterator callbacks_iter_t;

		T* getFirst() {
			tintaObservedBase<T>::mCurPos = tintaObservedBase<T>::m_vecCallbacks.begin();
			if(tintaObservedBase<T>::mCurPos == tintaObservedBase<T>::m_vecCallbacks.end())
				return NULL;

			return *tintaObservedBase<T>::mCurPos;

		}
		T* getNext() {			
			return tintaObservedBase<T>::movetoNext();
		}	

	};


	template<typename T>
	struct tintaObservedMasked 
		: public tintaObservedBase<T> {	

		
		/*
			Returns first object that satisfying  mask, observer must implement IObservMaskable
		*/
		T* getFirst( m_uint32 mask ) {

			tintaObservedBase<T>::mCurPos = tintaObservedBase<T>::m_vecCallbacks.begin();
			while( tintaObservedBase<T>::mCurPos!= tintaObservedBase<T>::m_vecCallbacks.end() && !((*tintaObservedBase<T>::mCurPos)->getObservMask() & mask) )
				tintaObservedBase<T>::mCurPos++;
			if( tintaObservedBase<T>::mCurPos == tintaObservedBase<T>::m_vecCallbacks.end() )
				return NULL;

			return *tintaObservedBase<T>::mCurPos;

		}

		/*
			Returns next object that satisfying mask, observer must implement IObservMaskable
		*/
		T* getNext(  m_uint32 mask ) {	

			if(tintaObservedBase<T>::mCurPos == tintaObservedBase<T>::m_vecCallbacks.end())
				return NULL;

			tintaObservedBase<T>::mCurPos++;

			while( tintaObservedBase<T>::mCurPos!= tintaObservedBase<T>::m_vecCallbacks.end() && !((*tintaObservedBase<T>::mCurPos)->getObservMask() & mask)  )
				tintaObservedBase<T>::mCurPos++;

			if( tintaObservedBase<T>::mCurPos == tintaObservedBase<T>::m_vecCallbacks.end() )
				return NULL;

			return *tintaObservedBase<T>::mCurPos;
		}
	};
}


#endif
