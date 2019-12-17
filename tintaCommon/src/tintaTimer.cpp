
#include "tintaTimer.h"

namespace Tinta {

tintaTimer::tintaTimer(void) {
}

tintaTimer::~tintaTimer(void) {
}

void tintaTimer::reset(){
	
    mStart = clock();

}

m_uint32 tintaTimer::getMilliseconds(){
	clock_t now = clock();
    return (now - mStart) / (CLOCKS_PER_SEC / 1000);

}


m_uint32 tintaTimer::getMicroseconds(){
    clock_t now = clock();
    return (now - mStart) / (CLOCKS_PER_SEC / 1000000);
}

}


