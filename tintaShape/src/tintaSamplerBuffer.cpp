/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaGraphicaPrecompile.h"
#include "tintaSamplerBuffer.h"

namespace Graphica {

    const int tintaSamplerBuffer::mComponents = 4;

    tintaSamplerBuffer::tintaSamplerBuffer() : 
                            mTextureID( 0 ), mDrawType(STATIC_DRAW){}

    tintaSamplerBuffer::tintaSamplerBuffer(SBufferDraw drawType , m_uint32 size ) : 
                                        mTextureID(0), mDrawType(drawType) {
        resize( size, true );
    }	

    tintaSamplerBuffer::~tintaSamplerBuffer(){	
        clear();
	}    

    void tintaSamplerBuffer::resize( m_uint32 size, bool bclear ) {
        if ( size > 0 )
            mData.resize(size * mComponents);      

        if ( bclear )
            clear();
    }

    void tintaSamplerBuffer::clear() {
        std::fill(mData.begin(), mData.end(), 0.f);
    }
        

    m_uint32 tintaSamplerBuffer::getDataSize() const {
        return (m_uint32)mData.size() * sizeof(float);
    }

    m_uint32 tintaSamplerBuffer::elements() const {
        return (m_uint32)mData.size();
    }

  
    tintaVector4f tintaSamplerBuffer::getValue( m_uint32 index ) const {
        m_uint32 pos = index * mComponents;
        if (pos + 3 < mData.size() )
            return tintaVector4f(mData.at(pos), mData.at(pos + 1), mData.at(pos + 2), mData.at(pos + 3) );

        return tintaVector4f(0.f, 0.f, 0.f, 0.f);
    }

    void tintaSamplerBuffer::setValue( m_uint32 index, const tintaVector4f &val ) {
        m_uint32 pos = index * mComponents;
        if (pos + 3 < mData.size() ) {
            mData.at(pos) = val.mx;
            mData.at(pos +1) = val.my;
            mData.at(pos +2) = val.mz;
            mData.at(pos +3) = val.mw;

            mUpdated = true;
        }
    }


    float tintaSamplerBuffer::getChannel(m_uint32 index, int channel) const {
        m_uint32 pos = index * mComponents;
        if ( pos + channel < mData.size() ) {           
            return mData.at( pos + channel );           
        }
        return 0.f;
    }

    void tintaSamplerBuffer::setChannel(m_uint32 index, int channel, float value) {
        m_uint32 pos = index * mComponents;
        if ( pos + channel < mData.size() ) {
            mData.at( pos + channel ) = value;
            mUpdated = true;
        }
    }

    bool tintaSamplerBuffer::updated() const {
        return mUpdated;
    }

    void tintaSamplerBuffer::resetUpdate() {
        mUpdated = false;
    }

    const void *tintaSamplerBuffer::getData() const {

        if ( mData.size() > 0 )
            return (void*)( &mData[0] );

        return NULL_M;
    }

    m_uint32 &tintaSamplerBuffer::getTextureId() {
        return mTextureID;
    }

    SBufferDraw tintaSamplerBuffer::getType() const {
        return mDrawType;
    }

    void tintaSamplerBuffer::setType(SBufferDraw type) {
        mDrawType = type;
    }

}