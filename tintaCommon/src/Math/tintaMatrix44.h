/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_MATRIX_44_H_
#define _TINTA_MATRIX_44_H_

#include "tintaQuatern.h"
#include "tintaVector3.h"
#include "tintaVector4.h"


namespace Tinta 
{
	
	class tintaMatrix33;

    //	c1  c2  c3
    // 	[0] [1] [2]   // [0][1]...[0][n]
    // 	[3] [4] [5]   // [1][1]...[1][n]
    // 	[6] [7] [8]   // [2][1]...[2][n]
    // 	[9] [10] [11] // [3][1]...[3][n]
	class _CoreExport tintaMatrix44 {
	public:
		inline tintaMatrix44() { identity(); }
		inline ~tintaMatrix44() {}
		//explicit tintaMatrix44( const tintaQuatern& quat );
		//explicit tintaMatrix44( const tintaMatrix33& matrix );		
		tintaMatrix44 (float fM00, float fM01, float fM02, float fM03,
					  float fM10, float fM11, float fM12, float fM13,
					  float fM20, float fM21, float fM22, float fM23, 
					  float fM30, float fM31, float fM32, float fM33);

		tintaMatrix44(const tintaMatrix44& other);
		tintaMatrix44& operator=(const tintaMatrix44& other);			
		
		float &operator()(m_uint32 i, m_uint32 j);
		float operator()(m_uint32 i, m_uint32 j) const;
		
		bool operator==( const tintaMatrix44& other ) const;
		bool operator!=( const tintaMatrix44& other ) const;

		inline const float* operator[] (int row) const;
		inline float* operator[] (int row);

		bool isZeroVal() const;
		bool isIdentity() const;
		
		void setRows( const tintaVector4f& row1, const tintaVector4f& row2, const tintaVector4f& row3, const tintaVector4f& row4 ); 
		void getRows( tintaVector4f& row1, tintaVector4f& row2, tintaVector4f& row3, tintaVector4f& row4 ); 

		void setColumns( const tintaVector4f& col1, const tintaVector4f& col2, const tintaVector4f& col3, const tintaVector4f& col4 ); 
		void getColumns( tintaVector4f& col1, tintaVector4f& col2, tintaVector4f& col3, tintaVector4f& col4 ); 
		void clean();
		void identity();

		tintaMatrix44 affineInverse();		

		tintaMatrix44 transpose();
		tintaMatrix44 transpose( const tintaMatrix44& mat );
	
		tintaMatrix44& translation( const tintaVector3f& xlate );

		
		tintaMatrix44 operator+( const tintaMatrix44& other ) const;
		tintaMatrix44& operator+=( const tintaMatrix44& other );
		tintaMatrix44 operator-( const tintaMatrix44& other ) const;
		tintaMatrix44& operator-=( const tintaMatrix44& other );
		tintaMatrix44 operator-() const;		
		tintaMatrix44& operator*=( const tintaMatrix44& matrix );
		tintaMatrix44 operator*( const tintaMatrix44& matrix ) const;		
		tintaVector4f operator*( const tintaVector4f& vector ) const;
		
		tintaMatrix44& operator*=( float scalar );
		friend tintaMatrix44 operator*( float scalar, const tintaMatrix44& matrix );
		tintaMatrix44 operator*( float scalar ) const;	
		tintaVector3f transform( const tintaVector3f& point ) const;		
		
		operator float*() { return mV; }
		operator const float*() const { return mV; }		
		float mV[16];
		const static tintaMatrix44 zero_val;
		const static tintaMatrix44 identity_val;
		const static tintaMatrix44 basis;
	};

	
	
	inline float& tintaMatrix44::operator()(m_uint32 i, m_uint32 j) {
		return mV[i + 4*j];
	}   

	inline float tintaMatrix44::operator()(m_uint32 i, m_uint32 j) const{
		return mV[i + 4*j];
	}

	inline const float* tintaMatrix44::operator[] (int row) const
	{
		return &mV[4*row];
	}

	inline float* tintaMatrix44::operator[] (int row)
	{
		return &mV[4*row];
	}

}


#endif
