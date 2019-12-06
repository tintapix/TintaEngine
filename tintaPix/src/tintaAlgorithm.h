/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTAPIX_TINTA_ALGORITHM_H_
#define _TINTAPIX_TINTA_ALGORITHM_H_

#include "tintaCoord2.h"
#include "tintaPrimitives.h"
#include <Math/tintaCommonMath.h>
#include <tintaLogger.h>
#include <Math/tintaMatrix33.h>
#include <Math/tintaVector3.h>
#include <tintaArray2d.h>
#include "tintaRandGen.h"


namespace Tinta
{

	typedef struct PredAngle{

		bool operator()(const coord2dI_t &l_val, const coord2dI_t &r_val){
			
			m_float32 angle1 = uangleBetween( tintaVector2f( (m_float32)( -mcenter.mValX + l_val.mValX), (m_float32)(-mcenter.mValY + l_val.mValY) ) , mzero  );		
			m_float32 angle2 = uangleBetween( tintaVector2f( (m_float32)( -mcenter.mValX + r_val.mValX ), (m_float32)(-mcenter.mValY +  r_val.mValY) ) , mzero  );		
			return angle1 < angle2;
		}

		PredAngle( const coord2dI_t &center, const tintaVector2f &zeroVec )
			:mcenter( center ),
			 mzero( zeroVec ) {}
		coord2dI_t mcenter;	// zero point
		tintaVector2f mzero;	// angle with this vector(zero vector)
	} PredAngle_t;


	typedef struct PointsArr{
		PointsArr()
			:m_coordArr(0),
			m_quantity(0){}
			
		coord2dI_t * m_coordArr;
		m_uint32	     m_quantity;
	} pointsArr_t;

	typedef struct PointsRandom{
		PointsRandom()
			:m_quantity(0),
			 m_minVal(0),
			 m_maxVal(0)
		{}
		coord2dI_t   m_coordCenter;
		m_uint32	     m_quantity;
		int			 m_minVal;
		int			 m_maxVal;					
		bool		 m_unic;
	} pointsRandom_t;

	//template< typename T >
	 typedef struct PointsRandomSpread
		: public PointsRandom {

			PointsRandomSpread():mQuantFact(0) {}			

		const m_float32			*mInterpFact;
		m_uint32			     mQuantFact;		
	} PointsRandomSpread_t;

	

	template <typename T>
	struct DistPointData {
		T mSrc;
		m_float32 angle;
		m_float32 distance;
		T mRez;
		T mCenter;
	};

	template< typename T>
	struct IndexVal {
		IndexVal()
			:mIndex(0){
		}	
		IndexVal( m_uint32 index, T val)
			:mIndex( index ),
			 mVal  ( val ){
		}	

		m_uint32 mIndex;
		T mVal;
	};
	typedef IndexVal<m_float32> IndexValFloat_t;

	typedef struct 
		IndexValFloatVec {			
		std::vector<IndexValFloat_t> mDataVec;
	} IndexValFloatVec_t;


	//typedef std::vector<IndexValFloat_t> IndexValFloatVec_t;
	typedef DistPointData<tintaVector2f> PtDataVec2_t;
	typedef DistPointData<coord2dI_t> PtDataCoord_t;
	

	typedef 
	struct pred_coord_modify 
	{
		int mx_offset_val;
		int my_offset_val;
		pred_coord_modify( int mx_val, int my_val )
			:mx_offset_val(mx_val), my_offset_val(my_val)
		{
		}
		void operator()(coord2dI_t &point ) {
			point.mValX -= mx_offset_val;
			point.mValY -= my_offset_val;
		}
	} PredCoordDecrim_t;

	
	namespace tintaAlgorithm {

			
			/*
				arr_in in array with not sorted values
				arr_size_in
				arr_out - not allocated array, fills with unique points
				arr_size_out - new size of the out array, unique points in array
				points_found - not unique points found
				predic_ - finding criterion
			*/
			template<typename predic>
			FORCEINL void findPoint( const GeomPoint *arr_in, m_uint32 arr_size_in, GeomPoint * &arr_out, m_uint32 &arr_size_out, m_uint32 &points_found,  predic predic_) 
			{

				vecPoins_t vals_found;
				typedef vecPoins_t::iterator TIter_points;
				const GeomPoint* pEnd = arr_in + arr_size_in;
				const GeomPoint* find_val = arr_in - 1; // begin
				while( true )	{
					find_val = std::find_if( find_val + 1, arr_in + arr_size_in, predic_ );
					
					if( find_val >= pEnd )
						break;		
					points_found++;
					// unique test
					TIter_points it  = std::find( vals_found.begin(), vals_found.end(), *find_val );			
					if( it == vals_found.end() )
						vals_found.push_back( *find_val );
				}

				if( vals_found.size() ){
					arr_out = ALLOC_T( GeomPoint, vals_found.size() );
					arr_size_out = vals_found.size();
					std::copy( vals_found.begin() , vals_found.end(), arr_out );
				}
			}

