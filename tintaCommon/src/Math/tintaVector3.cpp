/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaVector3.h"


namespace Tinta
{
    template<> const tintaVector3f  tintaVector3f::origin( 0.0f, 0.0f, 0.0f );
    template<> const tintaVector3f  tintaVector3f::xAxis( 1.0f, 0.0f, 0.0f );
    template<> const tintaVector3f  tintaVector3f::yAxis( 0.0f, 1.0f, 0.0f );
    template<> const tintaVector3f  tintaVector3f::zAxis( 0.0f, 0.0f, 1.0f );
    template<> const tintaVector3f  tintaVector3f::unitScale(1.0f, 1.0f, 1.0f);

}


