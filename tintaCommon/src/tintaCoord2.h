/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_COORD2_H__
#define _TINTA_COORD2_H__

#include "tintaPredefine.h"
#include <Math/tintaVector2.h>
#include <Math/tintaVector3.h>

namespace Tinta
{

	

	template<typename T> 
	struct tintaCoord2
	{
	
	public:
		T mValX; T mValY;
	public:

		explicit tintaCoord2(): mValX(-1), mValY(-1){}
		tintaCoord2(const tintaCoord2 &new_coordinate){
			*this = new_coordinate;
		}
		tintaCoord2(const T &x_value,const T &y_value): mValX(x_value), mValY(y_value){}        
		tintaCoord2 &operator=(const tintaCoord2 &rval){
			if(&rval == this)
				return *this;
			mValX = rval.mValX;
			mValY = rval.mValY;
			return *this;

		}

		tintaCoord2 operator*( const tintaCoord2 &rval){
			return tintaCoord2(mValX * rval.mValX, mValY * rval.mValY);
		}

		tintaCoord2 operator+( const tintaCoord2 &rval){
			return tintaCoord2(mValX + rval.mValX, mValY + rval.mValY);
		}

		tintaCoord2 operator-( const tintaCoord2 &rval){
			return tintaCoord2(mValX - rval.mValX, mValY - rval.mValY);
		}

		tintaCoord2 operator/( const tintaCoord2 &rval){
			return tintaCoord2(mValX / rval.mValX, mValY / rval.mValY);
		}
		
		bool operator==(const tintaCoord2 &rval)const{		
			return mValX == rval.mValX && mValY == rval.mValY;
		}
		bool operator!=(const tintaCoord2 &rval){		
			return !(*this == rval);
		}

        static T distance(const tintaCoord2<T> &v1, const tintaCoord2<T> &v2) {
            T x = v1.mValX - v2.mValX;
            T y = v1.mValY - v2.mValY;

            return sqrt(x*x + y*y);            
        }


	};
       
	//typedef tintaCoord2<m_uint32> coord2dU_t;
	typedef tintaCoord2<int> tintaCoord2int_t;
    //typedef tintaVector2f tintaVector2f;

    struct tintaCoord2Rec {
        // left bottom
        tintaVector2f mLB;
        // left upper
        tintaVector2f mLU;
        // right upper
        tintaVector2f mRU;
        // right bottom
        tintaVector2f mRB;

    };

    typedef std::vector<tintaVector2f> objPosfVec_t;
    typedef std::vector<tintaCoord2Rec> objRecPosfVec_t;

	typedef tintaCoord2int_t coord2dI_t;
	typedef std::list<tintaCoord2int_t> coord2dList_t;
	typedef std::vector<tintaCoord2int_t> coord2dIVec_t;
    typedef std::vector<tintaVector2f> coord2dfVec_t;
	typedef std::vector<tintaCoord2int_t>::iterator points_vec_iter;
	typedef std::vector<tintaCoord2int_t>::const_iterator points_vec_citer;
	typedef coord2dList_t::iterator points_list_iter;
	typedef coord2dList_t::const_iterator points_list_citer;
	typedef std::vector<tintaVector3f> splinePointsVec_t;
	// geometry object Point
	typedef coord2dI_t GeomPoint;
	

	typedef 	
	struct CoordFactor {
		// data
		tintaCoord2int_t mCoord;
		float mValue;

		// funcs
		CoordFactor(): mValue(0.f){}
		CoordFactor(const CoordFactor &rVal):mCoord(rVal.mCoord), mValue(rVal.mValue){}
		CoordFactor(tintaCoord2int_t coordinate, float value) : mCoord(coordinate), mValue(value){}
		bool operator == (const CoordFactor &rVal) const{
			return ( mValue == rVal.mValue );
		}

		CoordFactor &operator=(const CoordFactor &rval){
			if(&rval == this)
				return *this;
			mCoord = rval.mCoord;
			mValue = rval.mValue;
			return *this;
		}

		bool operator < (const CoordFactor &rVal) const{
			return ( mValue < rVal.mValue );
		}


	} CoordFactor_t;