			template<typename predic>
			FORCEINL void sortPoints(coord2dI_t *arr_in, m_uint32 arr_size_in, predic predic_){
				std::sort( arr_in , arr_in + arr_size_in, predic_ );
			}

			template<typename predic>
			FORCEINL void sortPoints(coord2dIVec_t &arr_in, predic predic_){
				std::sort( arr_in.begin(), arr_in.end(), predic_);
			}

			
			FORCEINL  void getExtrem(const coord2dI_t *arr_in, m_uint32 arr_size_in, int &xMin, int &yMin, int &xMax, int &yMax) {
				if( arr_size_in == 0 || !arr_in ){
					xMin = 0;
					yMin = 0;
					xMax = 0;
					yMax = 0;
					return;
				}
 				const coord2dI_t* find_xMin_val  = std::min_element (arr_in, arr_in + arr_size_in, Tinta::predXLess());
 				const coord2dI_t* find_yMin_val  = std::min_element (arr_in, arr_in + arr_size_in, Tinta::predYLess());
 				const coord2dI_t* find_xMax_val  = std::max_element (arr_in, arr_in + arr_size_in, Tinta::predXLess());
 				const coord2dI_t* find_yMax_val  = std::max_element (arr_in, arr_in + arr_size_in, Tinta::predYLess());

 				xMin = find_xMin_val->mValX;
 				yMin = find_yMin_val->mValY;
 				xMax = find_xMax_val->mValX;
 				yMax = find_yMax_val->mValY;

			}

			FORCEINL  void getExtrem(const coord2dI_t *arr_in, m_uint32 arr_size_in, TExtremValues2i &extreme) {

				getExtrem(arr_in, arr_size_in, extreme.mXMin, extreme.mYMin, extreme.mXMax, extreme.mYMax );
			}

			/// impl mev not effective
			/// calculates center coordinate of the points array
			FORCEINL  GeomPoint getCenter( const coord2dI_t *arr_in, m_uint32 arr_size_in ){
				int xMin=0;		
				int yMin=0;
				int xMax=0;
				int yMax=0;
				tintaAlgorithm::getExtrem( arr_in, arr_size_in, xMin, yMin, xMax, yMax );
				GeomPoint ret_val;

				if( ( xMax - xMin ) == 0  )		
					ret_val.mValX = TintaMath::max(xMax , xMin);
				else
				{
					ret_val.mValX = xMin + (int)ceil( (m_float32)((xMax - xMin) / 2) );
				}
				if( ( yMax - yMin ) == 0  )		
					ret_val.mValY = TintaMath::max(yMax , yMin);
				else
				{
					ret_val.mValY = yMin + (int)ceil( (m_float32)((yMax - yMin) / 2) );
				}


				return ret_val;

			}	

			// rotates points around center_point on angle in radians
			FORCEINL void rotatePoints( coord2dI_t * &arr_in, m_uint32 &arr_size_in, coord2dI_t center_point, m_float32 angle ) {	
				if(arr_size_in == 0 )
					return;
				/// changing coordinates relative to new coordinate system
				//for_each ( temp_points.begin(), temp_points.end(), pred_coord_modify(center_point.mValX, center_point.mValY) );
				std::for_each ( arr_in, arr_in + arr_size_in, PredCoordDecrim_t(center_point.mValX, center_point.mValY) );

				tintaMatrix33 rot_matrix;		
				rot_matrix.rotationZ(angle);

				for(m_uint32 iter = 0; iter < arr_size_in; iter++)
				{		
					Tinta::tintaVector3f point_new( (m_float32)arr_in[iter].mValX, (m_float32)arr_in[iter].mValY, 0 );			
					arr_in[iter].mValX = (int) ceil(rot_matrix.getColumn(0).mx * point_new.mx + rot_matrix.getColumn(0).my * point_new.my );
					arr_in[iter].mValY = (int) ceil(rot_matrix.getColumn(1).mx * point_new.mx + rot_matrix.getColumn(1).my * point_new.my );								
				}
				//for_each ( temp_points.begin(), temp_points.end(), pred_coord_modify(center_point.mValX, center_point.mValY) );
				/// returning coordinates to initial
				std::for_each ( arr_in, arr_in + arr_size_in, PredCoordDecrim_t( -center_point.mValX, -center_point.mValY ) );	

			}

			
			
			


