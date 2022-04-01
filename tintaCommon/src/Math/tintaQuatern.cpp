/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaCommonMath.h"
#include "tintaQuatern.h"
#include "tintaVector3.h"
#include "tintaMatrix33.h"

namespace Tinta {

	tintaQuatern tintaQuatern::zeroValue( 0.0f, 0.0f, 0.0f, 0.0f );
	tintaQuatern tintaQuatern::identityValue( 1.0f, 0.0f, 0.0f, 0.0f );


	tintaQuatern::tintaQuatern( const tintaVector3f& axis, float angle ){
		set( axis, angle );
	}

	tintaQuatern::tintaQuatern( const tintaVector3f& from, const tintaVector3f& to ){
		set( from, to );
	} 

	
	tintaQuatern::tintaQuatern( const tintaVector3f& vector ){
		set( 0.0f, vector.mx, vector.my, vector.mz );
	}   

	tintaQuatern::tintaQuatern( const tintaVector3f& xaxis, const tintaVector3f& yaxis, const tintaVector3f& zaxis ){
		tintaMatrix33 kRot;

		kRot[0][0] = xaxis.mx;
		kRot[1][0] = xaxis.my;
		kRot[2][0] = xaxis.mz;

		kRot[0][1] = yaxis.mx;
		kRot[1][1] = yaxis.my;
		kRot[2][1] = yaxis.mz;

		kRot[0][2] = zaxis.mx;
		kRot[1][2] = zaxis.my;
		kRot[2][2] = zaxis.mz;

		set(kRot);
	}


	tintaQuatern::tintaQuatern( const tintaMatrix33& rotation ){
		float trace = rotation.trace();
		if ( trace	> 0.0f ){
			float s = TintaMath::sqrt( trace + 1.0f );
			mw = s*0.5f;
			float recip = 0.5f/s;
			mx = (rotation(2,1) - rotation(1,2))*recip;
			my = (rotation(0,2) - rotation(2,0))*recip;
			mz = (rotation(1,0) - rotation(0,1))*recip;
		}
		else {
			m_uint32 i = 0;
			if ( rotation(1,1) > rotation(0,0) )
				i = 1;
			if ( rotation(2,2) > rotation(i,i) )
				i = 2;
			m_uint32 j = (i+1)%3;
			m_uint32 k = (j+1)%3;
			float s = TintaMath::sqrt( rotation(i,i) - rotation(j,j) - rotation(k,k) + 1.0f );
			(*this)[i] = 0.5f*s;
			float recip = 0.5f/s;
			mw = (rotation(k,j) - rotation(j,k))*recip;
			(*this)[j] = (rotation(j,i) + rotation(i,j))*recip;
			(*this)[k] = (rotation(k,i) + rotation(i,k))*recip;
		}

	}

	tintaQuatern::tintaQuatern(const tintaQuatern& other) : 
	mw( other.mw ),
	mx( other.mx ),
	my( other.my ),
	mz( other.mz ){}

	tintaQuatern& tintaQuatern::operator=(const tintaQuatern& other){		
		if ( this == &other )
			return *this;

		mw = other.mw;
		mx = other.mx;
		my = other.my;
		mz = other.mz;

		return *this;

	}



	float tintaQuatern::magnitude() const	{
		return TintaMath::sqrt( mw*mw + mx*mx + my*my + mz*mz );

	}



	float tintaQuatern::norm() const	{
		return ( mw*mw + mx*mx + my*my + mz*mz );
	}

    bool  tintaQuatern::equal(const tintaQuatern& other, float delta) const {
        if (   TintaMath::areEqual(other.mw, mw, delta)
            && TintaMath::areEqual(other.mx, mx, delta)
            && TintaMath::areEqual(other.my, my, delta)
            && TintaMath::areEqual(other.mz, mz, delta) )
            return true;

        return false;
    }


	bool tintaQuatern::operator==( const tintaQuatern& other ) const	{
		if (   TintaMath::isZero( other.mw - mw )
			&& TintaMath::isZero( other.mx - mx )
			&& TintaMath::isZero( other.my - my )
			&& TintaMath::isZero( other.mz - mz ) )
			return true;

		return false;   
	}   


	bool tintaQuatern::operator!=( const tintaQuatern& other ) const	{
		if ( TintaMath::isZero( other.mw - mw )
			|| TintaMath::isZero( other.mx - mx )
			|| TintaMath::isZero( other.my - my )
			|| TintaMath::isZero( other.mz - mz ) )
			return false;

		return true;
	}  


