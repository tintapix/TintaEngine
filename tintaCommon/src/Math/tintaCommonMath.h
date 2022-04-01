/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_COMMON_MATH_H__
#define _TINTA_COMMON_MATH_H__


#include <math.h>
#include <assert.h>
#include <algorithm>
#include <float.h>
#include <limits> 
#include "tintaConfig.h"
#include "tintaPlatform.h"
#include "tintaMemoryAllocator.h"


namespace TintaMath {

    /*
        Common mathematical constants and functions
    */
#define IEEE_FLOAT_MANTISSA_BITS	23
#define IEEE_FLOAT_EXPONENT_BITS	8
#define IEEE_FLOAT_EXPONENT_BIAS	127
#define IEEE_FLOAT_SIGN_BIT			31

#define IEEE_DOUBLE_MANTISSA_BITS	52
#define IEEE_DOUBLE_EXPONENT_BITS	11
#define IEEE_DOUBLE_EXPONENT_BIAS	1023
#define IEEE_DOUBLE_SIGN_BIT		63

#define IEEE_DBLE_MANTISSA_BITS	63
#define IEEE_DBLE_EXPONENT_BITS	15
#define IEEE_DBLE_EXPONENT_BIAS	0
#define IEEE_DBLE_SIGN_BIT		79

//detects float sign 1 + , 0 -
#define FLOATSIGNBITSET(f)		((*(const m_ulong32 *)&(f)) >> IEEE_FLOAT_SIGN_BIT)
#define FLOATNOTZERO(f)			((*(const m_ulong32 *)&(f)) & ~(1<<IEEE_FLOAT_SIGN_BIT) )
#define FLOATSIGNBITNOTSET(f)	((~(*(const m_ulong32 *)&(f))) >> IEEE_FLOAT_SIGN_BIT)

#define INTSIGNBITSET(i)		(((const m_ulong32)(i)) >> 31)

#define K_EPSILON    1.0e-6f
#define K_PI         3.1415926535897932384626433832795f
#define K2_PI        ((2)* (3.1415926535897932384626433832795f)) // 6.2831853071796


#define MIN_INT32         ((std::numeric_limits<m_int32>::min)())
#define MAX_INT32         ((std::numeric_limits<m_int32>::max)())
#define MIN_INT16         ((std::numeric_limits<m_int16>::min)())
#define MAX_INT16         ((std::numeric_limits<m_int16>::max)())
#define MIN_INT8         ((std::numeric_limits<m_int8>::min)())
#define MAX_INT8         ((std::numeric_limits<m_int8>::max)())

#define MIN_UINT32         ((std::numeric_limits<m_uint32>::min)())
#define MAX_UINT32         ((std::numeric_limits<m_uint32>::max)())
#define MIN_UINT16         ((std::numeric_limits<m_uint16>::min)())
#define MAX_UINT16         ((std::numeric_limits<m_uint16>::max)())
#define MIN_UINT8         ((std::numeric_limits<m_uint8>::min)())
#define MAX_UINT8         ((std::numeric_limits<m_uint8>::max)())

#define MIN_REAL32         ((std::numeric_limits<float>::min)())
#define MAX_REAL32         ((std::numeric_limits<float>::max)())

#define MIN_REAL64         ((std::numeric_limits<float>::min)())
#define MAX_REAL64         ((std::numeric_limits<float>::max)())



#define MAX_FLOAT FLT_MAX

#define  kScale  1000
	
	
	FORCEINL double floor(double val)	{
		return std::floor(val);

	}
	FORCEINL float floor(float val)	{
		return std::floor(val);
	}
	
		
	template<class T>
	const T& min(const T& a, const T& b)	{
		return a < b ? a : b;
	}

	template<class T>
	const T& max(const T& a, const T& b)	{
		return a < b ? b : a;
	}

	FORCEINL float ceil(float val)	{
		return std::ceil(val);
	}
	FORCEINL float exp(float val)	{
		return std::exp(val);
	}
	

