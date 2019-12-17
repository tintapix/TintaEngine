

#include "tintaCommonMath.h"
#include "tintaMatrix33.h"
#include "tintaQuatern.h"
#include "tintaVector3.h"

namespace Tinta
{


	tintaMatrix33::tintaMatrix33( const tintaQuatern& quat ){
		(void) rotation( quat );

	}   

	tintaMatrix33::tintaMatrix33(const tintaMatrix33& other){
		mV[0] = other.mV[0];
		mV[1] = other.mV[1];
		mV[2] = other.mV[2];

		mV[3] = other.mV[3];
		mV[4] = other.mV[4];
		mV[5] = other.mV[5];

		mV[6] = other.mV[6];
		mV[7] = other.mV[7];
		mV[8] = other.mV[8];

	}   

	tintaMatrix33::tintaMatrix33 (float fM00, float fM01, float fM02,
								float fM10, float fM11, float fM12,
								float fM20, float fM21, float fM22 ){
									
									mV[0] = fM00;
									mV[1] = fM01;
									mV[2] = fM02;

									mV[3] = fM10;
									mV[4] = fM11;
									mV[5] = fM12;

									mV[6] = fM20;
									mV[7] = fM21;
									mV[8] = fM22;
	}

		
	tintaMatrix33::tintaMatrix33( const tintaVector3f& axis, float angle )	{
		float c, s;
		TintaMath::sincos(angle, s, c);
		float t = 1.0f - c;

		tintaVector3f nAxis = axis;
		nAxis.normalize();

		
		float tx = t * nAxis.mx;  float ty = t*nAxis.my;  float tz = t*nAxis.mz;
		float sx = s*nAxis.mx;  float sy = s*nAxis.my;  float sz = s*nAxis.mz;
		float txy = tx*nAxis.my; float tyz = tx*nAxis.mz; float txz = tx*nAxis.mz;
		
		mV[0] = tx*nAxis.mx + c;
		mV[1] = txy - sz;
		mV[2] = txz + sy;
		mV[3] = txy + sz;
		mV[4] = ty*nAxis.my + c;
		mV[5] = tyz - sx;
		mV[6] = txz - sy;
		mV[7] = tyz + sx;
		mV[8] = tz*nAxis.mz + c;
	} 

	tintaMatrix33& tintaMatrix33::operator=(const tintaMatrix33& other){
		
		if ( this == &other )
			return *this;

		mV[0] = other.mV[0];
		mV[1] = other.mV[1];
		mV[2] = other.mV[2];
		mV[3] = other.mV[3];
		mV[4] = other.mV[4];
		mV[5] = other.mV[5];
		mV[6] = other.mV[6];
		mV[7] = other.mV[7];
		mV[8] = other.mV[8];

		return *this;

	}   

	tintaMatrix33 tintaMatrix33::TimesDiagonal (const tintaVector3f& rkDiag) const	{
		return tintaMatrix33(
			mV[0]*rkDiag[0],mV[1]*rkDiag[1],mV[2]*rkDiag[2],
			mV[3]*rkDiag[0],mV[4]*rkDiag[1],mV[5]*rkDiag[2],
			mV[6]*rkDiag[0],mV[7]*rkDiag[1],mV[8]*rkDiag[2]);
	}
	
	
	tintaMatrix33 tintaMatrix33::DiagonalTimes (const tintaVector3f& rkDiag) const	{
		return tintaMatrix33(
			rkDiag[0]*mV[0],rkDiag[0]*mV[1],rkDiag[0]*mV[2],
			rkDiag[1]*mV[3],rkDiag[1]*mV[4],rkDiag[1]*mV[5],
			rkDiag[2]*mV[6],rkDiag[2]*mV[7],rkDiag[2]*mV[8] 
		);
	}
	

	bool tintaMatrix33::operator==( const tintaMatrix33& other ) const	{
		for (m_uint32 i = 0; i < 9; ++i)
		{
			if ( !TintaMath::areEqual(mV[i], other.mV[i]) )
				return false;
		}
		return true;

	}  



	bool tintaMatrix33::operator!=( const tintaMatrix33& other ) const{
		for (m_uint32 i = 0; i < 9; ++i)
		{
			if ( !TintaMath::areEqual(mV[i], other.mV[i]) )
				return true;
		}
		return false;

	}   

	bool tintaMatrix33::isZeroVal() const{
		for (m_uint32 i = 0; i < 9; ++i)
		{
			if ( ! TintaMath::isZero( mV[i] ) )
				return false;
		}
		return true;
	}   