	bool tintaQuatern::isZeroVal() const	{
		return TintaMath::isZero(mw*mw + mx*mx + my*my + mz*mz);

	}   



	bool tintaQuatern::isUnit() const{
		return TintaMath::isZero(1.0f - mw*mw - mx*mx - my*my - mz*mz);

	}



	bool tintaQuatern::isDentity() const{
		return TintaMath::isZero(1.0f - mw)
			&& TintaMath::isZero( mx ) 
			&& TintaMath::isZero( my )
			&& TintaMath::isZero( mz );

	}  

	void tintaQuatern::set(const tintaVector3f& vector){
		set( 0.0f, vector.mx, vector.my, vector.mz );
	}
	void tintaQuatern::set( const tintaVector3f& axis, float angle ){
		
		float length = axis.lengthSquared();
		if ( TintaMath::isZero( length ) )
		{
			identity();
			return;
		}

		// take half-angle
		angle *= 0.5f;

		float sintheta, costheta;
        TintaMath::sincos(angle, sintheta, costheta);

		float scaleFactor = sintheta/TintaMath::sqrt( length );

		mw = costheta;
		mx = scaleFactor * axis.mx;
		my = scaleFactor * axis.my;
		mz = scaleFactor * axis.mz;

	}  



	void tintaQuatern::set( const tintaVector3f& from, const tintaVector3f& to ){
		// get axis of rotation
		tintaVector3f axis = from.cross( to );
		
		set(  from.dot( to ), axis.mx, axis.my, axis.mz );
		
		normalize();
		
		mw += 1.0f;
		
		if ( mw <= K_EPSILON )
		{
			
			if ( from.mz*from.mz > from.mx*from.mx )
				set( 0.0f, 0.0f, from.mz, -from.my );
			
			else
				set( 0.0f, from.my, -from.mx, 0.0f );
		}		
		normalize();

	}  


	void tintaQuatern::set( const tintaMatrix33& rotation ) {		
		

		float f_fuct = rotation[0][0] + rotation[1][1] + rotation[2][2];

		float f_root;
		if ( f_fuct > 0.0 )	{
			
			f_root = TintaMath::sqrt(f_fuct + 1.0f);  
			mw = 0.5f * f_root;
			f_root = 0.5f / f_root;  
			mx = (rotation[2][1]-rotation[1][2])*f_root;
			my = (rotation[0][2]-rotation[2][0])*f_root;
			mz = (rotation[1][0]-rotation[0][1])*f_root;
		}
		else{			
			static size_t s_iNext[3] = { 1, 2, 0 };
			size_t i = 0;
			if ( rotation[1][1] > rotation[0][0] )
				i = 1;
			if ( rotation[2][2] > rotation[i][i] )
				i = 2;
			size_t j = s_iNext[i];
			size_t k = s_iNext[j];

			f_root = TintaMath::sqrt(rotation[i][i]-rotation[j][j]-rotation[k][k] + 1.0f);
			float* apkQuat[3] = { &mx, &my, &mz };

			*apkQuat[i] = 0.5f * f_root;

			f_root = 0.5f/f_root;
			mw = (rotation[k][j]-rotation[j][k])*f_root;
			*apkQuat[j] = (rotation[j][i]+rotation[i][j])*f_root;
			*apkQuat[k] = (rotation[k][i]+rotation[i][k])*f_root;
		}

	}   

	void tintaQuatern::set( float zRotation, float yRotation, float xRotation ){
		zRotation *= 0.5f;
		yRotation *= 0.5f;
		xRotation *= 0.5f;

		
		float Cx, Sx;
        TintaMath::sincos(xRotation, Sx, Cx);

		float Cy, Sy;
        TintaMath::sincos(yRotation, Sy, Cy);

		float Cz, Sz;
        TintaMath::sincos(zRotation, Sz, Cz);

		
		mw = Cx*Cy*Cz - Sx*Sy*Sz;
		mx = Sx*Cy*Cz + Cx*Sy*Sz;
		my = Cx*Sy*Cz - Sx*Cy*Sz;
		mz = Cx*Cy*Sz + Sx*Sy*Cx;

	}  


