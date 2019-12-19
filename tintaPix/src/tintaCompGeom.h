/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_COMP_GEOM_H
#define _TINTA_COMP_GEOM_H


#include "tintaPredefine.h"
#include "tintaCoord2.h"
#include <Math/tintaCommonMath.h>
#include "tintaHermSpline.h"
#include <tintaExtreme.h>

namespace Tinta
{
	
	
	struct  tintaCompGeomProp {
	public:
		typedef double numberParam_t;
		typedef StringBasic strParam_t;		
		typedef tintaCoord2int_t coordParam_t;
		typedef coord2dList_t coord2dIListParam_t;
		typedef splnList_t splnNodelistParam_t;
		typedef tintaCompGeomProp::splnNodelistParam_t::const_reverse_iterator t_iter_node;
		typedef floatVec_t factorsParam_t;	
		typedef tintaCompGeomProp::factorsParam_t::const_reverse_iterator crit_t;
		typedef tintaCompGeomProp::coord2dIListParam_t::const_reverse_iterator 	coordit_t;

	
		typedef 
		struct ParamValues
		{
			ParamValues()
				:m_val_d(0.0),
				m_val_coord()
			{}

            ParamValues( const ParamValues &rVal ){
                *this = rVal;
            }

            ParamValues(ParamValues&& rVal) 
                :m_val_d{ rVal.m_val_d },
                 m_val_str( std::move(rVal.m_val_str) ),
                 m_val_coord{ std::move(rVal.m_val_coord) },
                 m_splinenodes( std::move(rVal.m_splinenodes) ),
                 m_coordinates( std::move(rVal.m_coordinates) ),
                 m_fuctors( std::move(rVal.m_fuctors) ){                
            }



			ParamValues(double d_val)//, const coordParam_t &val_coord, const splnNodelistParam_t &in_splinenodes, const coord2dIListParam_t &in_coord_list, const String &in_val_str)
				:m_val_d(d_val) 				
			{}

			/*,m_val_coord(val_coord)
				,m_splinenodes(in_splinenodes)
				,m_coordinates(in_coord_list)
				,m_val_str(in_val_str)*/
			ParamValues(const coordParam_t &val_coord)//, const coordParam_t &val_coord, const splnNodelistParam_t &in_splinenodes, const coord2dIListParam_t &in_coord_list, const String &in_val_str)
				:m_val_coord(val_coord) 				
			{}

			ParamValues(const splnNodelistParam_t &in_splinenodes)//, const coordParam_t &val_coord, const splnNodelistParam_t &in_splinenodes, const coord2dIListParam_t &in_coord_list, const String &in_val_str)
				:m_splinenodes(in_splinenodes) 				
			{}

			ParamValues(const coord2dIListParam_t &in_coord_list)//, const coordParam_t &val_coord, const splnNodelistParam_t &in_splinenodes, const coord2dIListParam_t &in_coord_list, const String &in_val_str)
				:m_coordinates(in_coord_list) 				
			{}

			ParamValues( const StringBasic &in_val_str)//, const coordParam_t &val_coord, const splnNodelistParam_t &in_splinenodes, const coord2dIListParam_t &in_coord_list, const String &in_val_str)
				:m_val_str(in_val_str) 				
			{}

			ParamValues( const factorsParam_t &in_val_fact)//, const coordParam_t &val_coord, const splnNodelistParam_t &in_splinenodes, const coord2dIListParam_t &in_coord_list, const String &in_val_str)
				:m_fuctors(in_val_fact) 				
			{}

			double				m_val_d;
			StringBasic		m_val_str;
			coordParam_t	    m_val_coord;
			splnNodelistParam_t m_splinenodes;
			coord2dIListParam_t m_coordinates;
			factorsParam_t	    m_fuctors;

 			ParamValues& operator = (const ParamValues& r_val){			
 				if ( this == &r_val )
 					return *this;
 
 				m_val_d = r_val.m_val_d;
 				m_val_coord = r_val.m_val_coord;	
				m_fuctors = r_val.m_fuctors; 				
 				m_splinenodes =r_val.m_splinenodes;
				m_coordinates = r_val.m_coordinates;
				m_val_str = r_val.m_val_str;
 				return *this;
 			}	

            ParamValues& operator= (ParamValues&& r_val){
                if (this == &r_val)
                    return *this;

                m_val_d = r_val.m_val_d;
                r_val.m_val_d = 0.;
                m_val_coord = r_val.m_val_coord;
                m_fuctors = std::move(r_val.m_fuctors);
                
                m_splinenodes = std::move(r_val.m_splinenodes);
                m_coordinates = std::move(r_val.m_coordinates);
                m_val_str = std::move(r_val.m_val_str);
                return *this;
            }
			void clear(){
				m_val_d = 0.;
				m_val_str = "";
				m_splinenodes.clear();
				m_coordinates.clear();
				m_fuctors.clear();
			}

		} param_values_t;

	public:
		tintaCompGeomProp()
			:m_prop_id(0), 
			m_str_name(""),
			m_values()
		{}
 		tintaCompGeomProp(StringBasic in_str_name)
 			:m_str_name(in_str_name)
			,mAssigned(false){
 			m_prop_id = Tinta::FastHash(m_str_name.c_str(), m_str_name.length());
 		}

