/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaMatrix44.h"
#include "tintaMatrix33.h"
#include "tintaVector4.h"
#include "tintaVector3.h"

using namespace Tinta;

const tintaMatrix44 tintaMatrix44::zero_val(0.0f,0.0f,0.0f,0.0f,
										 0.0f,0.0f,0.0f,0.0f,
										 0.0f,0.0f,0.0f,0.0f,
										 0.0f,0.0f,0.0f,0.0f);

const tintaMatrix44 tintaMatrix44::identity_val(1.0f,0.0f,0.0f,0.0f,
											 0.0f,1.0f,0.0f,0.0f,
											 0.0f,0.0f,1.0f,0.0f,
											 0.0f,0.0f,0.0f,1.0f);

const tintaMatrix44 tintaMatrix44::basis(	-0.5f, 1.5f,-1.5f, 0.5f,
									     1.0f,-2.5f, 2.0f,-0.5f,
									    -0.5f, 0.0f, 0.5f, 0.0f,
									     0.0f, 1.0f, 0.0f, 1.0f    );


	tintaMatrix44::tintaMatrix44(const tintaMatrix44& other) {
		mV[0] = other.mV[0];
		mV[1] = other.mV[1];
		mV[2] = other.mV[2];
		mV[3] = other.mV[3];
		mV[4] = other.mV[4];
		mV[5] = other.mV[5];
		mV[6] = other.mV[6];
		mV[7] = other.mV[7];
		mV[8] = other.mV[8];
		mV[9] = other.mV[9];
		mV[10] = other.mV[10];
		mV[11] = other.mV[11];
		mV[12] = other.mV[12];
		mV[13] = other.mV[13];
		mV[14] = other.mV[14];
		mV[15] = other.mV[15];
	}

	tintaMatrix44::tintaMatrix44 (float fM00, float fM01, float fM02, float fM03,
		float fM10, float fM11, float fM12, float fM13, float fM20, float fM21,
		float fM22, float fM23, float fM30, float fM31, float fM32, float fM33)	{
		mV[ 0] = fM00;
		mV[ 1] = fM01;
		mV[ 2] = fM02;
		mV[ 3] = fM03;
		mV[ 4] = fM10;
		mV[ 5] = fM11;
		mV[ 6] = fM12;
		mV[ 7] = fM13;
		mV[ 8] = fM20;
		mV[ 9] = fM21;
		mV[10] = fM22;
		mV[11] = fM23;
		mV[12] = fM30;
		mV[13] = fM31;
		mV[14] = fM32;
		mV[15] = fM33;

	}
	

	tintaMatrix44&	tintaMatrix44::operator=(const tintaMatrix44& other){
	
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
		mV[9] = other.mV[9];
		mV[10] = other.mV[10];
		mV[11] = other.mV[11];
		mV[12] = other.mV[12];
		mV[13] = other.mV[13];
		mV[14] = other.mV[14];
		mV[15] = other.mV[15];

		return *this;

	}
	
	
	bool tintaMatrix44::operator==( const tintaMatrix44& other ) const {
		for (m_uint32 i = 0; i < 16; ++i)
		{
			if ( !TintaMath::areEqual(mV[i], other.mV[i]) )
				return false;
		}
		return true;
	}   


	
	bool tintaMatrix44::operator!=( const tintaMatrix44& other ) const {
		for (m_uint32 i = 0; i < 16; ++i)
		{
			if ( !TintaMath::areEqual(mV[i], other.mV[i]) )
				return true;
		}
		return false;
	} 

	bool tintaMatrix44::isZeroVal() const {
		for (m_uint32 i = 0; i < 16; ++i)
		{
			if ( !TintaMath::isZero( mV[i] ) )
				return false;
		}
		return true;

	} 


	bool 	tintaMatrix44::isIdentity() const	{
		return TintaMath::areEqual( 1.0f, mV[0] )
			&& TintaMath::areEqual( 1.0f, mV[5] )
			&& TintaMath::areEqual( 1.0f, mV[10] )
			&& TintaMath::areEqual( 1.0f, mV[15] )
			&& TintaMath::isZero( mV[1] ) 
			&& TintaMath::isZero( mV[2] )
			&& TintaMath::isZero( mV[3] )
			&& TintaMath::isZero( mV[4] ) 
			&& TintaMath::isZero( mV[6] )
			&& TintaMath::isZero( mV[7] )
			&& TintaMath::isZero( mV[8] )
			&& TintaMath::isZero( mV[9] )
			&& TintaMath::isZero( mV[11] )
			&& TintaMath::isZero( mV[12] )
			&& TintaMath::isZero( mV[13] )
			&& TintaMath::isZero( mV[14] );

	} 

	
	void tintaMatrix44::clean() {
		for (m_uint32 i = 0; i < 16; ++i)
		{
			if ( TintaMath::isZero( mV[i] ) )
				mV[i] = 0.0f;
		}

	}

	void tintaMatrix44::identity()	{
		mV[0] = 1.0f;
		mV[1] = 0.0f;
		mV[2] = 0.0f;
		mV[3] = 0.0f;
		mV[4] = 0.0f;
		mV[5] = 1.0f;
		mV[6] = 0.0f;
		mV[7] = 0.0f;
		mV[8] = 0.0f;
		mV[9] = 0.0f;
		mV[10] = 1.0f;
		mV[11] = 0.0f;
		mV[12] = 0.0f;
		mV[13] = 0.0f;
		mV[14] = 0.0f;
		mV[15] = 1.0f;

	}  
	
	
	tintaMatrix44 tintaMatrix44::affineInverse(){
		
		float fA0 = mV[ 0]*mV[ 5] - mV[ 1]*mV[ 4];
		float fA1 = mV[ 0]*mV[ 6] - mV[ 2]*mV[ 4];
		float fA2 = mV[ 0]*mV[ 7] - mV[ 3]*mV[ 4];
		float fA3 = mV[ 1]*mV[ 6] - mV[ 2]*mV[ 5];
		float fA4 = mV[ 1]*mV[ 7] - mV[ 3]*mV[ 5];
		float fA5 = mV[ 2]*mV[ 7] - mV[ 3]*mV[ 6];
		float fB0 = mV[ 8]*mV[13] - mV[ 9]*mV[12];
		float fB1 = mV[ 8]*mV[14] - mV[10]*mV[12];
		float fB2 = mV[ 8]*mV[15] - mV[11]*mV[12];
		float fB3 = mV[ 9]*mV[14] - mV[10]*mV[13];
		float fB4 = mV[ 9]*mV[15] - mV[11]*mV[13];
		float fB5 = mV[10]*mV[15] - mV[11]*mV[14];

		float fDet = fA0*fB5-fA1*fB4+fA2*fB3+fA3*fB2-fA4*fB1+fA5*fB0;
		if ( TintaMath::isZero(fDet) ) {
			return tintaMatrix44::zero_val;
		}

		tintaMatrix44 result;

		result.mV[ 0] =+ mV[ 5]*fB5 - mV[ 6]*fB4 + mV[ 7]*fB3;
		result.mV[ 4] =- mV[ 4]*fB5 + mV[ 6]*fB2 - mV[ 7]*fB1;
		result.mV[ 8] =+ mV[ 4]*fB4 - mV[ 5]*fB2 + mV[ 7]*fB0;
		result.mV[12] =- mV[ 4]*fB3 + mV[ 5]*fB1 - mV[ 6]*fB0;
		result.mV[ 1] =- mV[ 1]*fB5 + mV[ 2]*fB4 - mV[ 3]*fB3;
		result.mV[ 5] =+ mV[ 0]*fB5 - mV[ 2]*fB2 + mV[ 3]*fB1;
		result.mV[ 9] =- mV[ 0]*fB4 + mV[ 1]*fB2 - mV[ 3]*fB0;
		result.mV[13] =+ mV[ 0]*fB3 - mV[ 1]*fB1 + mV[ 2]*fB0;
		result.mV[ 2] =+ mV[13]*fA5 - mV[14]*fA4 + mV[15]*fA3;
		result.mV[ 6] =- mV[12]*fA5 + mV[14]*fA2 - mV[15]*fA1;
		result.mV[10] =+ mV[12]*fA4 - mV[13]*fA2 + mV[15]*fA0;
		result.mV[14] =- mV[12]*fA3 + mV[13]*fA1 - mV[14]*fA0;
		result.mV[ 3] =- mV[ 9]*fA5 + mV[10]*fA4 - mV[11]*fA3;
		result.mV[ 7] =+ mV[ 8]*fA5 - mV[10]*fA2 + mV[11]*fA1;
		result.mV[11] =- mV[ 8]*fA4 + mV[ 9]*fA2 - mV[11]*fA0;
		result.mV[15] =+ mV[ 8]*fA3 - mV[ 9]*fA1 + mV[10]*fA0;

		float f_inv_det = 1.0f / fDet;
		result.mV[ 0] *= f_inv_det;
		result.mV[ 1] *= f_inv_det;
		result.mV[ 2] *= f_inv_det;
		result.mV[ 3] *= f_inv_det;
		result.mV[ 4] *= f_inv_det;
		result.mV[ 5] *= f_inv_det;
		result.mV[ 6] *= f_inv_det;
		result.mV[ 7] *= f_inv_det;
		result.mV[ 8] *= f_inv_det;
		result.mV[ 9] *= f_inv_det;
		result.mV[10] *= f_inv_det;
		result.mV[11] *= f_inv_det;
		result.mV[12] *= f_inv_det;
		result.mV[13] *= f_inv_det;
		result.mV[14] *= f_inv_det;
		result.mV[15] *= f_inv_det;

		return result;

	}  

	tintaMatrix44 tintaMatrix44::transpose() {
		return tintaMatrix44(
			mV[ 0],	mV[ 4],	mV[ 8],	mV[12],
			mV[ 1],	mV[ 5],	mV[ 9],	mV[13],
			mV[ 2],	mV[ 6],	mV[10],	mV[14],
			mV[ 3],	mV[ 7],	mV[11],	mV[15]);	

	}
	
	tintaMatrix44 tintaMatrix44::transpose( const tintaMatrix44& mat ){
		
		// P = A^T*B
		return tintaMatrix44(
			mV[ 0]*mat.mV[ 0] +
			mV[ 4]*mat.mV[ 4] +
			mV[ 8]*mat.mV[ 8] +
			mV[12]*mat.mV[12],

			mV[ 0]*mat.mV[ 1] +
			mV[ 4]*mat.mV[ 5] +
			mV[ 8]*mat.mV[ 9] +
			mV[12]*mat.mV[13],

			mV[ 0]*mat.mV[ 2] +
			mV[ 4]*mat.mV[ 6] +
			mV[ 8]*mat.mV[10] +
			mV[12]*mat.mV[14],

			mV[ 0]*mat.mV[ 3] +
			mV[ 4]*mat.mV[ 7] +
			mV[ 8]*mat.mV[11] +
			mV[12]*mat.mV[15],

			mV[ 1]*mat.mV[ 0] +
			mV[ 5]*mat.mV[ 4] +
			mV[ 9]*mat.mV[ 8] +
			mV[13]*mat.mV[12],

			mV[ 1]*mat.mV[ 1] +
			mV[ 5]*mat.mV[ 5] +
			mV[ 9]*mat.mV[ 9] +
			mV[13]*mat.mV[13],

			mV[ 1]*mat.mV[ 2] +
			mV[ 5]*mat.mV[ 6] +
			mV[ 9]*mat.mV[10] +
			mV[13]*mat.mV[14],

			mV[ 1]*mat.mV[ 3] +
			mV[ 5]*mat.mV[ 7] +
			mV[ 9]*mat.mV[11] +
			mV[13]*mat.mV[15],

			mV[ 2]*mat.mV[ 0] +
			mV[ 6]*mat.mV[ 4] +
			mV[10]*mat.mV[ 8] +
			mV[14]*mat.mV[12],

			mV[ 2]*mat.mV[ 1] +
			mV[ 6]*mat.mV[ 5] +
			mV[10]*mat.mV[ 9] +
			mV[14]*mat.mV[13],

			mV[ 2]*mat.mV[ 2] +
			mV[ 6]*mat.mV[ 6] +
			mV[10]*mat.mV[10] +
			mV[14]*mat.mV[14],

			mV[ 2]*mat.mV[ 3] +
			mV[ 6]*mat.mV[ 7] +
			mV[10]*mat.mV[11] +
			mV[14]*mat.mV[15],

			mV[ 3]*mat.mV[ 0] +
			mV[ 7]*mat.mV[ 4] +
			mV[11]*mat.mV[ 8] +
			mV[15]*mat.mV[12],

			mV[ 3]*mat.mV[ 1] +
			mV[ 7]*mat.mV[ 5] +
			mV[11]*mat.mV[ 9] +
			mV[15]*mat.mV[13],

			mV[ 3]*mat.mV[ 2] +
			mV[ 7]*mat.mV[ 6] +
			mV[11]*mat.mV[10] +
			mV[15]*mat.mV[14],

			mV[ 3]*mat.mV[ 3] +
			mV[ 7]*mat.mV[ 7] +
			mV[11]*mat.mV[11] +
			mV[15]*mat.mV[15]);

		

	}

	tintaMatrix44& tintaMatrix44::translation( const tintaVector3f& xlate ){
		mV[0] = 1.0f;
		mV[1] = 0.0f;
		mV[2] = 0.0f;
		mV[3] = xlate.mx;
		mV[4] = 0.0f;
		mV[5] = 1.0f;
		mV[6] = 0.0f;
		mV[7] = xlate.my;
		mV[8] = 0.0f;
		mV[9] = 0.0f;
		mV[10] = 1.0f;
		mV[11] = xlate.mz;
		mV[12] = 0;
		mV[13] = 0;
		mV[14] = 0;			
		mV[15] = 1.0f;

		return *this;

	}  

	
	tintaMatrix44 tintaMatrix44::operator+( const tintaMatrix44& other ) const {
		tintaMatrix44 result;

		for (m_uint32 i = 0; i < 16; ++i)
		{
			result.mV[i] = mV[i] + other.mV[i];
		}

		return result;

	}


	
	tintaMatrix44& tintaMatrix44::operator+=( const tintaMatrix44& other )	{
		for (m_uint32 i = 0; i < 16; ++i)
		{
			mV[i] += other.mV[i];
		}

		return *this;

	} 
	
	tintaMatrix44 tintaMatrix44::operator-( const tintaMatrix44& other ) const	{
		tintaMatrix44 result;

		for (m_uint32 i = 0; i < 16; ++i)
		{
			result.mV[i] = mV[i] - other.mV[i];
		}

		return result;

	} 
	
	tintaMatrix44& tintaMatrix44::operator-=( const tintaMatrix44& other )	{
		for (m_uint32 i = 0; i < 16; ++i)
		{
			mV[i] -= other.mV[i];
		}

		return *this;

	}
	
	tintaMatrix44 tintaMatrix44::operator-() const {
		tintaMatrix44 result;

		for (m_uint32 i = 0; i < 16; ++i)
		{
			result.mV[i] = -mV[i];
		}

		return result;

	}


	
	tintaMatrix44  tintaMatrix44::operator*( const tintaMatrix44& other ) const {
		

		return tintaMatrix44(
			mV[ 0]*other.mV[ 0] +
			mV[ 1]*other.mV[ 4] +
			mV[ 2]*other.mV[ 8] +
			mV[ 3]*other.mV[12],

			mV[ 0]*other.mV[ 1] +
			mV[ 1]*other.mV[ 5] +
			mV[ 2]*other.mV[ 9] +
			mV[ 3]*other.mV[13],

			mV[ 0]*other.mV[ 2] +
			mV[ 1]*other.mV[ 6] +
			mV[ 2]*other.mV[10] +
			mV[ 3]*other.mV[14],

			mV[ 0]*other.mV[ 3] +
			mV[ 1]*other.mV[ 7] +
			mV[ 2]*other.mV[11] +
			mV[ 3]*other.mV[15],

			mV[ 4]*other.mV[ 0] +
			mV[ 5]*other.mV[ 4] +
			mV[ 6]*other.mV[ 8] +
			mV[ 7]*other.mV[12],

			mV[ 4]*other.mV[ 1] +
			mV[ 5]*other.mV[ 5] +
			mV[ 6]*other.mV[ 9] +
			mV[ 7]*other.mV[13],

			mV[ 4]*other.mV[ 2] +
			mV[ 5]*other.mV[ 6] +
			mV[ 6]*other.mV[10] +
			mV[ 7]*other.mV[14],

			mV[ 4]*other.mV[ 3] +
			mV[ 5]*other.mV[ 7] +
			mV[ 6]*other.mV[11] +
			mV[ 7]*other.mV[15],

			mV[ 8]*other.mV[ 0] +
			mV[ 9]*other.mV[ 4] +
			mV[10]*other.mV[ 8] +
			mV[11]*other.mV[12],

			mV[ 8]*other.mV[ 1] +
			mV[ 9]*other.mV[ 5] +
			mV[10]*other.mV[ 9] +
			mV[11]*other.mV[13],

			mV[ 8]*other.mV[ 2] +
			mV[ 9]*other.mV[ 6] +
			mV[10]*other.mV[10] +
			mV[11]*other.mV[14],

			mV[ 8]*other.mV[ 3] +
			mV[ 9]*other.mV[ 7] +
			mV[10]*other.mV[11] +
			mV[11]*other.mV[15],

			mV[12]*other.mV[ 0] +
			mV[13]*other.mV[ 4] +
			mV[14]*other.mV[ 8] +
			mV[15]*other.mV[12],

			mV[12]*other.mV[ 1] +
			mV[13]*other.mV[ 5] +
			mV[14]*other.mV[ 9] +
			mV[15]*other.mV[13],

			mV[12]*other.mV[ 2] +
			mV[13]*other.mV[ 6] +
			mV[14]*other.mV[10] +
			mV[15]*other.mV[14],

			mV[12]*other.mV[ 3] +
			mV[13]*other.mV[ 7] +
			mV[14]*other.mV[11] +
			mV[15]*other.mV[15]);

	} 


	tintaMatrix44 &tintaMatrix44::operator*=( const tintaMatrix44& other )	{
		tintaMatrix44 result(
			mV[ 0]*other.mV[ 0] +
			mV[ 1]*other.mV[ 4] +
			mV[ 2]*other.mV[ 8] +
			mV[ 3]*other.mV[12],

			mV[ 0]*other.mV[ 1] +
			mV[ 1]*other.mV[ 5] +
			mV[ 2]*other.mV[ 9] +
			mV[ 3]*other.mV[13],

			mV[ 0]*other.mV[ 2] +
			mV[ 1]*other.mV[ 6] +
			mV[ 2]*other.mV[10] +
			mV[ 3]*other.mV[14],

			mV[ 0]*other.mV[ 3] +
			mV[ 1]*other.mV[ 7] +
			mV[ 2]*other.mV[11] +
			mV[ 3]*other.mV[15],

			mV[ 4]*other.mV[ 0] +
			mV[ 5]*other.mV[ 4] +
			mV[ 6]*other.mV[ 8] +
			mV[ 7]*other.mV[12],

			mV[ 4]*other.mV[ 1] +
			mV[ 5]*other.mV[ 5] +
			mV[ 6]*other.mV[ 9] +
			mV[ 7]*other.mV[13],

			mV[ 4]*other.mV[ 2] +
			mV[ 5]*other.mV[ 6] +
			mV[ 6]*other.mV[10] +
			mV[ 7]*other.mV[14],

			mV[ 4]*other.mV[ 3] +
			mV[ 5]*other.mV[ 7] +
			mV[ 6]*other.mV[11] +
			mV[ 7]*other.mV[15],

			mV[ 8]*other.mV[ 0] +
			mV[ 9]*other.mV[ 4] +
			mV[10]*other.mV[ 8] +
			mV[11]*other.mV[12],

			mV[ 8]*other.mV[ 1] +
			mV[ 9]*other.mV[ 5] +
			mV[10]*other.mV[ 9] +
			mV[11]*other.mV[13],

			mV[ 8]*other.mV[ 2] +
			mV[ 9]*other.mV[ 6] +
			mV[10]*other.mV[10] +
			mV[11]*other.mV[14],

			mV[ 8]*other.mV[ 3] +
			mV[ 9]*other.mV[ 7] +
			mV[10]*other.mV[11] +
			mV[11]*other.mV[15],

			mV[12]*other.mV[ 0] +
			mV[13]*other.mV[ 4] +
			mV[14]*other.mV[ 8] +
			mV[15]*other.mV[12],

			mV[12]*other.mV[ 1] +
			mV[13]*other.mV[ 5] +
			mV[14]*other.mV[ 9] +
			mV[15]*other.mV[13],

			mV[12]*other.mV[ 2] +
			mV[13]*other.mV[ 6] +
			mV[14]*other.mV[10] +
			mV[15]*other.mV[14],

			mV[12]*other.mV[ 3] +
			mV[13]*other.mV[ 7] +
			mV[14]*other.mV[11] +
			mV[15]*other.mV[15]);

		for (m_uint32 i = 0; i < 16; ++i)
		{
			mV[i] = result.mV[i];
		}

		return *this;

	}  


	
	tintaVector4f tintaMatrix44::operator*( const tintaVector4f& other ) const {
		

		 /*return tintaVector4f(
        mV[ 0]*other[0] +
        mV[ 1]*other[1] +
        mV[ 2]*other[2] +
        mV[ 3]*other[3],

        mV[ 4]*other[0] +
        mV[ 5]*other[1] +
        mV[ 6]*other[2] +
        mV[ 7]*other[3],

        mV[ 8]*other[0] +
        mV[ 9]*other[1] +
        mV[10]*other[2] +
        mV[11]*other[3],

        mV[12]*other[0] +
        mV[13]*other[1] +
        mV[14]*other[2] +
        mV[15]*other[3]);		*/

        return tintaVector4f(
        mV[ 0]*other[0] +
        mV[ 4]*other[1] +
        mV[ 8]*other[2] +
        mV[ 12]*other[3],

        mV[ 1]*other[0] +
        mV[ 5]*other[1] +
        mV[ 9]*other[2] +
        mV[ 13]*other[3],

        mV[ 2]*other[0] +
        mV[ 6]*other[1] +
        mV[10]*other[2] +
        mV[14]*other[3],

        mV[3]*other[0] +
        mV[7]*other[1] +
        mV[11]*other[2] +
        mV[15]*other[3]);		
	}


	tintaMatrix44& tintaMatrix44::operator*=( float scalar ) {
		mV[0] *= scalar;
		mV[1] *= scalar;
		mV[2] *= scalar;
		mV[3] *= scalar;
		mV[4] *= scalar;
		mV[5] *= scalar;
		mV[6] *= scalar;
		mV[7] *= scalar;
		mV[8] *= scalar;
		mV[9] *= scalar;
		mV[10] *= scalar;
		mV[11] *= scalar;
		mV[12] *= scalar;
		mV[13] *= scalar;
		mV[14] *= scalar;
		mV[15] *= scalar;

		return *this;
	}

	tintaMatrix44 operator*( float scalar, const tintaMatrix44& matrix ) {
		tintaMatrix44 result;
		result.mV[0] = matrix.mV[0] * scalar;
		result.mV[1] = matrix.mV[1] * scalar;
		result.mV[2] = matrix.mV[2] * scalar;
		result.mV[3] = matrix.mV[3] * scalar;
		result.mV[4] = matrix.mV[4] * scalar;
		result.mV[5] = matrix.mV[5] * scalar;
		result.mV[6] = matrix.mV[6] * scalar;
		result.mV[7] = matrix.mV[7] * scalar;
		result.mV[8] = matrix.mV[8] * scalar;
		result.mV[9] = matrix.mV[9] * scalar;
		result.mV[10] = matrix.mV[10] * scalar;
		result.mV[11] = matrix.mV[11] * scalar;
		result.mV[12] = matrix.mV[12] * scalar;
		result.mV[13] = matrix.mV[13] * scalar;
		result.mV[14] = matrix.mV[14] * scalar;
		result.mV[15] = matrix.mV[15] * scalar;

		return result;
	}


	tintaMatrix44 tintaMatrix44::operator*( float scalar ) const {
		tintaMatrix44 result;
		result.mV[0] = mV[0] * scalar;
		result.mV[1] = mV[1] * scalar;
		result.mV[2] = mV[2] * scalar;
		result.mV[3] = mV[3] * scalar;
		result.mV[4] = mV[4] * scalar;
		result.mV[5] = mV[5] * scalar;
		result.mV[6] = mV[6] * scalar;
		result.mV[7] = mV[7] * scalar;
		result.mV[8] = mV[8] * scalar;
		result.mV[9] = mV[9] * scalar;
		result.mV[10] = mV[10] * scalar;
		result.mV[11] = mV[11] * scalar;
		result.mV[12] = mV[12] * scalar;
		result.mV[13] = mV[13] * scalar;
		result.mV[14] = mV[14] * scalar;
		result.mV[15] = mV[15] * scalar;

		return result;
	}

	
	tintaVector3f tintaMatrix44::transform( const tintaVector3f& other ) const {

		tintaVector3f result;
		result.mx = mV[0]*other.mx + mV[1]*other.my + mV[2]*other.mz + mV[3];
		result.my = mV[4]*other.mx + mV[5]*other.my + mV[6]*other.mz + mV[7];
		result.mz = mV[8]*other.mx + mV[9]*other.my + mV[10]*other.mz + mV[11];

		return result;

	}
	
	