    void tintaQuatern::set(const tintaVector3f& xaxis, const tintaVector3f& yaxis, const tintaVector3f& zaxis){
        tintaMatrix33 kRot;

        kRot[0][0] = xaxis.mx;
        kRot[1][0] = xaxis.my;
        kRot[2][0] = xaxis.mz;

        kRot[0][1] = yaxis.mx;
        kRot[1][1] = yaxis.my;
        kRot[2][1] = yaxis.mz;

        kRot[0][2] = zaxis.mx;
        kRot[1][2] = zaxis.my;
        kRot[2][2] = zaxis.mz;

        set(kRot);
    }

	

	void tintaQuatern::toRotMatrix (tintaMatrix33& rot) const{
		float _x  = mx+mx;
		float _y  = my+my;
		float _z  = mz+mz;
		float _wx = _x*mw;
		float _wy = _y*mw;
		float _wz = _z*mw;
		float _xx = _x*mx;
		float _xy = _y*mx;
		float _xz = _z*mx;
		float _yy = _y*my;
		float _yz = _z*my;
		float _zz = _z*mz;

        
        rot[0][0] = 1.0f - (_yy + _zz);
        rot[0][1] = _xy-_wz;
        rot[0][2] = _xz+_wy;
        rot[1][0] = _xy+_wz;
        rot[1][1] = 1.0f-(_xx+_zz);
        rot[1][2] = _yz-_wx;
        rot[2][0] = _xz-_wy;
        rot[2][1] = _yz+_wx;
        rot[2][2] = 1.0f-(_xx+_yy);

	}

	void tintaQuatern::toAxes (tintaVector3f* akAxis) const{

		tintaMatrix33 kRot;
		toRotMatrix(kRot);

		for (size_t iCol = 0; iCol < 3; iCol++)
		{
			akAxis[iCol].mx = kRot[0][iCol];
			akAxis[iCol].my = kRot[1][iCol];
			akAxis[iCol].mz = kRot[2][iCol];
		}

	}

	void tintaQuatern::toAxes (tintaVector3f& xAxis, tintaVector3f& yAxis, tintaVector3f& zAxis) const {

		tintaMatrix33 kRot;

		toRotMatrix(kRot);

		xAxis.mx = kRot[0][0];
		xAxis.my = kRot[1][0];
		xAxis.mz = kRot[2][0];

		yAxis.mx = kRot[0][1];
		yAxis.my = kRot[1][1];
		yAxis.mz = kRot[2][1];

		zAxis.mx = kRot[0][2];
		zAxis.my = kRot[1][2];
		zAxis.mz = kRot[2][2];

	}

    void tintaQuatern::toAxesX(tintaVector3f& xAxis) const{
        tintaMatrix33 kRot;

        toRotMatrix(kRot);

        xAxis.mx = kRot[0][1];
        xAxis.my = kRot[1][1];
        xAxis.mz = kRot[2][1];

        

    }
    void tintaQuatern::toAxesY(tintaVector3f& yAxis) const{
        tintaMatrix33 kRot;

        toRotMatrix(kRot);

        yAxis.mx = kRot[0][0];
        yAxis.my = kRot[1][0];
        yAxis.mz = kRot[2][0];



    }

    void tintaQuatern::toAxesZ(tintaVector3f& zAxis) const{
        tintaMatrix33 kRot;

        toRotMatrix(kRot);

        zAxis.mx = kRot[0][2];
        zAxis.my = kRot[1][2];
        zAxis.mz = kRot[2][2];
    }

    float tintaQuatern::getRoll()const {
        return atan2f(2.f * (mx*my + mw*mz), mw*mw + mx*mx - my*my - mz*mz);
    }

    float tintaQuatern::getPitch()const {
        return atan2f(2.f * (my*mz + mw*mx), mw*mw + mx*mx - my*my + mz*mz);
    }

    float tintaQuatern::getYaw()const {
        return  asin(-2.f*(mx * mz - mw * my));
    }

    void tintaQuatern::getAxisAngle(tintaVector3f& axis, float& angle) const	{

        /*angle = 2.0f * acosf( mw );

        float length = TintaMath::sqrt(1.0f - mw*mw);
        if (TintaMath::isZero(length))
            axis.zero();
        else
        {
            length = 1.0f / length;
            axis.set(mx*length, my*length, mz*length);
        }*/


        float sqrLen = mx * mx + my * my + mz * mz;

        if ( sqrLen > 0.f ) {

            angle = 2.0f * acosf(mw);
            float len = 1.f / sqrLen;
            axis.mx = mx * len;
            axis.my = my * len;
            axis.mz = mz * len;

        }
        else {
            // angle is 0
            angle = 0.f;
            axis.mx = 1.f;
            axis.my = 0.f;
            axis.mz = 0.f;
        }
    }


