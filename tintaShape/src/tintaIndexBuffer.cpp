
#include "tintaGraphicaPrecompile.h"
#include "tintaIndexBuffer.h"

namespace Graphica {



tintaIndexBuffer::tintaIndexBuffer(int iIQuantity){
	CoreAssert(iIQuantity > 0,"iIQuantity > 0");

    mQuantity = iIQuantity;
	mData = ALLOC_T (int,mQuantity);
    memset(mData,0,mQuantity*sizeof(int));
    mOffset = 0;
}

tintaIndexBuffer::tintaIndexBuffer(const tintaIndexBuffer* pIBuffer){
    assert(pIBuffer);

    mQuantity = pIBuffer->mQuantity;

    mData = ALLOC_T (int,mQuantity);

    size_t uiSize = mQuantity * sizeof(int);

	Tinta::mlMemcpy( mData,uiSize,pIBuffer->mData,uiSize );

    mOffset = pIBuffer->mOffset;
}

tintaIndexBuffer::tintaIndexBuffer(){
    mQuantity = 0;
    mData = 0;
    mOffset = 0;
}

tintaIndexBuffer::~tintaIndexBuffer(){
   
    if ( mData )
	    FREE_T(mData);
    mData = NULL_M;
}

}