	/*
        integral base-2 logarithm of the floating point value
    */
	FORCEINL int log2( float f ) {
		return ( ( (*reinterpret_cast<int *>(&f)) >> IEEE_FLOAT_MANTISSA_BITS ) & ( ( 1 << IEEE_FLOAT_EXPONENT_BITS ) - 1 ) ) - IEEE_FLOAT_EXPONENT_BIAS;
	}

	FORCEINL int roundInt(float val){

		return (int)(val + 0.5f);
	}
	FORCEINL int roundInt(double param)	{
		return (int)floor(param);
	}	

	/*
        round x up to the nearest power of 2
    */
	FORCEINL int	ceilPowerOfTwo( int x ){
		x--;
		x |= x >> 1;
		x |= x >> 2;
		x |= x >> 4;
		x |= x >> 8;
		x |= x >> 16;
		x++;
		return x;
	}

	/*
        round x down to the nearest power of 2
    */
	FORCEINL int	floorPowerOfTwo( int x ){
		return ceilPowerOfTwo( x ) >> 1;
	}
    
	template<class T>
	inline constexpr T abs(const T& a){
		return a < (T)0 ? -a : a;
	}
	

	/*
        Converts sign Radians value to unsign 
    */
	FORCEINL  float angleToUnsign( float angle ){		
		if( angle < 0.f ){
			return ( K2_PI - TintaMath::abs(angle) );
		}
		return angle;
	}

	/*
        returns the nearest integer
    */
	FORCEINL float rint( float f ){
		return floorf( f + 0.5f );
	}

    FORCEINL  float max( float f_lval,  float f_rval){
		return std::max( f_lval  , f_rval);
	}

    FORCEINL  float max( float fXVal,  float fYVal, float fZVal){
		return std::max( std::max( fXVal  , fYVal) , fZVal);
	}

    FORCEINL  float min( float f_lval,  float f_rval ){
		return  std::min( f_lval  , f_rval);
	}

    FORCEINL  float min( float fXVal,  float fYVal, float fZVal ){
		return std::min( std::min( fXVal  , fYVal), fZVal);
	}
    
	
    FORCEINL  int max( int f_lval,  int f_rval){
		return f_lval ^ ((f_lval ^ f_rval) & -(f_lval < f_rval));
	}

	
    FORCEINL  int min( int f_lval,  int f_rval ){
		//return  (int)(std::min( (int)f_lval  , (int)f_rval));
		return (f_rval ^ ((f_lval ^ f_rval) & -(f_lval < f_rval) ) );
	}



    FORCEINL float piecewiseInterp(float x, int nknots, const float *knot) {

        if (nknots == 0)
            return 0.f;

        int segments = nknots - 1;

        // find segment and parameter
        int i = (int)TintaMath::floor((float)x * (float)segments);

        float y0 = knot[i];
        float y1 = knot[i + 1];

        float x0 = (float)i / (float)segments;
        float x1 = (float)(i + 1) / (float)segments;

        // finding weights
        float w0 = (x1 - x) / (x1 - x0);
        float w1 = (x - x0) / (x1 - x0);

        return y0 * w0 + y1 * w1;
    }
	

	template<typename T>
    constexpr T pow( T val, T power )        { return std::pow( val,power ) ; }
	//double TintaMath::pow( double val, double power )        { return pow( val,power ); }
	//FORCEINL int TintaMath::pow( int val, int power )        { return (int)pow( (float)val,(float)power ); }

	template<typename T>
    constexpr T sqrt( T val )        { return std::sqrt( val ); }


	template<typename T>
    constexpr T invsqrt( T val )        { return 1/TintaMath::sqrt( val ); }

	FORCEINL constexpr bool oposSigns( int lVal, int rVal){
		return ((lVal ^ rVal) < 0);
	}
	
	template<typename T>
    constexpr bool isZero( T a ) {
		return (abs(a) < std::numeric_limits<T>::epsilon() );

	}   

