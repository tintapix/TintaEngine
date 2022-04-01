
/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef __TINTA_MATRIX_33__H__
#define __TINTA_MATRIX_33__H__

#include "tintaPredefine.h"
#include "tintaVector2.h"
#include "tintaVector3.h"
namespace Tinta
{

	class tintaQuatern;
	

	//	c1  c2  c3
	// 	[0] [1] [2] // [0][1]...[0][n]
	// 	[3] [4] [5] // [1][1]...[1][n]
	// 	[6] [7] [8] // [2][1]...[2][n]
    
	class _CoreExport tintaMatrix33
	{
	public:
		
		FORCEINL tintaMatrix33() {}
		FORCEINL ~tintaMatrix33() {}
		tintaMatrix33 (float fM00, float fM01, float fM02,
					  float fM10, float fM11, float fM12,
					  float fM20, float fM21, float fM22 );
		
		tintaMatrix33( const tintaVector3f& axis, float angle );
		explicit tintaMatrix33( const tintaQuatern& quat );

		
		tintaMatrix33(const tintaMatrix33& other);
		tintaMatrix33& operator=(const tintaMatrix33& other);        	

		
		FORCEINL float& operator()(m_uint32 i, m_uint32 j);
		FORCEINL float operator()(m_uint32 i, m_uint32 j) const;

		
		bool operator==( const tintaMatrix33& other ) const;
		bool operator!=( const tintaMatrix33& other ) const;
		bool isZeroVal() const;
		bool isIdentity() const;

		
		void setRows( const tintaVector3f& row1, const tintaVector3f& row2, const tintaVector3f& row3 );
		void getRows( tintaVector3f& row1, tintaVector3f& row2, tintaVector3f& row3 ) const;
		tintaVector3f get_row( m_uint32 i ) const;

		void setColumns( const tintaVector3f& col1, const tintaVector3f& col2, const tintaVector3f& col3 );
		void getColumns( tintaVector3f& col1, tintaVector3f& col2, tintaVector3f& col3 ) const;
		tintaVector3f getColumn( m_uint32 i ) const;

		void clean();
		void identity();

		tintaMatrix33&       inverse();
		tintaMatrix33        inverse() const;
		friend tintaMatrix33 inverse( const tintaMatrix33& mat );

		
		tintaMatrix33 transpose() const;
		tintaMatrix33 transpose( const tintaMatrix33& mat );
		
		tintaMatrix33 adjoint() const;
		float determinant() const;
		float trace() const;
		
		tintaMatrix33& rotation( const tintaQuatern& rotate );
		tintaMatrix33& rotation( float z, float y, float x );
		tintaMatrix33& rotation( const tintaVector3f& axis, float angle );

		tintaMatrix33& scaling( const tintaVector3f& scale );

		tintaMatrix33& rotationX( float angle );
		tintaMatrix33& rotationY( float angle );
		tintaMatrix33& rotationZ( float angle );

		
		void getAxisAngle( tintaVector3f& axis, float& angle );
		
		
		tintaMatrix33 operator+( const tintaMatrix33& other ) const;
		tintaMatrix33& operator+=( const tintaMatrix33& other );
		tintaMatrix33 operator-( const tintaMatrix33& other ) const;
		tintaMatrix33& operator-=( const tintaMatrix33& other );
		tintaMatrix33 operator-() const;	
		tintaMatrix33& operator*=( const tintaMatrix33& matrix );
		tintaMatrix33 operator*( const tintaMatrix33& matrix ) const;


		tintaVector3f operator*( const tintaVector3f& vector ) const;

        tintaVector2f operator*(const tintaVector2f& vector) const;

		friend tintaVector3f operator*( const tintaVector3f& vector, const tintaMatrix33& matrix );

		FORCEINL const float* operator[] (int row) const;
		FORCEINL float* operator[] (int row);

		tintaMatrix33 operator/ (float fScalar) const;

		tintaMatrix33& operator*=( float scalar );
		friend tintaMatrix33 operator*( float scalar, const tintaMatrix33& matrix );
		tintaMatrix33 operator*( float scalar ) const;

        // M*D
		tintaMatrix33 TimesDiagonal (const tintaVector3f& rkDiag) const;  
        // D*M
		tintaMatrix33 DiagonalTimes (const tintaVector3f& rkDiag) const;  

		
		operator float*() { return mV; }
		operator const float*() const { return mV; }
		
		float mV[9];

	};

	FORCEINL float &tintaMatrix33::operator()( m_uint32 i, m_uint32 j )
	{
		return mV[i + 3*j];
	}

	FORCEINL float tintaMatrix33::operator()( m_uint32 i, m_uint32 j ) const
	{
		return mV[i + 3*j];
	}

	FORCEINL const float* tintaMatrix33::operator[] (int row) const
	{
		return &mV[3*row];
	}

	FORCEINL float* tintaMatrix33::operator[] (int row)
	{
		return &mV[3*row];
	}

}
#endif
