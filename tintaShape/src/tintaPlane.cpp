/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaGraphicaPrecompile.h"
#include "tintaPlane.h"

namespace Graphica{


	tintaPlane::tintaPlane (){	
	}


	tintaPlane::tintaPlane (const tintaPlane& plane)
        :mNormal(plane.mNormal)
	{
        mMag = plane.mMag;
	}
	

	tintaPlane::tintaPlane (const tintaVector3f& normal, float mag)
        :mNormal(normal)
	{
        mMag = mag;
	}
	

	tintaPlane::tintaPlane (const tintaVector3f& normal, const tintaVector3f& p)
        :mNormal(normal)
	{
        mMag = mNormal.dot(p);
	}
	

	tintaPlane::tintaPlane (const tintaVector3f& p0, const tintaVector3f& p1, const tintaVector3f& p2)
	{
        tintaVector3f edge1 = p1 - p0;
        tintaVector3f edge2 = p2 - p0;
        mNormal = edge1.crossUnit(edge2);
        mMag = mNormal.dot(p0);
	}
	

	tintaPlane& tintaPlane::operator= (const tintaPlane& rv)
	{
        if (this == &rv)
            return *this;

        mNormal = rv.mNormal;
        mMag = rv.mMag;
		return *this;
	}
	

	float tintaPlane::distanceTo (const tintaVector3f& rkP) const
	{
        return mNormal.dot(rkP) - mMag;
	}
	

	EnGeomSide tintaPlane::whichSide (const tintaVector3f& rkQ) const
	{
		float fDistance = distanceTo(rkQ);

		if (fDistance < (float)0.0){
            return SIDE_BACK;
		}

		if (fDistance > (float)0.0){
            return SIDE_FRONT;
		}

        return SIDE_NOSIDE;
	}


}