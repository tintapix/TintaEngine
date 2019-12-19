/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaPrecompile.h"
#include "tintaGeomTypes.h"
#include "tintaGeomSet.h"
#include "tintaAlgorithm.h"


namespace TintaGeomTypes {

/////////////////////////////////////////////////////////////// Spline ///////////////////////////////////////////////////////////////

const char * const tintaCompGeomSpline::str_geom_spline = "geom_spline";

tintaCompGeomSpline::tintaCompGeomSpline(void):
mXMin(-1),
mXMax(-1),
mYMin(-1),
mYMax(-1)
{
	init();
}

tintaCompGeomSpline::~tintaCompGeomSpline(void)
{
	m_spline.clearPoints();
}
void tintaCompGeomSpline::init(){
    tintaCompGeom::addProperty(tintaCompGeomProp(tintaCompGeom::str_prop_angle, 0.0));
 	
    tintaCompGeom::addProperty(tintaCompGeomProp(tintaCompGeom::str_outline_pixel_step, 0.0, true));
    tintaCompGeom::addProperty(tintaCompGeomProp(tintaCompGeom::str_prop_spline_node));
    tintaCompGeom::addProperty(tintaCompGeomProp(tintaCompGeom::str_prop_point)); // if autospline	
	//tintaCompGeom::addProperty(tintaCompGeomProp(tintaCompGeom::str_prop_blur_radius, 0.0));
}

void tintaCompGeomSpline::clearData() {
	
	m_node_points.clear();
	m_node_tangents.clear();
	m_pointsResFull.clear();
	
}


bool tintaCompGeomSpline::executeObj( ExecuteRez_t &rez ){

	if( m_prop_changed ){
 
		tintaCompGeomProp prop;	

 		clear();

		float  period_val = 0.f;
		
		if( getPropVal( tintaCompGeom::str_prop_point, prop) 
                                && prop.getVal().m_coordinates.size() > 0) { 

            auto iter = prop.getVal().m_coordinates.rbegin();
			for( ; iter != prop.getVal().m_coordinates.rend(); iter++){		
				m_node_points.push_back( tintaVector3f( (float)(*iter).mValX, (float)(*iter).mValY, 0.f) );						
			}
			
			if( m_node_points.size() == 0 ){
				rez.mTextError = "No nodes for spline.";
				rez.bRes = false;
				return false;
			}
			if( !m_spline.initializeNat( &m_node_points[0], m_node_points.size(), period_val ) ){
				rez.mTextError = "Spline initialization error.";
				rez.bRes = false;
				return false;
			}
		}



		int max_period = (int) period_val;
  		
		
		coord2dIVec_t vec_end_points;
		for(int i = 0; i <= max_period; i++){
			tintaVector3f rezVec = m_spline.evaluate((float)i);
			vec_end_points.push_back( tintaCoord2int_t((int)rezVec.mx, (int)rezVec.my) );
			
		}
		if( m_node_points.size() == 0 )
			return true;
		// if spline is not enough accurate
		if( vec_end_points[max_period].mValX != m_node_points[m_node_points.size() - 1].mx || vec_end_points[max_period].mValY != m_node_points[m_node_points.size() - 1].my ){

			vec_end_points.push_back(coord2dI_t(TintaMath::roundInt(m_node_points[m_node_points.size() - 1].mx), TintaMath::roundInt(m_node_points[m_node_points.size() - 1].my)) );
		}

        TintaPrimitives::GeomLine line;
		if(!getPropVal(tintaCompGeom::str_outline_pixel_step, prop)){		

			rez.mTextError = formPropWrongErr(tintaCompGeom::str_outline_pixel_step);
			rez.bRes = false;
			
			return false;
		}
		int i_outline_step = (int)prop.getVal().m_val_d; 
		//line.setStep(i_outline_step);
		
		
		assert(vec_end_points.size() > 0);
		tintaAlgorithm::getExtrem(&vec_end_points[0], vec_end_points.size(), mXMin, mYMin, mXMax, mYMax);
		const int factor = 4; // when line draws between points deviations could happen
		TUIntMap points_unique_test(mXMin - factor, mXMax + factor, mYMin - factor, mYMax + factor);

		

		for( size_t i = 0; i != vec_end_points.size()-1; i++ ){

			line.mValXBegin = vec_end_points[i].mValX;
			line.mValYBegin = vec_end_points[i].mValY;			
			
			line.mValXEnd = vec_end_points[i + 1].mValX;
			line.mValYEnd = vec_end_points[i + 1].mValY;
			

			size_t points_count = 0;
			
			point_t *points_line = line.getPoints(points_count);
			
			// k == 1 on the score of excepting point buplicating on the nodes of polygon
			for( size_t k = 0; k < points_count; k++ ){	

				if( points_unique_test.getVal(points_line[k].mValX, points_line[k].mValY) !=  0x01 ){
					m_pointsResFull.push_back(points_line[k]);
					points_unique_test.setVal(points_line[k].mValX, points_line[k].mValY, 0x01);
				}
			}
			line.clearPoints();
		}	
		if( i_outline_step > 0 ){

			//making resampling by step
			unsigned step_move = 0;
			unsigned points_count = m_pointsResFull.size();

			pointsVec_t pointsRes;
			pointsRes.reserve(points_count);
			while( step_move < points_count ){
				pointsRes.push_back(m_pointsResFull[step_move]);
				step_move+= i_outline_step;
			}
			TExtremValues2i ext( mXMin, mXMax, mYMin, mYMax );
			fillPoints( &pointsRes[0], pointsRes.size(), &ext );
		}
		else {			
			TExtremValues2i ext( mXMin, mXMax, mYMin, mYMax);
			fillPoints(&m_pointsResFull[0], m_pointsResFull.size(), &ext );	
		}
	}	
	m_prop_changed = false;
	return true;
}

const StringBasic& tintaCompGeomSpline::getName() const{
    static StringBasic name = tintaCompGeomSpline::str_geom_spline;
    return name;
}

const StringBasic& tintaCompGeomSplineFactory::getName( void ) const {
    static StringBasic name = tintaCompGeomSpline::str_geom_spline;
	return name;	
}



/////////////////////////////////////////////////////////////// PolyLine ///////////////////////////////////////////////////////////////

const char * const tintaCompGeomPolyLine::str_geom_polyline = "geom_polyline";

tintaCompGeomPolyLine::tintaCompGeomPolyLine(void){
	init();
}

tintaCompGeomPolyLine::~tintaCompGeomPolyLine(void){
}
void tintaCompGeomPolyLine::init()
{	
	//tintaCompGeom::addProperty(tintaCompGeomProp(tintaCompGeom::str_prop_transparency,1.0));
	tintaCompGeom::addProperty(tintaCompGeomProp(tintaCompGeom::str_prop_point));		
	tintaCompGeom::addProperty(tintaCompGeomProp(tintaCompGeom::str_outline_pixel_step, 0.0, true));
	
}

bool tintaCompGeomPolyLine::calculatePoints(ExecuteRez_t &rez){
	clear();
	m_line.clearPoints();
	m_polyline_nodes.clear();
	


	// reverse iterator gets begin of sequence of the inserted points
	//typedef tintaCompGeomProp::coord2dIListParam_t::const_reverse_iterator 	coordit_t;

	tintaCompGeomProp prop;
	if(!getPropVal(tintaCompGeom::str_prop_point, prop)){

		rez.mTextError = formPropWrongErr( tintaCompGeom::str_prop_point );
		rez.bRes = false;
		return false; 
	}
	// getting all points
	m_polyline_nodes.reserve(prop.getVal().m_coordinates.size());
	for(tintaCompGeomProp::coordit_t iter = prop.getVal().m_coordinates.rbegin(); iter != prop.getVal().m_coordinates.rend(); iter++){
		m_polyline_nodes.push_back(Tinta::GeomPoint(iter->mValX, iter->mValY));
	}

	if( m_polyline_nodes.size() < 2 ){

		StringStreamBasic msg;
		msg <<"Too few points in polyline. Count: ";
		msg<<m_polyline_nodes.size();
		rez.mTextError = msg.str();
		rez.bRes = false;
		return false; 
	}

	if(!getPropVal(tintaCompGeom::str_outline_pixel_step, prop)){
		rez.mTextError = formPropWrongErr( tintaCompGeom::str_outline_pixel_step );
		rez.bRes = false;
		
		return false;
	}
	int i_outline_step = (int)prop.getVal().m_val_d; 
	m_line.setStep(i_outline_step);

	if(m_polyline_nodes.size() == 0)
		return true;
	pointsVec_t pointsFull;
	for( size_t i = 0; i != m_polyline_nodes.size() - 1; i++ ){
		m_line.mValXBegin = m_polyline_nodes[i].mValX;
		m_line.mValYBegin = m_polyline_nodes[i].mValY;			
		m_line.mValXEnd = m_polyline_nodes[i + 1].mValX;
		m_line.mValYEnd = m_polyline_nodes[i + 1].mValY;

		size_t points_count_ = 0;
		point_t *points_line = m_line.getPoints(points_count_);
		pointsFull.reserve(points_count_);
		
		if( points_count_ > 0 ) {
			for(size_t k = 0; k < points_count_; k++ ){	
				pointsFull.push_back(points_line[k]);
			}
		}
		m_line.clearPoints();
	}	

	TExtremValues2i ext;
	tintaAlgorithm::getExtrem( &pointsFull[0], pointsFull.size(), ext ); 
	fillPoints( &pointsFull[0], pointsFull.size(), &ext );	

	return true;
}
bool tintaCompGeomPolyLine::executeObj(ExecuteRez_t &rez){

	if( m_prop_changed ){
		clear();
		if( !calculatePoints(rez) )
			return false;
	}	
	m_prop_changed = false;
	return true;
}

const StringBasic& tintaCompGeomPolyLine::getName() const{
    static StringBasic name = tintaCompGeomPolyLine::str_geom_polyline;
    return name;
}

const StringBasic& tintaCompGeomPolyLineFactory::getName(void) const{
    static StringBasic name = tintaCompGeomPolyLine::str_geom_polyline;
	return name;	
}




/////////////////////////////////////////////////////////////// GeomCircle ///////////////////////////////////////////////////////////////

const char * const tintaCompGeomCircle::str_geom_circle = "geom_circle";

tintaCompGeomCircle::tintaCompGeomCircle(void){
	init();
}



tintaCompGeomCircle::~tintaCompGeomCircle(void){
}
void tintaCompGeomCircle::init() {
	tintaCompGeom::addProperty(tintaCompGeomProp(tintaCompGeom::str_prop_circle_radius,1.0));	
		
	tintaCompGeom::addProperty(tintaCompGeomProp(tintaCompGeom::str_prop_geom_fill, 0., true));
	
	//tintaCompGeom::addProperty(tintaCompGeomProp(tintaCompGeom::str_prop_points_probability, 1.0));
	tintaCompGeom::addProperty(tintaCompGeomProp(tintaCompGeom::str_prop_center_point));	
}

bool tintaCompGeomCircle::calculatePoints() {
	
	return true;
}

bool tintaCompGeomCircle::executeObj( ExecuteRez_t &rez ) {

	//typedef Tinta::tintaPixel24b color_type;
	tintaCompGeomProp prop;
	
	if(!getPropVal(tintaCompGeom::str_prop_center_point, prop)){
		rez.mTextError = formPropWrongErr( tintaCompGeom::str_prop_center_point );
		rez.bRes = false;
		
		return false;
	}


	tintaCoord2int_t center = prop.getVal().m_val_coord;


	if( !getPropVal( tintaCompGeom::str_prop_geom_fill, prop ) ){

		rez.mTextError = formPropWrongErr( tintaCompGeom::str_prop_geom_fill );
		rez.bRes = false;

		return false;
	}

	bool fill_method = prop.getVal().m_val_d > 0.;
	

	if( !getPropVal(tintaCompGeom::str_prop_circle_radius, prop) ){
		rez.mTextError = formPropWrongErr( tintaCompGeom::str_prop_circle_radius );
		rez.bRes = false;

		return false;
	}

	int circle_radius = (unsigned)prop.getVal().m_val_d;
	m_circle.setCenter( center.mValX, center.mValY );
	m_circle.setRadius( circle_radius );

 	int outline_size( 0 );
	Tinta::GeomPoint *outline = m_circle.getCirclePoints(outline_size);
		
	TExtremValues2i ext;
	tintaAlgorithm::getExtrem( outline, outline_size, ext ); 
	fillPoints( outline, outline_size, &ext );

	return true;
}

const StringBasic& tintaCompGeomCircle::getName() const{
    static StringBasic name = tintaCompGeomCircle::str_geom_circle;
    return name;
}

const StringBasic& tintaCircleFactory::getName(void) const{
    static StringBasic name = tintaCompGeomCircle::str_geom_circle;
	return name;	
}




}
