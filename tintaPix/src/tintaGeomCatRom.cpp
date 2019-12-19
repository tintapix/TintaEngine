/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaPrecompile.h"
#include "tintaGeomCatRom.h"
#include "tintaGeomSet.h"



namespace Tinta {


    const char * const tintaGeomCatRom::str_geom_catrom = "geom_catrom";

    const char * const tintaGeomCatRom::str_prop_knotes = "knotes";

tintaGeomCatRom::tintaGeomCatRom( ){
	init();
}

tintaGeomCatRom::~tintaGeomCatRom( ){
}
void tintaGeomCatRom::init( ) {
    tintaCompGeom::addProperty(tintaCompGeomProp(tintaGeomCatRom::str_prop_knotes, 0.0));
}

void tintaGeomCatRom::clearData() {
	mFactors.clear();
}
bool  tintaGeomCatRom::executeObj( float factor, ExecuteRez_t &rez ) {	

	rez.bRes = true;

	if( m_prop_changed ){
		tintaCompGeomProp prop;

        if (!getPropVal(tintaGeomCatRom::str_prop_knotes, prop)){
            rez.mTextError = formPropWrongErr(tintaGeomCatRom::str_prop_knotes);
			rez.bRes = false;
			
		
			return false;
		}

		// getting all points	
		typedef tintaCompGeomProp::factorsParam_t::const_reverse_iterator crit_t;
		mFactors.reserve( prop.getVal().m_fuctors.size() + 2 );	
		// spline specific behaviour need 2 points minimum
		mFactors.push_back( 0.f );
		for( crit_t iter = prop.getVal().m_fuctors.rbegin(); iter != prop.getVal().m_fuctors.rend(); ++iter ){
			mFactors.push_back( *iter );
		}
		mFactors.push_back( 0.f);
	}
	m_prop_changed = false;
    rez.mValRez.m_val_d = (double)TintaMath::min(TintaMath::max(mSp.getValue(factor, mFactors.size(), &mFactors[0]), 0.f), 1.f);

	return true;
	
}
const StringBasic& tintaGeomCatRom::getName() const{
    static StringBasic name = tintaGeomCatRom::str_geom_catrom;
    return name;
}

const StringBasic& tintaCatRomFactory::getName(void) const{
    static StringBasic name = tintaGeomCatRom::str_geom_catrom;
	return name;	

}

}
