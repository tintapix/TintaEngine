/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */


#include "tintaBufferIO.h"
#include "tintaException.h"



namespace Tinta {
 


    size_t tintaIOUtil::UnpackString(  const m_int8* pBuffer, String &rez, int offset){

   // String rez;
    const m_int8 *buff = pBuffer;
    
    m_uint32 dataSize{ 0 };
    offset = ReadFromBuffer<m_uint32>(buff, offset, dataSize);
    m_uint8 charSize{ 0 };
    offset = ReadFromBuffer<m_uint8>(buff, offset, charSize);
    // subsrtuct charsize 
    size_t finalSize = dataSize;// -sizeof(m_uint8);
    if ( charSize == UTF8_SIZE ){

        StringBasic buffStr;
        UNIQPTRALLOC(char, nameArr, ALLOC_T(char, finalSize / sizeof(char) + sizeof(char)));
            offset = ReadArrayFromBuffer<char>(buff, offset, nameArr.get(), finalSize / sizeof(char));
        
        buffStr = nameArr.get();

        rez = std::move(buffStr);

        //rez = nameArr.get();
    }    

    return (size_t)offset;
}


    size_t tintaBufferIO::GetSize() const{
		return mSize;
	}


    const m_int8* tintaBufferIO::GetBuffer() const{
		return mBuffer;
	}

	
    m_int8* tintaBufferIO::AllocateBuffer(size_t iSize){
		if( mBuffer )
			DELETE_ARRAY_T( mBuffer, m_int8,  iSize);

		if( iSize == 0 )
		{
            // release
			mBuffer = NULL_M;
            mSize = 0;
			return NULL_M;
		}
		
		mBuffer = NEW_ARRAY_T(m_int8, iSize);
		mSize = iSize;
		

		::memset( mBuffer, 0, iSize );

		return mBuffer;
	}

    m_int8* tintaBufferIO::GetBufferEx(){
        CoreAssert( mBuffer,"mBuffer==NULL" );


		return mBuffer;
	}

    bool tintaBufferIO::SetData(const m_int8* pData, size_t iSize){
        CoreAssert( pData, "pData==NULL" );

		if( mSize < iSize )
			return false;


		
		mlMemcpy(mBuffer, iSize, pData, iSize);

		return true;
	}

    tintaBufferIO * tintaBufferIO::Copy() const{
		tintaBufferIO * pBuf = NEW_T(tintaBufferIO)();
		if ( !pBuf )
			return NULL_M;

		if ( pBuf->AllocateBuffer( GetSize() ) && pBuf->SetData( GetBuffer(), GetSize() ) )
			return pBuf;	

		return NULL_M;
	}	

    tintaBufferIO::tintaBufferIO() :
		mBuffer( NULL_M ),				
		mSize( 0 )		
	{
	}


    tintaBufferIO::tintaBufferIO(const tintaBufferIO &rVal) :
        mBuffer(NULL_M),
        mSize(0){
        *this = rVal;
    }

    tintaBufferIO::tintaBufferIO(size_t size) :
        mBuffer(NULL_M),
        mSize(size){
        AllocateBuffer(mSize);
    }

    tintaBufferIO& tintaBufferIO::operator = (const tintaBufferIO &rVal) {

        if (this == &rVal)
            return *this;

        if (rVal.GetSize() > 0) {
            AllocateBuffer(rVal.GetSize());
            SetData(rVal.GetBuffer(), GetSize());
        }
        return *this;
    }

    tintaBufferIO& tintaBufferIO::operator = (tintaBufferIO&& rVal) {

        if (this == &rVal)
            return *this;

        if (rVal.GetSize() > 0) {
            mSize = rVal.mSize;
            mBuffer = rVal.mBuffer;
            rVal.mBuffer = NULL_M;
            rVal.mSize = 0;
        }
        return *this;
    }

    tintaBufferIO::tintaBufferIO(tintaBufferIO&& rVal) :
        mBuffer(rVal.mBuffer),
        mSize( rVal.mSize ) {        
            rVal.mBuffer = NULL_M;
            rVal.mSize = 0;
    }

	
    tintaBufferIO::~tintaBufferIO(){
		if( mBuffer  )
			DELETE_ARRAY_T(mBuffer, m_int8,  mSize);
        mSize = 0;
	}


}

