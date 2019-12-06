/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_GEOM_TYPES_H_
#define _TINTA_GEOM_TYPES_H_

#include <tintaStdHeaders.h>
#include "tintaCompGeom.h"
#include "tintaPrimitives.h"
#include "tintaCoord2.h"
#include "tintaGeomFactory.h"

namespace TintaGeomTypes
{
    using namespace Tinta;

// compound object spline 
class _CoreExport tintaCompGeomSpline
	:public tintaCompGeom
{
public:
	tintaCompGeomSpline(void);
	virtual ~tintaCompGeomSpline(void);
public:
	virtual void	   init();		
	virtual bool	   executeObj( ExecuteRez_t &rez );	
	void			   clearData();
    virtual const StringBasic& getName() const;

    static const char * const str_geom_spline;

private:
	splineNodes_t m_nodes;
	Tinta::tintaHermSpline m_spline;
	
	// node points 2 - minimum
	splinePointsVec_t m_node_points;
	// keeping all tangents
	splinePointsVec_t m_node_tangents;
	// spline periods from node to node - if select too small or large , artifacts appears
	//floatVec_t m_node_periods;
	// keeping result points after connecting spline points with lines
	pointsVec_t m_pointsResFull;  

	int mXMin;
	int mXMax;
	int mYMin;
	int mYMax;
};

class _CoreExport tintaCompGeomSplineFactory: 
	public tintaGeomFactory {

public:
	tintaCompGeomSplineFactory(){}
	virtual ~tintaCompGeomSplineFactory() {}
	
	const StringBasic& getName(void) const;
	
	tintaCompGeom *createInstance(  ) const {
		return M_NEW tintaCompGeomSpline();
	}	
	void releaseInstance(tintaCompGeom * obj) {
		M_DELETE obj;
	}	
};


// compound object polyline 
class _CoreExport tintaCompGeomPolyLine
	:public tintaCompGeom
{
public:
	tintaCompGeomPolyLine(void);
	~tintaCompGeomPolyLine(void);
public:
	virtual void		init();	

	virtual bool		executeObj( ExecuteRez_t &rez );

    virtual const StringBasic& getName() const;

    static const char * const str_geom_polyline;
protected:
    TintaPrimitives::GeomLine m_line;
	pointsVec_t m_polyline_nodes;
	
	bool				calculatePoints(ExecuteRez_t &rez);
	
};

class _CoreExport tintaCompGeomPolyLineFactory: 
	public tintaGeomFactory {

public:
	tintaCompGeomPolyLineFactory(){}
	virtual ~tintaCompGeomPolyLineFactory() {}

	const StringBasic& getName(void) const;

	tintaCompGeom *createInstance(  ) const {
		return M_NEW tintaCompGeomPolyLine();
	}
	void releaseInstance(tintaCompGeom * obj) {
		M_DELETE obj;
	}
	
};


// geometry figure - circle
class _CoreExport tintaCompGeomCircle
	:public tintaCompGeom
{
public:
	tintaCompGeomCircle(void);
	~tintaCompGeomCircle(void);
public:
	virtual void	   init();	

	virtual bool	   executeObj( ExecuteRez_t &rez );	

	bool	           calculatePoints();	

    virtual const StringBasic& getName() const;

    static const char * const str_geom_circle;

private:
    TintaPrimitives::GeomCircle m_circle;
};

class _CoreExport tintaCircleFactory: 
	public tintaGeomFactory {

public:
	tintaCircleFactory(){}
	virtual ~tintaCircleFactory() {}

	const StringBasic& getName(void) const;

	tintaCompGeom *createInstance(  ) const {
		return M_NEW tintaCompGeomCircle();
	}	
	void releaseInstance(tintaCompGeom * obj) {
		M_DELETE obj;
	}

};



}

#endif