    template<typename T>
    constexpr bool isZero(T a, T delta) {
        return (TintaMath::abs(a) < ( std::numeric_limits<T>::epsilon() + TintaMath::abs(delta) ));

    }

	
	template<typename T>
    constexpr bool areEqual( T a, T b )
	{
		return ( TintaMath::isZero( a-b ) );

	}  

    template<typename T>
    constexpr bool areEqual(T a, T b, T delta)
    {
        return ( TintaMath::isZero(a - b, delta) );

    }

	FORCEINL constexpr float toRad( float angle_degree) {
		return ( angle_degree * (K_PI/180.f) );

	}
	
	FORCEINL constexpr float toDeg( float angle_radians) {
		return ( angle_radians  * 180.f) / K_PI ;

	}  
    template<typename T>
	T sin( T a ){
		return std::sin(a);
	}

    template<typename T>
	T cos( T a ){
		return std::cos(a);
	}

    template<typename T>
    T tan(T a){
		return std::tan(a);
	} 

    template<typename T>
	T atan( T a ){
		return std::atan(a);
	} 

    template<typename T>
    T atan2(T a, T b) {
        return std::atan2(a,b);
    }

    template<typename T>
    T acos(T a) {
        return std::acos(a);
    }

    template<typename T>
	void sincos( T a, T& sina, T& cosa ) {
		sina = sin(a);
		cosa = cos(a);
	}  
	FORCEINL int floatHash( const float *v, const int numFloats )
	{
		int i, hash = 0;
		const int *ptr;

		ptr = reinterpret_cast<const int *>( v );
		for ( i = 0; i < numFloats; i++ ) {
			hash ^= ptr[i];
		}

		return hash;
	}

	class tintaRandomV2 {
	public:
		
        tintaRandomV2( int seed = 0);

        void	setRandSeed(int seed);

		int					getRandSeed( void ) const;

        /*
            random integer in the range [0, MAX_RAND]
        */
		int					randomInt( void );			

        /*
            random integer in the range [0, max]
        */
		int					randomInt( int max );

        /*
            random integer in the range [min, max]
        */
		int					randomInt( int min,  int max );	

        /*
            random number in the range [0.0f, 1.0f]
        */
		float				randomFloat( void );		

        /*
            random number in the range [-1.0f, 1.0f]
        */
		float				randomFloatNeg( void );		

        /*
            random number in the manual range [min, max]
        */
		template <typename T> 
		T	randomInterv( T min, T max  )	
		{			
			double dRatio = ( ( double )randomInt() )/( ( double )( tintaRandomV2::MAX_RAND ) );
			return min + ( max - min )*( ( T )dRatio );
		}

		static const int	MAX_RAND = 0x7fff; // 32767

	private:
		int					mSeed;
	};
    

	

    /*
	    The Interpolation function returns a value between a and b based on the value x. 
	    When x equals 0, it returns a, and when x is 1, it returns b. 
	    When x is between 0 and 1, it returns some value between a and b.	
    */
	template<typename T>
	FORCEINL T lInterp(const T &a, const T &b,const float &x){
		return  (T)( (1.f - x) * a) + (T)(x * b);
	}

    /*
	    The Interpolation function returns a value between a and b based on the value x. 
	    When x equals 0, it returns a, and when x is 1, it returns b. 
	    When x is between 0 and 1, it returns some value between a and b.	
    */
	template<typename T>
	FORCEINL T cosInterp(const T &a, const T &b,const float &x){
			float ft = x * K_PI;			
			float f = ( 1.f - TintaMath::cos( ft ) ) * 0.5f;
			return  (T)(( 1.f - f)* a) + (T)(f * b);
	}
	
	template <typename T>
	FORCEINL T cubeInterp(const T &v0, const T &v1, const T &v2, const T &v3, const float &x){
			T PVal = (v3 - v2) - (v0 - v1);
			T QVal = (v0 - v1) - PVal;
			T RVal = v2 - v0;
			T SVal = v1;

            return (T)((float)PVal * std::pow(x, 3) + (float)QVal * std::pow(x, 2) + (float)RVal * x + SVal);
	}

    

}
#endif