	bool tintaMatrix33::isIdentity() const{
		return TintaMath::areEqual( 1.0f, mV[0] )
			&& TintaMath::areEqual( 1.0f, mV[4] )
			&& TintaMath::areEqual( 1.0f, mV[8] )
			&& TintaMath::isZero( mV[1] ) 
			&& TintaMath::isZero( mV[2] )
			&& TintaMath::isZero( mV[3] )
			&& TintaMath::isZero( mV[5] ) 
			&& TintaMath::isZero( mV[6] )
			&& TintaMath::isZero( mV[7] );

	}   



	void tintaMatrix33::setRows( const tintaVector3f& row1, const tintaVector3f& row2, const tintaVector3f& row3 ){

		mV[0] = row1.mx;
		mV[1] = row1.my;
		mV[2] = row1.mz;

		mV[3] = row2.mx;
		mV[4] = row2.my;
		mV[5] = row2.mz;

		mV[6] = row3.mx;
		mV[7] = row3.my;
		mV[8] = row3.mz;

	} 


	void tintaMatrix33::getRows( tintaVector3f& row1, tintaVector3f& row2, tintaVector3f& row3 ) const{
		row1.mx = mV[0];
		row1.my = mV[1];
		row1.mz = mV[2];

		row2.mx = mV[3];
		row2.my = mV[4];
		row2.mz = mV[5];

		row3.mx = mV[6];
		row3.my = mV[7];
		row3.mz = mV[8];
	}   



	tintaVector3f tintaMatrix33::get_row( m_uint32 i ) const{
		
		return tintaVector3f( mV[i], mV[i+1], mV[i+2] );

	}   



	void tintaMatrix33::setColumns( const tintaVector3f& col1, const tintaVector3f& col2, const tintaVector3f& col3 ){
		mV[0] = col1.mx;
		mV[3] = col1.my;
		mV[6] = col1.mz;

		mV[1] = col2.mx;
		mV[4] = col2.my;
		mV[7] = col2.mz;

		mV[2] = col3.mx;
		mV[5] = col3.my;
		mV[8] = col3.mz;

	}



	void tintaMatrix33::getColumns( tintaVector3f& col1, tintaVector3f& col2, tintaVector3f& col3 ) const{
		col1.mx = mV[0];
		col1.my = mV[3];
		col1.mz = mV[6];

		col2.mx = mV[3];
		col2.my = mV[4];
		col2.mz = mV[7];

		col3.mx = mV[6];
		col3.my = mV[7];
		col3.mz = mV[8];

	}   



	tintaVector3f tintaMatrix33::getColumn( m_uint32 i ) const {	
		return tintaVector3f( mV[i], mV[i + 3], mV[i + 6] );
	}   



	void tintaMatrix33::clean()
	{
		for (m_uint32 i = 0; i < 9; ++i)
		{
			if ( TintaMath::isZero( mV[i] ) )
				mV[i] = 0.0f;
		}
	}   

	void tintaMatrix33::identity(){
		mV[0] = 1.0f;
		mV[1] = 0.0f;
		mV[2] = 0.0f;
		mV[3] = 0.0f;
		mV[4] = 1.0f;
		mV[5] = 0.0f;
		mV[6] = 0.0f;
		mV[7] = 0.0f;
		mV[8] = 1.0f;
	}

	tintaMatrix33 inverse( const tintaMatrix33& mat ){
		tintaMatrix33 result;

		// determinant
		float cofactor0 = mat.mV[4]*mat.mV[8] - mat.mV[5]*mat.mV[7];
		float cofactor3 = mat.mV[2]*mat.mV[7] - mat.mV[1]*mat.mV[8];
		float cofactor6 = mat.mV[1]*mat.mV[5] - mat.mV[2]*mat.mV[4];
		float det = mat.mV[0]*cofactor0 + mat.mV[3]*cofactor3 + mat.mV[6]*cofactor6;

		if ( TintaMath::isZero( det )) {		
			return result;
		}

		// creating adjoint matrix and multiply by 1/det to get inverse
		float invDet = 1.0f/det;

		result.mV[0] *= invDet; 
		result.mV[1] *= invDet; 
		result.mV[2] *= invDet; 

		result.mV[3] *= invDet; 
		result.mV[4] *= invDet; 
		result.mV[5] *= invDet; 

		result.mV[6] *= invDet; 
		result.mV[7] *= invDet; 
		result.mV[8] *= invDet; 

		return result;

	}   
	
	