			/// method gets new point from src point, after rotation relatively any point
			// angle in radians
			FORCEINL void  getRotPoint(const coord2dI_t &center, const coord2dI_t &src_point, m_float32 angle, coord2dI_t &rezulpoint_t )
			{
				//!!! rotation  CCW
				tintaMatrix33 rot_matrix;		
				rot_matrix.rotationZ(angle);		
				// new coordinates
				Tinta::tintaVector3f point_new(  (m_float32)(src_point.mValX - center.mValX), (m_float32)(src_point.mValY - center.mValY) , 0.f );			
				rezulpoint_t.mValX = (int) ceil(rot_matrix.getColumn(0).mx * point_new.mx + rot_matrix.getColumn(0).my * point_new.my );
				rezulpoint_t.mValY = (int) ceil(rot_matrix.getColumn(1).mx * point_new.mx + rot_matrix.getColumn(1).my * point_new.my );	

				// return to old coordinate system
				rezulpoint_t.mValX +=  center.mValX;
				rezulpoint_t.mValY +=  center.mValY;

			}

			/// method gets new point relatively any point in any direction and at any distance
			// angle in radians
			FORCEINL void  getDistPoint(PtDataCoord_t &data) //const coord2dI_t &src_point, m_float32 angle, m_float32 distance,  coord2dI_t &rezulpoint_t )
			{
				//!!!! rotation  CCW
				tintaMatrix33 rot_matrix;		
				rot_matrix.rotationZ(data.angle);			
				// get point with the same y coordinate as the src point has
				tintaVector3f point_y_aligned( TintaMath::floor(data.distance), 0.f , 0.f );			
				data.mRez.mValX = (int) ceil(rot_matrix.getColumn(0).mx * point_y_aligned.mx + rot_matrix.getColumn(0).my * point_y_aligned.my );
				data.mRez.mValY = (int) ceil(rot_matrix.getColumn(1).mx * point_y_aligned.mx + rot_matrix.getColumn(1).my * point_y_aligned.my );			

				data.mRez.mValX += data.mSrc.mValX;
				data.mRez.mValY += data.mSrc.mValY;
			}

			/*
				method gets new point relatively any point in any direction and at any distance
				angle in radians 
			*/
			FORCEINL void  getDistPoint( PtDataVec2_t &data )
			{
				//!!!! rotation  CCW
				
				tintaMatrix33 rot_matrix;		
				rot_matrix.rotationZ(data.angle);			
				// get point with the same y coordinate as the src point has
				tintaVector3f point_y_aligned( TintaMath::floor(data.distance), 0.f , 0.f );			
				data.mRez.mx =  ceil(rot_matrix.getColumn(0).mx * point_y_aligned.mx + rot_matrix.getColumn(0).my * point_y_aligned.my );
				data.mRez.my =  ceil(rot_matrix.getColumn(1).mx * point_y_aligned.mx + rot_matrix.getColumn(1).my * point_y_aligned.my );			

				data.mRez.mx += data.mSrc.mx;
				data.mRez.my += data.mSrc.my;
			}

