/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaGraphicaPrecompile.h"
#include "tintaBoxBV.h"

namespace Graphica
{

    tintaBoxBV::tintaBoxBV()
        : mBox(tintaVector3f::origin, tintaVector3f::xAxis,
            tintaVector3f::yAxis, tintaVector3f::zAxis, 1.0f, 1.0f, 1.0f)
    {
    }

    tintaBoxBV::tintaBoxBV(const tintaBox3f& box)
        : mBox(box)
    {
    }

    tintaBoxBV::~tintaBoxBV() {
    }


    void tintaBoxBV::setCenter(const tintaVector3f& center) {
        mBox.mCenter = center;
    }

    tintaVector3f tintaBoxBV::getCenter() const {
        return mBox.mCenter;
    }

    tintaBox3f& tintaBoxBV::getBox() {
        return mBox;
    }

    const tintaBox3f& tintaBoxBV::getBox() const {
        return mBox;
    }

    void tintaBoxBV::setRadius(float rad) {
        mBox.mGabarit[0] = rad;
        mBox.mGabarit[1] = rad;
        mBox.mGabarit[2] = rad;
    }

    float tintaBoxBV::getRadius() const {

        float rad = mBox.mGabarit[0];

        if (rad < mBox.mGabarit[1]) {
            rad = mBox.mGabarit[1];
        }
        if (rad < mBox.mGabarit[2]) {
            rad = mBox.mGabarit[2];
        }
        return rad;
    }

    void tintaBoxBV::scale(float scaleFactor) {

        float toScale = TintaMath::abs(scaleFactor);

        if (scaleFactor != mScaleFactor) {

            // restore
            mBox.mGabarit[0] /= mScaleFactor;
            mBox.mGabarit[1] /= mScaleFactor;
            mBox.mGabarit[2] /= mScaleFactor;

            mBox.mAxes[0] /= mScaleFactor;
            mBox.mAxes[1] /= mScaleFactor;
            mBox.mAxes[2] /= mScaleFactor;

            // update 
            mScaleFactor = toScale;
            mBox.mGabarit[0] *= mScaleFactor;
            mBox.mGabarit[1] *= mScaleFactor;
            mBox.mGabarit[2] *= mScaleFactor;

            mBox.mAxes[0] *= mScaleFactor;
            mBox.mAxes[1] *= mScaleFactor;
            mBox.mAxes[2] *= mScaleFactor;
        }
    }


    void tintaBoxBV::setOrientation(const tintaQuatern &orientation) {
        mOrientation = orientation;
    }

    void tintaBoxBV::computeFromData(const tintaVertexBuffer* vBuff) {

        if (vBuff) {
            int q = vBuff->getVertexQuantity();

            int i;
            for (i = 0; i < q; i++) {
                //    std::cout << "Position " << vBuff->Position3(i).mx << " " << vBuff->Position3(i).my << " " << vBuff->Position3(i).mz << "\n";
                mBox.mCenter += vBuff->getPosition3(i);
            }
            mBox.mCenter /= (float)q;

            //std::cout <<  "Begin vert \n";

            mBox.mGabarit[0] = 0.f;
            mBox.mGabarit[1] = 0.f;
            mBox.mGabarit[2] = 0.f;

            for (i = 0; i < q; i++) {

                tintaVector3f offset = vBuff->getPosition3(i) - mBox.mCenter;

                if (abs(offset.mx) > mBox.mGabarit[0]) {
                    mBox.mGabarit[0] = TintaMath::abs(offset.mx);
                }
                if (abs(offset.my) > mBox.mGabarit[1]) {
                    mBox.mGabarit[1] = TintaMath::abs(offset.my);
                }
                if (abs(offset.mz) > mBox.mGabarit[2]) {
                    mBox.mGabarit[2] = TintaMath::abs(offset.mz);
                }
            }
            setRadius(TintaMath::max(TintaMath::max(mBox.mGabarit[0], mBox.mGabarit[1]), mBox.mGabarit[2]));
            //std::cout << "Gabarit "<< mBox.mGabarit[0] << " " << mBox.mGabarit[1] << " " << mBox.mGabarit[2] << "\n";
           // std::cout << "Center "<< mBox.mCenter.mx << " "<< mBox.mCenter.mx << " " << mBox.mCenter.mz << "\n";
           // std::cout << "End vert \n";            

        }
    }


    EnGeomSide tintaBoxBV::whichSide(const tintaPlane& plane) const {

        /* float projCenter = plane.mNormal.dot(mBox.mCenter) - plane.mMag;

         float fAbs0 = TintaMath::abs(plane.mNormal.dot(mBox.mAxes[0]));
         float fAbs1 = TintaMath::abs(plane.mNormal.dot(mBox.mAxes[1]));
         float fAbs2 = TintaMath::abs(plane.mNormal.dot(mBox.mAxes[2]));

         float projRadius = mBox.mGabarit[0] * fAbs0 + mBox.mGabarit[1] * fAbs1 + mBox.mGabarit[2] * fAbs2;

         if ( projCenter - projRadius >= 0.0f ){
             return SIDE_FRONT;
         }
         if ( projCenter + projRadius <= 0.0f ){
             return SIDE_BACK;
         }
         */
        return SIDE_NOSIDE;
    }

    bool tintaBoxBV::testIntersection(const tintaVector3f& rkOrigin,
        const tintaVector3f& rkDirection, float fTMin, float fTMax) const
    {
        return true;
    }

    bool tintaBoxBV::testIntersection(const tintaBoundingVolume* box) const {
        return true;
    }

    void tintaBoxBV::copyFrom(const tintaBoundingVolume* box) {
        mBox = ((tintaBoxBV*)box)->mBox;
    }

    void tintaBoxBV::growToContain(const tintaBoundingVolume* box) {

    }

    bool tintaBoxBV::contains(const tintaVector3f& point) const {

        // TODO Bug
        //tintaVector3f rotPoint = mOrientation * point;

        /*tintaVector3f rotPoint = mBox.mCenter + mScaleFactor * mOrientation * (point - mBox.mCenter);
        std::cout << rotPoint.mx << " " << rotPoint.my<<"\n";
        tintaVector2f x = { mBox.mCenter.mx - mBox.mGabarit[0], mBox.mCenter.mx + mBox.mGabarit[0] };
        tintaVector2f y = { mBox.mCenter.my - mBox.mGabarit[1], mBox.mCenter.my + mBox.mGabarit[1] };
        tintaVector2f z = { mBox.mCenter.mz - mBox.mGabarit[2], mBox.mCenter.mz + mBox.mGabarit[2] };

        return (rotPoint.mx >= x.mx && rotPoint.mx <= x.my
            && rotPoint.my >= y.mx && rotPoint.my <= y.my
            && rotPoint.mz >= z.mx && rotPoint.mz <= z.my);
        */





        float rad = TintaMath::max(TintaMath::max(mBox.mGabarit[0], mBox.mGabarit[1]), mBox.mGabarit[2]);
        return distance(point, mBox.mCenter) < rad;
    }
}