	tintaMatrix33& tintaMatrix33::inverse()
	{
		//const tintaMatrix33& mat = *this;
		*this = Tinta::inverse(*this);
		return *this;
	}  

	tintaMatrix33 tintaMatrix33::inverse() const
	{
		tintaMatrix33 mat_ret  = Tinta::inverse(*this);
		return mat_ret;
	}
	tintaMatrix33 tintaMatrix33::transpose() const
	{
        return tintaMatrix33 (mV[0], mV[3], mV[6], mV[1], mV[4], mV[7], mV[2], mV[5], mV[8]);
	}

	tintaMatrix33 tintaMatrix33::transpose( const tintaMatrix33& mat )
	{
		return tintaMatrix33(mV[0]*mat.mV[0] +
			mV[3]*mat.mV[3] +
			mV[6]*mat.mV[6],

			mV[0]*mat.mV[1] +
			mV[3]*mat.mV[4] +
			mV[6]*mat.mV[7],

			mV[0]*mat.mV[2] +
			mV[3]*mat.mV[5] +
			mV[6]*mat.mV[8],

			mV[1]*mat.mV[0] +
			mV[4]*mat.mV[3] +
			mV[7]*mat.mV[6],

			mV[1]*mat.mV[1] +
			mV[4]*mat.mV[4] +
			mV[7]*mat.mV[7],

			mV[1]*mat.mV[2] +
			mV[4]*mat.mV[5] +
			mV[7]*mat.mV[8],

			mV[2]*mat.mV[0] +
			mV[5]*mat.mV[3] +
			mV[8]*mat.mV[6],

			mV[2]*mat.mV[1] +
			mV[5]*mat.mV[4] +
			mV[8]*mat.mV[7],

			mV[2]*mat.mV[2] +
			mV[5]*mat.mV[5] +
			mV[8]*mat.mV[8]);

	} 



	float tintaMatrix33::determinant() const
	{
		return mV[0]*(mV[4]*mV[8] - mV[5]*mV[7]) 
			 + mV[1]*(mV[5]*mV[6] - mV[3]*mV[8])
			 + mV[2]*(mV[3]*mV[7] - mV[4]*mV[6]);

	} 

	tintaMatrix33 tintaMatrix33::adjoint() const
	{
		return tintaMatrix33 (
		// compute transpose of cofactors
		mV[4]*mV[8] - mV[5]*mV[7],  mV[2]*mV[7] - mV[1]*mV[8],	mV[1]*mV[5] - mV[2]*mV[4],

		mV[5]*mV[6] - mV[3]*mV[8],	mV[0]*mV[8] - mV[2]*mV[6],	mV[2]*mV[3] - mV[0]*mV[5],

		mV[3]*mV[7] - mV[4]*mV[6],	mV[1]*mV[6] - mV[0]*mV[7],	mV[0]*mV[4] - mV[1]*mV[3]
		);
	}  
	float tintaMatrix33::trace() const
	{
		return mV[0] + mV[4] + mV[8];

	}   

	tintaMatrix33& tintaMatrix33::rotation( const tintaQuatern& rotate )
	{
		
		float xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;

		xs = rotate.mx+rotate.mx;   
		ys = rotate.my+rotate.my;
		zs = rotate.mz+rotate.mz;

		wx = rotate.mw*xs;
		wy = rotate.mw*ys;
		wz = rotate.mw*zs;

		xx = rotate.mx*xs;
		xy = rotate.mx*ys;
		xz = rotate.mx*zs;

		yy = rotate.my*ys;
		yz = rotate.my*zs;
		zz = rotate.mz*zs;

		mV[0] = 1.0f - (yy + zz);
		mV[1] = xy - wz;
		mV[2] = xz + wy;

		mV[3] = xy + wz;
		mV[4] = 1.0f - (xx + zz);
		mV[5] = yz - wx;

		mV[6] = xz - wy;
		mV[7] = yz + wx;
		mV[8] = 1.0f - (xx + yy);

		return *this;

	}



