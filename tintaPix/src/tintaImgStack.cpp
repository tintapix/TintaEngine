/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaPrecompile.h"
#include "tintaImgStack.h"

// for export

namespace Tinta {    

    const StringBasic tintaImgStack::strCanvasContainerName = "images";

    tintaImgStack::~tintaImgStack() {

    }
    const StringBasic& tintaImgStack::getName() const {
        static StringBasic name = tintaImgStack::strCanvasContainerName;
        return name;
    }

    StringBasic tintaImgStack::GetObjectName(m_uint32 key) const {
        const tintaUInt8Image *v = getImage(key);
        if (v)
            return v->getName();
        return StringBasic();
    }

    size_t tintaImgStack::GetBufferSize( m_uint32 key ) const{
        const tintaUInt8Image *v = getImage(key);
        if (v)
            return v->GetBufferSize();// +StringPackSize(getName()) + StringPackSize(v->getName());

        return 0;
    }

    size_t tintaImgStack::packData( m_uint32 key, m_int8 *data, size_t offset ) const {
        size_t offsetcur = offset;
        
        const tintaUInt8Image *v = getImage(key);
        if ( v ){
            //offsetcur = WriteToBuffer<m_uint32>(data, offsetcur, v->getName().length() * sizeof(char_m) + sizeof(char_m));
            //offsetcur = PackString<String>(data, offsetcur, getName());
            //offsetcur = PackString<String>(data, offsetcur, v->getName());
            return v->packData(data, offsetcur);
        }

        return 0;
    }

    size_t tintaImgStack::unpackData( const StringBasic &type, const m_int8 *data, size_t offset, m_uint32 &rezId ){
        m_uint32 id = addImg( type ); //getImageEx(key);
        rezId = id;
        tintaUInt8Image *v = getImageEx(id);
        if ( v ){
            size_t offsetcur = offset;
            //offsetcur = tintaIOUtil::UnpackString(data, offsetcur, getName());
            //offsetcur = tintaIOUtil::UnpackString(data, offsetcur, v->getName());
            return v->unpackData(data, offsetcur);
        }

        return 0;

    }

}