/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaGraphicaPrecompile.h"
#include "tintaVertexBuffer.h"

namespace Graphica {



tintaVertexBuffer::tintaVertexBuffer ( const tintaAttributes& attrib, int vQuantity)
    : mAttributes(attrib) {
	CoreAssert(vQuantity > 0,"iVertexQuantity > 0");

    mVertexQuantity = vQuantity;

    mVertexSize = mAttributes.getChannelQuantity();

    mChannelQuantity = mVertexQuantity * mVertexSize;

    mData = ALLOC_T (float,mChannelQuantity);

    memset(mData,0,mChannelQuantity*sizeof(float));
}

tintaVertexBuffer::tintaVertexBuffer (const tintaVertexBuffer* rv){
	CoreAssert(rv,"rv == NULL");

    mAttributes = rv->mAttributes;

    mVertexQuantity = rv->mVertexQuantity;

    mVertexSize = mAttributes.getChannelQuantity();

    mChannelQuantity = mVertexQuantity*mVertexSize;

    mData = ALLOC_T (float,mChannelQuantity);

    size_t uiSize = mChannelQuantity*sizeof(float);

	Tinta::mlMemcpy( mData, uiSize, rv->mData, uiSize );
}

tintaVertexBuffer::tintaVertexBuffer () {
    mVertexQuantity = 0;
    mVertexSize = 0;
    mChannelQuantity = 0;
    mData = 0;
}

tintaVertexBuffer::~tintaVertexBuffer (){       
	FREE_T (mData);
}

float* tintaVertexBuffer::getPositions (int i){
    if (mAttributes.hasPosition() && 0 <= i && i < mVertexQuantity){
        int index = mVertexSize*i + mAttributes.getPositionOffset();
        return mData + index;
    }
    return 0;
}

const float* tintaVertexBuffer::getPositions(int i) const {
    if ( mAttributes.hasPosition() && 0 <= i && i < mVertexQuantity)  {
        int index = mVertexSize * i + mAttributes.getPositionOffset();
        return mData + index;
    }
    return 0;
}

float* tintaVertexBuffer::getNormals ( int i ) {
    if ( mAttributes.hasNormal() && 0 <= i && i < mVertexQuantity )  {
        int index = mVertexSize*i + mAttributes.getNormalOffset();
        return mData + index;
    }
    return 0;
}

const float* tintaVertexBuffer::getNormals(int i) const {
    if (mAttributes.hasNormal() && 0 <= i && i < mVertexQuantity)
    {
        int index = mVertexSize * i + mAttributes.getNormalOffset();
        return mData + index;
    }
    return 0;
}


float* tintaVertexBuffer::getTCoords (int unit, int i)
{
    if (mAttributes.hasTCoord(unit) && 0 <= i && i < mVertexQuantity)
    {
        int index = mVertexSize*i + mAttributes.getTCoordOffset(unit);
        return mData + index;
    }
    return 0;
}

const float* tintaVertexBuffer::getTCoords(int unit, int i) const {
    if ( mAttributes.hasTCoord(unit) && 0 <= i && i < mVertexQuantity){

        int index = mVertexSize * i + mAttributes.getTCoordOffset(unit);

        return mData + index;
    }
    return 0;
}


float& tintaVertexBuffer::getPosition1 (int i)
{
	CoreAssert(mAttributes.getPositionChannels() == 1,"mAttributes.getPositionChannels() == 1");
    int index = mVertexSize*i + mAttributes.getPositionOffset();
    return *(mData + index);
}

float tintaVertexBuffer::getPosition1 (int i) const
{
    CoreAssert( mAttributes.getPositionChannels() == 1,"mAttributes.getPositionChannels() == 1");
    int index = mVertexSize*i + mAttributes.getPositionOffset();
    return *(mData + index);
}

tintaVector2f& tintaVertexBuffer::getPosition2 (int i)
{
	CoreAssert(mAttributes.getPositionChannels() == 2,"mAttributes.getPositionChannels() == 2");
    int index = mVertexSize*i + mAttributes.getPositionOffset();
    return *(tintaVector2f*)(mData + index);
}

tintaVector2f tintaVertexBuffer::getPosition2 (int i) const
{
	CoreAssert(mAttributes.getPositionChannels() == 2,"mAttributes.getPositionChannels() == 2");
    int index = mVertexSize*i + mAttributes.getPositionOffset();
    return *(tintaVector2f*)(mData + index);
}

tintaVector3f& tintaVertexBuffer::getPosition3 (int i)
{
 	CoreAssert(mAttributes.getPositionChannels() == 3,"mAttributes.getPositionChannels() == 3");
    int index = mVertexSize*i + mAttributes.getPositionOffset();
    return *(tintaVector3f*)(mData + index);
}

tintaVector3f tintaVertexBuffer::getPosition3 (int i) const
{
	CoreAssert(mAttributes.getPositionChannels() == 3,"mAttributes.getPositionChannels() == 3");
    int index = mVertexSize*i + mAttributes.getPositionOffset();
    return *(tintaVector3f*)(mData + index);
}

tintaVector4f& tintaVertexBuffer::getPosition4 (int i)
{
	CoreAssert(mAttributes.getPositionChannels() == 4,"mAttributes.getPositionChannels() == 4");
    int index = mVertexSize*i + mAttributes.getPositionOffset();
    return *(tintaVector4f*)(mData + index);
}

tintaVector4f tintaVertexBuffer::getPosition4 (int i) const
{
    CoreAssert(mAttributes.getPositionChannels() == 4,"mAttributes.getPositionChannels() == 4");
    int index = mVertexSize*i + mAttributes.getPositionOffset();
    return *(tintaVector4f*)(mData + index);
}



float& tintaVertexBuffer::getNormal1 (int i)
{
	CoreAssert(mAttributes.getNormalChannels() == 1,"mAttributes.getNormalChannels() == 1");
    int index = mVertexSize*i + mAttributes.getNormalOffset();
    return *(mData + index);
}

float tintaVertexBuffer::getNormal1 (int i) const
{
    CoreAssert(mAttributes.getNormalChannels() == 1,"mAttributes.getNormalChannels() == 1");
    int index = mVertexSize*i + mAttributes.getNormalOffset();
    return *(mData + index);
}

tintaVector2f& tintaVertexBuffer::getNormal2 (int i)
{
    CoreAssert(mAttributes.getNormalChannels() == 2,"mAttributes.getNormalChannels() == 2");
    int index = mVertexSize*i + mAttributes.getNormalOffset();
    return *(tintaVector2f*)(mData + index);
}

tintaVector2f tintaVertexBuffer::getNormal2 (int i) const
{
    CoreAssert(mAttributes.getNormalChannels() == 2,"mAttributes.getNormalChannels() == 2");
    int index = mVertexSize*i + mAttributes.getNormalOffset();
    return *(tintaVector2f*)(mData + index);
}

tintaVector3f& tintaVertexBuffer::getNormal3 (int i)
{
    CoreAssert(mAttributes.getNormalChannels() == 3,"mAttributes.getNormalChannels() == 3");
    int index = mVertexSize*i + mAttributes.getNormalOffset();
    return *(tintaVector3f*)(mData + index);
}

tintaVector3f tintaVertexBuffer::getNormal3 (int i) const
{
    CoreAssert(mAttributes.getNormalChannels() == 3,"mAttributes.getNormalChannels() == 3");
    int index = mVertexSize*i + mAttributes.getNormalOffset();
    return *(tintaVector3f*)(mData + index);
}

tintaVector4f& tintaVertexBuffer::getNormal4 (int i)
{
    CoreAssert(mAttributes.getNormalChannels() == 4,"mAttributes.getNormalChannels() == 4");
    int index = mVertexSize*i + mAttributes.getNormalOffset();
    return *(tintaVector4f*)(mData + index);
}

tintaVector4f tintaVertexBuffer::getNormal4 (int i) const
{
    CoreAssert(mAttributes.getNormalChannels() == 4,"mAttributes.getNormalChannels() == 4");
    int index = mVertexSize*i + mAttributes.getNormalOffset();
    return *(tintaVector4f*)(mData + index);
}

float& tintaVertexBuffer::getTextureCoord1 (int iUnit, int i)
{
    CoreAssert(mAttributes.getTCoordChannels(iUnit) == 1,"mAttributes.getTCoordChannels(iUnit) == 1");
    int index = mVertexSize*i + mAttributes.getTCoordOffset(iUnit);
    return *(mData + index);
}

float tintaVertexBuffer::getTextureCoord1 (int iUnit, int i) const
{
    CoreAssert(mAttributes.getTCoordChannels(iUnit) == 1,"mAttributes.getTCoordChannels(iUnit) == 1");
    int index = mVertexSize*i + mAttributes.getTCoordOffset(iUnit);
    return *(mData + index);
}

tintaVector2f& tintaVertexBuffer::getTextureCoord2 (int iUnit, int i)
{
    CoreAssert(mAttributes.getTCoordChannels(iUnit) == 2,"mAttributes.getTCoordChannels(iUnit) == 2");
    int index = mVertexSize*i + mAttributes.getTCoordOffset(iUnit);
    return *(tintaVector2f*)(mData + index);
}

tintaVector2f tintaVertexBuffer::getTextureCoord2 (int iUnit, int i) const
{
    CoreAssert(mAttributes.getTCoordChannels(iUnit) == 2,"mAttributes.getTCoordChannels(iUnit) == 2");
    int index = mVertexSize*i + mAttributes.getTCoordOffset(iUnit);
    return *(tintaVector2f*)(mData + index);
}

tintaVector3f& tintaVertexBuffer::getTextureCoord3 (int iUnit, int i)
{
    CoreAssert(mAttributes.getTCoordChannels(iUnit) == 3,"mAttributes.getTCoordChannels(iUnit) == 3");
    int index = mVertexSize*i + mAttributes.getTCoordOffset(iUnit);
    return *(tintaVector3f*)(mData + index);
}

tintaVector3f tintaVertexBuffer::getTextureCoord3 (int iUnit, int i) const
{
    CoreAssert(mAttributes.getTCoordChannels(iUnit) == 3,"mAttributes.getTCoordChannels(iUnit) == 3");
    int index = mVertexSize*i + mAttributes.getTCoordOffset(iUnit);
    return *(tintaVector3f*)(mData + index);
}

tintaVector4f& tintaVertexBuffer::getTextureCoord4 (int iUnit, int i)
{
	CoreAssert(mAttributes.getTCoordChannels(iUnit) == 4,"mAttributes.getTCoordChannels(iUnit) == 4");
    int index = mVertexSize*i + mAttributes.getTCoordOffset(iUnit);
    return *(tintaVector4f*)(mData + index);
}

tintaVector4f tintaVertexBuffer::getTextureCoord4 (int iUnit, int i) const
{
    CoreAssert(mAttributes.getTCoordChannels(iUnit) == 4,"mAttributes.getTCoordChannels(iUnit) == 4");
    int index = mVertexSize*i + mAttributes.getTCoordOffset(iUnit);
    return *(tintaVector4f*)(mData + index);
}



void tintaVertexBuffer::packBuffer( int& size, float*& buffer) const
{
    
    std::vector<m_uint32> outData;    

    const m_uint32* data = NULL_M;    

    for (int i = 0, j; i < mVertexQuantity; i++) {
        if ( mAttributes.hasPosition() ) {

            int channels = mAttributes.getPositionChannels();

            data = (m_uint32*)getPositions( i );
           
            for (j = 0; j < channels; j++)
            {
                outData.push_back(*data++);
            }
            
        }

        /*if ( attrib.hasNormal() ){
           // iIChannels = rkIAttr.getNormalChannels();
            iVBChannels = mAttributes.getNormalChannels();
            puiData = (m_uint32*)NormalTuple(i);

            for (j = 0; j < iVBChannels; j++)
            {
                outData.push_back(*puiData++);
            }

        }*/
    }

    for ( int i = 0, j; i < mVertexQuantity; i++ ) {

        for ( int unit = 0; unit < (int)mAttributes.getMaxTCoords(); unit++ ) {
            if ( mAttributes.hasTCoord( unit ) ){
                
                int channels = mAttributes.getTCoordChannels(unit);
                data = (m_uint32*)getTCoords(unit, i);
                
                for ( j = 0; j < channels; j++ ){
                    outData.push_back( *data++ );
                }                
            }
        }
    }           

    size = (int)outData.size();

    if ( !buffer )    
        buffer = ALLOC_T(float, size);    

    size_t uiSize = size *sizeof(float);

	Tinta::mlMemcpy(buffer, uiSize, &outData.front(),uiSize);
}



tintaAttributes& tintaVertexBuffer::getAttributes()
{
    return mAttributes;
}

const tintaAttributes& tintaVertexBuffer::getAttributes() const
{
    return mAttributes;
}

int tintaVertexBuffer::getVertexSize() const
{
    return mVertexSize;
}

int tintaVertexBuffer::getVertexQuantity() const
{
    return mVertexQuantity;
}

int tintaVertexBuffer::getChannelQuantity() const
{
    return mChannelQuantity;
}

float* tintaVertexBuffer::getData()
{
    return mData;
}

const float* tintaVertexBuffer::getData() const
{
    return mData;
}

void tintaVertexBuffer::setVertexQuantity(int iVQuantity)
{
    mVertexQuantity = iVQuantity;
}


}