/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_ARRAY2d_H_
#define _TINTA_ARRAY2d_H_

#include "tintaPredefine.h"
#include "tintaExtreme.h"
#include "tintaException.h"
#include "tintaCoord2.h" 

namespace Tinta {

	
    /*
	    Implements common behavior for objects in the 2-dimensional array with shifted position.	
        Using for image objects, buffer containers, mapped values, OpenCL buffers
    */
	template < class T, class TExtrem >
	struct tintaArray2d {
	public:	

		tintaArray2d()
			: mExtrem(),
			mXSize(),
			mYSize()
		{

		}
		
		tintaArray2d(const tintaArray2d &rVal){
			*this = rVal;
		}

		tintaArray2d(const tintaExtreme<TExtrem> &extrem)
			:mExtrem(extrem),
			mXSize(mExtrem.mXMax - mExtrem.mXMin + 1),
			mYSize(mExtrem.mYMax - mExtrem.mYMin + 1){

			if (mXSize > 0 && mYSize > 0)
				mData.resize(mXSize*mYSize);
		}

		tintaArray2d(TExtrem xMin, TExtrem xMax,
                            TExtrem yMin, TExtrem yMax)
			: mExtrem(xMin, xMax, yMin, yMax),
			mXSize(mExtrem.mXMax - mExtrem.mXMin + 1),
			mYSize(mExtrem.mYMax - mExtrem.mYMin + 1){

			if (mXSize > 0 && mYSize > 0)
				mData.resize(mXSize*mYSize);
		}

		

        tintaArray2d(tintaArray2d&& rVal) 
            : mExtrem{ rVal.mExtrem } ,
              mXSize{ rVal.mXSize },
              mYSize{ rVal.mYSize },
              mData( std::move( rVal.mData ) ) {              
              rVal.mExtrem = tintaExtreme<TExtrem>();
              rVal.mXSize = 0;
              rVal.mYSize = 0;
        }
		~tintaArray2d(){
			clear();
		}
		tintaArray2d &operator=(const tintaArray2d &rVal){

			if (this == &rVal)
				return *this;
			
			mExtrem = rVal.mExtrem;
			mXSize = rVal.mXSize;
			mYSize = rVal.mYSize;
            mData = rVal.mData;

			return *this;
		}

        tintaArray2d &operator=(tintaArray2d&& rVal){

            if (this == &rVal)
                return *this;

            mExtrem = rVal.mExtrem;
            rVal.mExtrem = tintaExtreme<TExtrem>();
            mXSize = rVal.mXSize;
            rVal.mXSize = 0;
            mYSize = rVal.mYSize;           
            rVal.mYSize = 0;
            mData = std::move(rVal.mData);            

            return *this;
        }
	
    public:
		     
        /*
        *    Scales array with assigning  values in new positions
        */
        void scaleBox(int newW, int newH) {

            assert(newW > 0 && newH > 0);
  			
 			tintaArray2d<T, TExtrem > old( *this );		

            float factorX = (float)(old.width() - 1) / (float)(newW - 1);
            float factorY = (float)(old.height() - 1) / (float)(newH - 1);

            if ( prepare(mExtrem.mXMin, mExtrem.mXMin + newW - 1,
                mExtrem.mYMin, mExtrem.mYMin + newH - 1, false) ){				
                for ( int i = 0; i < newH; i++ ){					
                    for ( int k = 0; k < newW; k++ ){                        				
                        T val = old.getValReal((m_uint32)std::floor(k * factorX),
                                                        (m_uint32)std::floor(i * factorY));
						setValReal(toIndex(coord2dI_t(k, i)), val);
					}
				}
			}
		}
    
        /*
        *    Resizes box with saving values in old positions
        */
		bool resizeBox(TExtrem xMin, TExtrem xMax, TExtrem yMin, TExtrem yMax){
			return prepare(  tintaExtreme<TExtrem>(xMin, xMax, yMin, yMax), true );
		}		

		
		/*
		*    Preparing array, if flag bCopyOld is true - oldData copying into old positions
		*/
		bool prepare(const tintaExtreme<TExtrem> &extrem, bool bCopyOld = false){
			return prepare( extrem, *this, bCopyOld );
		}

		/*
		*    Preparing array, if flag bCopyOld is true - oldData copying into the new container
		*/
		bool prepare(TExtrem xMin, TExtrem xMax, TExtrem yMin, 
                            TExtrem yMax, bool bCopyOld = false){
			tintaExtreme<TExtrem> extrem(xMin, xMax, yMin, yMax);
			return prepare(extrem, *this, bCopyOld);
		}

