/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaGraphicaPrecompile.h"
#include "tintaSphereBV.h"
#include <tintaGeomFuncs.h>


namespace Graphica 
{

 	
	tintaSphereBV::tintaSphereBV ()
	    :mSphere( tintaVector3f::origin, 0.0f )
	{
	}
	
    tintaSphereBV::tintaSphereBV(const tintaSphere3f& sphere)
	    :mSphere(sphere)
	{
	}

	void tintaSphereBV::computeFromData (const tintaVertexBuffer* vBuffer){
		
        if (vBuffer){

            int q = vBuffer->getVertexQuantity();

			mSphere.mCenter = tintaVector3f::origin;
			mSphere.mRadius = 0.0f;
			int i;
			for ( i = 0; i < q; i++ ){
                mSphere.mCenter += vBuffer->getPosition3(i);
			}
			mSphere.mCenter /= (float)q;

			for (i = 0; i < q; i++){

                tintaVector3f offset = vBuffer->getPosition3( i ) - mSphere.mCenter;

				float rad = offset.lengthSquared();
				if ( rad  > mSphere.mRadius ){
					mSphere.mRadius = rad;
				}
			}

			mSphere.mRadius = TintaMath::sqrt( mSphere.mRadius );
		}
	}	

    void tintaSphereBV::setOrientation( const tintaQuatern    &orientation) {

    }

    void tintaSphereBV::scale( float scaleFactor ){

        float toScale = TintaMath::abs(scaleFactor);

        if (toScale != mScaleFactor  ) {
            // restore        

            mSphere.mRadius /= mScaleFactor;        

            mScaleFactor = toScale;

            mSphere.mRadius *= mScaleFactor;
            
        }
    }
	
	EnGeomSide tintaSphereBV::whichSide (const tintaPlane& plane) const {

        float dist = plane.distanceTo(mSphere.mCenter);

		if (dist <= -mSphere.mRadius)
		{
            return SIDE_BACK;
		}

		if (dist >= mSphere.mRadius){
            return SIDE_FRONT;
		}

        return SIDE_NOSIDE;
	}

	
	void tintaSphereBV::copyFrom (const tintaBoundingVolume* sphere)
	{
        mSphere = ((tintaSphereBV*)sphere)->mSphere;
	}
	
    void tintaSphereBV::growToContain( const tintaBoundingVolume* sphere )
	{
        mSphere = MergeSpheres( mSphere, ( (tintaSphereBV*)sphere)->mSphere );
	}
	
	bool tintaSphereBV::contains ( const tintaVector3f& p ) const	{
        return distance(mSphere.mCenter, p) <= mSphere.mRadius;		
	}


    void tintaSphereBV::setCenter(const tintaVector3f& center)
    {
        mSphere.mCenter = center;
    }

    void tintaSphereBV::setRadius(float rad)
    {
        mSphere.mRadius = rad;
    }

    tintaVector3f tintaSphereBV::getCenter() const
    {
        return mSphere.mCenter;
    }

    float tintaSphereBV::getRadius() const
    {
        return mSphere.mRadius;
    }

    tintaSphere3f& tintaSphereBV::getSphere()
    {
        return mSphere;
    }

    const tintaSphere3f& tintaSphereBV::getSphere() const {
        return mSphere;
    }
}
