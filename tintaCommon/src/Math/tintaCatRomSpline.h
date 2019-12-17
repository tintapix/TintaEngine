/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_CATROM_SPLINE_H_
#define _TINTA_CATROM_SPLINE_H_

#include "tintaPredefine.h"


namespace Tinta {

	/*
		Implemention of the Catmull-Rom interpolating spline
		example :
		tintaCatRomSpline spline;
		float arr[] = {0.f, 0.5f, 0.1f, 0.f};
		for(int i = 0 ; i < 100; i++ ){			
			Tinta::tintaLogger::getPtr()->logMsgFast("spline x %f y %f" , (float)((float)i/100.f), spline.getValue( (float)((float)i/100.f) , sizeof(arr)/4, arr) );
		}		
	*/
	class tintaCatRomSpline
	{
	public:
		tintaCatRomSpline(void);

		static float getValue(float x, int nknots, const float *knot);	

		virtual ~tintaCatRomSpline(void);	

	};

}

#endif
