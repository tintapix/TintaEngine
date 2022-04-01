/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef __TINTA_QUATERN_H_
#define __TINTA_QUATERN_H_

#include "tintaVector3.h"
namespace Tinta
{	
	class tintaMatrix33;

	class tintaQuatern
	{
	public:
	
		FORCEINL tintaQuatern() : mw(1.0f), mx(0.0f), my(0.0f), mz(0.0f)
		{}
		FORCEINL tintaQuatern( float _w, float _x, float _y, float _z ) :
		mw(_w), mx(_x), my(_y), mz(_z)
		{
		}
		tintaQuatern( const tintaVector3f& axis, float angle );
		tintaQuatern( const tintaVector3f& xaxis, const tintaVector3f& yaxis, const tintaVector3f& zaxis );
		tintaQuatern( const tintaVector3f& from, const tintaVector3f& to );
		explicit tintaQuatern( const tintaVector3f& vector );
		explicit tintaQuatern( const tintaMatrix33& rotation );
		FORCEINL ~tintaQuatern() {}

	
		tintaQuatern(const tintaQuatern& other);
		tintaQuatern& operator=(const tintaQuatern& other);		

		
		FORCEINL float& operator[]( m_uint32 i )         { return (&mx)[i]; }
		FORCEINL float operator[]( m_uint32 i ) const    { return (&mx)[i]; }

		float magnitude() const;
		float norm() const;

        bool equal(const tintaQuatern& other, float delta) const;
		bool operator==( const tintaQuatern& other ) const;
		bool operator!=( const tintaQuatern& other ) const;
		bool isZeroVal() const;
		bool isUnit() const;
		bool isDentity() const;

	
		FORCEINL void set( float w, float x, float y, float z );
		void set( const tintaVector3f& axis, float angle );
		void set(const tintaVector3f& vector);
		void set( const tintaVector3f& from, const tintaVector3f& to );
		void set( const tintaMatrix33& rotation );
		void set( float zRotation, float yRotation, float xRotation ); 

        void set(const tintaVector3f& xaxis, const tintaVector3f& yaxis, const tintaVector3f& zaxis);

		void toRotMatrix (tintaMatrix33& kRot) const;
		void toAxes (tintaVector3f* akAxis) const;
		void toAxes (tintaVector3f& xAxis, tintaVector3f& yAxis, tintaVector3f& zAxis) const;
        void toAxesX(tintaVector3f& xAxis) const;
        void toAxesY(tintaVector3f& yAxis) const;
        void toAxesZ(tintaVector3f& zAxis) const;

        float getRoll()const;

        float getPitch()const;

        float getYaw()const;

		void getAxisAngle( tintaVector3f& axis, float& angle )const;

        float getAxisAngle( )const;

        void fromAngleAxis( float angle, const tintaVector3f& axis);
		
		void clean();       
		
		FORCEINL void zero(); 
		
		void normalize();   
		
		FORCEINL void identity();    
		
		friend tintaQuatern conjugate( const tintaQuatern& quat );
		const tintaQuatern& conjugate();

		
		friend tintaQuatern inverse( const tintaQuatern& quat );
		const tintaQuatern& inverse();
		
		tintaQuatern operator+( const tintaQuatern& other ) const;
		tintaQuatern& operator+=( const tintaQuatern& other );
		tintaQuatern operator-( const tintaQuatern& other ) const;
		tintaQuatern& operator-=( const tintaQuatern& other );

		tintaQuatern operator-() const;
		
		friend tintaQuatern    operator*( float scalar, const tintaQuatern& vector );
		tintaQuatern&          operator*=( float scalar );
		
		tintaQuatern operator*( const tintaQuatern& other ) const;
		tintaQuatern& operator*=( const tintaQuatern& other );		
		tintaVector3f operator* ( const tintaVector3f& vec) const;
		
		float              dot( const tintaQuatern& vector ) const;
		friend float       dot( const tintaQuatern& vector1, const tintaQuatern& vector2 );
		
		tintaVector3f   rotate( const tintaVector3f& vector ) const;

       
		
		friend tintaQuatern lerp( const tintaQuatern& start, const tintaQuatern& end, float t );
		friend tintaQuatern slerp(  const tintaQuatern& start, const tintaQuatern& end, float t );
		friend tintaQuatern approxSlerp( const tintaQuatern& start, const tintaQuatern& end, float t );
		
		static tintaQuatern   zeroValue;
		static tintaQuatern   identityValue;
		
		float mw, mx, my, mz;      

	};


    tintaQuatern lerp(const tintaQuatern& start, const tintaQuatern& end, float t);
    tintaQuatern slerp(const tintaQuatern& start, const tintaQuatern& end, float t);
    tintaQuatern approxSlerp(const tintaQuatern& start, const tintaQuatern& end, float t);

	FORCEINL void tintaQuatern::set( float _w, float _x, float _y, float _z )
	{
		mw = _w; mx = _x; my = _y; mz = _z;
	} 


	FORCEINL void tintaQuatern::zero()
	{
		mx = my = mz = mw = 0.0f;
	}   


	FORCEINL void tintaQuatern::identity()
	{
		mx = my = mz = 0.0f;
		mw = 1.0f;
	}


    // Based on Stan Melax's article in Game Programming Gems
    template<typename T>
    tintaQuatern getRotationTo(const tintaVector3<T>& src, const tintaVector3<T>& dest,
        const tintaVector3<T>& fallbackAxis = tintaVector3<T>::origin) {

        tintaQuatern q;

        tintaVector3<T> v0 = src;
        tintaVector3<T> v1 = dest;

        v0.normalize();
        v1.normalize();

        float d = v0.dot( v1 );
                
        if ( d >= 1.0f )
        {
            return tintaQuatern::identityValue;
        }
        if ( d < (1e-6f - 1.0f) )
        {
            if (fallbackAxis != tintaVector3<T>::origin)
            {
                // rotate 180 degrees about the fallback axis
                q.fromAngleAxis(K_PI, fallbackAxis);
            }
            else
            {
                // Generate an axis
                tintaVector3<T> axis = tintaVector3<T>::xAxis.cross(src);
                if ( axis.isZeroVal() ) // pick another if colinear
                    axis = tintaVector3<T>::yAxis.cross(src);
                axis.normalize();
                q.fromAngleAxis(K_PI, axis);
            }
        }
        else
        {
            float s = TintaMath::sqrt((1 + d) * 2);
            float invs = 1 / s;

            tintaVector3<T> c = v0.cross(v1);

            q.mx = c.mx * invs;
            q.my = c.my * invs;
            q.mz = c.mz * invs;
            q.mw = s * 0.5f;
            q.normalize();
        }

        return q;
    }
}

#endif