    float tintaQuatern::getAxisAngle() const {
        return 2.0f * acosf(mw);        
    }


    void tintaQuatern::fromAngleAxis( float angle, const tintaVector3f& axis ) {        
       
        //   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)        
        float halfAngle(angle / 2.f);
        float fSin = TintaMath::sin(halfAngle);
        mw = TintaMath::cos(halfAngle);
        mx = fSin * axis.mx;
        my = fSin * axis.my;
        mz = fSin * axis.mz;        
    }


	void tintaQuatern::clean(){
		if ( !TintaMath::isZero( mw ) )
			mw = 0.0f;
		if ( !TintaMath::isZero( mx ) )
			mx = 0.0f;
		if ( !TintaMath::isZero( my ) )
			my = 0.0f;
		if ( !TintaMath::isZero( mz ) )
			mz = 0.0f;

	}   



	void tintaQuatern::normalize()	{
		float lengthsq = mw*mw + mx*mx + my*my + mz*mz;

		if ( TintaMath::isZero( lengthsq ) )
		{
			zero();
		}
		else
		{
			float factor = TintaMath::sqrt( lengthsq );
			mw *= factor;
			mx *= factor;
			my *= factor;
			mz *= factor;
		}

	}   


	tintaQuatern conjugate( const tintaQuatern& quat ) 	{
		return tintaQuatern( quat.mw, -quat.mx, -quat.my, -quat.mz );

	}   



	const tintaQuatern& tintaQuatern::conjugate()	{
		mx = -mx;
		my = -my;
		mz = -mz;

		return *this;

	}   



	tintaQuatern inverse( const tintaQuatern& quat )	{
		float norm = quat.mw*quat.mw + quat.mx*quat.mx + quat.my*quat.my + quat.mz*quat.mz;
		
		if ( !TintaMath::isZero( norm ) )		
			return tintaQuatern();
		

		float normRecip = 1.0f / norm;
		return tintaQuatern( normRecip*quat.mw, -normRecip*quat.mx, -normRecip*quat.my, 
			-normRecip*quat.mz );

	}   


	const tintaQuatern& tintaQuatern::inverse()	{
		float norm = mw*mw + mx*mx + my*my + mz*mz;
		
		if ( TintaMath::isZero( norm ) )
			return *this;

		float normRecip = 1.0f / norm;
		mw = normRecip*mw;
		mx = -normRecip*mx;
		my = -normRecip*my;
		mz = -normRecip*mz;

		return *this;

	}   



	tintaQuatern tintaQuatern::operator+( const tintaQuatern& other ) const	{
		return tintaQuatern( mw + other.mw, mx + other.mx, my + other.my, mz + other.mz );

	}   



	tintaQuatern& tintaQuatern::operator+=( const tintaQuatern& other )	{
		mw += other.mw;
		mx += other.mx;
		my += other.my;
		mz += other.mz;

		return *this;

	}  



	tintaQuatern tintaQuatern::operator-( const tintaQuatern& other ) const	{
		return tintaQuatern( mw - other.mw, mx - other.mx, my - other.my, mz - other.mz );

	}   



	tintaQuatern& tintaQuatern::operator-=( const tintaQuatern& other )	{
		mw -= other.mw;
		mx -= other.mx;
		my -= other.my;
		mz -= other.mz;

		return *this;

	}   



	tintaQuatern tintaQuatern::operator-() const	{
		return tintaQuatern(-mw, -mx, -my, -mz);
	}   

	tintaQuatern  operator*( float scalar, const tintaQuatern& quat )	{
		return tintaQuatern( scalar*quat.mw, scalar*quat.mx, scalar*quat.my, scalar*quat.mz );

	}   



	tintaQuatern& tintaQuatern::operator*=( float scalar )	{
		mw *= scalar;
		mx *= scalar;
		my *= scalar;
		mz *= scalar;

		return *this;

	}   
	
