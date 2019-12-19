/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef TINTA_GEOM_CAT_ROM_H_
#define TINTA_GEOM_CAT_ROM_H_

#include "tintaPredefine.h"
#include "tintaGeomFactory.h"
#include "tintaGeomTypes.h"
#include "Math/tintaCatRomSpline.h"

namespace Tinta {

/*
	Fills the spaces between points, using selected algorithm
*/
class tintaGeomCatRom
	: public tintaCompGeom
{
public:
	
	
	tintaGeomCatRom(void);
	virtual ~tintaGeomCatRom(void);

public:
	virtual void		init();		
	virtual bool		executeObj(float factor, ExecuteRez_t &rez);
	virtual void clearData();

    virtual const StringBasic& getName() const;

    // catmoll-rom spline
    static const char * const str_geom_catrom;
    static const char * const str_prop_knotes;

protected:
	tintaCatRomSpline mSp;
	tintaCompGeomProp::factorsParam_t mFactors;
	
};



class _CoreExport tintaCatRomFactory: 
	public tintaGeomFactory {

public:
	tintaCatRomFactory(){}
	virtual ~tintaCatRomFactory() {}

	const StringBasic& getName(void) const;

	tintaCompGeom *createInstance(  ) const {
		return M_NEW tintaGeomCatRom();
	}		
	void releaseInstance(tintaCompGeom * obj) {
		M_DELETE obj;
	}
};


}


#endif