	tintaMatrix33 &tintaMatrix33::rotation( float zRotation, float yRotation, float xRotation )
	{
		// This is an "unrolled" contatenation of rotation matrices X Y & Z
		float Cx, Sx;
		TintaMath::sincos(xRotation, Sx, Cx);

		float Cy, Sy;
        TintaMath::sincos(yRotation, Sy, Cy);

		float Cz, Sz;
        TintaMath::sincos(zRotation, Sz, Cz);

		mV[0] =  (Cy * Cz);
		mV[1] = -(Cy * Sz);  
		mV[2] =  Sy;

		mV[3] =  (Sx * Sy * Cz) + (Cx * Sz);
		mV[4] = -(Sx * Sy * Sz) + (Cx * Cz);
		mV[5] = -(Sx * Cy); 

		mV[6] = -(Cx * Sy * Cz) + (Sx * Sz);
		mV[7] =  (Cx * Sy * Sz) + (Sx * Cz);
		mV[8] =  (Cx * Cy);

		return *this;

	} 	
	
	tintaMatrix33 &
	tintaMatrix33::rotation( const tintaVector3f& axis, float angle )
	{
		float c, s;
        TintaMath::sincos(angle, s, c);
		float t = 1.0f - c;

		tintaVector3f nAxis = axis;
		nAxis.normalize();

		// intermediate values
		float tx = t*nAxis.mx;  float ty = t*nAxis.my;  float tz = t*nAxis.mz;
		float sx = s*nAxis.mx;  float sy = s*nAxis.my;  float sz = s*nAxis.mz;
		float txy = tx*nAxis.my; float tyz = tx*nAxis.mz; float txz = tx*nAxis.mz;

		// set matrix
		mV[0] = tx*nAxis.mx + c;
		mV[1] = txy - sz;
		mV[2] = txz + sy;
		mV[3] = txy + sz;
		mV[4] = ty*nAxis.my + c;
		mV[5] = tyz - sx;
		mV[6] = txz - sy;
		mV[7] = tyz + sx;
		mV[8] = tz*nAxis.mz + c;

		return *this;

	} 



	tintaMatrix33& tintaMatrix33::scaling( const tintaVector3f& scaleFactors )
	{
		mV[0] = scaleFactors.mx;
		mV[1] = 0.0f;
		mV[2] = 0.0f;
		mV[3] = 0.0f;
		mV[4] = scaleFactors.my;
		mV[5] = 0.0f;
		mV[6] = 0.0f;
		mV[7] = 0.0f;
		mV[8] = scaleFactors.mz;

		return *this;
	}  

	
	tintaMatrix33& tintaMatrix33::rotationX( float angle )
	{
		// RH
		float sintheta, costheta;
		TintaMath::sincos(angle, sintheta, costheta);

		mV[0] = 1.0f;
		mV[1] = 0.0f;
		mV[2] = 0.0f;
		mV[3] = 0.0f;
		mV[4] = costheta;
		mV[5] = sintheta;
		mV[6] = 0.0f;
		mV[7] = -sintheta;
		mV[8] = costheta;

		return *this;

	}   

	tintaMatrix33& tintaMatrix33::rotationY( float angle )
	{
		float sintheta, costheta;
		TintaMath::sincos(angle, sintheta, costheta);

		mV[0] = costheta;
		mV[1] = 0.0f;
		mV[2] = -sintheta;
		mV[3] = 0.0f;
		mV[4] = 1.0f;
		mV[5] = 0.0f;
		mV[6] = sintheta;
		mV[7] = 0.0f;
		mV[8] = costheta;

		return *this;

	}

	tintaMatrix33& tintaMatrix33::rotationZ( float angle )
	{
		float sintheta, costheta;
		TintaMath::sincos(angle, sintheta, costheta);

		mV[0] = costheta;
		mV[1] = sintheta;
		mV[2] = 0.0f;
		mV[3] = -sintheta;
		mV[4] = costheta;
		mV[5] = 0.0f;
		mV[6] = 0.0f;
		mV[7] = 0.0f;
		mV[8] = 1.0f;

		return *this;
	}