		tintaCompGeomProp(StringBasic in_str_name, double in_value, bool assigned = false)
			:m_prop_id(0), 
			m_str_name(in_str_name),
			m_values(in_value),
			mAssigned(assigned){
			m_prop_id = Tinta::FastHash(m_str_name.c_str(), m_str_name.length());
		}

		tintaCompGeomProp(StringBasic in_str_name, StringBasic in_value, bool assigned = false)
			:m_prop_id(0), 
			m_str_name(in_str_name),
			m_values(in_value),
			mAssigned(assigned){
			m_prop_id = Tinta::FastHash(m_str_name.c_str(), m_str_name.length());
		}

		tintaCompGeomProp(StringBasic in_str_name, coordParam_t in_value, bool assigned = false)
			:m_prop_id(0), 
			m_str_name(in_str_name),
			m_values(in_value),
			mAssigned(assigned){
			m_prop_id = Tinta::FastHash(m_str_name.c_str(), m_str_name.length());	
		}

		
		tintaCompGeomProp(StringBasic in_str_name, coord2dIListParam_t in_value, bool assigned = false)
			:m_prop_id(0), 
			m_str_name(in_str_name),
			m_values(in_value),
			mAssigned(assigned){
			m_prop_id = Tinta::FastHash(m_str_name.c_str(), m_str_name.length());	
		}

		tintaCompGeomProp(StringBasic in_str_name, const splnNodelistParam_t &in_splinenode_list, bool assigned = false)
			:m_prop_id(0), 
			m_str_name(in_str_name),
			m_values(in_splinenode_list),
			mAssigned(assigned){

			m_prop_id = Tinta::FastHash(m_str_name.c_str(), m_str_name.length());	
		}

		tintaCompGeomProp(StringBasic in_str_name, const factorsParam_t &in_fact_list, bool assigned = false)
			:m_prop_id(0), 
			m_str_name(in_str_name),
			m_values(in_fact_list),
			mAssigned(assigned){

				m_prop_id = Tinta::FastHash(m_str_name.c_str(), m_str_name.length());	
		}

		const StringBasic &GetName() const {
			return m_str_name;
		}

		void SetName( const StringBasic &name) {
			m_str_name = name;
		}


		bool operator<(const tintaCompGeomProp&r_val)const
		{
			return m_prop_id < r_val.m_prop_id;
		}		

		tintaCompGeomProp& operator=(const tintaCompGeomProp& r_val)
		{			
			if ( this == &r_val )
				return *this;

			m_prop_id = r_val.m_prop_id;
			m_str_name = r_val.m_str_name;			
			m_values = r_val.m_values;
			mAssigned = r_val.mAssigned;		

			return *this;
		} 

		void setAssigned(bool assigned ){
			mAssigned = assigned;
		}

		bool getAssigned()const {
			return mAssigned;
		}

		void clear(){
			m_values.clear();
		}
		void setVal(numberParam_t in_value)
		{
			m_values.m_val_d = in_value;
		}

		void setVal(const coordParam_t &in_value)
		{
			m_values.m_val_coord = in_value;
		}

		
		void setVal(StringBasic in_value)
		{
			m_values.m_val_str = in_value;
		}

		void setVal(const coord2dIListParam_t &in_value)
		{
			//m_values.m_splinenodes.insert(m_values.m_splinenodes.begin(),in_value.begin(), in_value.end());
			m_values.m_coordinates = in_value;
		}
		void setVal(const splnNodelistParam_t &in_value)
		{
			//m_values.m_splinenodes.insert(m_values.m_splinenodes.begin(),in_value.begin(), in_value.end());
			m_values.m_splinenodes = in_value;
		}

		void setVal(const factorsParam_t &in_value)
		{
			//m_values.m_splinenodes.insert(m_values.m_splinenodes.begin(),in_value.begin(), in_value.end());
			m_values.m_fuctors = in_value;
		}


		// adding all container values
		void addVals(const param_values_t &in_values){
			if( in_values.m_splinenodes.size() > 0)
				m_values.m_splinenodes.insert(m_values.m_splinenodes.begin(),in_values.m_splinenodes.begin(), in_values.m_splinenodes.end());			
			else if( in_values.m_coordinates.size() > 0)
				m_values.m_coordinates.insert(m_values.m_coordinates.begin(),in_values.m_coordinates.begin(), in_values.m_coordinates.end());
			else if( in_values.m_fuctors.size() > 0)
				m_values.m_fuctors.insert(m_values.m_fuctors.begin(),in_values.m_fuctors.begin(), in_values.m_fuctors.end());
		}

		const param_values_t &getVal() const
		{
			return m_values;
		}
		param_values_t &getValEx()
		{
			return m_values;
		}
		void setVals(const param_values_t &in_values)
		{
			m_values = in_values;
		}
	private:		

		// hashed name
		unsigned m_prop_id;

		StringBasic m_str_name;

		ParamValues m_values;