		/*
		*	Preparing array, if flag bCopyOld is true - oldData copying into the new container
		*/
		bool prepare(const tintaExtreme<TExtrem> &extrem, 
                            const tintaArray2d<T, TExtrem> &from, bool bCopyOld = false){			

			if ( !bCopyOld ){
				return initSize(extrem);
			}
			else {
					
				tintaArray2d<T, TExtrem> old( from );				
				if ( initSize( extrem ) ){
					TExtrem xMinCorrect = TintaMath::max(extrem.mXMin, mExtrem.mXMin);
					TExtrem xMaxCorrect = TintaMath::min(extrem.mXMax, mExtrem.mXMax);
					TExtrem yMinCorrect = TintaMath::max(extrem.mYMin, mExtrem.mYMin);
					TExtrem yMaxCorrect = TintaMath::min(extrem.mYMax, mExtrem.mYMax);

					if ( xMaxCorrect > xMinCorrect && yMaxCorrect > yMinCorrect ){
						for ( TExtrem x = xMinCorrect; x <= xMaxCorrect; x++ ){
							for ( TExtrem y = yMinCorrect; y <= yMaxCorrect; y++ ){
								if ( old.inBoundary(x, y) ){
									setVal(x, y, old.getVal(x, y));
								}
							}
						}
					}
					return true;
				}
			}					
			return false;
		}
        
        /*
        *   Shifts boundaries
        */
		void shift(  const coord2dI_t &offset ) {
			mExtrem.mXMin += offset.mValX;
			mExtrem.mXMax += offset.mValX;
			mExtrem.mYMin += offset.mValY;
			mExtrem.mYMax += offset.mValY;
		}
		
		
		/*
		*    Checks the boundary in with real indices
		*/
		bool inBoundaryReal(TExtrem x, TExtrem y) const {
			return x >= 0 && y >= 0 && x < mXSize && y < mYSize;
		}

		/*
		*   Checks the boundary
		*/
		bool inBoundary( TExtrem x, TExtrem y ) const {
			return inBoundaryReal( x - mExtrem.mXMin, y - mExtrem.mYMin );
		}		
        
		virtual m_uint32 width() const {
			return mXSize;
		}

        virtual m_uint32 height() const {
			return mYSize;
		}

		int getxMin() const {
			return mExtrem.mXMin;
		}

		int getxMax() const {
			return mExtrem.mXMax;
		}

		int getyMin() const {
			return mExtrem.mYMin;
		}

		int getyMax() const {
			return mExtrem.mYMax;
		}

		const tintaExtreme<TExtrem> &getExtrem() const {
			return mExtrem;
		}
                
        /*  
        * Clears data and extreme
        */
		void clear( ) {
			mData.clear(); 
			
			mExtrem.clear(0);
			mXSize = 0;
			mYSize = 0;
		}
		
		void filldata( T val ) {						
			fill( mData.begin(), mData.end(), val );
			
		}

		bool setVal( TExtrem x, TExtrem y, T val ) {
			if( inBoundaryReal( x - mExtrem.mXMin, y - mExtrem.mYMin) ){
				m_uint32 pos = toIndex( x - mExtrem.mXMin , y - mExtrem.mYMin );
				CoreAssert(mData.size() > pos, "mData.size() > pos");
				mData.at(pos) = val;
				return true;
			}			
			return false;
		}

		T getVal( TExtrem x, TExtrem y ) const {
			if( inBoundaryReal( x - mExtrem.mXMin, y - mExtrem.mYMin ) ){
				//return mMatrix[x - mExtrem.mXMin][y - mExtrem.mYMin];
				m_uint32 pos = toIndex( x - mExtrem.mXMin , y - mExtrem.mYMin );
				CoreAssert(mData.size() > pos, "mData.size() > pos");
				return mData.at(pos);
			}
			return T();			
		}

		/*
			Get value using real position in array
		*/
		T getValReal( m_uint32  pos ) const {
			if( pos < mData.size() ){				
				return mData.at( pos );
			}
			return T();			
		}

		/*	
			allows work with box in vector(stack) mode
		*/
		bool pushBack( const T &val ){
			// if we have Y dimension push is not allowed
			if( mExtrem.mYMin == 0 
				&& mExtrem.mYMax == 0 
				&& mExtrem.mYMin == 0 ){				

				mData.push_back( val );
				mExtrem.mXMax++;
				mXSize++;
				return true;
			}
			return false;
		}