	tintaQuatern tintaQuatern::operator*( const tintaQuatern& other ) const	{
		return tintaQuatern( mw*other.mw - mx*other.mx - my*other.my - mz*other.mz,
			                 mw*other.mx + mx*other.mw + my*other.mz - mz*other.my,
			                 mw*other.my + my*other.mw + mz*other.mx - mx*other.mz,
			                 mw*other.mz + mz*other.mw + mx*other.my - my*other.mx );

	}  
	tintaVector3f tintaQuatern::operator* (const tintaVector3f& rkVector) const {
        		 
        // nVidia SDK implementation
		tintaVector3f qvec(mx, my, mz);
        tintaVector3f uv = qvec.cross(rkVector);
        tintaVector3f uuv = qvec.cross(uv);
		uv *= (2.0f * mw);
		uuv *= 2.0f;

		return rkVector + uv + uuv;

	}



	tintaQuatern& tintaQuatern::operator*=( const tintaQuatern& other )	{
		set( mw*other.mw - mx*other.mx - my*other.my - mz*other.mz,
			mw*other.mx + mx*other.mw + my*other.mz - mz*other.my,
			mw*other.my + my*other.mw + mz*other.mx - mx*other.mz,
			mw*other.mz + mz*other.mw + mx*other.my - my*other.mx );

		return *this;

	}  



	float  tintaQuatern::dot( const tintaQuatern& quat ) const	{
		return ( mw*quat.mw + mx*quat.mx + my*quat.my + mz*quat.mz);

	}   



	float dot( const tintaQuatern& quat1, const tintaQuatern& quat2 )	{		
        return (quat1.mw*quat2.mw + quat1.mx*quat2.mx + quat1.my*quat2.my + quat1.mz*quat2.mz);

	}  



	tintaVector3f  tintaQuatern::rotate( const tintaVector3f& vector ) const	{
		
		tintaVector3f uv, uuv;
		tintaVector3f qvec(mx, my, mz);
		uv = qvec.cross(vector);
		uuv = qvec.cross(uv);
		uv *= (2.0f * mw);
		uuv *= 2.0f;

		return vector + uv + uuv;
	}   


	
	
    tintaQuatern lerp( const tintaQuatern& start, const tintaQuatern& end, float t )	{
		
        tintaQuatern result;
		float cosTheta = start.dot( end );
		
		result = t * end;

	
		if ( cosTheta >= K_EPSILON )				
			result += (1.0f-t)*start;
		
		else			
			result += (t-1.0f)*start;
		
        return result;
	} 



	
    tintaQuatern slerp( const tintaQuatern& start, const tintaQuatern& end, float t )
	{
        tintaQuatern result;
		float cosTheta = start.dot( end );
		float startInterp, endInterp;

		
		if ( cosTheta >= K_EPSILON ){
			
			if ( (1.0f - cosTheta) > K_EPSILON ){
				
				float theta = acosf( cosTheta );
				float recipSinTheta = 1.0f/TintaMath::sin( theta );

				startInterp = TintaMath::sin( (1.0f - t)*theta )*recipSinTheta;
				endInterp = TintaMath::sin( t*theta )*recipSinTheta;
			}			
			else
			{				
				startInterp = 1.0f - t;
				endInterp = t;
			}
		}		
		else	{			
			if ( (1.0f + cosTheta) > K_EPSILON ){
				
				float theta = acosf( -cosTheta );
				float recipSinTheta = 1.0f/TintaMath::sin( theta );

				startInterp = TintaMath::sin( (t-1.0f)*theta )*recipSinTheta;
				endInterp = TintaMath::sin( t*theta )*recipSinTheta;
			}			
			else{				
				startInterp = t - 1.0f;
				endInterp = t;
			}
		}

		result = startInterp*start + endInterp*end;
        return result;
	}

	
    tintaQuatern approxSlerp( const tintaQuatern& start, const tintaQuatern& end, float t )	{
        tintaQuatern result;
		float cosTheta = start.dot( end );

		
		float factor = 1.0f - 0.7878088f*cosTheta;
		float k = 0.5069269f;
		factor *= factor;
		k *= factor;

		float b = 2*k;
		float c = -3*k;
		float d = 1 + k;

		t = t*(b*t + c) + d;
		
		result = t*end;
		
		if ( cosTheta >= K_EPSILON ){
			
			result += (1.0f-t)*start;
		}
		else{
			
			result += (t-1.0f)*start;
		}
        return result; 

	}   
}
