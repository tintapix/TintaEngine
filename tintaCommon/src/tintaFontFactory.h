/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_FONT_FACTORY_H_
#define _TINTA_FONT_FACTORY_H_

#include "tintaPredefine.h"
#include "tintaFactoryObj.h"
#include "tintaFont.h"


namespace Tinta {

    
	class _CoreExport tintaFontFactory
		: public tintaFactoryObj<tintaFont>
		, public AllocatedMemObject {
public:
		virtual ~tintaFontFactory(){}
	};

}

#endif
