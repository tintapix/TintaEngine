/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTAPIX_TINTA_PRIMITIVES_H_
#define _TINTAPIX_TINTA_PRIMITIVES_H_

#include "tintaCoord2.h"
#include <tintaPredefine.h>
#include <Math/tintaCommonMath.h>
#include <algorithm>
#include <Math/tintaVector2.h>


namespace TintaPrimitives
{
    using namespace Tinta;

	struct  GeomLine
	{
		GeomLine(int iXBegin, int iYBegin, int iXEnd, int iYEnd,  int inStep = 0)
			:mValXBegin(iXBegin)
			,mValYBegin(iYBegin)
			,mValXEnd(iXEnd)
			,mValYEnd(iYEnd)
			,m_iLength(0)			
			,mStep(inStep)
		{}
		GeomLine()
		:mValXBegin(0)
		,mValYBegin(0)
		,mValXEnd(0)
		,mValYEnd(0)
		,m_iLength(0)		
		,mStep(0)
		{}

		int mValXBegin;
		int mValYBegin;
		int mValXEnd;
		int mValYEnd;
		int m_iLength;
		
		// distance between two points 
		int mStep;
		vecPoins_t vecPoins;
		
	private:
				
		void calcPoints( int xBeg,int yBeg, int xEnd,int yEnd )
		{
			int length = (int)distance(tintaVector2f((float)xBeg, (float)yBeg), tintaVector2f((float)xEnd, (float)yEnd));

			// step too large
			if( mStep > length )
				mStep = length;

			int dx = xEnd - xBeg, dy = yEnd - yBeg, steps, k;
			
			float xIncr, yIncr, x = (float)xBeg, y = (float)yBeg;

// 			if( TintaMath::abs( dx ) > TintaMath::abs( dy ) )
// 				steps = TintaMath::abs( dx );
// 			else
// 				steps = TintaMath::abs( dy );	

			steps = TintaMath::max(TintaMath::abs( dx ), TintaMath::abs( dy ) );
			
			xIncr = dx/(float)steps;
			yIncr = dy/(float)steps;		
			
			vecPoins.push_back( GeomPoint( (int)x,(int)y ) );
			
			if( mStep > 0 )
			{
				int step = mStep;
				while( step < steps + mStep )
				{
					x += xIncr*mStep;
					y += yIncr*mStep;
					vecPoins.push_back(GeomPoint((int)x,(int)y));

					step += mStep;
				}
				
			}
			else
			{			
				for(k = 0; k < steps; k++)
				{
					x += xIncr;
					y += yIncr;
					vecPoins.push_back(GeomPoint((int)x,(int)y));
				}
			}
		}
	public:
		void setStep(int inStep)
		{
			mStep = inStep;
		}
		
		/*
			Lines are colinear or parallel	
		*/
// 		bool areNotSkew( const GeomLine &line  ){
// 			return ( ( ( mValXBegin == mValXEnd ) && ( line.mValXBegin == line.mValXEnd ) ) 
// 				|| ( (   mValYBegin == mValYEnd ) && ( line.mValYBegin == line.mValYEnd ) ));
// 		}
// 
// 		tintaVector2f intersec( const GeomLine &rVal ){
// 
// 			tintaVector2f vBeg1( (float)mValXBegin, (float)mValYBegin );
// 			tintaVector2f vEnd1( (float)mValXEnd, (float)mValYEnd );
// 
// 			tintaVector2f vBeg2( (float)rVal.mValXBegin, (float)rVal.mValYBegin );
// 			tintaVector2f vEnd2( (float)rVal.mValXEnd, (float)rVal.mValYEnd );
// 			tintaVector2f n( ( vEnd2 - vBeg2 ).m_x, ( vBeg2 - vEnd2 ).m_y );
// 			float denom = n.dot(vEnd1 - vBeg1);
// 			float num = n.dot(vBeg1 - vBeg2);
// 			
// 			float s = -num/denom;
// 
// 			return ( tintaVector2f( vBeg1 + s * (vEnd1 - vBeg1)  ) );			
// 		}

		

