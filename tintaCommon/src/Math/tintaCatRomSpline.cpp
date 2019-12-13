/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaCatRomSpline.h"
#include "tintaCommon.h"


namespace Tinta {

/*
    Constants 
*/
#define CROO -0.5f
#define CR01 1.5f
#define CR02 -1.5f
#define CR03 0.5f
#define CR10 1.0f
#define CR11 -2.5f
#define CR12 2.0f
#define CR13 -0.5f
#define CR20 -0.5f
#define CR21 0.0f
#define CR22 0.5f
#define CR23 0.0f
#define CR30 0.0f
#define CR31 1.0f
#define CR32 0.0f
#define CR33 0.0f


tintaCatRomSpline::tintaCatRomSpline(void)
{
}

tintaCatRomSpline::~tintaCatRomSpline(void)
{
}

float tintaCatRomSpline::getValue(float x, int nknots, const float *knot)
{
    int span{0};
	int nspans = nknots - 3;

    /* factors of the cubic*/
	float cO, cl, c2, c3; 
    if (nspans < 1){
		return 0.f;
	}

	/* Find the appropriate 4-point span of the spline. */
	x = clamp(x, 0, 1) * nspans;
	span = (int) x;

	if ( span >= nknots - 3 )
		span = nknots - 3;

	x -= span;
	knot += span;

	/* Evaluate factors at x using Horner’s method*/
	c3 = CROO*knot[0] + CR01*knot[1] + CR02*knot[2] + CR03*knot[3];
	c2 = CR10*knot[0] + CR11*knot[1] + CR12*knot[2] + CR13*knot[3];
	cl = CR20*knot[0] + CR21*knot[1] + CR22*knot[2] + CR23*knot[3];
	cO = CR30*knot[0] + CR31*knot[1] + CR32*knot[2] + CR33*knot[3];

	return ( (c3*x + c2)*x + cl )*x + cO;
}

}
