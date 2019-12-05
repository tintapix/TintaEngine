/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaVector4.h"

namespace Tinta
{

	template<> tintaVector4f  tintaVector4f::xAxis( 1.0f, 0.0f, 0.0f, 0.0f );
	template<> tintaVector4f  tintaVector4f::yAxis( 0.0f, 1.0f, 0.0f, 0.0f );	
	template<> tintaVector4f  tintaVector4f::zAxis( 0.0f, 0.0f, 1.0f, 0.0f );
	template<> tintaVector4f  tintaVector4f::wAxis( 0.0f, 0.0f, 0.0f, 1.0f );
	template<> tintaVector4f  tintaVector4f::origin( 0.0f, 0.0f, 0.0f, 0.0f );

}


