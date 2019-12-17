/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef __TINTA_SPHERE_3_H_
#define __TINTA_SPHERE_3_H_

#include "tintaVector3.h"

namespace Tinta
{

    template <typename T>
class _CoreExport tintaSphere3
{
public:

   
    tintaSphere3(){};
    template <typename T>
    tintaSphere3(const tintaVector3<T>& center, float radius)
        :mCenter(center),
        mRadius(radius)
    {
    }
    template <typename T>
    tintaSphere3(const tintaSphere3<T>& rkSphere)
        : mCenter(rkSphere.mCenterCenter),
        mRadius(rkSphere.mRadius)
    {
    }

    template <typename T>
    tintaSphere3<T>& operator= (const tintaSphere3<T>& rv)
    {
        mCenter = rv.mCenter;
        mRadius = rv.mRadius;
        return *this;
    }

    tintaVector3<T> mCenter;
    float          mRadius;
};

typedef tintaSphere3<float> tintaSphere3f;

}

#endif
