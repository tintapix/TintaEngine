/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaPrecompile.h"
#include <tintaConfig.h>
#ifndef TINTA_NO_INTERACT


#include "tintaInteractUnitsSet.h"
#include "tintaUnitsSet.h"
#include "tintaTexSpringMain.h"

namespace Tinta {


tintaInteractUnitsSet::tintaInteractUnitsSet(void)
{
	tintaChildProc*	ch = NULL;		
    if (Tinta::tintaTexSpringMain::getPtr())
        ch = Tinta::tintaTexSpringMain::getPtr()->getInteract();
	if( ch ){
		tintaAsioInteract* interact = ch->getInteract();
        if (interact)
		    interact->addConnectListener(this);
	}
}

tintaInteractUnitsSet::~tintaInteractUnitsSet(void)
{
	interactunits_t::iterator ifind = mInteractUnint.begin( );
	for(; ifind != mInteractUnint.end(); ){
		DELETE_T( ifind->second , tintaInteractUnit );
		mInteractUnint.erase(ifind++);
	}
}

void tintaInteractUnitsSet::onUnplagged( m_ulong32 id ) {
	
	interactunits_t::iterator ifind = mInteractUnint.find( id );
	if( ifind!= mInteractUnint.end() ){
		DELETE_T( ifind->second , tintaInteractUnit );
		mInteractUnint.erase(ifind);
			//ifind mInteractUnint.find( id );
	}


}

void tintaInteractUnitsSet::onErrorOccured( m_ulong32 id , long error ){

}

m_uint32 tintaInteractUnitsSet::getObservMask() const {
	return tintaConnectionEvents::enUnplagged | tintaConnectionEvents::enError | tintaConnectionEvents::enConnect;
}

void tintaInteractUnitsSet::onConnected( m_ulong32 id ){

	tintaChildProc*	ch = NULL;		
    if (Tinta::tintaTexSpringMain::getPtr())
        ch = Tinta::tintaTexSpringMain::getPtr()->getInteract();
	if( ch ){

		tintaAsioInteract* interact = ch->getInteract();
        if ( !interact )
            return;

		CoreAssert(interact,"tintaAsioInteract* interact = ch->getInteract();");

        servedPtr_t client = interact->getClient(id);
		if( client ) {

			tintaInteractUnit *newUnit = NEW_T(tintaInteractUnit)(client.get());
			std::pair<interactunits_t::iterator, bool> rez = mInteractUnint.insert(interactunits_t::value_type(id, newUnit));

			if( !rez.second ){
				StringStreamBasic msg;
				msg << "Error while generating id ";							
				EXCEPTION( msg.str().c_str() );	
			}
			// state have to be received from th client connection
			//rez.first->second->setState(tintaExecutingUnit::enReady);
		}	
	}	
	
}

const tintaInteractUnit* tintaInteractUnitsSet::findUnit( m_ulong32 id )const {

	interactunits_t::const_iterator ifind = mInteractUnint.find( id );
	if( ifind!= mInteractUnint.end() ){
		return ifind->second;	
	}
	return NULL;
}



template<> tintaInteractUnitsSet* Singleton<tintaInteractUnitsSet>::mPtr = 0;

tintaInteractUnitsSet* tintaInteractUnitsSet::getPtr(void)	{
	return mPtr;
}

}
#endif