	// with greater to front
	struct CoordFactorGreater {

	public:
		CoordFactorGreater(){}

		bool operator()(const CoordFactor &lVal, const CoordFactor &rVal){
			return lVal.mValue > rVal.mValue;
		}

	};

	// with less to front
	struct CoordFactorLess {

	public:
		CoordFactorLess(){}

		bool operator()(const CoordFactor &lVal, const CoordFactor &rVal){
			return lVal.mValue < rVal.mValue;
		}

	};

	typedef std::vector<CoordFactor_t>  CoordFactorVec_t;
	typedef std::list<CoordFactor_t>    CoordFactorList_t;
	typedef CoordFactorList_t::iterator CoordFactorListIt_t;
	typedef CoordFactorList_t::reverse_iterator CoordFactorListRIt_t;


	//spline node (point, tangent)
	typedef struct SplineNode
	{
		SplineNode(){}
		SplineNode(tintaCoord2int_t in_coordinate, tintaCoord2int_t in_tangent)
			:m_coordinate(in_coordinate)
			,m_tangent(in_tangent)
		{}
		tintaCoord2int_t m_coordinate;
		tintaCoord2int_t m_tangent;

	} spline_node_t;

	typedef std::list<spline_node_t> splnList_t;
	typedef splnList_t::iterator nodeListIter_t;


typedef std::vector<GeomPoint> vecPoins_t;


struct predXEqual
{
private:
	int value_find;
public:
	predXEqual(int x_val):value_find(x_val){}

	bool operator()(const coord2dI_t &val){
		return val.mValX == value_find;
	}
};

struct predXEqualTwo
{
public:
	predXEqualTwo(){}

	bool operator()(const coord2dI_t &lVal, const coord2dI_t &rVal){
		return lVal.mValX == rVal.mValX;
	}
};

struct predYEqualTwo
{
public:
	predYEqualTwo(){}

	bool operator()(const coord2dI_t &lVal, const coord2dI_t &rVal){
		return lVal.mValY == rVal.mValY;
	}
};

struct predYEqual
{
private:
	int value_find;
public:
	predYEqual(int y_val):value_find(y_val){}

	bool operator()(const coord2dI_t &val){
		return val.mValY == value_find;
	}
};

struct predXLess
{
	bool operator()(const coord2dI_t &l_val, const coord2dI_t &r_val){
		return l_val.mValX < r_val.mValX;
	}
} ;

struct predYLess
{
	bool operator()(const coord2dI_t &l_val, const coord2dI_t &r_val){
		return l_val.mValY < r_val.mValY;
	}
} ;


struct predXGreater
{
	bool operator()(const coord2dI_t &l_val, const coord2dI_t &r_val){
		return l_val.mValX > r_val.mValX;
	}
} ;

struct predYGreater
{
	bool operator()(const coord2dI_t &l_val, const coord2dI_t &r_val){
		return l_val.mValY > r_val.mValY;
	}
} ;


typedef struct predXYLess
{
	bool operator()(const coord2dI_t &l_val, const coord2dI_t &r_val){
		return l_val.mValY < r_val.mValY && l_val.mValX < r_val.mValX;
	}
} t_pred_xy_less;

typedef struct predXEqYLess
{
	bool operator()(const coord2dI_t &l_val, const coord2dI_t &r_val){
		return l_val.mValY < r_val.mValY && l_val.mValX == r_val.mValX;
	}
} t_pred_x_eq_y_less;

typedef struct predXLessYEq
{
	bool operator()(const coord2dI_t &l_val, const coord2dI_t &r_val){
		return l_val.mValY == r_val.mValY && l_val.mValX < r_val.mValX;
	}
} t_pred_x_less_y_eq;




struct point_t_pair
{
	point_t_pair():m_point_first(GeomPoint()),m_point_second(GeomPoint()){}
	point_t_pair(const GeomPoint &point_first, const GeomPoint &point_second )
		:m_point_first(point_first),m_point_second(point_second)
	{
	}
	GeomPoint m_point_first;
	GeomPoint m_point_second;		 
};

typedef std::vector<point_t_pair> ptPairVec_t;




}

#endif
