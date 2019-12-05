/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaVector2.h"

namespace Tinta
{	 

	template<> tintaVector2f  tintaVector2f::origin( 0.0f, 0.0f );
	template<> tintaVector2f tintaVector2f::xAxis( 1.0f, 0.0f );
	template<> tintaVector2f tintaVector2f::yAxis( 0.0f, 1.0f );
}

