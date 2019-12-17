/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_BOX3_H_
#define _TINTA_BOX3_H_

#include "tintaVector3.h"

namespace Tinta {

/*
    Common box3 class
*/
template <typename T>
class  tintaBox3
{

private:
    static const m_uint8 axies = 3;
public:
    tintaBox3(){}
    template <typename T>
    tintaBox3(const tintaVector3<T>& cent,
        const tintaVector3<T>* axes, const T* extent)
        : mCenter(cent)
    {
        for (int i = 0; i < axies; i++)
        {
            mAxes[i] = axes[i];
            mGabarit[i] = extent[i];
        }
    }
    template <typename T>
    tintaBox3(const tintaVector3<T>& cent, const tintaVector3<T>& ax0,
        const tintaVector3<T>& ax1, const tintaVector3<T>& ax2, T ex0, T ex1, T ex2)
        :mCenter(cent)
    {
        mAxes[0] = ax0;
        mAxes[1] = ax1;
        mAxes[2] = ax2;
        mGabarit[0] = ex0;
        mGabarit[1] = ex1;
        mGabarit[2] = ex2;
    }



    template <typename T>
    void computeVert(tintaVector3<T> vert[8]) const
    {
        tintaVector3f axes[3] =
        {
            mGabarit[0] * mAxes[0],
            mGabarit[1] * mAxes[1],
            mGabarit[2] * mAxes[2]
        };

        vert[0] = mCenter - axes[0] - axes[1] - axes[2];
        vert[1] = mCenter + axes[0] - axes[1] - axes[2];
        vert[2] = mCenter + axes[0] + axes[1] - axes[2];
        vert[3] = mCenter - axes[0] + axes[1] - axes[2];
        vert[4] = mCenter - axes[0] - axes[1] + axes[2];
        vert[5] = mCenter + axes[0] - axes[1] + axes[2];
        vert[6] = mCenter + axes[0] + axes[1] + axes[2];
        vert[7] = mCenter - axes[0] + axes[1] + axes[2];
    }

    tintaVector3<T> mCenter;

    tintaVector3<T> mAxes[axies];

    T	 mGabarit[axies];

};

typedef tintaBox3<float> tintaBox3f;

}


#endif
