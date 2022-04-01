/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef __TINTA_VEWCTOR3__H__
#define __TINTA_VEWCTOR3__H__

#include "tintaPredefine.h"
#include "tintaCommonMath.h"

namespace Tinta
{
    template<typename T>
	class tintaVector3
	{
	public:
		
		tintaVector3<T>() {}
		tintaVector3<T>( T _x, T _y, T _z ) :
		mx(_x)
		, my(_y)
		, mz(_z){}
	

		
		tintaVector3<T>(const tintaVector3<T>& other) : 
		mx( other.mx ),
		my( other.my ),
		mz( other.mz ){}   

		tintaVector3<T>& operator=(const tintaVector3<T>& other)	{
			
			if ( this == &other )
				return *this;

			mx = other.mx;
			my = other.my;	
			mz = other.mz;	
			return *this;
		} 
		
		// accessors
		float& operator[]( m_uint32 i )          { return (&mx)[i]; }
		float operator[]( m_uint32 i ) const { return (&mx)[i]; }

		float length() const	{		
			return sqrt( mx*mx + my*my + mz*mz);
		}   

		float lengthSquared() const	{
			return (mx*mx + my*my+ mz*mz );

		}   	

	
		template<class U>
		friend float distSquared( const tintaVector3<U>& p0, const tintaVector3<U>& p1 );

		
		bool operator==( const tintaVector3<T>& other ) const{
			if (TintaMath::areEqual( other.mx, mx )
				&& TintaMath::areEqual( other.my, my )
				&& TintaMath::areEqual( other.mz, mz ))
				return true;

			return false;   
		}   


        bool equal(const tintaVector3<T>& other, T delta ) const {
            if (    TintaMath::areEqual(other.mx, mx, delta)
                && TintaMath::areEqual(other.my, my, delta)
                && TintaMath::areEqual(other.mz, mz, delta))
                return true;

            return false;
        }

		bool operator!=( const tintaVector3<T>& other ) const{
			if ( TintaMath::areEqual( other.mx, mx )
				&& TintaMath::areEqual( other.my, my )
				&& TintaMath::areEqual( other.mz, mz ))
				return false;
			return true;
		}   

		bool isZeroVal() const	{
			return TintaMath::isZero(mx*mx + my*my+ mz*mz );

		}   
		bool isUnit() const	{
			return TintaMath::isZero(1 - mx*mx - my*my - mz*mz);
		} 

        

		void  set( T _x, T _y, T _z ){
			mx = _x;
			my = _y; 
			mz = _z;
		}  

		void clean(){
			if ( TintaMath::isZero( mx ) )
				mx = (T)0;
			if ( TintaMath::isZero( my ) )
				my = (T)0;
			if ( TintaMath::isZero( mz ) )
				mz = (T)0;

		} 
		void zero()	{
			mx = my = mz = (T)0;
		}   

		void normalize()	{
			T lengthsq = mx*mx + my*my+ mz*mz;

			if ( TintaMath::isZero( lengthsq ) ){
				zero();
			}
			else{
				T factor = 1/ TintaMath::sqrt( lengthsq );
				mx *= factor;
				my *= factor;			
				mz *= factor;
			}
		} 

		tintaVector3<T> &truncate( T length ){

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
				}
			}

