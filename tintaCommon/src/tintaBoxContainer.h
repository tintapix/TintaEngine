/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_BOX_CONTAINER_H_
#define _TINTA_BOX_CONTAINER_H_

#include "tintaPredefine.h"

#include "tintaObjCreator.h"
#include "tintaArrayBox.h"
#include "tintaObjContainer.h"
#include "tintaSingleton.h"
#include <tintaISerialisableSet.h>

namespace Tinta {

	// to create singleton
	struct _CoreExport tintaBoxContainer 
		: public tintaObjContainer < tintaArrayBox, tintaArrayBoxObjFactory >,		  
          public tintaISerialisableSet,
          public tintaINamed {

			tintaBoxContainer();
			virtual ~tintaBoxContainer();

            virtual const StringBasic& getName() const;

            virtual StringBasic GetObjectName(m_uint32 key) const;

            virtual size_t GetBufferSize(m_uint32 key) const;

            virtual size_t packData(m_uint32 key, m_int8 *data, size_t offset) const;

            virtual size_t unpackData(const StringBasic &type, const m_int8 *data, size_t offset, m_uint32 &rezId);

            static const StringBasic strMapsContainerName;
	};

}
#endif