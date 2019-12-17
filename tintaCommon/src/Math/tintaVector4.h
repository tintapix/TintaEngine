/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef __TINTA_VEWCTOR4__H__
#define __TINTA_VEWCTOR4__H__

#include "tintaPredefine.h"
#include "tintaCommonMath.h"

namespace Tinta
{
template<typename T>
	class tintaVector4
	{
	public:		
		tintaVector4<T>() {}
		tintaVector4<T>( T _x, T _y, T _z, T _w ) :
		  mx(_x)
		, my(_y)
		, mz(_z)
		, mw(_w){
        }

		
		tintaVector4<T>(const tintaVector4<T>& other) : 
		mx( other.mx ),
		my( other.my ),
		mz( other.mz ),
		mw( other.mw ){
        }   

		tintaVector4<T>& operator=(const tintaVector4<T>& other){
			
			if ( this == &other )
				return *this;

			mx = other.mx;
			my = other.my;	
			mz = other.mz;	
			mw = other.mw;
			return *this;
		} 
		
		
		float& operator[]( m_uint32 i )          { return (&mx)[i]; }
		float operator[]( m_uint32 i ) const { return (&mx)[i]; }

		float length() const{		
			return sqrt( mx*mx + my*my + mz*mz + mw*mw);
		}   
		float lengthSquared() const{
			return (mx*mx + my*my+ mz*mz+ mw*mw);

		}   	

		
		template<class U>
		friend float distSquared( const tintaVector4<U>& p0, const tintaVector4<U	>& p1 );

		
		bool operator==( const tintaVector4<T>& other ) const{
			if (   areEqual( other.mx, mx )
				&& areEqual( other.my, my )
				&& areEqual( other.mz, mz )
				&& areEqual( other.mw, mw ))
				return true;

			return false;   
		}   

		bool operator!=( const tintaVector4<T>& other ) const{
			if ( areEqual( other.mx, mx )
				&& areEqual( other.my, my )
				&& areEqual( other.mz, mz )
				&& areEqual( other.mw, mw ))
				return false;
			return true;
		}   

		bool isZeroVal() const{
			return TintaMath::isZero(mx*mx + my*my+ mz*mz + mw*mw );

		}   
		bool isUnit() const	{
			return TintaMath::isZero(1 - mx*mx - my*my - mz*mz - mw*mw);
		} 
		

		
		void  set( T _x, T _y, T _z, T _w )	{
			mx = _x;
			my = _y; 
			mz = _z;
			mw = _w;
		}  

		void clean()	{
			if ( TintaMath::isZero( mx ) )
				mx = (T)0;
			if ( TintaMath::isZero( my ) )
				my = (T)0;
			if ( TintaMath::isZero( mz ) )
				mz = (T)0;
			if ( TintaMath::isZero( mw ) )
				mw = (T)0;

		} 
		void zero()	{		
            mx = my = mz= mw = (T)0;
		}   

		void normalize()	{
			T lengthsq = mx*mx + my*my+ mz*mz+ mw*mw;

			if ( TintaMath::isZero( lengthsq ) ){
				zero();
			}
			else{
				T factor = 1/ TintaMath::sqrt( lengthsq );
				mx *= factor;
				my *= factor;			
				mz *= factor;
				mw *= factor;
			}

		} 

		tintaVector4<T> &truncate( T length ){

			T length2;
			T ilength;

			if ( !length ) {
				zero();
			}
			else {
				length2 = lengthSquared();
				if ( length2 > length * length ) {
					ilength = length * (1/ TintaMath::sqrt( length2 ));
					mx *= ilength;
					my *= ilength;
					mz *= ilength;
					mw *= ilength;
				}
			}

			return *this;
		}

		T dot (const tintaVector4<T>& rkV) const	{
			return mx*rkV.mx + my*rkV.my+ mz*rkV.mz+ mw*rkV.mw;
		}

		
		tintaVector4<T> operator+( const tintaVector4<T>& other ) const	{
			return tintaVector4<T>( mx + other.mx, my + other.my, mz + other.mz, mw+ other.mw);

		}

		
		
		tintaVector4<T> operator-( const tintaVector4<T>& other ) const	{
			return tintaVector4<T>( mx - other.mx, my - other.my, my - other.mz, mw - other.mw );
		} 

		

