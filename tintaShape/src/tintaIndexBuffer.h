
#ifndef TINTA_INDEXBUFFER_H
#define TINTA_INDEXBUFFER_H

#include "tintaGraphicaPredefine.h"
#include "tintaResource.h"

namespace Graphica {

class  _CoreExport tintaIndexBuffer
    :public AllocatedMemObject, 
     public tintaResource < BufferID >
{    

public:
    tintaIndexBuffer(int iIQuantity);
    tintaIndexBuffer(const tintaIndexBuffer* pIBuffer);
    virtual ~tintaIndexBuffer();


   // int operator[] (int i) const;
   // int& operator[] (int i);

   
    int  GetIndexQuantity () const;

    int* GetData ();
    
    const int* GetData () const;
    
    void SetIndexQuantity (int q);
    
    void SetOffset (int offset);

    int  GetOffset () const;
       
protected:
    
    tintaIndexBuffer();

    int  mQuantity;
    int* mData;
    int  mOffset;
};


inline int tintaIndexBuffer::GetIndexQuantity () const
{
	return mQuantity;
}

inline int* tintaIndexBuffer::GetData ()
{
	return mData;
}

inline const int* tintaIndexBuffer::GetData () const
{
	return mData;
}

/*
inline int IndexBuffer::operator[] (int i) const
{
	CoreAssert(0 <= i && i < mQuantity,"0 <= i && i < mQuantity");
	return mData[i];
}

inline int& IndexBuffer::operator[] (int i)
{
	CoreAssert(0 <= i && i < mQuantity,"0 <= i && i < mQuantity");
	return mData[i];
}
*/

inline void tintaIndexBuffer::SetIndexQuantity (int q)
{
	mQuantity = q;
}

inline void tintaIndexBuffer::SetOffset(int offset)
{
    CoreAssert(offset >= 0, "iOffset >= 0");
    mOffset = offset;
}

inline int tintaIndexBuffer::GetOffset () const{
	return mOffset;
}


//typedef Tinta::SharedPtr<IndexBuffer> IndexBufferPtr;



}

#endif