		// some properties have to be assigned explicitly before using, if it has true in constructor assigned is not necessary
		bool mAssigned;

	};

	/*
		Describes error if it has been raised
	*/
	typedef struct ExecuteRez {

		typedef tintaCompGeomProp::param_values_t val_param_t;
		ExecuteRez():bRes(false), mTextError(""){}
		ExecuteRez( bool res, const StringBasic &text ): bRes(res), mTextError(text){}	
		ExecuteRez( const StringBasic &text ):bRes(false), mTextError(text){}
// 		operator bool()const {
// 			return ( mCode != -1 || mTextError != "" );
// 		}
		bool bRes;
		StringBasic  mTextError;
		// keeping result
		val_param_t		 mValRez;
	} ExecuteRez_t;




	// base class for compound geom objects having properties
	class  _CoreExport tintaCompGeom
		: public AllocatedObjectCompGeometry,
          public tintaINamed
	{
	public:
		

		tintaCompGeom( bool selectable );
		tintaCompGeom();
		virtual ~tintaCompGeom(void);
	public:
			typedef std::map<StringBasic, tintaCompGeomProp> TProperSet;

			typedef TProperSet::const_iterator TProperSetConstIter;

			typedef TProperSet::iterator TProperSetIter;

			// don`t use in inside executing functions, instead use const function, course flag
			// of changing will be true
			tintaCompGeomProp	   *getPropValPtr(const StringBasic &propNme);

	  const tintaCompGeomProp	   *getPropValPtr(const StringBasic &propNme) const;

			

	virtual	bool addProperty(const tintaCompGeomProp &prop);

	virtual	bool setPropVal(const tintaCompGeomProp &prop);

	virtual	bool addPropVal(const tintaCompGeomProp &prop);

	virtual bool getPropVal(const char *propNme, tintaCompGeomProp &prop)const;	

	



	// declaring properties
	virtual void init() = 0;
		
	// clear all properties
	void clearProperties();
	//clears result points and updates extreme values
	void clear();

	// clear all data include result
	virtual void clearData();

	//virtual bool executeObj( const coord2dI_t *points, m_uint32 count, bool draw );
	
	//virtual bool executeObj( bool draw );

	// for catmul splines
	virtual bool executeObj( float factor, ExecuteRez_t &rez );

	virtual bool executeObj( ExecuteRez_t &rez );

	virtual void fillPoints( const GeomPoint *src, size_t size, const TExtremValues2i *extremes = NULL  );
	
	///returns points for geometry objects
	GeomPoint *getPoints( size_t &quantity );

	///returns points quantity
	size_t pointsQuantity( ) const;

	/// return pixels for brushes
	//virtual const pixel_t *getPixels(unsigned &outPixQuantity) const;
	
	TExtremValues2i getExtreme()const;



    static const char * const str_prop_angle;// = "angle";

    static const char * const str_prop_circle_radius;// = "circle_radius";
    // minimal point step in random figure
    static const char * const str_prop_min_step;// = "min_step";
    // maximal point step in random figure
    static const char * const str_prop_max_step;// = "max_step";
    // nodes in graph
    static const char * const str_prop_node_quantity;// = "node_quantity";
    // max branches on node  (3 as default)
    static const char * const str_prop_branches_in_node;// = "branches_in_node";

    // graph node
    static const char * const str_prop_node;// = "node";
    // graph max size in pixels
    static const char * const str_prop_graph_max_size;// = "graph_max_size";
    static const char * const str_prop_point;// = "point";
    // fill method
    static const char * const str_prop_geom_fill;// = "fill";
    static const char * const str_prop_center_point;// = "center";
    static const char * const str_prop_spline_node;// = "spline_node";

    // property for fill_random_spread - for geometry   
    // pixel steps where drawing any not cycled figure
    static const char * const str_outline_pixel_step;// = "outline_pixel_step";


    // filler
    static const char * const str_prop_buffer_id; // = "buffer_id";
    // minimal distance to find nearest point
    static const char *const str_prop_min_distance;// = "min_distance";
    // minimal points quantity must be found around point on minimal distance
    static const char *const str_prop_around_points;// = "around_points";
    static const char *const str_fill_method;// = "fill_method";
    // nearest front points algorithm
    static const char *const str_fill_nearast_front;// = "fill_nearfront";
    // rectangle intersection algorithm
    static const char *const str_fill_rec_intersect;// = "fill_rectangle";
    // creates polygons from nearest points
    static const char *const str_fill_polygon;// = "fill_polygon";

    static const char *const str_geom_filler;// = "geom_filler";

	protected:		
		TProperSet m_properties;

		// common defs
 		typedef Tinta::GeomPoint point_t;
 
 		typedef std::vector<point_t> pointsVec_t;
 
 		//typedef std::list<point_t> pointsList_t;
 
 		typedef Tinta::splnList_t splineNodes_t;

		static StringBasic formPropWrongErr( const char *propName );
		// indicates that one of properties has changed
		bool m_prop_changed;

	private:
		// moved to private to prevent changing from children
		pointsVec_t m_pointsRes;
		// extreme values of result points
		TExtremValues2i			 mExtreme;
	};

}

#endif