		/*	
			allows work with box in vector(stack) mode
		*/
		bool popBack( ) {
			// if we have Y dimension pop is not allowed
			if( mExtrem.mYMin == 0 
				&& mExtrem.mYMax == 0 
				&& mExtrem.mYMin == 0 
				&& mData.size() > 0 ){		

				mData.pop_back();
				mExtrem.mXMax--;
				mXSize--;
				return true;
			}
			return false;
		}

		/*
			Set value using real position in the array
		*/
		bool setValReal( m_uint32  pos, const T &val ) {
			if( pos < mData.size() ){				
				mData.at( pos ) = val;
				return true;
			}					
			return false;
		}

		/*
			Set value using real position in the array without boundary test
		*/
		void setFast( m_uint32  pos, const T &val ) {				
				mData.at( pos ) = val;			
		}

		/*	
			Returns value by real indices
		*/
		T getValReal(TExtrem x, TExtrem y) const {
			if( inBoundaryReal( x, y ) ){				
				m_uint32 pos = toIndex( x, y );
				CoreAssert(mData.size() > pos, "mData.size() > pos");
				return mData.at(pos);
			}
			return T();			
		}

		T *getPtr( size_t &size ) {			

			size = mData.size();
			if( size == 0 )
				return 0;

			return &mData[0];
		}

		T *getPtr( ) {			

			if( mData.size() == 0 )
				return NULL;

			return &mData[0];
		}

		virtual const void *getMemPtr() const  {

			if (mData.size() == 0)
				return NULL;

			return (void*)&mData[0];
		}

        virtual void *getMemPtrEx(){

			if (mData.size() == 0)
				return NULL;

			return (void*)&mData[0];
		}

		const T *getPtr( size_t &size  ) const {
			size = mData.size();
			if( size == 0 )
				return NULL;

			return &mData[0];
		}

		const T *getPtr(  ) const {			
			if( mData.size() == 0 )
				return NULL;

			return &mData[0];
		}

		/*
			Returns data size in bytes elements quantity * sizeof(T)
		*/
		size_t getDataSize( ) const {
				return mData.size() * sizeof( T );
		}

		/*
			Returns data type size in bytes sizeof(T)
		*/
		size_t getTypeSize( ) const {
				return sizeof( T );
		}

		/*
			Returns elements quantity
		*/
		virtual size_t getSize( ) const {
				return mData.size();
		}
		
		/*
			Returns relative coordinates by real
		*/
		void getRelative(TExtrem xReal, TExtrem yReal, TExtrem &xRelat, TExtrem &yRelat){
			xRelat = xReal + mExtrem.mXMin;
			yRelat = yReal + mExtrem.mYMin;
		}

		void setValFast(TExtrem x, TExtrem y, T val) {			
			CoreAssert(inBoundaryReal(x - mExtrem.mXMin, y - mExtrem.mYMin), "out of boundary");
			m_uint32 pos = toIndex( x - mExtrem.mXMin , y - mExtrem.mYMin );
			mData.at(pos) = val;		
		}

		T getValFast(TExtrem x, TExtrem y) const {
			//return mMatrix[x - mExtrem.mXMin][y - mExtrem.mYMin];	
			CoreAssert(inBoundaryReal(x - mExtrem.mXMin, y - mExtrem.mYMin), "out of boundary");
			m_uint32 pos = toIndex( x - mExtrem.mXMin , y - mExtrem.mYMin );
			return mData.at(pos);
		}

		const std::vector< T > &getArray()const {
			return mData;
		}
		protected:

		virtual m_uint32    toIndex(const coord2dI_t& coord) const {
			return coord.mValY * mXSize + coord.mValX;
		}

		virtual m_uint32    toIndex(TExtrem x, TExtrem y) const {
			return y * mXSize + x;
		}

		virtual coord2dI_t  getCoord(m_uint32 index) const {
			int y_pos = (TExtrem)(index / mXSize);
			int x_pos = index - mXSize * y_pos;

			return coord2dI_t(x_pos, y_pos);
		}

		bool initSize(const tintaExtreme<TExtrem> &extrem){

			TExtrem xSize = extrem.mXMax - extrem.mXMin + 1;
			TExtrem ySize = extrem.mYMax - extrem.mYMin + 1;
			if (xSize > 0 && ySize > 0) {
				mData.clear();
				mXSize = xSize;
				mYSize = ySize;
				mData.resize(mXSize * mYSize);
				mExtrem = extrem;
				return true;
			}
			return false;
		}
	protected:
		typedef std::vector< T > array_t;
		// shift
		tintaExtreme<TExtrem> mExtrem;
		TExtrem mXSize;
		TExtrem mYSize;
		// data
		array_t mData;
		
	};