	void tintaMatrix33::getAxisAngle( tintaVector3f& axis, float& angle )
	{
		float trace = mV[0] + mV[4] + mV[8];
		float cosTheta = 0.5f*(trace - 1.0f);
		angle = acosf( cosTheta );

		// angle is zero, axis can be anything
		if ( TintaMath::isZero( angle ) )
		{
			axis = tintaVector3f::xAxis;
		}
		// standard case
		else if ( angle < K_PI-K_EPSILON )
		{
			axis.set( mV[7] - mV[5], mV[2] - mV[6], mV[3] - mV[1] );
			axis.normalize();
		}
		// angle is 180 degrees
		else
		{
			float fHalfInverse;

			if (mV[0] >= mV[4])
			{
				// r00 >= r11
				if (mV[0] >= mV[8])
				{
					// r00 is maximum diagonal term
					axis[0] = 0.5f*TintaMath::sqrt(mV[0] - mV[4] - mV[8] + 1.0f);
					fHalfInverse = 0.5f / axis[0];
					axis[1] = fHalfInverse*mV[1];
					axis[2] = fHalfInverse*mV[2];
				}
				else
				{
					// r22 is maximum diagonal term
					axis[2] = 0.5f * TintaMath::sqrt(mV[8] - mV[0] - mV[4] + 1.0f);
					fHalfInverse =  0.5f / axis[2];
					axis[0] = fHalfInverse*mV[2];
					axis[1] = fHalfInverse*mV[5];
				}
			}
			else {
				// r11 > r00
				if (mV[4] >= mV[8])
				{
					// r11 is maximum diagonal term
					axis[1] = 0.5f*TintaMath::sqrt(mV[4] -mV[0] - mV[8] + 1.0f);
					fHalfInverse  = 0.5f/axis[1];
					axis[0] = fHalfInverse*mV[1];
					axis[2] = fHalfInverse*mV[5];
				}
				else
				{
					// r22 is maximum diagonal term
					axis[2] = 0.5f*TintaMath::sqrt(mV[8] -  mV[0] - mV[4] +1.0f);
					fHalfInverse = 0.5f / axis[2];
					axis[0] = fHalfInverse*mV[2];
					axis[1] = fHalfInverse*mV[5];
				}
			}
		}

	}

	tintaMatrix33 tintaMatrix33::operator+( const tintaMatrix33& other ) const
	{
		tintaMatrix33 result;

		for (m_uint32 i = 0; i < 9; ++i)
		{
			result.mV[i] = mV[i] + other.mV[i];
		}

		return result;
	}   


	tintaMatrix33& tintaMatrix33::operator+=( const tintaMatrix33& other )
	{
		for (m_uint32 i = 0; i < 9; ++i)
		{
			mV[i] += other.mV[i];
		}

		return *this;

	} 



	tintaMatrix33 tintaMatrix33::operator-( const tintaMatrix33& other ) const
	{
		tintaMatrix33 result;

		for (m_uint32 i = 0; i < 9; ++i)
		{
			result.mV[i] = mV[i] - other.mV[i];
		}

		return result;

	}  



	tintaMatrix33& tintaMatrix33::operator-=( const tintaMatrix33& other )
	{
		for (m_uint32 i = 0; i < 9; ++i)
		{
			mV[i] -= other.mV[i];
		}
		return *this;
	}   



	tintaMatrix33 tintaMatrix33::operator-() const
	{
		tintaMatrix33 result;

		for (m_uint32 i = 0; i < 16; ++i)
		{
			result.mV[i] = -mV[i];
		}

		return result;

	}    



	tintaMatrix33 tintaMatrix33::operator*( const tintaMatrix33& other ) const
	{
		
		return tintaMatrix33(mV[0]*other.mV[0] +
			mV[1]*other.mV[3] +
			mV[2]*other.mV[6],

			mV[0]*other.mV[1] +
			mV[1]*other.mV[4] +
			mV[2]*other.mV[7],

			mV[0]*other.mV[2] +
			mV[1]*other.mV[5] +
			mV[2]*other.mV[8],

			mV[3]*other.mV[0] +
			mV[4]*other.mV[3] +
			mV[5]*other.mV[6],

			mV[3]*other.mV[1] +
			mV[4]*other.mV[4] +
			mV[5]*other.mV[7],

			mV[3]*other.mV[2] +
			mV[4]*other.mV[5] +
			mV[5]*other.mV[8],

			mV[6]*other.mV[0] +
			mV[7]*other.mV[3] +
			mV[8]*other.mV[6],

			mV[6]*other.mV[1] +
			mV[7]*other.mV[4] +
			mV[8]*other.mV[7],

			mV[6]*other.mV[2] +
			mV[7]*other.mV[5] +
			mV[8]*other.mV[8]);
	
	} 



