/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_BUFFERIO_H_
#define _TINTA_BUFFERIO_H_

#include "tintaPredefine.h"
#include "tintaCommon.h"

namespace Tinta {
 

    template< class T >
    static int WriteToBuffer(m_int8* pBuffer, int iCurOffset, const T& Value){
        *(T*)(pBuffer + iCurOffset) = Value;
        return iCurOffset + sizeof(T);
    }

    template< class T >
    int WriteToBufferString(m_int8* pBuffer, int iCurOffset, const String& str){
        size_t offset = iCurOffset;
        for (size_t c = 0; c < str.length(); c++){
            offset = WriteToBuffer<T>(pBuffer, offset, str[c]);
        }
        // terminating symbol
        offset = WriteToBuffer<T>(pBuffer, offset, _M('\0'));
        return offset;
    }
    
    template< class T >
    static size_t ReadFromBuffer(const m_int8* pBuffer, size_t iCurOffset, T& Value){
        Value = *(const T*)(pBuffer + iCurOffset);
        return iCurOffset + sizeof(T);
    }


    template< class T >
    static size_t WriteArrayToBuffer(m_int8* pBuffer, size_t iCurOffset, const T* pValue, int iSize){

        mlMemcpy(pBuffer + iCurOffset, iSize * sizeof(T), (const m_int8*)pValue, iSize * sizeof(T));
        return iCurOffset + iSize * sizeof(T);
    }


    template< class T >
    static size_t ReadArrayFromBuffer(const m_int8* pBuffer, size_t iCurOffset, T* pValue, int iSize){

        mlMemcpy(pValue, iSize * sizeof(T), pBuffer + iCurOffset, iSize * sizeof(T));
        return iCurOffset + iSize * sizeof(T);
    }


    /*
        For packng string in to the packet
    */
    template< class T >
    size_t StringPackSize(const T &v){
        // size = data_length + char_size + data_size + \0
        return sizeof(m_uint32) + sizeof(m_uint8) + v.length()*sizeof(char_m) + sizeof(char_m);
    }

    /*
        Pack string in to the packet
    */
    template< class T, class T2>
    size_t PackString(m_int8* pBuffer, size_t iCurOffset, const T2& str){
        size_t offset = iCurOffset;
        // data_length = length * char_size + terminating_symbol
        offset = WriteToBuffer<m_uint32>( pBuffer, offset, str.length() * sizeof(T) + sizeof(T) ); // total length
        offset = WriteToBuffer<m_uint8>(pBuffer, offset, sizeof(T)); // char size
        for (size_t c = 0; c < str.length(); c++){
            offset = WriteToBuffer<T>(pBuffer, offset, str[c]);
        }
        // terminating symbol
        offset = WriteToBuffer<T>(pBuffer, offset, _M('\0'));
        return offset;


    }
    struct _CoreExport tintaIOUtil{
        static size_t UnpackString(const m_int8* pBuffer, String &rez, int offset_ = 0);
    };

    /*
        Common buffer with dynamic memory
    */
    struct _CoreExport tintaBufferIO
    {	
		tintaBufferIO();

		tintaBufferIO(const tintaBufferIO &rVal);

        tintaBufferIO(tintaBufferIO&& rVal);

		tintaBufferIO(size_t size);

		virtual ~tintaBufferIO();

        size_t GetSize() const;

		const m_int8* GetBuffer() const;
	
        m_int8* AllocateBuffer(size_t iSize);

        m_int8* GetBufferEx();

        bool SetData(const m_int8* pData, size_t iSize);

        tintaBufferIO * Copy() const;
		
        tintaBufferIO& operator = (const tintaBufferIO &rVal);

        tintaBufferIO& operator = (tintaBufferIO&& rVal);

        

    private:
	
	    m_int8* mBuffer;     
	    size_t   mSize;	
    };


typedef UNIQPTRDEF_T(tintaBufferIO) tintaBufferIOUnqPtr;
}
#endif 