			FORCEINL void movePoints(coord2dI_t *in_arr, m_uint32 in_arr_size, m_float32 in_angle, m_float32 in_distance)
			{
				coord2dI_t rezulpoint_t;
				m_uint32 count = 0;
				PtDataCoord_t coordData;

				for( count = 0; count < in_arr_size; count++){	
					coordData.mSrc = in_arr[count];
					coordData.angle = in_angle;
					coordData.distance = in_distance;
					
					getDistPoint( coordData );
					in_arr[count] = coordData.mRez;
				}
			}


		

// 		void FORCEINL findNearest( const coord2dIVec_t &srcBasis)
// 		{
// 
// 		}
		/* Finds nearest basis point or points(count parameter) */
		void FORCEINL findNearest( const coord2dIVec_t &srcBasis, CoordFactorVec_t &foundBasis, const coord2dI_t &from, m_uint32 count = 1 ){

			if( srcBasis.size() == 0 )
				return;

			count = count > srcBasis.size() ? srcBasis.size() : count;  

			coord2dIVec_t::const_iterator it  = srcBasis.begin( );
			
			if( count == 1 ){
				CoordFactor_t rez( from, MAX_FLOAT );
				
				for( ; it != srcBasis.end(); it++ ){
					m_float32 distVal = Tinta::distance( Tinta::tintaVector2f( (m_float32)from.mValX	, (m_float32)from.mValY)
													,  Tinta::tintaVector2f( (m_float32)it->mValX, (m_float32)it->mValY ) );

					if( rez.mValue > distVal ){
						rez.mValue = distVal;
						rez.mCoord = *it;
					}
				}
				foundBasis.push_back( rez );
			}
			else  {
				CoordFactorList_t sorted;
				sorted.resize( count, CoordFactor_t( from, MAX_FLOAT ));
				for( ; it != srcBasis.end(); it++ ){

					m_float32 distVal = Tinta::distance( Tinta::tintaVector2f( (m_float32)from.mValX	, (m_float32)from.mValY)
													,  Tinta::tintaVector2f( (m_float32)it->mValX, (m_float32)it->mValY ) );


					
					CoordFactorList_t::iterator itL = sorted.begin();					
					
					for( ; itL != sorted.end(); itL++ ){							
						if( itL->mValue >= distVal ){							
							sorted.insert( itL, 1, CoordFactor_t( *it, distVal ) );
							break;							
						}
					}		
					sorted.resize(count);					
				}
			
				foundBasis.resize( sorted.size() );

				std::copy( sorted.begin(), sorted.end(), foundBasis.begin() );
			}
		}


		/* Finds distant basis point or points(count parameter) */
		void FORCEINL findDistant( const coord2dIVec_t &srcBasis, CoordFactorVec_t &foundBasis, const coord2dI_t &from, m_uint32 count = 1 ){

			if( srcBasis.size() == 0 )
				return;

			count = count > srcBasis.size() ? srcBasis.size() : count;  

			coord2dIVec_t::const_iterator it  = srcBasis.begin( );

			if( count == 1 ){
				CoordFactor_t rez( from, 0.f );

				for( ; it != srcBasis.end(); it++ ){
					m_float32 distVal = Tinta::distance( 
						Tinta::tintaVector2f( (m_float32)from.mValX
						, (m_float32)from.mValY)
						,  Tinta::tintaVector2f( (m_float32)it->mValX, (m_float32)it->mValY 
						) );

					if( rez.mValue < distVal ){
						rez.mValue = distVal;
						rez.mCoord = *it;
					}
				}
				foundBasis.push_back( rez );
			}
			else  {
				CoordFactorList_t sorted;
				sorted.resize( count, CoordFactor_t( from, 0.f ));
				for( ; it != srcBasis.end(); it++ ){
					m_float32 distVal = Tinta::distance( 
						Tinta::tintaVector2f( (m_float32)from.mValX
						, (m_float32)from.mValY)
						,  Tinta::tintaVector2f( (m_float32)it->mValX, (m_float32)it->mValY 
						) );

					CoordFactorList_t::iterator itL = sorted.begin();					

					for( ; itL != sorted.end(); itL++ ){							
						if( itL->mValue < distVal ){							
							sorted.insert( itL, 1, CoordFactor_t( *it, distVal ) );
							break;							
						}
					}		
					sorted.resize(count);					
				}

				foundBasis.resize( sorted.size() );

				std::copy( sorted.begin(), sorted.end(), foundBasis.begin() );
			}
		}