		tintaVector4<T> operator-() const	{
			return tintaVector4<T>(-mx, -my, -mz, -mw );
		}    

	
		tintaVector4<T> operator*( T scalar ){
			tintaVector4<T> rez ( scalar*mx, scalar*my, scalar*mz, scalar*mw);
			return rez;
		}   
		

		tintaVector4<T>& operator*=( T scalar )	{
			mx *= scalar;
			my *= scalar;		
			mz *= scalar;
			mw *= scalar;
			return *this;
		}   

		tintaVector4<T> crossUnit( const tintaVector4<T>& vector ) const	{
				tintaVector4<T> vec(my*vector.mz - mz*vector.my,
					mz*vector.mx - mx*vector.mz,
					mx*vector.my - my*vector.mx,
					mx*vector.mw - my*vector.mw  );
				vec.normalize();
				return vec;

		}


		tintaVector4<T> operator/( T scalar )	{
			return tintaVector4<T>( mx/scalar, my/scalar );
		}   

		tintaVector4<T>& operator/=( T scalar ){
			mx /= scalar;
			my /= scalar;
			mz /= scalar;
			mw /= scalar;

			return *this;
		}   


	tintaVector4<T>& operator-=( const tintaVector4<T>& other ){
		mx -= other.mx;
		my -= other.my;		
		mz -= other.mz;
		mw -= other.mw;       
		return *this;
	}   


    tintaVector4<T>& operator*=(const tintaVector4<T>& other) {
        mx *= other.mx;
        my *= other.my;
        mz *= other.mz;
        mw *= other.mw;
        return *this;
    }

    tintaVector4<T>& operator/=(const tintaVector4<T>& other) {
        mx /= other.mx;
        my /= other.my;
        mz /= other.mz;
        mw /= other.mw;
        return *this;
    }

    tintaVector4<T> operator*(const tintaVector4<T>& other) {
        return tintaVector4<T>(mx * other.mx, my * other.my, mz * other.mz, mw * other.mw);
    }

    tintaVector4<T> operator/(const tintaVector4<T>& other) {
        return tintaVector4<T>(mx / other.mx, my / other.my, mz / other.mz, mw / other.mw);
    }

	   


	tintaVector4<T>& operator+=( const tintaVector4<T>& other ){
		mx += other.mx;
		my += other.my;	
		mz += other.mz;
		mw += other.mw;
		return *this;
	}   
		
		// useful defaults
		static tintaVector4<T>    xAxis;
		static tintaVector4<T>    yAxis;	
		static tintaVector4<T>    zAxis;	
		static tintaVector4<T>    wAxis;	
		static tintaVector4<T>    origin;
		// member variables
		T	mx;	
		T	my;
		T	mz;
		T	mw;
	};


	template <typename T>
	tintaVector4<T>& operator+=( tintaVector4<T>& self, const tintaVector4<T>& other ){
		self.mx += other.mx;
		self.my += other.my;
		self.mz += other.mz;
		self.mw += other.mw;
		return self;
	}   

	template <typename T>
	tintaVector4<T>& operator-=( const tintaVector4<T>& self, const tintaVector4<T>& other ){
		self.mx -= other.mx;
		self.my -= other.my;		
		self.mz -= other.mz;
		self.mw -= other.mw;
		return self;
	} 

	template <typename T>
	T distSquared( const tintaVector4<T>& pt1, const tintaVector4<T>& pt2 ){
		T x = pt1.mx - pt2.mx;
		T y = pt1.my - pt2.my;
		T z = pt1.mz - pt2.mz;	
		T w = pt1.mw - pt2.mw;

		return ( x*x + y*y + z*z+ w*w );

	}  
	template <typename T>
	T distance( const tintaVector4<T>& pt1, const tintaVector4<T>& pt2 ){
		T x = pt1.mx - pt2.mx;
		T y = pt1.my - pt2.my;	
		T z = pt1.mz - pt2.mz;
		T w = pt1.mw - pt2.mw;
		
		return (T)sqrt ( double(x*x + y*y + z*z + w*w) );
	}     

	
	template <typename T>
	tintaVector4<T> operator*( T scalar, const tintaVector4<T>& vector )	{
		return tintaVector4<T>( scalar*vector.mx, scalar*vector.my, scalar*vector.mz );
	}	

	typedef tintaVector4<float> tintaVector4f;
    typedef tintaVector4<int> tintaVector4i;
}

#endif
