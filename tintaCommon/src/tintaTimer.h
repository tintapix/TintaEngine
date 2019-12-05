#ifndef _TINTA_TIMER_H_
#define _TINTA_TIMER_H_


#include "tintaPredefine.h"
#include "tintaStdHeaders.h"

namespace Tinta {

 /*
    Simple synchronous timer
 */
class tintaTimer
{

private:
	clock_t mStart;	

public:
	tintaTimer(void);
	~tintaTimer(void);
	
	void reset();	
    m_uint32 getMilliseconds();
    m_uint32 getMicroseconds();
};

}

#endif