		/* 
			Finds all basis points from another point on distance in sorted order			
		*/
		void FORCEINL findOnDist( const tintaArray2dByte_t &uPoints, CoordFactorVec_t &foundBasis, const coord2dI_t &center, m_float32 fdist, m_uint32 val  ){


			//int i = 0;
			int dist = (int)fdist; 
			int wBeg = center.mValX - dist;
			int hBeg = center.mValY - dist;
			int h = hBeg;
			int w = wBeg;

			assert( dist > 0 );

			if( dist <= 0 )
				return;	

			int wEnd = center.mValX + dist + 1;
			int hEnd =  center.mValY + dist + 1;
			//uPoints.setVal(arrPoints[i].mValX, arrPoints[i].mValY,0x05  );

			for( ; h < hEnd; h++ ){
				w = wBeg;
				//coord2dIVec_t temp;
				for( ; w < wEnd; w++ ){
					if( uPoints.getVal( w, h ) == val ){

						m_float32 distBase  = Tinta::distance( 
							Tinta::tintaVector2f( (m_float32)center.mValX
							, (m_float32)center.mValY)
							,  Tinta::tintaVector2f( (m_float32)w
							, (m_float32)h) );

						//if( distBase <= dist )
                        foundBasis.push_back(CoordFactor_t(coord2dI_t{ w, h }, distBase));
					}

				}				
			}

			sort( foundBasis.begin(), foundBasis.end() );

			int t = foundBasis.size() - 1;
			while( t >= 0 &&  floor( foundBasis[t--].mValue ) > fdist  )
				foundBasis.pop_back();

		}

		/*
			Finds first appropriate point in direction in worst case returns point from border of the map
		*/
		bool FORCEINL findInDir(  const tintaArray2dByte_t &mapPoints, coord2dI_t &rez , const coord2dI_t &from, m_float32 angleInRad, m_uint32 val ){
			
			m_float32 maxDist  =  TintaMath::sqrt ( (m_float32)TintaMath::pow( (m_float32)mapPoints.height(), 2.f ) 
					 + TintaMath::pow( (m_float32)mapPoints.width(),2.f ) );

			// finding distant point
			PtDataCoord_t coordData;
			coordData.mSrc = from;
			coordData.angle = angleInRad;
			coordData.distance = maxDist;
			
			Tinta::tintaAlgorithm::getDistPoint( coordData );	

            TintaPrimitives::GeomLine ray( from.mValX, from.mValY, coordData.mRez.mValX, coordData.mRez.mValY );
			
 			
 			rez = from;

			size_t k( 0 );
            TintaPrimitives::GeomPoint *points = ray.getPoints( k );
			for( size_t p = 0; p < k; p++ )	{
				//searching for first appropriate point
				if( !mapPoints.inBoundary( points[p].mValX, points[p].mValY ) )
					break; // no points found

				if( mapPoints.getValFast( points[p].mValX, points[p].mValY )  == val ){
						rez = points[p];
						return true;
				}			
			}			

			return false;
		}

		/*
			Finds last appropriate point in direction in worst case returns point from border of map
		*/
		bool FORCEINL findLastDir(  const tintaArray2dByte_t &mapPoints, coord2dI_t &rez , const coord2dI_t &from, m_float32 angleInRad, m_uint32 val ){
			
			m_float32 maxDist  =  TintaMath::sqrt ( TintaMath::pow( (m_float32)mapPoints.height(),2.f ) 
								     + TintaMath::pow( (m_float32)mapPoints.width(),2.f ) );

			// finding distant point
			PtDataCoord_t coordData;
			coordData.mSrc = from;
			coordData.angle = angleInRad;
			coordData.distance = maxDist;
			
			Tinta::tintaAlgorithm::getDistPoint( coordData );	

            TintaPrimitives::GeomLine ray( from.mValX, from.mValY, coordData.mRez.mValX, coordData.mRez.mValY );
			
			
			rez = from;
			size_t k( 0 );
			GeomPoint *points = ray.getPoints( k );	

			// start point can not be the right result
			if( k < 2 )
				return false;
			/*int test = 0;*/
			for( size_t p = 0; p < k; p++ )	{
				//searching for first appropriate point
				if( !mapPoints.inBoundary( points[p].mValX, points[p].mValY ) ){
					rez = points[p - 1];
					return true;
				}

				if( mapPoints.getValFast( points[p].mValX, points[p].mValY )  == val ){
						rez = points[p - 1];
						return true;
				}			
			}			

			return false;
		}


