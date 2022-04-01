/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_HERM_SPLINE_H_
#define _TINTA_HERM_SPLINE_H_ 

#include <Math/tintaVector3.h>

namespace Tinta
{
    /*    
        Cubic Hermite spline implemention
    */
	class _CoreExport tintaHermSpline
	{
	private:
		static const float m_points_factor;
	public:
		
		tintaHermSpline(void);

		virtual ~tintaHermSpline(void);		        	
		
		void clearPoints();

		// evaluate position
		tintaVector3f evaluate( float t );		
        		
		// return length of curve between t1 and t2
		float arcLength( float t1, float t2 );

		// get total length of curve
		inline float getLength() { return m_total_length; }

		// natural spline
		bool initializeNat( const tintaVector3f* positions,m_uint32 count, float &max_count );

		// cyclic. begin - end direction is same. Using in patches
		bool initializeCyclic( const tintaVector3f* positions, m_uint32 count, float &max_count );

		// render curve
		//void render();

	protected:
		// return length of curve between u1 and u2
		float segmentArcLength( unsigned i, float u1, float u2 );
		tintaVector3f* m_positions;     // sample positions
		tintaVector3f* m_in_tangents;    // incoming tangents on each segment
		tintaVector3f* m_out_tangents;   // outgoing tangents on each segment
		float*          m_times;         // time to arrive at each point
		float*          m_lengths;       // length of each curve segment
		float           m_total_length;   // total length of curve
		m_uint32    	m_count;         // number of points and times

	private:		
		tintaHermSpline( const tintaHermSpline& other );
		tintaHermSpline& operator=( const tintaHermSpline& other );
	};

}


#endif
