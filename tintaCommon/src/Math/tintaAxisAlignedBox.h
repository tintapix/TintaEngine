/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_AXIS_ALIGNED_BOX__H_
#define _TINTA_AXIS_ALIGNED_BOX__H_


#include "tintaPredefine.h"

namespace Tinta
{

    template<typename T>
class tintaAxisAlignedBox
{
public:
    
    tintaAxisAlignedBox<T>() : mAxes{ axies }{}
    
    tintaAxisAlignedBox<T>(T xMin, T xMax, T yMin, 
                                T yMax, T zMin, T zMax){
        mMin[0] = xMin;
        mMax[0] = xMax;
        mMin[1] = yMin;
        mMax[1] = yMax;
        mMin[2] = zMin;
        mMax[2] = zMax;
    }
    
    template <T>
    bool hasxOverlap( const tintaAxisAlignedBox<T>& box) const{
        return mMax[0] >= box.mMin[0]
            && mMin[0] <= box.mMax[0];
    }
    template <T>
    bool hasyOverlap(const tintaAxisAlignedBox& box) const {
        return mMax[1] >= box.mMin[1]
            && mMin[1] <= box.mMax[1];
    }
    template <T>
    bool haszOverlap(const tintaAxisAlignedBox& box) const {
        return mMax[2] >= box.mMin[2]
            && mMin[2] <= box.mMax[2];
    }
    template <T>
    bool intersection(const tintaAxisAlignedBox& box) const{
        for (int i = 0; i < mAxes; i++){
            if (mMax[i] < box.mMin[i] || mMin[i] > box.mMax[i]){
                return false;
            }
        }
        return true;
    }
    
    template <T>
    bool findIntersection(const tintaAxisAlignedBox& box, 
                                        tintaAxisAlignedBox& intersect) const {
        
        for (auto i = 0; i < mAxes; i++) {
            if (mMax[i] < box.Min[i] || mMin[i] > box.mMax[i])
            {
                return false;
            }
        }

        for (auto i = 0; i < mAxes; i++){
            if (mMax[i] <= box.mMax[i])
                intersect.mMax[i] = mMax[i];
            else
                intersect.mMax[i] = box.mMax[i];

            if (mMin[i] <= box.mMin[i])
                intersect.mMin[i] = box.mMin[i];
            else
                intersect.mMin[i] = mMin[i];
            
        }
        return true;
    }
    T mMin[axies];
    T mMax[axies];
    unsigned mAxes;
private:
    const m_uint8 axies = 3;
};


typedef tintaAxisAlignedBox<float> tintaAxisAlignedBoxf;

}

#endif