		GeomPoint *getPoints(size_t &size)
		{	
			calcPoints(mValXBegin, mValYBegin, mValXEnd, mValYEnd );
			size = vecPoins.size();
			return &vecPoins[0];			
		}

		void clearPoints()
		{
			vecPoins.clear();
		}

	};


	struct  GeomCircle
	{
	public:		
		GeomCircle(int iX, int iY, int radius)
			: m_iCenterX(iX)
			,m_iCenterY(iY)
			,m_iRadius(radius)			
		{}
		GeomCircle(): m_iCenterX(0),m_iCenterY(0), m_iRadius(0){}		
		
	protected:
		int m_iCenterX;
		int m_iCenterY;
		int m_iRadius;		
		//bool m_filled;
		vecPoins_t vecPoins;
		void createCircle(int xCent, int yCent, int r) {

			vecPoins.clear();

			int x = 0;
			int y = r;
			int p = 1 - r;

			setCirclePoints( xCent, yCent, x, y );
			
			while(x < y)
			{
				x++;
				if( p < 0 )
					p += 2 * x + 1;
				else
				{
					y--;
					p += 2 * (x - y ) + 1;
				}
				setCirclePoints(xCent, yCent, x,y);				
			}
		}
	private:
		void setCirclePoints( int xCent, int yCent, int x, int y ){
			vecPoins.push_back(GeomPoint(xCent + x,yCent + y));
			vecPoins.push_back(GeomPoint(xCent - x,yCent + y));
			vecPoins.push_back(GeomPoint(xCent + x,yCent - y));
			vecPoins.push_back(GeomPoint(xCent - x,yCent - y));

			vecPoins.push_back(GeomPoint(xCent + y,yCent + x));
			vecPoins.push_back(GeomPoint(xCent - y,yCent + x));
			vecPoins.push_back(GeomPoint(xCent + y,yCent - x));
			vecPoins.push_back(GeomPoint(xCent - y,yCent - x));
		}

	public:

		void setCenter( int x_val, int y_val ) {
			m_iCenterX = x_val;
			m_iCenterY = y_val;
		}
		void setRadius( int radius ) {
			m_iRadius = radius;			
		}

// 		void set_fill_sign( bool fill_sign ) {
// 			m_filled = fill_sign;			
// 		}


		GeomPoint *getCirclePoints(int &size) {
 			createCircle(m_iCenterX, m_iCenterY, m_iRadius);		

			size = vecPoins.size();			
			return &vecPoins[0];
		}
	};


	struct  GeomEllipse
	{
		GeomEllipse(int xCen,int  yCen, int radX, int radY, int thickness = 1, bool filled = false)
		:	m_xCen(xCen),
			m_yCen(yCen), 
			m_radX(radX),
			m_radY(radY),
			m_thickness(thickness),
			m_filled(filled)			
		{}

		GeomEllipse():m_xCen(0),m_yCen(0), m_radX(0),m_radY(0),m_thickness(1), m_filled(false)
		{}		

		int m_xCen;
		int m_yCen;
		int m_radX;
		int m_radY;
		int m_thickness;
		bool m_filled;