			return *this;
		}

		T dot (const tintaVector3<T>& rkV) const	{
			return mx*rkV.mx + my*rkV.my+ mz*rkV.mz;
		}

		tintaVector3<T> operator+( const tintaVector3<T>& other ) const{
			return tintaVector3<T>( mx + other.mx, my + other.my, mz + other.mz);

		}

		tintaVector3<T> cross( const tintaVector3<T>& vector1, const tintaVector3<T>& vector2 ){
			return tintaVector3<T>( vector1.my*vector2.mz - vector1.mz*vector2.my,
				vector1.mz*vector2.mx - vector1.mx*vector2.mz,
				vector1.mx*vector2.my - vector1.my*vector2.mx );

		}

		tintaVector3<T> cross( const tintaVector3<T>& vector ) const	{
			return tintaVector3<T>( my*vector.mz - mz*vector.my,
				mz*vector.mx - mx*vector.mz,
				mx*vector.my - my*vector.mx );
		}

		tintaVector3<T> crossUnit( const tintaVector3<T>& vector ) const	{
				tintaVector3<T> vec(my*vector.mz - mz*vector.my,
					mz*vector.mx - mx*vector.mz,
					mx*vector.my - my*vector.mx );
				vec.normalize();
				return vec;

		}

		
		tintaVector3<T> operator-( const tintaVector3<T>& other ) const	{
			return tintaVector3<T>( mx - other.mx, my - other.my, mz - other.mz );
		} 

		

		tintaVector3<T> operator-() const{
			return tintaVector3<T>(-mx, -my, -mz);
		}    


		tintaVector3<T> operator*( T scalar ){
			tintaVector3<T> rez ( scalar*mx, scalar*my, scalar*mz);
			return rez;
		}   
		

		tintaVector3<T>& operator*=( T scalar ){
			mx *= scalar;
			my *= scalar;		
			mz *= scalar;
			return *this;
		}   

		tintaVector3<T> operator/( T scalar ){
			return tintaVector3<T>( mx/scalar, my/scalar, mz / scalar);
		}   

		tintaVector3<T>& operator/=( T scalar )	{
			mx /= scalar;
			my /= scalar;
			mz /= scalar;

			return *this;
		}   


	tintaVector3<T>& operator-=( const tintaVector3<T>& other ){
		mx -= other.mx;
		my -= other.my;		
		mz -= other.mz;
		return *this;
	}   
	

	   


	tintaVector3<T>& operator+=( const tintaVector3<T>& other ){
		mx += other.mx;
		my += other.my;	
		mz += other.mz;
		return *this;
	}   

    tintaVector3<T>& operator*=(const tintaVector3<T>& other) {
        mx *= other.mx;
        my *= other.my;
        mz *= other.mz;
        return *this;
    }

    tintaVector3<T>& operator/=(const tintaVector3<T>& other) {
        mx /= other.mx;
        my /= other.my;
        mz /= other.mz;
        return *this;
    }
    
    tintaVector3<T> operator*(const tintaVector3<T>& other) const {
        return tintaVector3<T>(mx * other.mx, my * other.my, mz * other.mz);
    }

    tintaVector3<T> operator/(const tintaVector3<T>& other) const  {
        return tintaVector3<T>(mx / other.mx, my / other.my, mz / other.mz);
    }




	
	static void orthonormalize ( tintaVector3<T>& u,  tintaVector3<T>& v,  tintaVector3<T>& w){
        		
		u.normalize();
		
		T f_dot0 = u.dot(v); 
		v -= f_dot0*u;
		v.normalize();
		
		T fDot1 = v.dot(w);
		f_dot0 = u.dot(w);
		w -= f_dot0*u + fDot1*v;
		w.normalize();
	}	


		
		const static tintaVector3<T>    xAxis;
        const static tintaVector3<T>    yAxis;
        const static tintaVector3<T>    zAxis;
        const static tintaVector3<T>    origin;
        const static tintaVector3<T>    unitScale;
	
		T	mx;	
		T	my;
		T	mz;
	};


	template <typename T>
	tintaVector3<T>& operator+=( tintaVector3<T>& self, const tintaVector3<T>& other ){
		self.mx += other.mx;
		self.my += other.my;
		self.mz += other.mz;
		return self;
	}   

	template <typename T>
	tintaVector3<T>& operator-=( const tintaVector3<T>& self, const tintaVector3<T>& other ){
		self.mx -= other.mx;
		self.my -= other.my;		
		self.mz -= other.mz;
		return self;
	}   


	template <typename T>
	T distSquared( const tintaVector3<T>& pt1, const tintaVector3<T>& pt2 ){
		T x = pt1.mx - pt2.mx;
		T y = pt1.my - pt2.my;
		T z = pt1.mz - pt2.mz;	

		return ( x*x + y*y + z*z );

	}  
	template <typename T>
	T distance( const tintaVector3<T>& pt1, const tintaVector3<T>& pt2 ){
		T x = pt1.mx - pt2.mx;
		T y = pt1.my - pt2.my;	
		T z = pt1.mz - pt2.mz;
		
		return (T)sqrt ( (double)(x*x + y*y + z*z) );
	}   	


    template <typename T>
    tintaVector3<T>  lerp(const tintaVector3<T> &v1, const tintaVector3<T> &v2, const T l) {

        tintaVector3<T> rez;
        if (l <= 0) {
            rez = v1;
        }
        else if (l >= 1) {
            rez = v2;
        }
        else {
            rez = v1 + l * (v2 - v1);
        }
        return rez;
    }


	template <typename T>
	tintaVector3<T> operator*( T scalar, const tintaVector3<T>& vector )	{
		return tintaVector3<T>( scalar*vector.mx, scalar*vector.my, scalar*vector.mz );
	}

     

    /*
    //https://keithmaggio.wordpress.com/2011/02/15/math-magician-lerp-slerp-and-nlerp/
    template <typename T>    
    tintaVector3<T> Slerp(tintaVector3<T> start, tintaVector3<T> end, float factor)
    {
        // Dot product - the cosine of the angle between 2 vectors.
        float dot = start.dot(end);
        // Clamp it to be in the range of Acos()
        // This may be unnecessary, but floating point
        // precision can be a fickle mistress.
        clamp(dot, -1.0f, 1.0f);
        // Acos(dot) returns the angle between start and end,
        // And multiplying that by percent returns the angle between
        // start and the final result.
        float theta = tintaAcos(dot) * factor;
        tintaVector3<T> RelativeVec = end - start * dot;
        RelativeVec.Normalize();     // Orthonormal basis
                                     // The final result.
        return ( ( start*Mathf.Cos( theta ) ) + (RelativeVec*Mathf.Sin(theta)));
    }
    */

	typedef tintaVector3<float> tintaVector3f;
    typedef tintaVector3<int> tintaVector3i;
    typedef std::vector<tintaVector3f> VecVector3f_t;
    typedef std::list<tintaVector3f> ListVector3f_t;
    
}

#endif
