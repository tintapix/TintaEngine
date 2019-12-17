/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */


#include "tintaBoxContainer.h"



namespace Tinta {


	tintaBoxContainer::tintaBoxContainer(){
	}

	tintaBoxContainer::~tintaBoxContainer(){
	}

    const StringBasic tintaBoxContainer::strMapsContainerName = "boxes";

    const StringBasic& tintaBoxContainer::getName() const {
        static StringBasic name = tintaBoxContainer::strMapsContainerName;
        return name;
    }

    StringBasic tintaBoxContainer::GetObjectName(m_uint32 key) const {
        const tintaArrayBox *v = getObj(key);
        if (v)
            return v->getName();
        return StringBasic();
    }

    size_t tintaBoxContainer::GetBufferSize(m_uint32 key) const{
        const tintaArrayBox *v = getObj(key);
        if ( v )
            return v->GetBufferSize();

        return 0;
    }

    size_t tintaBoxContainer::packData(m_uint32 key, m_int8 *data, size_t offset) const {

        const tintaArrayBox *v = getObj(key);
        if (v)
            return v->packData(data, offset);
        
        return 0;
    }

    size_t tintaBoxContainer::unpackData(const StringBasic &type, const m_int8 *data, size_t offset, m_uint32 &rezId){
        m_uint32 key = addObj(type);
        rezId = key;
        tintaArrayBox *v = getObjEx(key);
        if ( v ){            
            return v->unpackData(data, offset);
        }

        return 0;

    }


	// for export
	template<> Tinta::tintaBoxContainer* Tinta::Singleton<tintaBoxContainer>::mPtr = 0;

}