	private:
		vecPoins_t vecPoins;
#define ROUND(a)((int)(a + 0.5))
		void calcEllipse()	{
			int Rx2 = m_radX * m_radX;
			int Ry2 =  m_radY * m_radY;
			int twoRx2 = 2 * Rx2;
			int twoRy2 = 2 * Ry2;
			int p;
			int x = 0;
			int y = m_radY;
			int px = 0;
			int py = twoRx2 * y;
			setPoints(m_xCen, m_yCen, x, y);
			p = (int)TintaMath::floor(Ry2 - (Rx2 * m_radY) + (0.25f * Rx2  ));
			//p = ROUND(Ry2 - (Rx2 * m_radY) + (0.25 * Rx2  ));
			while(px < py)
			{
				x++;
				px += twoRy2;
				if(p < 0)
					p += Ry2 + px;
				else
				{
					y--;
					py -= twoRx2;
					p += Ry2 + px - py; 
				}
				setPoints(m_xCen, m_yCen,x,y);
			}
			
			
			//second region
			//p = ROUND(Ry2 * (x + 0.5) *( x + 0.5 ) + Rx2 *(y - 1) * (y - 1) - Rx2 * Ry2);
			p =  (int)TintaMath::floor(Ry2 * (x + 0.5f) *( x + 0.5f ) + Rx2 *(y - 1) * (y - 1) - Rx2 * Ry2);
			while(y > 0)
			{
				 y--;
				 py -= twoRx2;
				 if(p > 0)
					 p += Rx2  - py;
				 else
				 {
					 x++;
					 px += twoRy2;
					 p += Rx2 - py  + px;
				 }
				setPoints(m_xCen, m_yCen,x,y);
			}

		}
		void setPoints(int xCent, int yCent, int x, int y)
		{
			vecPoins.push_back(GeomPoint(xCent + x, yCent + y));
			vecPoins.push_back(GeomPoint(xCent - x, yCent + y));
			vecPoins.push_back(GeomPoint(xCent + x, yCent - y));
			vecPoins.push_back(GeomPoint(xCent - x, yCent - y));
		}
	public:
		GeomPoint *getPoints(size_t &size) {		
			   calcEllipse();
			  
// 			   if( m_filled && vecPoins.size() )  {
// 
// 				   GeomPoint * arr_out = 0;
// 				   unsigned size_arr_out = 0;
// 				   fillBoundary_ex(&vecPoins[0], vecPoins.size(), arr_out, size_arr_out);
// 
// 				   size = size_arr_out;
// 
// 				   if(size_arr_out == 0){
// 					   return 0;
// 				   }
// 
// 				   vecPoins.resize(size_arr_out);
// 				   //vecPoins.
// 				   copy( arr_out, arr_out + size_arr_out, vecPoins.begin() );
// 				   FREE_T(arr_out);
// 
// 
// 			   }
			    size = vecPoins.size();
			   return &vecPoins[0];
		  }



	};


	struct GeomRectangle {
		GeomRectangle():
			m_i_xcoord(0),
			m_i_ycoord(0),
			m_i_height(0),
			m_i_width(0),
			m_bfilled(false)
		{}
		GeomRectangle(int i_xcoord , int  i_ycoord, int i_width, int i_height,  bool bfilled = false):
			m_i_xcoord(i_xcoord),
			m_i_ycoord(i_ycoord),
			m_i_height(i_height),
			m_i_width(i_width),
			m_bfilled(bfilled)
		{}

	private:
		int m_i_xcoord;
		int m_i_ycoord;
		int m_i_height;
		int m_i_width;
		bool m_bfilled;		

		vecPoins_t vecPoins;
		// saving contour
		//std::vector<GeomPoint> vecPoins_outline;
	public:

