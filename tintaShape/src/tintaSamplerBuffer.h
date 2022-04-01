/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef TINTA_SAMPLER_BUFFER_H
#define TINTA_SAMPLER_BUFFER_H


#include "tintaTexture.h"
#include <Math/tintaVector4.h>

namespace Graphica {


class _CoreExport tintaSamplerBuffer
    : public  tintaResource < BufferID >  {

public:
    tintaSamplerBuffer();   

    tintaSamplerBuffer(SBufferDraw drawType, m_uint32 size );
   
    virtual ~tintaSamplerBuffer( );     

    /*
        Create buffer
    */
    void resize(m_uint32 size, bool clear = false);

    /*
        Clear data
    */
    void clear();

    /*
        Return data size
    */
    m_uint32 getDataSize() const;

    /*
        Return elements quantity
    */
    m_uint32 elements() const;

    tintaVector4f getValue( m_uint32 index ) const;    

    void setValue( m_uint32 index, const tintaVector4f &val);

    float getChannel(m_uint32 index, int channel ) const;

    void setChannel(m_uint32 index, int channel, float value);

    const void *getData() const;

    m_uint32 &getTextureId();

    SBufferDraw getType() const;

    void setType(SBufferDraw type);

    bool updated() const;

    void resetUpdate();

    static const int mComponents;

private:
    typedef std::vector<float> SamplerBufferData_t;

    SamplerBufferData_t mData;

    m_uint32 mTextureID;

    SBufferDraw mDrawType;
     
    bool mUpdated = true;

    

};

}

#endif
