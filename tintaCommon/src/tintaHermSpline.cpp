/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */


#include "tintaHermSpline.h"
#include "Math/tintaCommonMath.h"
#include "tintaStdHeaders.h"

namespace Tinta
{

    /*
    Inverts matrix n by n
    will store results in A
    */
    bool InvertMatrix(float* A, m_uint32 n){


        UNIQPTRALLOC(m_uint32, swap, ALLOC_T(m_uint32, n));

        // do one pass for each diagonal element
        for (m_uint32 pivot = 0; pivot < n; ++pivot) {

            m_uint32 row = 0, col = 0;

            // find the largest magnitude element in the current column
            m_uint32 maxrow = pivot;
            float maxelem = TintaMath::abs(A[maxrow + n * pivot]);
            for (row = pivot + 1; row < n; ++row)
            {
                float elem = TintaMath::abs(A[row + n*pivot]);
                if (elem > maxelem)
                {
                    maxelem = elem;
                    maxrow = row;
                }
            }

            if (TintaMath::isZero(maxelem))  {
                return false;
            }

            // if not in the current row, swap rows
            swap.get()[pivot] = maxrow;
            if (maxrow != pivot){
                // swap the row
                for (col = 0; col < n; ++col){
                    float temp = A[maxrow + n*col];
                    A[maxrow + n*col] = A[pivot + n*col];
                    A[pivot + n*col] = temp;
                }
            }

            // multiply current row by 1/pivot to "set" pivot to 1
            float pivotRecip = 1.0f / A[n*pivot + pivot];
            for (col = 0; col < n; ++col){
                A[pivot + n*col] *= pivotRecip;
            }

            // copy 1/pivot to pivot point (doing inverse in place)
            A[pivot + n*pivot] = pivotRecip;

            // now zero out pivot column in other rows 
            for (row = 0; row < n; ++row){
                // don't subtract from pivot row
                if (row == pivot)
                    continue;

                // subtract multiple of pivot row from current row,
                // such that pivot column element becomes 0
                float factor = A[row + n*pivot];

                // clear pivot column element (doing inverse in place)
                // will end up setting this element to -factor*pivotInverse
                A[row + n*pivot] = 0.0f;

                // subtract multiple of row
                for (col = 0; col < n; ++col)
                {
                    A[row + n*col] -= factor*A[pivot + n*col];
                }
            }
        }
        m_uint32 p = n;
        do
        {
            --p;
            if (swap.get()[p] != p){
                // swap the corresponding column
                for (m_uint32 row = 0; row < n; ++row)
                {
                    float temp = A[row + n*swap.get()[p]];
                    A[row + n*swap.get()[p]] = A[row + n*p];
                    A[row + n*p] = temp;
                }
            }
        } while (p > 0);


        return true;
    }

const float tintaHermSpline::m_points_factor = 0.42244f;

tintaHermSpline::tintaHermSpline(void):
m_positions( 0 ),
m_in_tangents( 0 ),
m_out_tangents( 0 ),
m_times( 0 ),
m_lengths( 0 ),
m_total_length( 0.0f ),
m_count( 0 )
{
}

tintaHermSpline::~tintaHermSpline(void){
	clearPoints();
}

bool tintaHermSpline::initializeNat( const tintaVector3f* positions, m_uint32 count, float &max_count  ) {
		
    if ( m_count != 0 || count < 3 || !positions )
		return false;

	
	m_uint32 n = count;	
    UNIQPTRALLOC(float, A, ALLOC_T(float, n*n));

	memset( A.get(), 0, sizeof( float)*n*n );

    A.get()[0] = 2.0f;
    A.get()[n] = 1.0f;
	m_uint32 i;
	for ( i = 1; i < n-1; ++i )
	{
        A.get()[i + n*i - n] = 1.0f;
        A.get()[i + n*i] = 4.0f;
        A.get()[i + n*i + n] = 1.0f;
	}
    A.get()[n*(n - 1) - 1] = 1.0f;
    A.get()[n*n - 1] = 2.0f;

	// invert it
	// we'd might get better accuracy if we solve the linear system 3 times,
	// once each for x, y, and z, but this is more efficient
	if ( !InvertMatrix( A.get(), n ) ) {		
		return false;
	}

	// set up arrays
	m_in_tangents  = NEW_ARRAY_T(tintaVector3f,count);	
	m_out_tangents = NEW_ARRAY_T(tintaVector3f,count);
	m_positions    = NEW_ARRAY_T(tintaVector3f,count); 

	m_times = ALLOC_T( float, count );
	m_count = count;

	// set up the tangents
	for ( i = 0; i < count; ++i )
	{
		// copy position and time
		m_positions[i] = positions[i];
		//mTimes[i] = times[i];

		// multiply b by inverse of A to get tangents
		// compute count-1 incoming tangents
		if ( i < count-1 ) {
            m_in_tangents[i] = 3.0f * A.get()[i] * (positions[1] - positions[0]) + 3.0f * A.get()[i + n * n - n] * (positions[n - 1] - positions[n - 2]);
			for ( unsigned j = 1; j < n-1; ++j ) {

				tintaVector3f b_j = 3.0f * ( positions[j+1] - positions[j-1] );
                m_in_tangents[i] += A.get()[i + n * j] * b_j;
			}
			// out tangent is in tangent of next segment
			if (i > 0)
				m_out_tangents[i-1] = m_in_tangents[i];
		}
		// compute final outgoing tangent
		else {
            m_out_tangents[i - 1] = 3.0f * A.get()[i] * (positions[1] - positions[0]) + 3.0f * A.get()[i + n * n - n] * (positions[n - 1] - positions[n - 2]);

			for ( unsigned j = 1; j < n-1; ++j ) {

				tintaVector3f b_j = 3.0f * ( positions[j+1] - positions[j-1] );
                m_out_tangents[i - 1] += A.get()[i + n * j] * b_j;
			}
		}
	}

	// set up curve segment lengths
	//mLengths = new float[count-1];
	m_lengths = ALLOC_T(float, count-1);

	m_total_length = 0.0f;
	for ( i = 0; i < count-1; ++i )
	{
		m_lengths[i] = segmentArcLength( i, 0.0f, 1.0f );
		m_total_length += m_lengths[i];
	}

	m_times[0] = 0.f;
	for ( i = 1; i < count; ++i ){
		m_times[i] = m_times[i-1] + m_points_factor * m_lengths[ i - 1 ];
	}
	max_count = *std::max_element(m_times, m_times + count);	

	//FREE_T(A);

	return true;
}
bool tintaHermSpline::initializeCyclic( const tintaVector3f* positions, m_uint32 count, float &max_count ){
    	
    if (m_count != 0 || count < 4 || !positions)
		return false;
    	
	m_uint32 n = count-1;

    UNIQPTRALLOC(float, A, ALLOC_T(float, n*n));

	memset(A.get(), 0, sizeof(float)*n*n);

    A.get()[0] = 4.0f;
    A.get()[n] = 1.0f;
    A.get()[n*n - n] = 1.0f;
	m_uint32 i;
	for ( i = 1; i < n-1; ++i )
	{
        A.get()[i + n*i - n] = 1.0f;
        A.get()[i + n*i] = 4.0f;
        A.get()[i + n*i + n] = 1.0f;
	}
    A.get()[n*(n - 2) - 1] = 1.0f;
    A.get()[n*(n - 1) - 1] = 1.0f;
    A.get()[n*n - 1] = 4.0f;

	// invert it
	// we'd might get better accuracy if we solve the linear system 3 times,
	// once each for x, y, and z, but this is more efficient
    if (!InvertMatrix(A.get(), n))
	{
		//FREE_T(A);
		return false;
	}

	// set up arrays
	m_in_tangents  = NEW_ARRAY_T( tintaVector3f, count );	
	m_out_tangents = NEW_ARRAY_T( tintaVector3f, count );
	m_positions    = NEW_ARRAY_T( tintaVector3f, count ); 
    	
	m_times = ALLOC_T( float, count );
	m_count = count;

	tintaVector3f b0 = 3.0f*(positions[1]-positions[0] + positions[count-1]-positions[count-2]);

	// set up the tangents
	for ( i = 0; i < count; ++i ){

		// copy position and time
		m_positions[i] = positions[i];

		// multiply b by inverse of A to get tangents
		// compute count-1 incoming tangents
		if ( i < count-1 )
		{
            m_in_tangents[i] = A.get()[i] * b0;
			for ( unsigned j = 1; j < n; ++j )
			{
				tintaVector3f b_j = 3.0f*(positions[j+1]-positions[j-1]);
                m_in_tangents[i] += A.get()[i + n*j] * b_j;
			}
			// outgoing tangent is incoming tangent of next segment
			if (i > 0)
				m_out_tangents[i-1] = m_in_tangents[i];
		}
	}
	m_out_tangents[count-2] = m_in_tangents[0];

	// set up curve segment lengths
	m_lengths = ALLOC_T(float, count-1); //new float[count-1];
	m_total_length = 0.0f;
	for ( i = 0; i < count-1; ++i ) {
		m_lengths[i] = segmentArcLength(i, 0.0f, 1.0f);
		m_total_length += m_lengths[i];
	}

	m_times[0] = 0.f;
	for ( i = 1; i < count; ++i ){
		m_times[i] = m_times[i-1] + m_points_factor * m_lengths[ i - 1 ];
	}
	max_count = *std::max_element(m_times, m_times + count);

	return true;
}
void tintaHermSpline::clearPoints(){
    //delete [] m_positions;
	if(m_count > 0 ) {	
		
		DELETE_ARRAY_T(m_positions, tintaVector3f, m_count);		
		DELETE_ARRAY_T(m_in_tangents, tintaVector3f, m_count);
		DELETE_ARRAY_T(m_out_tangents, tintaVector3f, m_count);		
		FREE_T(m_times);
		FREE_T(m_lengths);
	}
    m_total_length = 0.0f;
    m_count = 0;

}  

float tintaHermSpline::segmentArcLength( unsigned i, float u1, float u2 ){

    static const float x[] =  { 0.0000000000f, 0.5384693101f, -0.5384693101f, 0.9061798459f, -0.9061798459f  };

    static const float c[] = {  0.5688888889f, 0.4786286705f, 0.4786286705f, 0.2369268850f, 0.2369268850f    };

    

    if ( u2 <= u1 )
        return 0.0f;

    if ( u1 < 0.0f )
        u1 = 0.0f;

    if ( u2 > 1.0f )
        u2 = 1.0f;

    // Gaussian quadrature
    float sum = 0.0f;
    // set up for computation of Hermite derivative
    tintaVector3f A =  2.0f * m_positions[i] - 2.0f * m_positions[i+1] + m_in_tangents[i]		+ m_out_tangents[i];
    tintaVector3f B = -3.0f * m_positions[i] + 3.0f * m_positions[i+1] - 2.0f * m_in_tangents[i] - m_out_tangents[i];
    tintaVector3f C = m_in_tangents[i];
    
    for ( unsigned j = 0; j < 5; ++j )
    {
        float u = 0.5f *( (u2 - u1) * x[j] + u2 + u1 );
        tintaVector3f derivative = C + u*(2.0f * B + 3.0f * u * A);
        sum += c[j] * derivative.length();
    }
    sum *= 0.5f * (u2-u1);

    return sum;

}  


tintaVector3f tintaHermSpline::evaluate( float t ){
    if ( m_count < 2 )
        return tintaVector3f::origin;

    // handle boundary conditions
    if ( t <= m_times[0] )
        return m_positions[0]; 

    else if ( t >= m_times[m_count-1] )
        return m_positions[m_count-1];

    // find segment and parameter
    m_uint32 i;
    for ( i = 0; i < m_count-1; ++i ){
        if ( t < m_times[i+1] )
            break;       
    }
    float t0 = m_times[i];
    float t1 = m_times[i+1];
    float u = (t - t0)/(t1 - t0);

    // evaluate
    tintaVector3f A =  2.0f * m_positions[i] - 2.0f * m_positions[i+1] + m_in_tangents[i]      + m_out_tangents[i];
    tintaVector3f B = -3.0f * m_positions[i] + 3.0f * m_positions[i+1] - 2.0f * m_in_tangents[i] - m_out_tangents[i];
    
    return m_positions[i] + u*(m_in_tangents[i] + u*(B + u*A));

}   


float tintaHermSpline::arcLength( float t1, float t2 )
{
    if ( t2 <= t1 )
        return 0.0f;

    if ( t1 < m_times[0] )
        t1 = m_times[0];

    if ( t2 > m_times[m_count-1] )
        t2 = m_times[m_count-1];

    // find segment and parameter
    m_uint32 seg1;
    for ( seg1 = 0; seg1 < m_count-1; ++seg1 )
    {
        if ( t1 < m_times[seg1+1] )
        {
            break;
        }
    }
    float u1 = (t1 - m_times[seg1])/(m_times[seg1+1] - m_times[seg1]);
    
    // find segment and parameter
    m_uint32 seg2;
    for ( seg2 = 0; seg2 < m_count-1; ++seg2 )
    {
        if ( t2 <= m_times[seg2+1] )
        {
            break;
        }
    }
    float u2 = (t2 - m_times[seg2])/(m_times[seg2+1] - m_times[seg2]);
    
    float result;
    // both parameters lie in one segment
    if ( seg1 == seg2 )
    {
        result = segmentArcLength( seg1, u1, u2 );
    }
    // parameters cross segments
    else
    {
        result = segmentArcLength( seg1, u1, 1.0f );
        for ( unsigned i = seg1+1; i < seg2; ++i )
            result += m_lengths[i];
        result += segmentArcLength( seg2, 0.0f, u2 );
    }

    return result;

}   
}