		/*
			Counts all  points to another point
		*/
		int FORCEINL countToPoint(  const tintaArray2dByte_t &mapPoints, const coord2dI_t &from, const coord2dI_t &distPoint, m_uint32 val, bool stop = false ){

            TintaPrimitives::GeomLine ray( from.mValX, from.mValY, distPoint.mValX, distPoint.mValY );


			//rez = from;
			int rez = 0;
			size_t k( 0 );
			GeomPoint *points = ray.getPoints( k );	

			// start point can not be the right result
			if( k < 2 )
				return rez;

			// discarding first point
			for( size_t p = 1; p < k; p++ ){			

				// boundary test
				if( !mapPoints.inBoundary( points[p].mValX, points[p].mValY ) ){					
					return rez;
				}

				m_uint32 v = mapPoints.getValFast( points[p].mValX, points[p].mValY );

				if( stop ){

					// breaking if inverted value has occurred
					if( v  != val ){												
						return rez;	
					}	
					rez++;
				}
				else {
					if( v  == val ){						
						rez++;
					}	
				}				
			}			

			return rez;

		}

		/*
			Counts all  points in direction
		*/
		int FORCEINL countInDir(  const tintaArray2dByte_t &mapPoints, const coord2dI_t &from, m_float32 angleInRad, m_uint32 val ){
			
			m_float32 maxDist  =  TintaMath::sqrt (  TintaMath::pow( (m_float32)mapPoints.height(),2.f ) 
									 +	TintaMath::pow( (m_float32)mapPoints.width(),2.f) );

			// finding distant point
			PtDataCoord_t coordData;
			coordData.mSrc = from;
			coordData.angle = angleInRad;
			coordData.distance = maxDist;			
			
			// getting point in direction and distance
			Tinta::tintaAlgorithm::getDistPoint( coordData );	
			
			return countToPoint( mapPoints, coordData.mSrc, coordData.mRez, val, true );


// 			GeomLine ray( from.mValX, from.mValY, coordData.mRez.mValX, coordData.mRez.mValY );
// 			
// 			
// 			//rez = from;
// 			int rez = 0;
// 			int k( 0 );
// 			GeomPoint *points = ray.getPoints( k );	
// 
// 			// start point can not be the right result
// 			if( k < 2 )
// 				return rez;
// 			
// 			// discarding first point
// 			for( int p = 1; p < k; p++ ){			
// 
// 				// boundary test
// 				if( !mapPoints.inBoundary( points[p].mValX, points[p].mValY ) ){					
// 					return rez;
// 				}
// 
// 				if( mapPoints.getValFast( points[p].mValX, points[p].mValY )  != val ){
// 						//rez = points[p - 1];
// 					return rez;	
// 				}		
// 				rez++;
// 			}			
// 
// 			return rez;
		}


		/* 
			Finds all adjoint points from another point on distance in sorted order			
		*/
// 		void FORCEINL findAdjoinDist( const tintaArray2dByte_t &uPoints, CoordFactorVec_t &foundBasis, const coord2dI_t &center, m_float32 fdist, m_uint32 maskBasis  ){
// 
// 
// 			//int i = 0;
// 			int dist = ( int )fdist; 
// 			int wBeg = center.mValX - dist;
// 			int hBeg = center.mValY - dist;
// 			int h = hBeg;
// 			int w = wBeg;
// 
// 			assert( dist > 0 );
// 
// 			if( dist <= 0 )
// 				return;	
// 
// 			int wEnd = center.mValX + dist + 1;
// 			int hEnd =  center.mValY + dist + 1;
// 			//uPoints.setVal(arrPoints[i].mValX, arrPoints[i].mValY,0x05  );
// 
// 			for( ; h < hEnd; h++ ){
// 				w = wBeg;
// 				//coord2dIVec_t temp;
// 				for( ; w < wEnd; w++ ){
// 					if( uPoints.getVal( w, h ) == maskBasis ){
// 
// 						//m_float32 distBase  = Tinta::distance( Tinta::tintaVector2f( (m_float32)center.mValX, (m_float32)center.mValY),  Tinta::tintaVector2f( (m_float32)w, (m_float32)h) );						
// 						//foundBasis.push_back( CoordFactor_t ( coord2dI_t( w, h ) , distBase ) );					
// 					}
// 				}				
// 			}
// 			
//  			sort( foundBasis.begin(), foundBasis.end() );
//  
//  			size_t t = foundBasis.size() - 1;
//  			while( t >= 0 &&  foundBasis[t--].mValue >= dist  )
//  				foundBasis.pop_back();
// 
// 		}


	}
}

#endif