    template < class T, class TExtrem >
    size_t getBufferSizeArray2d(const tintaArray2d< T, TExtrem > &array2d){
        // extreme + elements + data
        return array2d.getExtrem().GetBufferSize() + sizeof(size_t) + array2d.getDataSize();
    }

    template < class T, class TExtrem >
    size_t packArray2d(const tintaArray2d< T, TExtrem > &array2d,
        m_int8 *data, size_t offset) {
        size_t iCurOffset = offset;
        iCurOffset = array2d.getExtrem().packData(data, iCurOffset);
        iCurOffset = WriteToBuffer<sizetype_t>(data, iCurOffset, array2d.getSize());
        if (array2d.getSize() > 0)
            iCurOffset = WriteArrayToBuffer<T>(data, iCurOffset, array2d.getPtr(), array2d.getSize());
        return iCurOffset;
    }

    template < class T, class TExtrem >
    size_t unpackArray2d(tintaArray2d< T, TExtrem > &array2d, const m_int8 *data, size_t offset){

        size_t iCurOffset = offset;


        tintaExtreme<TExtrem> ext;
        iCurOffset = ext.unpackData(data, iCurOffset);

        array2d.prepare(ext);
        size_t elements = 0;
        iCurOffset = ReadFromBuffer< sizetype_t >(data, iCurOffset, elements);
        if (elements)
            iCurOffset = ReadArrayFromBuffer<T>(data, iCurOffset, array2d.getPtr(), elements);

        return iCurOffset;
    }

    template < class String, class TExtrem >
    size_t getBufferSizeArray2dStr(const tintaArray2d< String, TExtrem > &array2d){
        // extreme + elements + str_data
        size_t rez = array2d.getExtrem().GetBufferSize() + sizeof(sizetype_t);// +StringPackSize() + array2d.getDataSize();

        size_t q = 0;
        const String *s = array2d.getPtr(q);
        for (size_t i = 0; i < q; i++){
            rez += sizeof(sizetype_t); //string buffer size
            rez += StringPackSize(*(s++));
        }

        return rez;
    }


    template < class String, class TExtrem >
    size_t packArray2dStr(const tintaArray2d< String, TExtrem > &array2d,
        m_int8 *data, size_t offset) {
        size_t iCurOffset = offset;
        iCurOffset = array2d.getExtrem().packData(data, iCurOffset);
        iCurOffset = WriteToBuffer<sizetype_t>(data, iCurOffset, array2d.getSize());
        if (array2d.getSize() > 0) {
            size_t q = 0;
            const String *s = array2d.getPtr(q);
            for (size_t i = 0; i < q; i++){
                StringPackSize(*s);
                //iCurOffset = WriteToBuffer<sizetype_t>(data, iCurOffset, buffSize);
                iCurOffset = PackString<char_m, String>(data, iCurOffset, *(s++));
            }
        }
        return iCurOffset;
    }

    template < class String, class TExtrem >
    size_t unpackArray2dStr(tintaArray2d< String, TExtrem > &array2d, const m_int8 *data, size_t offset){

        size_t iCurOffset = offset;

        tintaExtreme<TExtrem> ext;
        iCurOffset = ext.unpackData(data, iCurOffset);

        array2d.prepare(ext);
        size_t elements = 0;
        iCurOffset = ReadFromBuffer< sizetype_t >(data, iCurOffset, elements);

        for (size_t k = 0; k < elements; k++){

            String str;
            //size_t buffSize = 0;
            //iCurOffset = ReadFromBuffer< sizetype_t >(data, iCurOffset, buffSize);
            iCurOffset = tintaIOUtil::UnpackString(data, str, iCurOffset);
            if (str.length() > 0)
                array2d.setValReal(k, str);            
        }

        return iCurOffset;
    }


	typedef tintaArray2d<m_uint32, m_int32> tintaArray2dUInt_t;
	typedef tintaArray2d<float, m_int32> tintaArray2dFloat_t;
    
    typedef tintaArray2d<tintaVector3f, m_int32> tintaArray2dVec3f_t;
	typedef tintaArray2d<m_int32, m_int32> tintaArray2dInt_t;
	typedef tintaArray2d<String, m_int32> tintaArray2dString_t;
	typedef tintaArray2d<m_int16, m_int32> tintaArray2dInt16_t;
	typedef tintaArray2d<m_byte, m_int32> tintaArray2dByte_t;
	typedef tintaArray2dUInt_t TMap2dUInt_t;
	typedef tintaArray2dFloat_t TMap2dFloat_t;
	
	
	typedef TMap2dUInt_t TUIntMap;
	typedef  tintaArray2dInt_t TIntMap;

	//typedef tintaArray2dUInt_t TUIntMap;
}


#endif
