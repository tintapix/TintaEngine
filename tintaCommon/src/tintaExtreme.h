/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_EXTREME_H_
#define _TINTA_EXTREME_H_

#include "tintaPredefine.h"
#include "tintaCoord2.h"
#include "tintaISerialisable.h"

namespace Tinta {

    /*
        Implements structure keeping extrem values
    */
	template < typename T >
	struct tintaExtreme
		: public tintaISerialisable {
		tintaExtreme()
			: mXMin( T() )	
			, mXMax( T() )	
			, mYMin( T() )					
			, mYMax( T() ){}

		tintaExtreme( const tintaExtreme &rVal )
			: mXMin( rVal.mXMin )
			, mXMax( rVal.mXMax )
			, mYMin( rVal.mYMin )
			, mYMax( rVal.mYMax ){}

		tintaExtreme( T xMin, T xMax, T yMin, T yMax )
			: mXMin( xMin )
            , mXMax( xMax )			
            , mYMin( yMin )
			, mYMax( yMax ){}
		T mXMin;
        T mXMax;
		T mYMin;		
		T mYMax;

		bool isZero() const {
			return ( mXMin + mYMin + mXMax + mYMax ) == T();
		}
		/*
            if new max value  is grater or new min value is less , then  updates value and returns result of the updating
		    true - object was updated
         */
		bool extend(const tintaCoord2<T> &point){			
			bool rez = false;

			if( mXMin > point.mValX ){
				mXMin = point.mValX;
				rez = true;
			}

			if(mXMax < point.mValX ){
				mXMax = point.mValX;
				rez = true;
			}

			if( mYMin > point.mValY ){
				mYMin = point.mValY;
				rez = true;
			}

			if( mYMax < point.mValY ){
				mYMax = point.mValY;
				rez = true;
			}
			return rez;			
		}
        		

		// if new max value  is grater or new min value is less , then  updates value and returns result of updating
		// true - if object has updated
		bool extend( const tintaExtreme &rv){			
			bool rez = false;

			if( mXMin > rv.mXMin ){
				mXMin = rv.mXMin;
				rez = true;
			}

			if(mXMax < rv.mXMax ){
				mXMax = rv.mXMax;
				rez = true;
			}

			if( mYMin > rv.mYMin ){
				mYMin = rv.mYMin;
				rez = true;
			}

			if( mYMax < rv.mYMax ){
				mYMax = rv.mYMax;
				rez = true;
			}
			return rez;			
		}

		// if new max value is less or new min value is grater, then  updates value and returns result of updating
		// true - if object has updated
 		bool narrow(const tintaExtreme &rv){			
 			bool rez = false;
 
 			if( mXMin < rv.mXMin ){
 				mXMin = rv.mXMin;
 				rez = true;
 			}
 
 			if(mXMax > rv.mXMax ){
 				mXMax = rv.mXMax;
 				rez = true;
 			}
 
 			if( mYMin < rv.mYMin ){
 				mYMin = rv.mYMin;
 				rez = true;
 			}
 
 			if( mYMax > rv.mYMax ){
 				mYMax = rv.mYMax;
 				rez = true;
 			}
 			return rez;				
 		}

		void clear() {
			mXMin = T(), mYMin= T(), mXMax= T(), mYMax = T();
		}

		void clear(const T &val) {
			mXMin = val, mYMin= val, mXMax= val, mYMax = val;
		}

		bool operator==( const tintaExtreme &rVal ){
			return mXMin == rVal.mXMin && mYMin == rVal.mYMin && mXMax == rVal.mXMax && mYMax == rVal.mYMax;
		}
		bool operator!=( const tintaExtreme &rVal ){
			return mXMin != rVal.mXMin || mYMin != rVal.mYMin || mXMax != rVal.mXMax || mYMax != rVal.mYMax;
		}

		tintaExtreme &operator=(const tintaExtreme &rval){
			if(&rval == this)
				return *this;
			mXMin = rval.mXMin;
			mYMin = rval.mYMin;
			mXMax = rval.mXMax;
			mYMax = rval.mYMax;
			return *this;
		}

		virtual size_t GetBufferSize() const{
			//mXMin + mYMin + mXMax + mYMax
			
			return sizeof(mXMin) + sizeof(mYMin) + sizeof(mXMax) + sizeof(mYMax);
		}

		virtual size_t packData( m_int8 *data, size_t offset ) const {
            assert(data);
			//	return offset;

			// preparing string
			int iCurOffset = offset;
			iCurOffset = WriteToBuffer<T>( data, iCurOffset, mXMin );		
			iCurOffset = WriteToBuffer<T>( data, iCurOffset, mYMin );		
			iCurOffset = WriteToBuffer<T>( data, iCurOffset, mXMax );		
			iCurOffset = WriteToBuffer<T>( data, iCurOffset, mYMax );	
			return (size_t)iCurOffset;
		}

		virtual size_t unpackData( const m_int8 *data, size_t offset ){
            assert(data);
			int iCurOffset = offset;
			iCurOffset = ReadFromBuffer< T >( data, iCurOffset, mXMin );
			iCurOffset = ReadFromBuffer< T >( data, iCurOffset, mYMin );
			iCurOffset = ReadFromBuffer< T >( data, iCurOffset, mXMax );
			iCurOffset = ReadFromBuffer< T >( data, iCurOffset, mYMax );
			return iCurOffset;
		}	
	} ;
	typedef tintaExtreme<int> TExtremValues2i;

}

#endif
