/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaPrecompile.h"

#ifndef TINTA_NO_INTERACT

#include "tintaInteractUnit.h"
#include "tintaUnitsSet.h"
#include "tintaChildProc.h"
#include "tintaTexSpringMain.h"

namespace Tinta {


tintaInteractUnit::tintaInteractUnit(void)
:mPInteract(NULL){
	if (Tinta::tintaUnitsSet::getPtr())
		Tinta::tintaUnitsSet::getPtr()->addUnit(this);

	
}


tintaInteractUnit::tintaInteractUnit( tintaAsioClient * pInteract)
:mPInteract(pInteract){
	if (Tinta::tintaUnitsSet::getPtr())
		Tinta::tintaUnitsSet::getPtr()->addUnit(this);

	CoreAssert(mPInteract->getHandler(), "mPInteract->getHandler()" );
	if( mPInteract->getHandler() )
		mPInteract->getHandler()->setUnitsCallback( this );
}

tintaAsioClient * tintaInteractUnit::getInteractClient(){
	return mPInteract;
}

const tintaAsioClient * tintaInteractUnit::getInteractClient() const {
	return mPInteract;
}

void tintaInteractUnit::addtInteractClient( tintaAsioClient *add ) {
	if( mPInteract->getHandler() )
		mPInteract->getHandler()->setUnitsCallback( this );

	mPInteract = add;
}

tintaInteractUnit::~tintaInteractUnit(void)
{
	if (Tinta::tintaUnitsSet::getPtr())
		Tinta::tintaUnitsSet::getPtr()->removeUnit(mId);
}

void tintaInteractUnit::setState( UnitState state ){
	   
	if ( state == enBusy ){
		// must be received from the remote unit - OnUnitStateReceived callback
		return;
	}

    tintaExecutingUnit::setState(state);		
}

void tintaInteractUnit::OnPriorReceived( float prior ){

	mPrior = prior;
	if ( mClb )
		mClb->onPriorChanged( mId, mPrior );

}

void tintaInteractUnit::OnNameReceived( const String &name ){

	mId.mname = name;
	if ( mClb )
		mClb->onNameChanged( mId, mId.mname );
}

tintaExecutingUnit::UnitId tintaInteractUnit::getUnitId() const {
	return mId;
}

bool tintaInteractUnit::execute( ){

	return true;
}

void tintaInteractUnit::OnUnitStateReceived( tintaExecutingUnit::UnitState state ){
        
    tintaExecutingUnit::setState( state );

    
}

bool tintaInteractUnit::execute( const tintaExecutingTask &task ){
	
	if( !mPInteract )
		return false;

#if DEBUG_MODE
    if (task.getCommand().length() == 544)
    {
        int t = 10;
    }
#endif

	mTask = task;
	size_t size = mTask.GetBufferSize();
	//m_int8 *data = ALLOC_T( m_int8, size );

	//tintaBufferIO *dataBuff = NEW_T  (tintaBufferIO)();

    UNIQPTRDEF(tintaBufferIO, dataBuff, NEW_T(tintaBufferIO)());

	dataBuff->AllocateBuffer( size );
	// filling unit id
	//mTask.setOwner( getId().mid );

	
    auto s = mTask.packData(dataBuff->GetBufferEx(), 0);

    assert( s == size );
	
    tintaBufferIOUnqPtr buffPacket = tintaInteractHandlerImpl::allocatePacketDataPtr( tintaInteractHandlerImpl::enTaskToexecute, dataBuff.get() );

	tintaChildProc * chProc = tintaTexSpringMain::getPtr()->getInteract();
	CoreAssert(chProc," tintaChildProc * chProc = tintaTexSpringMain::getPtr()->getInteract();");
	tintaAsioInteract* interact =  chProc->getInteract();

	size_t idSend = 0;
	CoreAssert(mPInteract,"mPInteract");	
	m_ulong32 idClient = mPInteract->getId();


    //StringStream msg;
    //msg << _M("############### interact execute mTask owner: ") << mTask.getOwner();
    //Tinta::tintaLogger::getPtr()->logMsg(msg.str());

    
	interact->sendData( buffPacket.get(), idSend, idClient ); 
	
	//DELETE_T( buffPacket, tintaBufferIO );

	return true;
}



tintaExecutingUnit::UnitType tintaInteractUnit::getType()const{
	return tintaExecutingUnit::enInteractUnit;
}


}
#endif
