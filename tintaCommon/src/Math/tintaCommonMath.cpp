/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */





#include "tintaCommonMath.h"


namespace TintaMath {

    tintaRandomV2::tintaRandomV2(int seed )
                    : mSeed(seed){        
    }

    void	tintaRandomV2::setRandSeed(int seed ) {
        this->mSeed = seed;
    }

     int tintaRandomV2::getRandSeed(void) const {
        return mSeed;
    }

     int tintaRandomV2::randomInt(void) {

        return(((mSeed = mSeed * 214013L + 2531011L) >> 16) & 0x7fff);
    }

     int tintaRandomV2::randomInt(int max) {
        if (max == 0) {
            return 0;
        }
        return randomInt() % (max + 1);
    }

     int tintaRandomV2::randomInt(int min, int max) {

        if (max == 0 || (max - min) <= 0) {
            return max;
        }

        return  randomInt(max - min) + min;
    }

     float tintaRandomV2::randomFloat(void) {
        return (randomInt() / (float)(tintaRandomV2::MAX_RAND + 1));
    }

     float tintaRandomV2::randomFloatNeg(void) {
        return (2.0f * (randomFloat() - 0.5f));
    }
}