		GeomPoint *getPoints(size_t &size)	{
			vecPoins.clear();
			GeomLine line_upp(m_i_xcoord, m_i_ycoord, m_i_xcoord + m_i_width,m_i_ycoord);
			GeomLine line_right(m_i_xcoord + m_i_width, m_i_ycoord, m_i_xcoord + m_i_width, m_i_ycoord + m_i_height);
			GeomLine line_down(m_i_xcoord + m_i_width, m_i_ycoord + m_i_height, m_i_xcoord, m_i_ycoord + m_i_height);
			GeomLine line_left(m_i_xcoord, m_i_ycoord + m_i_height, m_i_xcoord, m_i_ycoord);
			size_t size_line = 0;
			GeomPoint *points = line_upp.getPoints(size_line);

			for(size_t i = 0; i < size_line; i++)
			{
				vecPoins.push_back(points[i]);
			}

			size_line = 0;
			points = line_right.getPoints(size_line);

			for(size_t i = 0; i < size_line; i++)
			{
				vecPoins.push_back(points[i]);
			}

			size_line = 0;
			points = line_down.getPoints(size_line);

			for(size_t i = 0; i < size_line; i++)
			{
				vecPoins.push_back(points[i]);
			}

			size_line = 0;
			points = line_left.getPoints(size_line);

			for(size_t i = 0; i < size_line; i++)
			{
				vecPoins.push_back(points[i]);
			}

			//std::fill(vecPoins.begin(),  vecPoins.end(), vecPoins_outline.begin());
// 			if(m_bfilled &&  vecPoins.size() )
// 			{
// 
// //   				Tinta::ptPairVec_t points_pair_vec;
// //   				Tinta::fill_points_pairs(&vecPoins[0], vecPoins.size(), points_pair_vec);
// //   
// //   				typedef Tinta::ptPairVec_t::const_iterator ptPairVec_t_iter;
// //   
// //   				for(ptPairVec_t_iter it = points_pair_vec.begin(); it != points_pair_vec.end(); it++)
// //   				{
// //   					for(int i  = it->m_point_first.mValX + 1; i < it->m_point_second.mValX; i++)
// //   						vecPoins.push_back(GeomPoint(i, it->m_point_first.mValY));					   
// //   				}
// 
//   				GeomPoint * arr_out = 0;
//   				unsigned size_arr_out = 0;
//   				//fillBoundary(&vecPoins[0], vecPoins.size(), arr_out, size_arr_out);
// 				fillBoundary_ex( &vecPoins[0], vecPoins.size(), arr_out, size_arr_out);
//   
//   				size = size_arr_out;
//   
//   				if(size_arr_out == 0){
//   					return 0;
//   				}
//   
//   				vecPoins.resize(size_arr_out);
//   				//vecPoins.
//   				copy( arr_out, arr_out + size_arr_out, vecPoins.begin() );
//   				FREE_T(arr_out);
// 			}
			
			size = vecPoins.size();

			return  &vecPoins[0];
		}
	};


	struct GeomPolygone {		
	public:
		GeomPolygone()
			:m_step_in_line(0){
		}		

		void addPoints( GeomPoint *points,unsigned points_count){
			for(unsigned i = 0 ; i < points_count; i++)
				vec_points_node.push_back(points[i]);
		}
		
		void addPoint( const GeomPoint &point){
			vec_points_node.push_back(point);
		}
		
		void setStep(int inStep){
			m_step_in_line = inStep;
		}

		void clearPoints(){
			vec_points_node.clear();
			pointsRez.clear();			
		}


		GeomPoint *getPoints(size_t &size){						
			if(!pointsRez.size())
					calculate();

			size = pointsRez.size();
			if(size == 0) 
				return 0;
			return &pointsRez[0];
		}		

	private:

		bool calculate(){		

			if( vec_points_node.size() < 3 ){ // min 3 points 							
				return 0;
			}
			vec_points_node.push_back( vec_points_node[0] ); // pushing finish point from begin						
			GeomLine line;
			line.setStep( m_step_in_line );
			for( size_t i = 0; i != vec_points_node.size() - 1; i++ ){				
				line.mValXBegin = vec_points_node[i].mValX;
				line.mValYBegin = vec_points_node[i].mValY;			
				line.mValXEnd = vec_points_node[i + 1].mValX;
				line.mValYEnd = vec_points_node[i + 1].mValY;

				size_t points_count_ = 0;
				GeomPoint *points_line = line.getPoints(points_count_);			
				
				for(size_t k = 0; k < points_count_; k++ ){						
						pointsRez.push_back(points_line[k]);
				}
				line.clearPoints();
			}
			line.clearPoints();	
			
			return true;
		}		
		vecPoins_t vec_points_node;
		vecPoins_t pointsRez;

		int m_step_in_line;
	};	
}

#endif
