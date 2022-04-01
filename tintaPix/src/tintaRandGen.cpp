/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaPrecompile.h"
#include "tintaRandGen.h"
#include <tintaException.h>


namespace Tinta {


tintaRandGen::tintaRandGen(void)
    :gen(rd()){
}

tintaRandGen::tintaRandGen( int seed )
    : gen(seed){
}

int tintaRandGen::getNext() {

    std::uniform_int_distribution<> num; //(1, 6);

    return num(gen);
}

int tintaRandGen::getNext( int min, int max ) {
    std::uniform_int_distribution<> num(min,max);

    return num(gen);
}

tintaRandGen* tintaRandGen::getPtr(void)
{
	return mPtr;
}

tintaRandGen::~tintaRandGen(void)
{
}

template<> tintaRandGen* Singleton<tintaRandGen>::mPtr = 0;

}
