/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef __TINTA_VECTOR2__H__
#define __TINTA_VECTOR2__H__

#include "tintaPredefine.h"
#include "tintaCommonMath.h"

namespace Tinta
{	 
	template<typename T>
	class tintaVector2
	{
	public:
		
		tintaVector2<T>() {}
		tintaVector2<T>( T _x, T _y ) :
		mx(_x), my(_y)
		{
		}
	

		
		tintaVector2<T>(const tintaVector2<T>& other) : 
		mx( other.mx ),
			my( other.my ){}   

		tintaVector2<T>& operator=(const tintaVector2<T>& other)	{
			
			if ( this == &other )
				return *this;

			mx = other.mx;
			my = other.my;	

			return *this;

		} 
		
		
		float& operator[]( m_uint32 i )          { return (&mx)[i]; }
		float operator[]( m_uint32 i ) const { return (&mx)[i]; }

		T length() const{		
			return sqrt( mx*mx + my*my );
		}   

		T lengthSquared() const	{
			return (mx*mx + my*my );

		}   	

		
		template<class U>
		friend float distSquared( const tintaVector2<T>& p0, const tintaVector2<T>& p1 );

		
		bool operator==( const tintaVector2<T>& other ) const	{
			if (   areEqual( other.mx, mx )
				&& areEqual( other.my, my ))
				return true;

			return false;   
		}   

		bool operator!=( const tintaVector2<T>& other ) const	{
			if ( areEqual( other.mx, mx )
				&& areEqual( other.my, my ))
				return false;
			return true;
		}   

		bool isZeroVal() const	{
			return TintaMath::isZero(mx*mx + my*my );

		}   
		bool isUnit() const	{
			return TintaMath::isZero(1 - mx*mx - my*my );
		} 
		

		
		void  set( T _x, T _y )	{
			mx = _x; my = _y; 
		}  

		void clean()	{
			if ( TintaMath::isZero( mx ) )
				mx = (T)0;
			if ( TintaMath::isZero( my ) )
				my = (T)0;		

		} 
		void zero()	{
			mx = my = (T)0;
		}   

		void normalize()	{
			T lengthsq = mx*mx + my*my;

			if ( TintaMath::isZero( lengthsq ) )
			{
				zero();
			}
			else
			{
				T factor = 1/ TintaMath::sqrt( lengthsq );
				mx *= factor;
				my *= factor;			
			}

		} 

		inline tintaVector2<T> &truncate( T length ){

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
				}
			}

			return *this;
		}

		T dot (const tintaVector2<T>& rkV) const	{
			return mx*rkV.mx + my*rkV.my;
		}
        		
		tintaVector2<T> operator+( const tintaVector2<T>& other ) const	{
			return tintaVector2<T>( mx + other.mx, my + other.my );

		}
		
		tintaVector2<T> operator-() const{
			return tintaVector2<T>(-mx, -my);
		}  

        tintaVector2<T> operator-(const tintaVector2<T>& other ) const {
            return tintaVector2<T>(mx - other.mx, my - other.my);
        }

		// scalar multiplication
		tintaVector2<T> operator*( T scalar ){
			tintaVector2<T> rez ( scalar*mx, scalar*my);
			return rez;
		}   
		

		tintaVector2<T>& operator*=( T scalar )	{
			mx *= scalar;
			my *= scalar;		
			return *this;
		}   

		tintaVector2<T> operator/( T scalar )	{
			return tintaVector2<T>( mx/scalar, my/scalar );
		}   

		tintaVector2<T>& operator/=( T scalar )	{
			mx /= scalar;
			my /= scalar;

			return *this;
		}   

        bool equal(const tintaVector2<T>& other, T delta) const {
            if ( TintaMath::areEqual(other.mx, mx, delta )
                && TintaMath::areEqual(other.my, my, delta) )
                return true;
            return false;
        }


        tintaVector2<T>& operator-=(const tintaVector2<T>& other){
            mx -= other.mx;
            my -= other.my;
            return *this;
        }

	   


	tintaVector2<T>& operator+=( const tintaVector2<T>& other ){
		mx += other.mx;
		my += other.my;	
		return *this;
	}  


	static tintaVector2<T>    xAxis;
	static tintaVector2<T>    yAxis;
	static tintaVector2<T>    origin;
		
		T	mx;	
		T	my;			
	};


	template <typename T>
	T distSquared( const tintaVector2<T>& pt1, const tintaVector2<T>& pt2 )
	{
		T x = pt1.mx - pt2.mx;
		T y = pt1.my - pt2.my;	

		return ( x*x + y*y );

	}  
	template <typename T>
	T distance( const tintaVector2<T>& pt1, const tintaVector2<T>& pt2 )
	{
		T x = pt1.mx - pt2.mx;
		T y = pt1.my - pt2.my;	
		
		return sqrt ( x*x + y*y );
	}   

	
	template <typename T>
	tintaVector2<T>  lerp( const tintaVector2<T> &v1, const tintaVector2<T> &v2, const T l ) {

		tintaVector2<T> rez;
		if ( l <= 0 ) {
			rez = v1;
		} else if ( l >= 1 ) {
			rez = v2;
		} else {
			rez = v1 + l * ( v2 - v1 );
		}
		return rez;
	}

	

	/*
		Returns angle between vectors with the maximum -Pi	
	*/
	template <typename T>
	T angleBetween(const tintaVector2<T>& pt1, const tintaVector2<T>& pt2){
		return (atan2(pt1.mx*pt2.my - pt2.mx*pt1.my, pt1.mx*pt2.mx + pt1.my*pt2.my));
	}

	/*
		Returns angle between vectors with the maximum Pi * 2 in radians	
	*/
	template <typename T>
	T uangleBetween(const tintaVector2<T>& pt1, const tintaVector2<T>& pt2){
		return (TintaMath::angleToUnsign( atan2(pt1.mx*pt2.my - pt2.mx*pt1.my, pt1.mx*pt2.mx + pt1.my*pt2.my)) ) ;
	}

	template <typename T>
	tintaVector2<T> operator*( T scalar, const tintaVector2<T>& vector )
	{
		return tintaVector2<T>( scalar*vector.mx, scalar*vector.my );

	}

    

	typedef tintaVector2<float> tintaVector2f;
    typedef tintaVector2<double> tintaVector2d;
    typedef tintaVector2<int> tintaVector2i;
    typedef std::vector<tintaVector2f> VecVector2f_t;
    typedef std::list<tintaVector2f> ListVector2f_t;
}


#endif
