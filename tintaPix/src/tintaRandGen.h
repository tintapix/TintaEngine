/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_RAND_GEN_H_
#define _TINTA_RAND_GEN_H_

#include <tintaPredefine.h>
#include <tintaSingleton.h>


namespace Tinta { 



class tintaRandGen : 
	public Tinta::Singleton< tintaRandGen >
{
public:
	tintaRandGen(void);
	tintaRandGen( int seed );
	virtual ~tintaRandGen(void);

	int getNext();

	int getNext(int min, int max );

	//void reset( );

    static tintaRandGen* getPtr(void);

private:
	//boost::mt11213b rng; 	
    std::random_device rd;
    std::mt19937 gen;
};

}


#endif