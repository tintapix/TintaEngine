/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_GEOM_FUNCS_H_
#define _TINTA_GEOM_FUNCS_H_


#include <Math/tintaBox3.h>
#include <Math/tintaSphere3.h>
#include <Math/tintaQuatern.h>

namespace Tinta
{
    FORCEINL bool InBox(const tintaVector3f& p, const tintaBox3f& box)	{

        tintaVector3f diff = p - box.mCenter;
		for (int i = 0; i < 3; i++)	{
            float f = diff.dot(box.mAxes[i]);
            if (TintaMath::abs(f) > box.mGabarit[i]) {
				return false;
			}
		}
		return true;
	}

    FORCEINL tintaSphere3f MergeSpheres(const tintaSphere3f& sphere1, const tintaSphere3f& sphere2)
	{
        tintaVector3f diff = sphere2.mCenter - sphere1.mCenter;
        float sqr = diff.lengthSquared();
        float radDiff = sphere2.mRadius - sphere1.mRadius;
        float diffSqr = radDiff*radDiff;

        if (diffSqr >= sqr){
            return (radDiff >= 0.f ? sphere2 : sphere1);
		}

        float len = TintaMath::sqrt(sqr);
        tintaSphere3f kSphere;
        if ( !TintaMath::isZero(len) ) {
            float fCoeff = (len + radDiff) / (2.f*len);
            kSphere.mCenter = sphere1.mCenter + fCoeff*diff;
		}
		else {
            kSphere.mCenter = sphere1.mCenter;
		}

        kSphere.mRadius = ( 0.5f*(len + sphere1.mRadius + sphere2.mRadius) );

		return kSphere;
	}
	
    FORCEINL bool InSphere(const tintaVector3f& p, const tintaSphere3f& sphere){
        tintaVector3f diff = p - sphere.mCenter;
        return diff.length() <= sphere.mRadius;
	}
}
#endif