	tintaMatrix33& tintaMatrix33::operator*=( const tintaMatrix33& other )
	{
		tintaMatrix33 result(mV[0]*other.mV[0] +
			mV[1]*other.mV[3] +
			mV[2]*other.mV[6],

			mV[0]*other.mV[1] +
			mV[1]*other.mV[4] +
			mV[2]*other.mV[7],

			mV[0]*other.mV[2] +
			mV[1]*other.mV[5] +
			mV[2]*other.mV[8],

			mV[3]*other.mV[0] +
			mV[4]*other.mV[3] +
			mV[5]*other.mV[6],

			mV[3]*other.mV[1] +
			mV[4]*other.mV[4] +
			mV[5]*other.mV[7],

			mV[3]*other.mV[2] +
			mV[4]*other.mV[5] +
			mV[5]*other.mV[8],

			mV[6]*other.mV[0] +
			mV[7]*other.mV[3] +
			mV[8]*other.mV[6],

			mV[6]*other.mV[1] +
			mV[7]*other.mV[4] +
			mV[8]*other.mV[7],

			mV[6]*other.mV[2] +
			mV[7]*other.mV[5] +
			mV[8]*other.mV[8]);

		

		for (m_uint32 i = 0; i < 9; ++i)
		{
			mV[i] = result.mV[i];
		}

		return *this;
	}   



	tintaVector3f tintaMatrix33::operator*( const tintaVector3f& other ) const
	{
		tintaVector3f result;

		result.mx = mV[0]*other.mx + mV[1]*other.my + mV[2]*other.mz;
		result.my = mV[3]*other.mx + mV[4]*other.my + mV[5]*other.mz;
		result.mz = mV[6]*other.mx + mV[7]*other.my + mV[8]*other.mz;

		return result;
	}   


    tintaVector2f tintaMatrix33::operator*(const tintaVector2f& other) const
    {
        tintaVector2f result;

        result.mx = mV[0] * other.mx + mV[1] * other.my + mV[2];
        result.my = mV[3] * other.mx + mV[4] * other.my + mV[5];

        return result;
    }



	tintaVector3f operator*( const tintaVector3f& vector, const tintaMatrix33& mat )
	{
		tintaVector3f result;

		result.mx = mat.mV[0]*vector.mx + mat.mV[1]*vector.my + mat.mV[2]*vector.mz;
		result.my = mat.mV[3]*vector.mx + mat.mV[4]*vector.my + mat.mV[5]*vector.mz;
		result.mz = mat.mV[6]*vector.mx + mat.mV[7]*vector.my + mat.mV[8]*vector.mz;

		return result;

	} 



	tintaMatrix33& tintaMatrix33::operator*=( float scalar )
	{
		mV[0] *= scalar;
		mV[1] *= scalar;
		mV[2] *= scalar;
		mV[3] *= scalar;
		mV[4] *= scalar;
		mV[5] *= scalar;
		mV[6] *= scalar;
		mV[7] *= scalar;
		mV[8] *= scalar;
		return *this;
	}  



	tintaMatrix33 operator*( float scalar, const tintaMatrix33& matrix )
	{
		tintaMatrix33 result;
		result.mV[0] = matrix.mV[0] * scalar;
		result.mV[1] = matrix.mV[1] * scalar;
		result.mV[2] = matrix.mV[2] * scalar;
		result.mV[3] = matrix.mV[3] * scalar;
		result.mV[4] = matrix.mV[4] * scalar;
		result.mV[5] = matrix.mV[5] * scalar;
		result.mV[6] = matrix.mV[6] * scalar;
		result.mV[7] = matrix.mV[7] * scalar;
		result.mV[8] = matrix.mV[8] * scalar;

		return result;
	}



	tintaMatrix33 tintaMatrix33::operator*( float scalar ) const
	{
		tintaMatrix33 result;
		result.mV[0] = mV[0] * scalar;
		result.mV[1] = mV[1] * scalar;
		result.mV[2] = mV[2] * scalar;
		result.mV[3] = mV[3] * scalar;
		result.mV[4] = mV[4] * scalar;
		result.mV[5] = mV[5] * scalar;
		result.mV[6] = mV[6] * scalar;
		result.mV[7] = mV[7] * scalar;
		result.mV[8] = mV[8] * scalar;

		return result;
	}

	tintaMatrix33 tintaMatrix33::operator/ (float fScalar) const
	{
		if (!TintaMath::isZero(fScalar)) {
			float fInvScalar = ((float)1.0)/fScalar;
			return tintaMatrix33(
				fInvScalar*mV[0],
				fInvScalar*mV[1],
				fInvScalar*mV[2],
				fInvScalar*mV[3],
				fInvScalar*mV[4],
				fInvScalar*mV[5],
				fInvScalar*mV[6],
				fInvScalar*mV[7],
				fInvScalar*mV[8]);
		}

		return tintaMatrix33(
			FLT_MAX,
			FLT_MAX,
			FLT_MAX,
			FLT_MAX,
			FLT_MAX,
			FLT_MAX,
			FLT_MAX,
			FLT_MAX,
			FLT_MAX);
	}


}
